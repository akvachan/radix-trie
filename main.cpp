#include "radix_trie.hpp"
#include <iostream>
#include <vector>

int main() {
  using namespace eff_aut;

  Radix_Trie trie;

  std::vector<std::string> words_to_insert = {"cat",  "car",  "carbon", "dog",
                                              "dove", "doom", "can"};

  std::vector<std::string> words_to_lookup = {"cat",  "car",  "carbon", "cab",
                                              "dog",  "dove", "do",     "doom",
                                              "cane", "can"};

  std::cout << "=== Inserting words ===\n";
  for (const auto &word : words_to_insert) {
    trie.insert(word);
    std::cout << "Inserted: " << word << '\n';
  }

  std::cout << "\n=== Testing print ===\n";
  trie.print();

  std::cout << "\n=== Testing print_md ===\n";
  trie.print_md();

  std::cout << "\n=== Testing find_node ===\n";
  for (const auto &word : words_to_lookup) {
    auto result = trie.find_node(word);

    if (result) {
      const Radix_Node *node = *result;
      std::cout << word << ": prefix exists";
      if (node->is_word) {
        std::cout << " (is a full word)";
      } else {
        std::cout << " (only a prefix)";
      }
    } else {
      std::cout << word << ": not found";
    }
    std::cout << '\n';
  }

  return 0;
}
