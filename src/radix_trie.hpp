#pragma once

#include <format>
#include <iostream>
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

      char word_char = word[word_idx];
      if (!curr_node->table.contains(word_char)) {
        curr_node->table[word_char] =
            new Radix_Node{word.substr(word_idx, word_size)};
        return;
      }

      prev_node = curr_node;
      curr_node = curr_node->table[word_char];

      size_t curr_node_size = curr_node->val.size();
      size_t curr_idx = 0;
      while (curr_idx < curr_node_size and word_idx < word_size) {

        if (word[word_idx] != curr_node->val[curr_idx]) {
          Radix_Node *common_node =
              new Radix_Node{false, curr_node->val.substr(0, curr_idx)};
          common_node->table[word[word_idx]] =
              new Radix_Node{word.substr(word_idx, word_size)};
          _rebind(common_node, prev_node, curr_node, curr_idx);
          return;
        }

        word_idx++;
        curr_idx++;
      }

      if (curr_idx < curr_node_size and word_idx == word_size) {
        Radix_Node *common_node =
            new Radix_Node{curr_node->val.substr(0, curr_idx)};
        _rebind(common_node, prev_node, curr_node, curr_idx);
        return;
      }
    }

    if (word_idx == word_size)
      curr_node->is_word = true;
  }

  std::optional<const Radix_Node *>
  find_node(const std::string_view &val) const {
    Radix_Node *curr_node = _root;

    size_t word_idx = 0;
    while (word_idx < val.size()) {

      char ch = val[word_idx];
      if (!curr_node->table.contains(ch))
        return {};

      curr_node = curr_node->table[ch];
      std::string_view curr_val = curr_node->val;

      if (val.substr(word_idx, curr_val.size()) != curr_val)
        return {};

      word_idx += curr_val.size();
    }

    return curr_node;
  }

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
