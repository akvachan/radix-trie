#include "radix_trie.hpp"

int main() {
  eff_aut::Radix_Trie radix_trie;

  radix_trie.insert("appreciation");
  radix_trie.insert("appreciation");
  radix_trie.insert("casio");
  radix_trie.insert("interest");
  radix_trie.insert("cat");
  radix_trie.insert("superb");
  radix_trie.insert("interesting");
  radix_trie.insert("hello");
  radix_trie.insert("super");
  radix_trie.insert("apple");
  radix_trie.insert("application");
  radix_trie.insert("apple");
  radix_trie.insert("car");
  radix_trie.insert("banana");
  radix_trie.insert("application");
  radix_trie.insert("applied");
  radix_trie.insert("cartoon");
  radix_trie.insert("Bratislava");
  radix_trie.insert("-");
  radix_trie.insert("applied");
  radix_trie.insert("cartoon");
  radix_trie.insert("supercalifragilisticexpialidocious");
  radix_trie.insert("superman");
  radix_trie.insert("apple");
  radix_trie.insert("Berlin");
  radix_trie.insert(" ");
  radix_trie.insert("worldview");
  radix_trie.insert("intervention");
  radix_trie.insert("absolution");
  radix_trie.insert("applying");
  radix_trie.insert("world");
  radix_trie.insert("absolutismus");
  radix_trie.insert("abs");
  radix_trie.insert("");
  radix_trie.insert("app");
  radix_trie.insert("interested");
  radix_trie.insert("moon");
  radix_trie.insert("band");
  radix_trie.insert(" ");
  radix_trie.insert("absolutely");
  radix_trie.insert("caterpillar");
  radix_trie.insert("helloworld");
  radix_trie.insert("bat");
  radix_trie.insert(" ");
  radix_trie.insert("");

  radix_trie.tree();
}
