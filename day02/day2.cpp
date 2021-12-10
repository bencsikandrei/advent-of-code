#include <algorithm>
#include <charconv>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string_view>

#include <fmt/format.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << fmt::format("Provide input file: {} <file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  std::ifstream input{argv[1]};
  if (!input.good()) {
    std::cout << fmt::format("Invalid input file: {} <file>\n", argv[1]);
    return EXIT_FAILURE;
  }

  constexpr std::string_view delim = " ";
  std::string line;
  int depth = 0;
  int distance = 0;

  while (std::getline(input, line)) {

    std::string_view sv = line;
    auto where = sv.find_first_of(" ");
    int temp = 0;
    std::from_chars(line.c_str() + where + 1, line.c_str() + line.size(), temp);

    switch (line[0]) {
    case 'f':
      distance += temp;
      break;
    case 'u':
      depth -= temp;
      break;
    case 'd':
      depth += temp;
      break;
    }
  }

  std::cout << "Final: " << depth * distance << "\n";

  std::cout << fmt::format("{}: Done!\n", argv[0]);
}
