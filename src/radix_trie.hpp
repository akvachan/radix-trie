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
   *
   * @param val   The string segment this node represents.
   */
  Radix_Node(std::string val) : val(val), is_word(true) {}

  /**
   * @brief Constructs a node with a given word flag and value.
   *
   * @param is_word   Whether this node marks the end of a word.
   * @param val       The string segment this node represents.
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
   * Space complexity:  O(1).
   * Time complexity:   O(n), n is the length of the word.
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

        // Maybe can be replaced via starts_with in the future
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

      // Can we merge it with the while loop above?
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
   * If allow_partial is enabled the last node is returned which prefix
   * partially matches the query.
   *
   * Space complexity:    O(1).
   * Time complexity:     O(n); n is the length of the val.
   *
   * @param val           The string to search for.
   * @param allow_partial Enable partial string search, so only if the prefix
   *                      of the node matches the string.
   * @return              Optional node pointer if the path exists, otherwise
   * std::nullopt.
   */
  std::optional<const Radix_Node *>
  find_node(const std::string &val, bool allow_partial = false) const {
    Radix_Node *curr_node = _root;
    size_t val_idx = 0;

    while (val_idx < val.size()) {
      char ch = val[val_idx];
      if (!curr_node->children.contains(ch))
        return {};

      curr_node = curr_node->children[ch];
      const std::string &node_val = curr_node->val;

      size_t match_len = 0;
      while (match_len < node_val.size() && val_idx + match_len < val.size() &&
             node_val[match_len] == val[val_idx + match_len]) {
        match_len++;
      }

      if (match_len < node_val.size()) {
        if (val_idx + match_len == val.size() && allow_partial) {
          return curr_node;
        }
        return {};
      }

      val_idx += match_len;
    }

    return curr_node;
  }

  /**
   * @brief Prints all complete words stored in the trie.
   *
   * Space complexity:  O(n); n is the height of the trie.
   * Time complexity:   O(n); n is the number of nodes.
   *
   */
  void print() const { _print(_root, ""); }

  /**
   * @brief Prints the structure of the trie in markdown (MD) format.
   * '𐄂' means that the node forms a word.
   *
   * Space complexity:  O(n); n is the tree height.
   * Time complexity:   O(n); n is the number of nodes.
   *
   */
  void print_md() const { _print_md(_root, "#"); }

  /**
   * @brief Recursively removes a node or node path that completes the word.
   * Returns true if node or node path was deleted successfully.
   * If the final node is a word, it will be deleted.
   * If the final node has children, it will only be deactivated via is_word.
   * If the final node has only one child left, they will be merged.
   *
   * Space complexity:  O(n); n is the size of the recursion stack.
   * Time complexity:   O(n); n is the length of the word.
   *
   * @param word        The string to be deleted.
   * @return            True if deletion or deactivation was successful, else
   *                    false.
   */
  bool remove(const std::string &word) { return _remove(_root, word, 0); }

  /**
   * @brief Finds all completions for a given prefix that form a word.
   * Pass in an output vector, that should be filled with completions.
   *
   * Space complexity:  O(n); n is the size of the out_vec.
   * Time complexity:   O(n*h); n is the size of the prefix, h is number of
   *                    nodes in the relevant subtree.
   *
   * @param prefix      A string that needs to be completed.
   */
  void complete(const std::string &prefix, std::vector<std::string> &out_vec) {
    Radix_Node *curr_node = _root;
    size_t prefix_idx = 0;

    std::string collected_prefix;

    while (prefix_idx < prefix.size()) {
      char ch = prefix[prefix_idx];
      if (!curr_node->children.contains(ch)) {
        return;
      }

      curr_node = curr_node->children[ch];
      std::string &node_val = curr_node->val;

      size_t match_len = 0;
      while (match_len < node_val.size() && prefix_idx < prefix.size() &&
             node_val[match_len] == prefix[prefix_idx]) {
        match_len++;
        prefix_idx++;
      }

      if (match_len < node_val.size()) {
        if (prefix_idx == prefix.size()) {
          std::string suffix = node_val.substr(match_len);
          _complete(curr_node, out_vec, suffix);
        }
        return;
      }

      collected_prefix += node_val;
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
   * Space complexity:  O(n); n is the tree height.
   * Time complexity:   O(n); n is the number of nodes.
   *
   * @param curr_node   Current node being visited.
   * @param base        Accumulated prefix string.
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
   * Space complexity:  O(n); n is the tree height.
   * Time complexity:   O(n); n is the number of nodes.
   *
   * @param curr_node   Current node being visited.
   * @param base        Indentation or visual prefix.
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
   * Space complexity:       O(1).
   * Time complexity:        O(1).
   *
   * @param common_node     New intermediate node representing the shared
   *                        prefix.
   * @param prev_node       Parent of the node being split.
   * @param curr_node       Node being split and moved under common_node.
   * @param curr_node_idx   Index at which to split curr_node's val.
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
   * Handles both full node deletions (when a node becomes unnecessary) and
   * logical deletions (by marking the `is_word` flag as false). The method also
   * compresses nodes when appropriate (i.e., merges a node with its single
   * child after deletion, if the resulting node no longer represents a word).
   *
   * Space complexity:  O(n); n is the size of the recursion stack.
   * Time complexity:   O(n); n is the length of the word.
   *
   * @param curr_node   Pointer to the current node being examined.
   * @param prev_node   Pointer to the parent of the current node.
   * @param word        The word to be removed from the Radix Tree.
   * @param word_idx    The current index in the word being matched against
   *                    nodes.
   * @return            True if node was removed or deactivated.
   */
  bool _remove(Radix_Node *node, const std::string &word, size_t index) {
    if (!node)
      return false;

    if (index == word.length()) {
      if (!node->is_word)
        return false;
      node->is_word = false;
    } else {
      char ch = word[index];
      if (!node->children.contains(ch))
        return false;

      Radix_Node *child = node->children[ch];
      if (!_remove(child, word, index + child->val.length()))
        return false;

      if (!child->is_word && child->children.empty()) {
        delete child;
        node->children.erase(ch);
      } else if (!child->is_word && child->children.size() == 1) {
        auto &grandchild_entry = *child->children.begin();
        Radix_Node *grandchild = grandchild_entry.second;
        child->val += grandchild->val;
        child->is_word = grandchild->is_word;
        child->children = std::move(grandchild->children);
        delete grandchild;
      }
    }

    return true;
  }

  /**
   * @brief Recursively collects all complete words under the given node.
   *
   * The method ensures that only valid, complete words are added to the
   * completion list, excluding internal path segments that do not terminate
   * words.
   *
   * Space complexity:  O(n); n is the size of the output vector.
   * Time complexity:   O(h); h is the number of nodes in the relevant subtree.
   *
   * @param curr_node   Pointer to the current node in the subtree.
   * @param out_vec     Reference to a vector where completed words will be
   *                    stored.
   * @param base        A string representing the prefix accumulated along the
   *                    current path.
   */
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

} // namespace radix_trie
