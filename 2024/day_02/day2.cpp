#include "../common/defines.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>

struct ScopedFile
{
	ScopedFile(int f) noexcept : fd(f) {}
	~ScopedFile() noexcept { if (fd != -1) close(fd); }
	operator bool() const { return fd != -1; }
	int fd;
};

struct ScopedMmap
{
	ScopedMmap(void* m, unsigned m_size) noexcept : mapping(m) , mapping_size(m_size) {}
	~ScopedMmap() noexcept { if (mapping && mapping != MAP_FAILED) munmap(mapping, mapping_size); }
	operator bool() const { return mapping != nullptr && mapping != MAP_FAILED; }
	void* mapping = nullptr;
	u64 mapping_size = 0U;
};

struct StringView
{
	const char* data;
	i64 size;
};

constexpr i32 parse_fixed(const StringView number) 
{
	i32 result = 0;
	for (int i = 0; i < number.size; ++i) {
		const char c = number.data[i];
		result = result * 10 + (c - '0');
	}
	return result;
};

constexpr u32 basic_abs(i32 a)
{
	return a < 0 ? -a : a;
}

#include <cstring>

constexpr i32 parse_digits(const char **str) 
{
	i32 result = 0;
	for (u8 digit = 0; 
			(digit = ((*(*str)++) - '0')) <= 9U; 
			result = result * 10 + digit);
	return result;
}

int main(int argc, char** argv)
{
	if (argc < 2) 
	{
		printf("Usage: %s <input file with 2 columns>\n", argv[0]);
		return -1;
	}
	ScopedFile file(open(argv[1], O_RDONLY));
	if (!file) 
	{
		printf("Can't open file: %s\n", argv[0]);
		return -1;
	}
	struct stat sb;
	if (fstat(file.fd, &sb) == -1) {
		printf("Can't stat file: %s\n", argv[0]);
		return -1;
	}
	ScopedMmap data(mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, file.fd, 0), sb.st_size);
	if (!data)
	{
		printf("Can't mmap file: %s with size %lu\n", argv[0], sb.st_size);
		return -1;
	}
	printf("File %s has size %ld\n", argv[1], sb.st_size);

	const char* begin = (const char*)data.mapping;
	const char* end = (const char*)data.mapping + data.mapping_size;

	const char* crt = begin;
	const char* line_start = crt;

	i32 bytes = end - crt;
	i32 count_safe = 0;
	for (; (crt = (char *) memchr(crt, '\n', bytes)); ++crt, line_start = crt) 
	{
		const char* digit_str = line_start;
		i32 first = parse_digits(&digit_str);
		bool safe = true;
		bool is_ascending = false;
		bool is_descending = false;
		bool tolerated_one = false;
		while (digit_str <= crt) 
		{
			i32 second = parse_digits(&digit_str);
			int diff = second - first;	
			if (diff < 0) { 
			is_ascending = true;
			if (is_descending && !tolerated_one)
			{
				tolerated_one = true;
				is_ascending = false;
			}
			}
			else if (diff > 0) { 
			is_descending = true;
			if (is_ascending && !tolerated_one) 
			{
				tolerated_one = true;
				is_descending = false;
			}
			}
			else {
				if (!tolerated_one) { tolerated_one = true; }
				else safe = false;
			}
			if (!tolerated_one) {
			    if (basic_abs(diff) > 3) tolerated_one = true;
			}
			else {
		            safe &= basic_abs(diff) <= 3;
			}
			first = second;
		}
		count_safe += (safe & (is_ascending ^ is_descending));
		bytes = end - crt;
	}

	printf("Safe reports: %d\n", count_safe);
}
