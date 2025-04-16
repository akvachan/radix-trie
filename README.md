# Radix Trie implementation

## Description
Radix trie is a compressed prefix trie. 

Here is a sample "tree" output:

```
# 
## do
### om
### g
### ve
## ca
### n
### t
### r
#### bon
```

**Explanation**: This is a markdown representation of the radix trie. It should be read as follows:
```
# Root
## Child 1
### Grandchild 1
### Grandchild 2
```

## Install

You can setup the radix trie using **one** of the following methods:

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
- [x] print: Print out every word in the trie. 
- [x] print\_md: Print out the markdown representation. 
- [x] find\_node: Checks if a path exists.

## Planned methods
- [ ] complete: Completes a given prefix.
- [ ] load: Create a trie from a dictionary.
- [ ] unload: Create a dictionary from a trie.
- [ ] reorder: Reorder the trie using a certain policy, for example ascending or descending.
- [ ] delete: Deletes a word from the trie.

## Planned other
- [ ] constructors: To fully support moves, copies and swaps.
- [ ] errors and logs: Proper Go-style error handling and logging. 
- [ ] unit tests: Proper unit tests using ctest suite. 
- [ ] iterator: Conveniently iterates the trie via a for-loop using a certain policy, for example bfs or dfs.
- [ ] metadata: Support templated metadata for each node and corresponding get() method to retrieve it.
- [ ] smart pointers: Implementation via smart pointers.
- [ ] overloads (==, +, -): Tries comparison, merge or diff.

## Visualization in Raylib
- [ ] View trie.
- [ ] Move nodes.
- [ ] Insert nodes.
- [ ] Delete nodes. 
- [ ] Search.

## Additional information
I also plan to make a video and a table on runtime/space complexity comparison between the normal trie (check out algorithms.cpp repo) and radix trie.

## DISCLAIMER
This implementation is for educational purposes only and is not intended for production environments.
