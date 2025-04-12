# Radix Trie implementation

## Description
Radix trie is a compressed prefix trie. 

## Available methods 
Current implementation is a one-header library with following methods:
- insert: Inserts a word into the trie.
- print: Print the trie if `EFF_AUT_TEST_MODE` is not defined, else tests with the values inserted in main.cpp.

## Planned methods
- delete: Deletes a word from the trie.
- complete: Completes a given prefix.
- check: Checks if a word exists.

## Planned implementations
- iterator: Conveniently iterates the trie via a for-loop.
- overloads (==, +, -): Tries comparison, merge or diff.
- constructors: To fully support moves, copies and swaps.
- smart pointers: Implementation via smart pointers
- templates: Templated implementation 

## Additional information
- I also plan to make a video and a table on runtime/space complexity comparison between the normal trie (check out algorithms.cpp repo) and radix trie.
