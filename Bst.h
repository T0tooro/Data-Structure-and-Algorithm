#pragma once
#include <iostream>   // traversals print with std::cout

/**
 * @class Bst
 * @brief Templated Binary Search Tree storing unique keys of type T.
 *
 * Invariant: for every node, all left-subtree keys are smaller and all
 * right-subtree keys are larger. T must provide operator< (and operator
 * for the traversals to print). All tree operations are recursive.
 */
template <class T>
class Bst
{
public:
    Bst();                                  ///< Builds an empty tree.
    ~Bst();                                 ///< Frees every node (no leaks).
    Bst(const Bst<T>& other);               ///< Deep-copies another tree.
    Bst<T>& operator=(const Bst<T>& other); ///< Deep-copy assignment.

    void Insert(const T& value);            ///< Inserts one key; duplicates ignored.
    bool Search(const T& value) const;      ///< True if the key is present.

    void InOrder()   const;                 ///< Prints keys ascending.
    void PreOrder()  const;                 ///< Prints node before its subtrees.
    void PostOrder() const;                 ///< Prints node after its subtrees.

    bool CheckRI() const;                   ///< Debug: confirms the invariant holds.

private:
    /// One tree node: a key and links to two subtrees.
    struct Node
    {
        T     data;
        Node* left;
        Node* right;
    };

    Node* m_root;                           ///< Top of the tree; nullptr when empty.

    void Insert(Node*& node, const T& value);
    bool Search(const Node* node, const T& value) const;
    void InOrder(const Node* node)   const;
    void PreOrder(const Node* node)  const;
    void PostOrder(const Node* node) const;
    void DeleteTree(Node* node);                          // called by ~Bst()
    Node* Clone(const Node* node) const;
    bool CheckRI(const Node* node, const T* lo, const T* hi) const;
};

// ---------- implementation (in the header — templates require it) ----------

template <class T>
Bst<T>::Bst() : m_root(nullptr) {}

template <class T>
Bst<T>::~Bst() { DeleteTree(m_root); }

template <class T>
Bst<T>::Bst(const Bst<T>& other) : m_root(Clone(other.m_root)) {}

template <class T>
Bst<T>& Bst<T>::operator=(const Bst<T>& other)
{
    if (this != &other)                 // guard against self-assignment
    {
        DeleteTree(m_root);             // free our own nodes first
        m_root = Clone(other.m_root);   // then deep-copy theirs
    }
    return *this;
}

template <class T>
void Bst<T>::Insert(const T& value) { Insert(m_root, value); }

template <class T>
void Bst<T>::Insert(Node*& node, const T& value)
{
    if (node == nullptr)
        node = new Node{value, nullptr, nullptr};   // empty spot -> create leaf
    else if (value < node->data)
        Insert(node->left, value);                  // smaller -> go left
    else if (node->data < value)
        Insert(node->right, value);                 // larger  -> go right
    // neither smaller nor larger -> duplicate, ignored
}

template <class T>
bool Bst<T>::Search(const T& value) const { return Search(m_root, value); }

template <class T>
bool Bst<T>::Search(const Node* node, const T& value) const
{
    if (node == nullptr)     return false;          // ran off the tree
    if (value < node->data)  return Search(node->left,  value);
    if (node->data < value)  return Search(node->right, value);
    return true;                                     // neither -> equal -> found
}

template <class T>
void Bst<T>::InOrder()   const { InOrder(m_root);   std::cout << '\n'; }
template <class T>
void Bst<T>::PreOrder()  const { PreOrder(m_root);  std::cout << '\n'; }
template <class T>
void Bst<T>::PostOrder() const { PostOrder(m_root); std::cout << '\n'; }

template <class T>
void Bst<T>::InOrder(const Node* node) const
{
    if (node == nullptr) return;
    InOrder(node->left);
    std::cout << node->data << ' ';
    InOrder(node->right);
}

template <class T>
void Bst<T>::PreOrder(const Node* node) const
{
    if (node == nullptr) return;
    std::cout << node->data << ' ';
    PreOrder(node->left);
    PreOrder(node->right);
}

template <class T>
void Bst<T>::PostOrder(const Node* node) const
{
    if (node == nullptr) return;
    PostOrder(node->left);
    PostOrder(node->right);
    std::cout << node->data << ' ';
}

template <class T>
void Bst<T>::DeleteTree(Node* node)
{
    if (node == nullptr) return;
    DeleteTree(node->left);
    DeleteTree(node->right);
    delete node;
}

template <class T>
typename Bst<T>::Node* Bst<T>::Clone(const Node* node) const
{
    if (node == nullptr) return nullptr;
    Node* copy = new Node{node->data, nullptr, nullptr};
    copy->left  = Clone(node->left);
    copy->right = Clone(node->right);
    return copy;
}

template <class T>
bool Bst<T>::CheckRI() const { return CheckRI(m_root, nullptr, nullptr); }

template <class T>
bool Bst<T>::CheckRI(const Node* node, const T* lo, const T* hi) const
{
    if (node == nullptr) return true;                       // empty is valid
    if (lo != nullptr && !(*lo < node->data)) return false; // must be > lower bound
    if (hi != nullptr && !(node->data < *hi)) return false; // must be < upper bound
    return CheckRI(node->left,  lo, &node->data) &&
           CheckRI(node->right, &node->data, hi);
}
