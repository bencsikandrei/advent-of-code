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

constexpr i32 parse_fixed(const StringView number) {
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

#include <algorithm>
#include <map>

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

	int first[1'024]{};
	int second[1'024]{};

	int line = 0;
	while (begin < end) {
		first[line] = parse_fixed(StringView{begin, 5});
		second[line] = parse_fixed(StringView{begin + 8, 5});
		// printf("first[%d] = %d, second[%d] = %d\n", line, first[line], line, second[line]);
		begin += 14;
		++line;
	}

	std::sort(first, first + line);
	std::sort(second, second + line);

	i64 total_difference = 0;
	for (int i = 0; i < line; ++i) { total_difference += basic_abs(first[i] - second[i]); }

	printf("Total difference is: %lld\n", total_difference);

	std::map<int, int> count;
	for (int i = 0; i < line; ++i) { count[second[i]]++; }

	i64 similarity_score = 0;
	for (int i = 0; i < line; ++i)
	{
		const auto it = count.find(first[i]);
		similarity_score += (it != count.end() ? first[i] * it->second : 0);
	}
	printf("Similarity score is: %lld\n", similarity_score);
}
