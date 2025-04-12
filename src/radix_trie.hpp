#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#define EFF_AUT_TEST_MODE

#ifdef EFF_AUT_TEST_MODE
#include <algorithm>
#include <set>
#include <vector>
#endif

namespace eff_aut {

struct Radix_Node {
  bool is_word;
  std::string val;
  std::unordered_map<char, Radix_Node *> table;

  Radix_Node() = default;
  Radix_Node(std::string val) : is_word(true), val(val) {}
  Radix_Node(bool is_word, std::string val) : is_word(is_word), val(val) {}

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

  void insert(std::string word) {

#ifdef EFF_AUT_TEST_MODE
    _words.insert(word);
#endif

    Radix_Node *curr_node = _root;
    Radix_Node *prev_node = _root;

    size_t word_size = word.size();

    size_t word_idx = 0;
    while (word_idx < word_size) {

      if (curr_node->table.contains(word[word_idx])) {
        prev_node = curr_node;
        curr_node = curr_node->table[word[word_idx]];

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
      }

      else {
        curr_node->table[word[word_idx]] =
            new Radix_Node{word.substr(word_idx, word_size)};

        return;
      }
    }

    if (word_idx == word_size)
      curr_node->is_word = true;
  }

#ifdef EFF_AUT_TEST_MODE
  void print() {
    std::vector<std::string> res;
    _print(_root, "", res);

    std::sort(res.begin(), res.end());
    std::vector<std::string> vec_words =
        std::vector(_words.begin(), _words.end());

    if (res == vec_words)
      std::cout << "Test passed." << std::endl;
    else
      std::cout << std::format("\nTest failed. \n\n Mismatch result \n\n {} "
                               "\n\n vs correct is \n\n {}",
                               res, vec_words)
                << std::endl;
  }
#else
  void print() { _print(_root, ""); }
#endif

private:
  Radix_Node *_root;

#ifdef EFF_AUT_TEST_MODE
  std::set<std::string> _words;

  void _print(Radix_Node *curr_node, const std::string &base,
              std::vector<std::string> &res) {

    if (curr_node->is_word) {
      res.push_back(base);
    }

    if (curr_node->table.empty()) {
      return;
    }

    for (const auto &[_, next_node] : curr_node->table) {
      std::string new_base = base + next_node->val;
      _print(next_node, new_base, res);
    }
  }
#else
  void _print(Radix_Node *curr_node, const std::string &base) {

    if (curr_node->is_word) {
      std::cout << base << '\n';
    }

    if (curr_node->table.empty()) {
      return;
    }

    for (const auto &[_, next_node] : curr_node->table) {
      std::string new_base = base + next_node->val;
      _print(next_node, new_base);
    }
  }
#endif

  void _rebind(Radix_Node *common_node, Radix_Node *prev_node,
               Radix_Node *curr_node, size_t curr_node_idx) {
    common_node->table[curr_node->val[curr_node_idx]] = curr_node;
    prev_node->table[curr_node->val[0]] = common_node;
    curr_node->val =
        curr_node->val.substr(curr_node_idx, curr_node->val.size());
  }
};

} // namespace eff_aut
