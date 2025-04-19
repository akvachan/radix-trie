/**
 * @file        main.cpp
 * @brief       Examples of usage of radix_trie.
 *
 * @author      Arsenii Kvachan
 * @date        2025-04-19
 * @copyright   MIT License (see LICENSE file for details)
 */

#include "radix_trie.hpp"
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

void test_trie() {
  std::cout << "\n====================\n";
  std::cout << "Examples\n";
  std::cout << "====================\n";

  using namespace radix_trie;

  Radix_Trie trie;

  std::vector<std::string> words = {
      "galaxy",   "galactic", "gamma",     "gravity", "graviton",
      "nebula",   "neutron",  "nova",      "quantum", "quark",
      "quasar",   "photon",   "plasma",    "planet",  "pulsar",
      "asteroid", "astro",    "astronomy", "cosmic",  "cosmos"};

  std::vector<std::string> words_shuffled = words;
  std::shuffle(words_shuffled.begin(), words_shuffled.end(),
               std::mt19937{std::random_device{}()});

  for (const auto &w : words_shuffled) {
    trie.insert(w);
    std::cout << "Inserted: " << w << '\n';
  }

  std::cout << "\nTrie structure (markdown format):\n";
  trie.print_md();

  std::cout << "\nFinding exact and partial matches...\n";
  std::vector<std::string> queries = {
      "galaxy",  "galac", "gamma",   "gravi", "gravity", "gravit",    "nebu",
      "neutron", "quant", "quantum", "astro", "astron",  "astronomy", "plasma",
      "photon",  "quas",  "quasar",  "cos",   "cosmic",  "comet"};

  for (const auto &q : queries) {
    auto result_exact = trie.find_node(q, false);
    auto result_partial = trie.find_node(q, true);

    std::cout << std::format(
        "{:<10} | exact: {:<15} | partial: {}\n", q,
        result_exact
            ? ((*result_exact)->is_word ? "found (word)" : "prefix only")
            : "not found",
        result_partial ? (*result_partial)->val : "not found");
  }

  std::vector<std::string> to_complete = {"gal", "gr",  "qua", "as",
                                          "cos", "pla", "ph",  "ne"};

  std::cout << '\n';
  for (const auto &prefix : to_complete) {
    std::vector<std::string> out_vec;
    trie.complete(prefix, out_vec);
    std::cout << std::format("Completions for '{}': ", prefix);
    for (const auto &w : out_vec) {
      std::cout << prefix + w << ", ";
    }
    std::cout << '\n';
  }

  std::vector<std::string> to_remove = {"galactic", "gravity",   "quantum",
                                        "pulsar",   "astronomy", "cosmic"};

  std::cout << '\n';
  for (const auto &w : to_remove) {
    std::cout << "Removing: " << w << '\n';
    trie.remove(w);
  }

  std::cout << "\nTrie after deletions:\n";
  trie.print_md();

  std::cout << '\n';
  for (const auto &w : words) {
    auto result = trie.find_node(w);
    std::cout << std::format(
        "{:<10}: {}\n", w,
        result ? ((*result)->is_word ? "exists" : "prefix only") : "gone");
  }

}

int main() {
  test_trie();

  return 0;
}
