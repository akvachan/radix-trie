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

#include <filesystem>
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
   * @param val       The string segment this node represents.
   * @param is_word   Whether this node marks the end of a word.
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
   * @brief Constructs Radix Trie from a JSON or TXT.
   */
  Radix_Trie(const std::filesystem::path &file_path) : _root(new Radix_Node) {
    std::cout << file_path << std::endl;
  }

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
   * @param word        The word to insert.
   */
  void insert(const std::string &word) {
    Radix_Node *curr = _root;
    Radix_Node *prev = _root;

    size_t w_size = word.size();
    size_t w_idx = 0;
    while (w_idx < w_size) {

      char c = word[w_idx];
      if (!curr->children.contains(c)) {
        curr->children[c] = new Radix_Node{word.substr(w_idx, w_size)};
        return;
      }

      prev = curr;
      curr = curr->children[c];

      size_t curr_size = curr->val.size();
      size_t curr_idx = 0;
      while (curr_idx < curr_size && w_idx < w_size) {

        if (word[w_idx] != curr->val[curr_idx]) {
          Radix_Node *common =
              new Radix_Node{curr->val.substr(0, curr_idx), false};
          common->children[word[w_idx]] =
              new Radix_Node{word.substr(w_idx, w_size)};
          _rebind(common, prev, curr, curr_idx);
          return;
        }

        w_idx++;
        curr_idx++;
      }

      if (curr_idx < curr_size && w_idx == w_size) {
        Radix_Node *common = new Radix_Node{curr->val.substr(0, curr_idx)};
        _rebind(common, prev, curr, curr_idx);
        return;
      }
    }

    if (w_idx == w_size)
      curr->is_word = true;
  }

  /**
   * @brief Finds the node corresponding to the given string.
   *
   * This returns a node if the full string exists as a path in the trie.
   * Check if the final node produces a valid word via is_word.
   * If allow_partial is enabled the last node is returned which prefix
   * partially matches the query.
   *
   * Space complexity:      O(1).
   * Time complexity:       O(n); n is the length of the val.
   *
   * @param val             The string to search for.
   * @param allow_partial   Enable partial string search, so only if the prefix
   *                        of the node matches the string. Default is false.
   * @return                Optional node pointer if the path exists, otherwise
   *                        std::nullopt.
   */
  std::optional<const Radix_Node *>
  find(const std::string &val, const bool allow_partial = false) const {
    Radix_Node *curr = _root;
    size_t val_idx = 0;

    while (val_idx < val.size()) {
      char c = val[val_idx];
      if (!curr->children.contains(c))
        return {};

      curr = curr->children[c];
      const std::string &curr_val = curr->val;

      size_t match_len = 0;
      while (match_len < curr_val.size() && val_idx + match_len < val.size() &&
             curr_val[match_len] == val[val_idx + match_len]) {
        match_len++;
      }

      if (match_len < curr_val.size()) {
        if (val_idx + match_len == val.size() && allow_partial) {
          return curr;
        }
        return {};
      }

      val_idx += match_len;
    }

    return curr;
  }

  /**
   * @brief Visualizes content of the trie, either by printing out each word or
   * the structure of the trie in markdown format.
   * When using markdown, '𐄂' annotates nodes that form a word.
   *
   * Space complexity:  O(n); n is the height of the trie.
   * Time complexity:   O(n); n is the number of nodes.
   *
   * @param format      Give "list" for a list of words, "md" for
   *                    markdown, "tree" for a tree-like representation.
   *                    Default is "md".
   */
  void print(const std::string &format = "md") const {
    if (format == "md")
      _print_md(_root, "#");
    else if (format == "list")
      _print_list(_root, "");
    else
      throw std::invalid_argument(
          std::format("Invalid print argument format=\"{}\". Valid arguments "
                      "are \"list\", \"md\", \"tree\".",
                      format));
  }

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
   * @param pref        A string that needs to be completed.
   * @param out_vec     A vector of strings that should be populated with
   *                    completions.
   */
  void complete(const std::string &pref,
                std::vector<std::string> &out_vec) const {
    Radix_Node *curr = _root;
    size_t pref_idx = 0;

    while (pref_idx < pref.size()) {
      char c = pref[pref_idx];
      if (!curr->children.contains(c)) {
        return;
      }

      curr = curr->children[c];
      std::string &curr_val = curr->val;

      size_t match_len = 0;
      while (match_len < curr_val.size() && pref_idx < pref.size() &&
             curr_val[match_len] == pref[pref_idx]) {
        match_len++;
        pref_idx++;
      }

      if (match_len < curr_val.size()) {
        if (pref_idx == pref.size()) {
          _complete(curr, out_vec, curr_val.substr(match_len));
        }
        return;
      }
    }

    _complete(curr, out_vec, "");
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
   * @param curr        Current node being visited.
   * @param base        Accumulated prefix string.
   */
  void _print_list(const Radix_Node *curr, const std::string &base) const {
    if (curr->is_word)
      std::cout << base << '\n';

    if (curr->children.empty())
      return;

    for (const auto &entry : curr->children) {
      std::string new_base = base + entry.second->val;
      _print_list(entry.second, new_base);
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
   * @param curr        Current node being visited.
   * @param base        Indentation or visual prefix.
   */
  void _print_md(const Radix_Node *curr, const std::string &base) const {
    if (curr->is_word)
      std::cout << std::format("{} {} 𐄂", base, curr->val) << std::endl;
    else
      std::cout << std::format("{} {}", base, curr->val) << std::endl;

    if (curr->children.empty())
      return;

    for (const auto &entry : curr->children) {
      std::string new_base = "#" + base;
      _print_md(entry.second, new_base);
    }
  }

  /**
   * @brief Helper to rebind a node during insertion when a prefix match splits.
   *
   * Space complexity:      O(1).
   * Time complexity:       O(1).
   *
   * @param common_node     New intermediate node representing the shared
   *                        prefix.
   * @param prev            Parent of the node being split.
   * @param curr            Node being split and moved under common_node.
   * @param curr            Index at which to split curr_node's val.
   */
  inline void _rebind(Radix_Node *common, Radix_Node *prev, Radix_Node *curr,
                      size_t curr_idx) {
    common->children[curr->val[curr_idx]] = curr;
    prev->children[curr->val[0]] = common;
    curr->val = curr->val.substr(curr_idx, curr->val.size());
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
   * @param curr        Pointer to the current node being examined.
   * @param word        The word to be removed from the Radix Tree.
   * @param word_idx    The current index in the word being matched against
   *                    nodes.
   * @return            True if node was removed or deactivated.
   */
  bool _remove(Radix_Node *curr, const std::string &word, size_t word_idx) {
    if (!curr)
      return false;

    if (word_idx == word.length()) {
      if (!curr->is_word)
        return false;
      curr->is_word = false;
    } else {
      char c = word[word_idx];
      if (!curr->children.contains(c))
        return false;

      Radix_Node *child = curr->children[c];
      if (!_remove(child, word, word_idx + child->val.length()))
        return false;

      if (!child->is_word && child->children.empty()) {
        delete child;
        curr->children.erase(c);
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
   * @param curr        Pointer to the current node in the subtree.
   * @param out_vec     Reference to a vector where completed words will be
   *                    stored.
   * @param base        A string representing the prefix accumulated along the
   *                    current path.
   */
  void _complete(const Radix_Node *curr, std::vector<std::string> &out_vec,
                 const std::string &base) const {
    if (curr->is_word && base != "")
      out_vec.push_back(base);

    if (curr->children.empty())
      return;

    for (const auto &entry : curr->children) {
      std::string new_base = base + entry.second->val;
      _complete(entry.second, out_vec, new_base);
    }
  }
};

} // namespace radix_trie
