#pragma once

#include <iostream>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

namespace eff_aut {

struct Radix_Node {
  bool is_word;
  std::string_view val;
  std::unordered_map<char, Radix_Node *> table;

  Radix_Node() = default;
  Radix_Node(std::string_view val) : is_word(true), val(val) {}
  Radix_Node(bool is_word, std::string_view val) : is_word(is_word), val(val) {}

  ~Radix_Node() {
    for (auto &entry : table) {
      delete entry.second;
    }
  }
};

class Radix_Trie {
public:
  explicit Radix_Trie() : _root(new Radix_Node) {}
  ~Radix_Trie() { delete _root; }

  void insert(const std::string_view &word) {

    Radix_Node *curr_node = _root;
    Radix_Node *prev_node = _root;

    size_t word_size = word.size();
    size_t word_idx = 0;
    while (word_idx < word_size) {

      if (curr_node->table.contains(word[word_idx])) {
        prev_node = curr_node;
        curr_node = curr_node->table[word[word_idx]];

        // Below is potential optimization: instead of looking for mismatch
        // letter by letter Compare node content to the next val.size()
        // characters directly, if the next val.size() of the word are not equal
        // to the node content, proceed with linear comparison.
        // TODO: Measure performance gain.
        if (curr_node->val == word.substr(word_idx, curr_node->val.size())) {
          size_t curr_pref = word_idx + curr_node->val.size();
          if (curr_pref == word_size) {
            break;
          } else if (curr_pref < word_size) {
            word_idx += curr_node->val.size();
            continue;
          }
        }

        size_t curr_idx = 0;
        while (curr_idx < curr_node->val.size() and word_idx < word_size) {

          if (word[word_idx] != curr_node->val[curr_idx]) {
            Radix_Node *common_node =
                new Radix_Node{false, curr_node->val.substr(0, curr_idx)};
            common_node->table[word[word_idx]] =
                new Radix_Node{word.substr(word_idx, word_size)};
            _rebind(common_node, prev_node, curr_node, curr_idx);
            return;
          }

          curr_idx++;
          word_idx++;
        }

        if (curr_idx < curr_node->val.size() and word_idx == word_size) {
          Radix_Node *common_node =
              new Radix_Node{curr_node->val.substr(0, curr_idx)};
          _rebind(common_node, prev_node, curr_node, curr_idx);
          return;
        }

      } else {
        curr_node->table[word[word_idx]] =
            new Radix_Node{word.substr(word_idx, word_size)};
        return;
      }
    }

    if (word_idx == word_size)
      curr_node->is_word = true;
  }

  // bool check(std::string_view word) const {
  //   throw std::runtime_error(
  //       std::format("Search for the "
  //                   "word {} is not supported yet.",
  //                   word));
  //   return false;
  // }

  void print() const { _print(_root, ""); }
  void tree() const { _tree(_root, "#"); }

private:
  Radix_Node *_root;

  void _print(Radix_Node *curr_node, const std::string &base) const {

    if (curr_node->is_word)
      std::cout << base << '\n';

    if (curr_node->table.empty())
      return;

    for (const auto &entry : curr_node->table) {
      std::string new_base = std::format("{}{}", base, entry.second->val);
      _print(entry.second, new_base);
    }
  }

  void _tree(Radix_Node *curr_node, const std::string &base) const {

    std::cout << std::format("{} {}", base, curr_node->val) << std::endl;

    if (curr_node->table.empty())
      return;

    for (const auto &entry : curr_node->table) {
      std::string new_base = "#" + base;
      _tree(entry.second, new_base);
    }
  }

  inline void _rebind(Radix_Node *common_node, Radix_Node *prev_node,
                      Radix_Node *curr_node, size_t curr_node_idx) {
    common_node->table[curr_node->val[curr_node_idx]] = curr_node;
    prev_node->table[curr_node->val[0]] = common_node;
    curr_node->val =
        curr_node->val.substr(curr_node_idx, curr_node->val.size());
  }
};

} // namespace eff_aut
