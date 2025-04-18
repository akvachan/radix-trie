/**
 * @file        radix_trie.hpp
 * @brief       Implementation of radix trie.
 *
 * @details     Contains radix node struct, as well as radix trie class.
 *
 *
 * @author      Arsenii Kvachan
 * @date        2025-04-16
 * @copyright   MIT License (see LICENSE file for details)
 */

#include "radix_trie.hpp"
#include <iostream>
#include <vector>

using namespace radix_trie;

void test_tree_1() {
  std::cout << "\n====================\n";
  std::cout << "TREE 1: Basic Insert/Find/Remove\n";
  std::cout << "====================\n";

  Radix_Trie trie;

  std::vector<std::string> words = {"apple", "ape", "apex", "bat", "bake"};
  for (const auto &w : words) {
    trie.insert(w);
    std::cout << "Inserted: " << w << '\n';
  }

  std::cout << "\nTrie structure:\n";
  trie.print_md();

  std::vector<std::string> to_find = {"apple", "apex", "bat", "bake", "app"};
  std::cout << "\nFinding nodes:\n";
  for (const auto &w : to_find) {
    auto result = trie.find_node(w);
    std::cout << w << ": "
              << (result
                      ? ((*result)->is_word ? "found (word)" : "found (prefix)")
                      : "not found")
              << '\n';
  }

  std::cout << "\nRemoving: apex, apple\n";
  trie.remove("apex");
  trie.remove("apple");

  std::cout << "\nAfter removal:\n";
  trie.print_md();
}

void test_tree_2() {
  std::cout << "\n====================\n";
  std::cout << "TREE 2: Prefix Sharing & Branching\n";
  std::cout << "====================\n";

  Radix_Trie trie;
  std::vector<std::string> words = {"car",    "cart", "carton", "carve",
                                    "carbon", "dog",  "dot",    "dodge"};

  for (const auto &w : words) {
    trie.insert(w);
    std::cout << "Inserted: " << w << '\n';
  }

  std::cout << "\nTrie structure:\n";
  trie.print_md();

  std::vector<std::string> to_find = {"car", "cart", "carton", "carbon",
                                      "cat", "do",   "dot",    "dodge"};
  std::cout << "\nFinding nodes:\n";
  for (const auto &w : to_find) {
    auto result = trie.find_node(w);
    std::cout << w << ": "
              << (result
                      ? ((*result)->is_word ? "found (word)" : "found (prefix)")
                      : "not found")
              << '\n';
  }

  std::cout << "\nRemoving: carton, cart, carbon\n";
  trie.remove("carton");
  trie.remove("cart");
  trie.remove("carbon");

  std::cout << "\nAfter removal:\n";
  trie.print_md();
}

void test_tree_3() {
  std::cout << "\n====================\n";
  std::cout << "TREE 3: Edge Cases & Cleanup\n";
  std::cout << "====================\n";

  Radix_Trie trie;

  std::vector<std::string> words = {"a", "ab", "abc", "abcd", "abcde"};
  for (const auto &w : words) {
    trie.insert(w);
    std::cout << "Inserted: " << w << '\n';
  }

  std::cout << "\nTrie structure:\n";
  trie.print_md();

  std::vector<std::string> to_find = {"a",    "ab",    "abc",
                                      "abcd", "abcde", "abcdef"};
  std::cout << "\nFinding nodes:\n";
  for (const auto &w : to_find) {
    auto result = trie.find_node(w);
    std::cout << w << ": "
              << (result
                      ? ((*result)->is_word ? "found (word)" : "found (prefix)")
                      : "not found")
              << '\n';
  }

  std::cout << "\nRemoving: abcde, abcd, abc\n";
  trie.remove("abcde");
  trie.remove("abcd");
  trie.remove("abc");

  std::cout << "\nAfter cleanup (should collapse nodes):\n";
  trie.print_md();
}

void test_tree_4() {
  std::cout << "\n====================\n";
  std::cout << "TREE 4: Completions \n";
  std::cout << "====================\n";

  Radix_Trie trie;

  std::vector<std::string> to_insert = {
      "go",         "python",     "java",    "c",       "rust",    "julia",
      "c++",        "d",          "haskell", "fortran", "prolog",  "c#",
      "javascript", "typescript", "sh",      "bash",    "adalang", "lua"};
  for (const auto &w : to_insert) {
    trie.insert(w);
    std::cout << "Inserted: " << w << '\n';
  }

  std::cout << "\nTrie structure:\n";
  trie.print_md();

  std::vector<std::string> to_complete = {"java", "p",   "c", "javascri",
                                          "lu",   "pro", "somethingelse"};

  for (const auto &w : to_complete) {
    std::vector<std::string> out_vec;
    trie.complete(w, out_vec);
    std::cout << "\nCompletions of " << w << ": ";
    for (const auto &_w : out_vec) {
      std::cout << _w << ", ";
    }
  }
}

int main() {
  test_tree_1();
  test_tree_2();
  test_tree_3();
  test_tree_4();

  return 0;
}
