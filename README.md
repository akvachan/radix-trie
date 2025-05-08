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

## DISCLAIMER
This implementation is for educational purposes only and is not intended for production environments.
