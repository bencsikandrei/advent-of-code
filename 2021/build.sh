#!/usr/bin/env sh

rm -fr ../aoc-2021-build/

conan install . -if ../aoc-2021-build/

cmake -B ../aoc-2021-build/ -S .

cmake --build ../aoc-2021-build/ --target all --parallel ${nproc}
