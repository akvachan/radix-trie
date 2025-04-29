# Radix Trie implementation

## Description
Radix trie is a fast and space-efficient prefix trie with input-length time complexity for all operations. 

Here is a sample "tree" output:

```
# 
## p
### hoton 𐄂
### ulsar 𐄂
### la
#### net 𐄂
#### sma 𐄂
## n
### e
#### bula 𐄂
#### utron 𐄂
### ova 𐄂
## cosm
### os 𐄂
### ic 𐄂
## ast
### ro 𐄂
#### nomy 𐄂
### eroid 𐄂
## qua
### rk 𐄂
### sar 𐄂
### ntum 𐄂
## g
### a
#### la
##### ctic 𐄂
##### xy 𐄂
#### mma 𐄂
### ravit
#### on 𐄂
#### y 𐄂
```

**Explanation**: This is a markdown representation of the radix trie. It should be read as follows:
```
# Root
## Child 1
### Grandchild 1
### Grandchild 2
```

`𐄂` marks a node that makes up a complete word. 

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

## Examples
You can find examples in [main](main.cpp) file.

## Available methods 
Current implementation is a one-header library with following methods:
- [x] insert: Inserts a word into the trie.
- [x] print: Visually show the content of the trie on the console. 
- [x] find: Searches for a stored string.
- [x] remove: Deletes a word from the trie.
- [x] complete: Completes a given prefix.

## Completions API
Most notable and useful feature of radix trie is efficient completion.

```
Completions for 'gal': galactic, galaxy, 
Completions for 'gr': graviton, gravity, 
Completions for 'qua': quark, quantum, quasar, 
Completions for 'as': astro, astronomy, asteroid, 
Completions for 'cos': cosmic, cosmos, 
Completions for 'pla': planet, plasma, 
Completions for 'ph': photon, 
Completions for 'ne': nebula, neutron, 
```

## Planned methods
- [ ] json/txt constructor: Create a trie from a list of words in plain TXT format or a JSON specification.
- [ ] to\_json: Export all words from the trie to a JSON.
- [ ] to\_txt: Export all words from the trie to a list of words in TXT.

## Planned features
- [ ] constructors: To fully support moves, copies and swaps.
- [ ] smart pointers: Implementation via smart pointers, making radix trie memory safe.
- [ ] errors and logs: Proper Go-style error handling and logging. 
- [ ] unit tests: Proper unit tests using ctest suite. 
- [ ] debug: Memory and performance debugging. 
- [ ] metadata: Support templated metadata for each node and corresponding get() method to retrieve it.
- [ ] iterator: Conveniently iterates the trie via a for-loop using a certain policy, for example bfs or dfs.
- [ ] overloads (==, +, -): Tries comparison, merge or diff.
- [ ] various optimizations:
    1. No-deletion radix trie.
    2. Ordering.
    3. Thread-safeness.

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
