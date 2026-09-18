#ifndef BINARY_TREE_H
#define BINARY_TREE_H

template <class keyType, class dataType>
class binaryTree {
public:
    binaryTree();
    ~binaryTree();
    binaryTree(const binaryTree&) = delete;
    binaryTree& operator=(const binaryTree&) = delete;

    bool insert(const keyType&, const dataType&);
    bool empty() const;
    // search returns a default-constructed value if a key is missing;
    // prefer retrieve() when absence must be distinguished from valid data.
    dataType search(const keyType&) const;
    bool retrieve(const keyType&, dataType&) const;
    void traverse() const;  // in-order, written to stdout
    void preorder() const;  // iterative, written to stdout
    void levelorder() const; // iterative, written to stdout
    void graph() const;
    void remove(const keyType&);

private:
    struct treeNode {
        keyType key;
        dataType data;
        treeNode* left;
        treeNode* right;
    };
    using NodePointer = treeNode*;
    NodePointer root;

    dataType search2(NodePointer, const keyType&) const;
    bool retrieve2(NodePointer, const keyType&, dataType&) const;
    bool insert2(NodePointer&, const keyType&, const dataType&);
    void traverse2(NodePointer) const;
    void graph2(int, NodePointer) const;
    void parentSearch(const keyType&, bool&, NodePointer&, NodePointer&) const;
};

#endif
