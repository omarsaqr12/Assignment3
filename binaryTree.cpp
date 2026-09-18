// Template implementation; include this file after BinaryTree.h in callers.
#include "BinaryTree.h"

#include <iomanip>
#include <iostream>
#include <queue>
#include <vector>

template <class K, class D>
binaryTree<K, D>::binaryTree() : root(nullptr) {}

template <class K, class D>
binaryTree<K, D>::~binaryTree() {
    std::vector<NodePointer> pending;
    if (root) pending.push_back(root);
    while (!pending.empty()) {
        NodePointer node = pending.back();
        pending.pop_back();
        if (node->left) pending.push_back(node->left);
        if (node->right) pending.push_back(node->right);
        delete node;
    }
}

template <class K, class D>
bool binaryTree<K, D>::empty() const { return root == nullptr; }

template <class K, class D>
bool binaryTree<K, D>::insert(const K& key, const D& value) {
    return insert2(root, key, value);
}

template <class K, class D>
bool binaryTree<K, D>::insert2(NodePointer& node, const K& key, const D& value) {
    if (!node) {
        node = new treeNode{key, value, nullptr, nullptr};
        return true;
    }
    if (key == node->key) return false;
    if (key < node->key) return insert2(node->left, key, value);
    return insert2(node->right, key, value);
}

template <class K, class D>
D binaryTree<K, D>::search(const K& key) const { return search2(root, key); }

template <class K, class D>
D binaryTree<K, D>::search2(NodePointer node, const K& key) const {
    D result{};
    retrieve2(node, key, result);
    return result;
}

template <class K, class D>
bool binaryTree<K, D>::retrieve(const K& key, D& result) const {
    return retrieve2(root, key, result);
}

template <class K, class D>
bool binaryTree<K, D>::retrieve2(NodePointer node, const K& key, D& result) const {
    while (node) {
        if (key == node->key) {
            result = node->data;
            return true;
        }
        node = key < node->key ? node->left : node->right;
    }
    return false;
}

template <class K, class D>
void binaryTree<K, D>::traverse() const { traverse2(root); }

template <class K, class D>
void binaryTree<K, D>::traverse2(NodePointer node) const {
    if (!node) return;
    traverse2(node->left);
    std::cout << node->key << ' ' << node->data << '\n';
    traverse2(node->right);
}

template <class K, class D>
void binaryTree<K, D>::preorder() const {
    std::vector<NodePointer> pending;
    if (root) pending.push_back(root);
    while (!pending.empty()) {
        NodePointer node = pending.back();
        pending.pop_back();
        std::cout << node->key << ' ' << node->data << '\n';
        if (node->right) pending.push_back(node->right);
        if (node->left) pending.push_back(node->left);
    }
}

template <class K, class D>
void binaryTree<K, D>::levelorder() const {
    std::queue<NodePointer> pending;
    if (root) pending.push(root);
    while (!pending.empty()) {
        NodePointer node = pending.front();
        pending.pop();
        std::cout << node->key << ' ' << node->data << '\n';
        if (node->left) pending.push(node->left);
        if (node->right) pending.push(node->right);
    }
}

template <class K, class D>
void binaryTree<K, D>::graph() const { graph2(0, root); }

template <class K, class D>
void binaryTree<K, D>::graph2(int indent, NodePointer node) const {
    if (!node) return;
    graph2(indent + 4, node->right);
    std::cout << std::setw(indent) << " " << node->key << '\n';
    graph2(indent + 4, node->left);
}

template <class K, class D>
void binaryTree<K, D>::remove(const K& key) {
    NodePointer* link = &root;
    while (*link && (*link)->key != key)
        link = key < (*link)->key ? &(*link)->left : &(*link)->right;
    if (!*link) return;
    NodePointer node = *link;
    if (node->left && node->right) {
        NodePointer* successor = &node->right;
        while ((*successor)->left) successor = &(*successor)->left;
        node->key = (*successor)->key;
        node->data = (*successor)->data;
        link = successor;
        node = *link;
    }
    *link = node->left ? node->left : node->right;
    delete node;
}

template <class K, class D>
void binaryTree<K, D>::parentSearch(const K& key, bool& found,
                                     NodePointer& located, NodePointer& parent) const {
    parent = nullptr;
    located = root;
    while (located && located->key != key) {
        parent = located;
        located = key < located->key ? located->left : located->right;
    }
    found = located != nullptr;
}
