# Radix Trie implementation

## Description
Radix trie is a compressed prefix trie. 

## Install

### 1. No build
Just copy [radix\_trie](src/radix_trie.hpp) to your project.

### 2. CMake
Go to the repository root and execute:
```
cmake -B build
cmake --build build --parallel 10
build/radix-trie
```

### 3. Bash script
Go to the repository root and execute:
```
./build.sh
```

## Available methods 
Current implementation is a one-header library with following methods:
- [x] insert: Inserts a word into the trie.
- [x] print: Print the trie if `EFF_AUT_TEST_MODE` is not defined, else tests with the values inserted in main.cpp.

## Planned methods
- [ ] delete: Deletes a word from the trie.
- [ ] complete: Completes a given prefix.
- [ ] check: Checks if a word exists.
- [ ] load: Create a trie from a dictionary.
- [ ] unload: Create a dictionary from a trie.

## Planned implementations
- [ ] iterator: Conveniently iterates the trie via a for-loop.
- [ ] metadata: Support templated metadata for each node.
- [ ] overloads (==, +, -): Tries comparison, merge or diff.
- [ ] constructors: To fully support moves, copies and swaps.
- [ ] smart pointers: Implementation via smart pointers.

## Visualization in Raylib
- [ ] View trie.
- [ ] Move nodes.
- [ ] Insert nodes.
- [ ] Delete nodes. 
- [ ] Search.

## Additional information
I also plan to make a video and a table on runtime/space complexity comparison between the normal trie (check out algorithms.cpp repo) and radix trie.

## DISCLAIMER
This implementation is for educational purposes only and is not intended for production environments
