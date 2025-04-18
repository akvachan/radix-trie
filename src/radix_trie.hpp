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

#pragma once

#include <format>
#include <iostream>
#include <optional>
#include <unordered_map>
#include <vector>

namespace radix_trie {

/**
 * @brief Represents a node in the Radix Trie.
 */
struct Radix_Node {
  /**
   * @brief The string value held by this node.
   */
  std::string val;

  /**
   * @brief The child nodes, indexed by the next character.
   */
  std::unordered_map<char, Radix_Node *> children;

  /**
   * @brief Indicates whether this node represents the end of a valid word.
   */
  bool is_word;

  /**
   * @brief Default constructor.
   */
  Radix_Node() = default;

  /**
   * @brief Constructs a terminal node with a given value.
   * @param val The string segment this node represents.
   */
  Radix_Node(std::string val) : val(val), is_word(true) {}

  /**
   * @brief Constructs a node with a given word flag and value.
   * @param is_word Whether this node marks the end of a word.
   * @param val The string segment this node represents.
   */
  Radix_Node(std::string val, bool is_word) : val(val), is_word(is_word) {}

  /**
   * @brief Destructor. Frees all dynamically allocated child nodes.
   */
  ~Radix_Node() {
    for (auto &entry : children)
      delete entry.second;
  }
};

/**
 * @brief A Radix Trie (Compact Prefix Tree) implementation
 */
class Radix_Trie {
public:
  /**
   * @brief Constructs an empty Radix Trie.
   */
  explicit Radix_Trie() : _root(new Radix_Node) {}

  /**
   * @brief Destroys the trie and deallocates all nodes.
   */
  ~Radix_Trie() { delete _root; }

  /**
   * @brief Inserts a word into the trie.
   *
   * Space complexity: O(1).
   * Time complexity: O(n), n is the length of the word.
   *
   * @param word The word to insert.
   */
  void insert(const std::string &word) {
    Radix_Node *curr_node = _root;
    Radix_Node *prev_node = _root;

    size_t word_size = word.size();
    size_t word_idx = 0;
    while (word_idx < word_size) {

      char word_char = word[word_idx];
      if (!curr_node->children.contains(word_char)) {
        curr_node->children[word_char] =
            new Radix_Node{word.substr(word_idx, word_size)};
        return;
      }

      prev_node = curr_node;
      curr_node = curr_node->children[word_char];

      size_t curr_node_size = curr_node->val.size();
      size_t curr_idx = 0;
      while (curr_idx < curr_node_size && word_idx < word_size) {

        if (word[word_idx] != curr_node->val[curr_idx]) {
          Radix_Node *common_node =
              new Radix_Node{curr_node->val.substr(0, curr_idx), false};
          common_node->children[word[word_idx]] =
              new Radix_Node{word.substr(word_idx, word_size)};
          _rebind(common_node, prev_node, curr_node, curr_idx);
          return;
        }

        word_idx++;
        curr_idx++;
      }

      if (curr_idx < curr_node_size && word_idx == word_size) {
        Radix_Node *common_node =
            new Radix_Node{curr_node->val.substr(0, curr_idx)};
        _rebind(common_node, prev_node, curr_node, curr_idx);
        return;
      }
    }

    if (word_idx == word_size)
      curr_node->is_word = true;
  }

  /**
   * @brief Finds the node corresponding to the given string.
   *
   * This returns a node if the full string exists as a path in the trie.
   * Check if the final node produces a valid word via is_word.
   *
   * Space complexity: O(1).
   * Time complexity: O(n); n is the length of the val.
   *
   * @param val The string to search for.
   * @return Optional node pointer if the path exists, otherwise std::nullopt.
   */
  std::optional<const Radix_Node *> find_node(const std::string &val) const {
    Radix_Node *curr_node = _root;

    size_t word_idx = 0;
    while (word_idx < val.size()) {
      char ch = val[word_idx];
      if (!curr_node->children.contains(ch))
        return {};

      curr_node = curr_node->children[ch];
      std::string curr_val = curr_node->val;

      if (val.substr(word_idx, curr_val.size()) != curr_val)
        return {};

      word_idx += curr_val.size();
    }

    return curr_node;
  }

  /**
   * @brief Prints all complete words stored in the trie.
   *
   * Space complexity: O(n); n is the height of the trie.
   * Time complexity: O(n); n is the number of nodes.
   *
   */
  void print() const { _print(_root, ""); }

  /**
   * @brief Prints the structure of the trie in markdown (MD) format.
   * '𐄂' means that the node forms a word.
   *
   * Space complexity: O(n); n is the tree height.
   * Time complexity: O(n); n is the number of nodes.
   *
   */
  void print_md() const { _print_md(_root, "#"); }

  /**
   * @brief Recursively removes a node or node path that completes the word.
   *
   * Returns true if node or node path was deleted successfully.
   * If the final node is a word, it will be deleted.
   * If the final node has children, it will only be deactivated via is_word.
   * If the final node has only one child left, they will be merged.
   *
   * Space complexity: O(n); n is the size of the recursion stack.
   * Time complexity: O(n); n is the length of the word.
   *
   * @param word The string to be deleted.
   * @return True if deletion or deactivation was successful, else false.
   */
  bool remove(const std::string &word) {
    Radix_Node *curr_node = _root;
    bool is_removed = false;
    _remove(curr_node, curr_node, word, 0, is_removed);

    return is_removed;
  }

