#!/bin/sh

set -Eeuxo pipefail

cmake -B build
cmake --build build --parallel 10
build/radix-trie
