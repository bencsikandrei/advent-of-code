#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

int oneAtATime(std::istream &input) {
  int count = 0;
  auto begin = std::istream_iterator<int>(input);
  int first = 0;
  for (auto it = std::next(begin); it != std::istream_iterator<int>(); ++it) {
    if (*begin < *it) {
      ++count;
    }
    begin = it;
  }
  return count;
}

int threeAtATime(std::istream &input) {
  int count = 0;

  auto one = std::istream_iterator<int>(input);
  auto o = *one;
  std::advance(one, 1);
  auto t = *one;
  std::advance(one, 1);
  auto th = *one;
  for (auto it = std::next(one); it != std::istream_iterator<int>();
       std::advance(it, 1)) {
    auto i = *it;
    if (o < i) {
      ++count;
    }
    o = t;
    t = th;
    th = i;
  }
  return count;
}

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

  // std::cout << "Total (1): " << oneAtATime(input) << '\n';
  std::cout << "Total (3): " << threeAtATime(input) << '\n';
  std::cout << fmt::format("{}: Done!\n", argv[0]);
}
