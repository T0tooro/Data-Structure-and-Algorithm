#pragma once

/**
 * @class intBst
 * @brief Non-templated Binary Search Tree storing unique integers.
 *
 * Invariant: for every node, all left-subtree keys are smaller and all
 * right-subtree keys are larger. All tree operations are recursive.
 */
class intBst
{
public:
    intBst();                          ///< Builds an empty tree.
    ~intBst();                         ///< Frees every node (no leaks).

    intBst(const intBst& other);               ///< Deep-copies another tree.
    intBst& operator=(const intBst& other);    ///< Deep-copy assignment.

    void Insert(int value);            ///< Inserts one key; duplicates ignored.
    bool Search(int value) const;      ///< True if the key is present.

    void InOrder()   const;            ///< Prints keys ascending (left,node,right).
    void PreOrder()  const;            ///< Prints node before its subtrees.
    void PostOrder() const;            ///< Prints node after its subtrees.

    bool CheckRI() const;              ///< Debug: confirms the invariant holds.

private:
    /// One tree node: a key and links to two subtrees.
    struct Node
    {
        int   data;
        Node* left;
        Node* right;
    };

    Node* m_root;                      ///< Top of the tree; nullptr when empty.

    // Private recursive helpers — the real work happens here.
    void Insert(Node*& node, int value);
    bool Search(const Node* node, int value) const;
    void InOrder(const Node* node)   const;
    void PreOrder(const Node* node)  const;
    void PostOrder(const Node* node) const;
    void DeleteTree(Node* node);                       // called by ~intBst()
    bool CheckRI(const Node* node, int lo, int hi) const;
    Node* Clone(const Node* node) const;       ///< recursively copies a subtree.
};

