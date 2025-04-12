#include "radix_trie.hpp"

int main() {
  eff_aut::Radix_Trie radix_trie;

  radix_trie.insert("appreciation");
  radix_trie.insert("app");
  radix_trie.insert("application");
  radix_trie.insert("interest");
  radix_trie.insert("cat");
  radix_trie.insert("car");
  radix_trie.insert("superb");
  radix_trie.insert("interesting");
  radix_trie.insert("hello");
  radix_trie.insert("super");
  radix_trie.insert("card");
  radix_trie.insert("interestingandmalicious");
  radix_trie.insert("cardboard");
  radix_trie.insert("applied");

  radix_trie.tree();
}