  /**
   * @brief Finds all completions for a given prefix that form a word.
   *
   * Space complexity: O(n); n is the size of the out_vec.
   * Time complexity: O(n*h); n is the size of the prefix, h is number of nodes
   * in the relevant subtree.
   *
   * @param prefix A string that needs to be completed.
   * @return out_vec An output vector that will be populated with completions.
   */
  void complete(const std::string &prefix, std::vector<std::string> &out_vec) {
    Radix_Node *curr_node = _root;

    size_t prefix_idx = 0;
    while (prefix_idx < prefix.size()) {

      char ch = prefix[prefix_idx];
      if (!curr_node->children.contains(ch)) {
        return;
      }

      curr_node = curr_node->children[ch];
      std::string curr_prefix = curr_node->val;

      if (prefix.substr(prefix_idx, curr_prefix.size()) != curr_prefix) {
        if (curr_prefix.starts_with(prefix.substr(prefix_idx, prefix.size()))) {
          out_vec.push_back(curr_prefix.substr(prefix_idx, curr_prefix.size()));
        }
        return;
      }

      prefix_idx += curr_prefix.size();
    }

    _complete(curr_node, out_vec, "");
  }

private:
  /**
   * @brief The root node of the trie.
   */
  Radix_Node *_root;

  /**
   * @brief Recursively prints all full words in the trie.
   *
   * Space complexity: O(n); n is the tree height.
   * Time complexity: O(n); n is the number of nodes.
   *
   * @param curr_node Current node being visited.
   * @param base Accumulated prefix string.
   */
  void _print(Radix_Node *curr_node, const std::string &base) const {
    if (curr_node->is_word)
      std::cout << base << '\n';

    if (curr_node->children.empty())
      return;

    for (const auto &entry : curr_node->children) {
      std::string new_base = base + entry.second->val;
      _print(entry.second, new_base);
    }
  }

  /**
   * @brief Recursively prints a visual tree structure of the trie in markdown
   * (MD) format.
   * '𐄂' means that the node forms a word.
   *
   * Space complexity: O(n); n is the tree height.
   * Time complexity: O(n); n is the number of nodes.
   *
   * @param curr_node Current node being visited.
   * @param base Indentation or visual prefix.
   */
  void _print_md(Radix_Node *curr_node, const std::string &base) const {

    if (curr_node->is_word)
      std::cout << std::format("{} {} 𐄂", base, curr_node->val) << std::endl;
    else
      std::cout << std::format("{} {}", base, curr_node->val) << std::endl;

    if (curr_node->children.empty())
      return;

    for (const auto &entry : curr_node->children) {
      std::string new_base = "#" + base;
      _print_md(entry.second, new_base);
    }
  }

  /**
   * @brief Helper to rebind a node during insertion when a prefix match splits.
   *
   * Space complexity: O(1).
   * Time complexity: O(1).
   *
   * @param common_node New intermediate node representing the shared prefix.
   * @param prev_node Parent of the node being split.
   * @param curr_node Node being split and moved under common_node.
   * @param curr_node_idx Index at which to split curr_node's val.
   */
  inline void _rebind(Radix_Node *common_node, Radix_Node *prev_node,
                      Radix_Node *curr_node, size_t curr_node_idx) {
    common_node->children[curr_node->val[curr_node_idx]] = curr_node;
    prev_node->children[curr_node->val[0]] = common_node;
    curr_node->val =
        curr_node->val.substr(curr_node_idx, curr_node->val.size());
  }

  /**
   * @brief Recursively removes a word from a Radix Tree.
   *
   * This helper method is used internally to traverse the Radix Tree and remove
   * a specific word, if it exists. It handles both full node deletions (when a
   * node becomes unnecessary) and logical deletions (by marking the `is_word`
   * flag as false). The method also compresses nodes when appropriate (i.e.,
   * merges a node with its single child after deletion, if the resulting node
   * no longer represents a word).
   *
   * Space complexity: O(n); n is the size of the recursion stack.
   * Time complexity: O(n); n is the length of the word.
   *
   * @param curr_node   Pointer to the current node being examined.
   * @param prev_node   Pointer to the parent of the current node.
   * @param word        The word to be removed from the Radix Tree.
   * @param word_idx    The current index in the word being matched against
   * nodes.
   * @param is_removed  Reference to a flag that indicates whether the word was
   * successfully removed.
   */
  void _remove(Radix_Node *curr_node, Radix_Node *prev_node,
               const std::string &word, size_t word_idx, bool &is_removed) {
    if (!curr_node)
      return;

    if (word_idx == word.size()) {
      curr_node->is_word = false;
      if (curr_node->children.empty()) {
        prev_node->children.erase(curr_node->val[0]);
        delete curr_node;
      }
      is_removed = true;
      return;
    }

    if (word_idx < word.size()) {
      if (!curr_node->children.contains(word[word_idx]))
        return;

      prev_node = curr_node;
      curr_node = curr_node->children[word[word_idx]];
      std::string curr_word = curr_node->val;

      if (word.substr(word_idx, curr_word.size()) != curr_word)
        return;

      word_idx += curr_word.size();
      _remove(curr_node, prev_node, word, word_idx, is_removed);
    }

    if (curr_node->children.size() == 1 && !curr_node->is_word) {
      auto &child_entry = *curr_node->children.begin();
      Radix_Node *child = child_entry.second;
      curr_node->val += child->val;
      curr_node->is_word = child->is_word;
      curr_node->children = std::move(child->children);
      delete child;
    }

    return;
  }

  void _complete(Radix_Node *curr_node, std::vector<std::string> &out_vec,
                 const std::string &base) {
    if (curr_node->is_word && base != "")
      out_vec.push_back(base);

    if (curr_node->children.empty())
      return;

    for (const auto &entry : curr_node->children) {
      std::string new_base = base + entry.second->val;
      _complete(entry.second, out_vec, new_base);
    }
  }
};

} // namespace eff_aut
