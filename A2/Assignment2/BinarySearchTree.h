#pragma once

/**
 * @class BinarySearchTree
 * @brief  Templated Binary Search Tree storing unique keys of type T
 *
 * All tree operations are implemented recursively.
 * Duplicate keys are ignored, so every key held in the tree is unique.
 *
 *
 * The tree does no processing of the data it stores. Traversals call back a
 * client-supplied function at each node, so the client decides what happens
 * to each key.\n
 * Type T must supply operator< so that keys can be ordered.\n
 * Type T must be copy-constructible so that keys can be stored and cloned.\n
 * The tree manages its own memory: the destructor frees every node, and
 * copying a tree performs a deep copy.\n
 *
 * @author Jun Rong
 * @version 02
 * @date 08/07/2026 Jun Rong, Started
 * @date 19/07/2026 Jun Rong, Print traversals replaced by function-pointer traversals
 *
 *
 * @todo Compare iterative traversals against the recursive ones for timing.
 */

template <class T>
class BinarySearchTree
{
public:

    /// Type of the client call-back invoked at each node during a traversal
    typedef void (*VisitFn)(const T&);

    /**
     * @brief  Builds an empty tree
     *
     * The root is set to nullptr, so the tree initially holds no nodes.
     *
     * @return void
     */
    BinarySearchTree();

    /**
     * @brief  Frees every node in the tree
     *
     * Calls the private DeleteTree helper on the root, which recursively frees
     * both subtrees before freeing each node, so no memory is leaked.
     *
     * @return void
     */
    ~BinarySearchTree();

    /**
     * @brief  Deep-copies another tree
     *
     * Every node of the source tree is duplicated, so this tree owns its own
     * nodes and can be destroyed independently of the original.
     *
     * @param  other - the tree to copy from
     * @return void
     */
    BinarySearchTree(const BinarySearchTree<T>& other);

    /**
     * @brief  Deep-copy assignment
     *
     * Frees this tree's existing nodes, then deep-copies the source tree.
     *
     * @param  other - the tree to copy from
     * @return BinarySearchTree<T>& - reference to this tree
     * @pre self-assignment is guarded against
     */
    BinarySearchTree<T>& operator=(const BinarySearchTree<T>& other);

    /**
     * @brief  Inserts one key into the tree
     *
     * Recursively walks left or right until an empty link is reached, then
     * creates the new node at that link. Duplicate keys are ignored.
     *
     * @param  value - the key to insert
     * @return void
     * @post the representation invariant still holds
     */
    void Insert(const T& value);

    /**
     * @brief  Reports whether a key is present in the tree
     *
     * Recursively walks left or right, comparing against the key at each node.
     *
     * @param  value - the key to look for
     * @return bool - true if the key is in the tree, false otherwise
     */
    bool Search(const T& value) const;

    /**
     * @brief  Visits every key in ascending order, calling back the client
     *
     * Visits the left subtree, then the node, then the right subtree. The
     * supplied function is called once per node and receives the key by
     * const reference, so a call-back cannot alter a key and break the
     * ordering invariant.
     *
     * @param  visit - the client call-back to invoke at each node
     * @return void
     * @pre visit is not nullptr
     */
    void InOrderTraversal(VisitFn visit) const;

    /**
     * @brief  Visits each node before its subtrees, calling back the client
     *
     * @param  visit - the client call-back to invoke at each node
     * @return void
     * @pre visit is not nullptr
     */
    void PreOrderTraversal(VisitFn visit) const;

    /**
     * @brief  Visits each node after its subtrees, calling back the client
     *
     * @param  visit - the client call-back to invoke at each node
     * @return void
     * @pre visit is not nullptr
     */
    void PostOrderTraversal(VisitFn visit) const;

    /**
     * @brief  Debug method confirming the representation invariant holds
     *
     * Checks that at every node all left-subtree keys are smaller and all
     * right-subtree keys are larger, and that no key appears more than once.
     *
     * @return bool - true if the invariant holds, false if it is broken
     */
    bool CheckRI() const;

private:

    /// One tree node: a key and links to its two subtrees
    struct Node
    {
        /// The key stored at this node
        T     data;
        /// Link to the subtree of smaller keys, nullptr if there is none
        Node* left;
        /// Link to the subtree of larger keys, nullptr if there is none
        Node* right;
    };

    /// Top of the tree, nullptr when the tree is empty
    Node* m_root;

    /**
     * @brief  Recursive helper inserting a key at or below the given link
     *
     * The link is passed by reference so that the new node can be attached
     * directly to the parent's left or right pointer.
     *
     * @param  node - reference to the link being examined, may be nullptr
     * @param  value - the key to insert
     * @return void
     */
    void Insert(Node*& node, const T& value);

    /**
     * @brief  Recursive helper searching the subtree at the given node
     *
     * @param  node - the subtree to search, may be nullptr
     * @param  value - the key to look for
     * @return bool - true if the key is found
     */
    bool Search(const Node* node, const T& value) const;

    /**
     * @brief  Recursive helper visiting the subtree in ascending order
     *
     * @param  visit - the client call-back to invoke at each node
     * @param  node - the subtree to visit, may be nullptr
     * @return void
     */
    void InOrderTraversal(VisitFn visit, const Node* node) const;

    /**
     * @brief  Recursive helper visiting each node before its subtrees
     *
     * @param  visit - the client call-back to invoke at each node
     * @param  node - the subtree to visit, may be nullptr
     * @return void
     */
    void PreOrderTraversal(VisitFn visit, const Node* node) const;

    /**
     * @brief  Recursive helper visiting each node after its subtrees
     *
     * @param  visit - the client call-back to invoke at each node
     * @param  node - the subtree to visit, may be nullptr
     * @return void
     */
    void PostOrderTraversal(VisitFn visit, const Node* node) const;

    /**
     * @brief  Recursively frees the subtree at the given node
     *
     * Both subtrees are freed before the node itself, so no link is followed
     * after the memory it points to has been released.
     *
     * @param  node - the subtree to free, may be nullptr
     * @return void
     */
    void DeleteTree(Node* node);

    /**
     * @brief  Recursively duplicates the subtree at the given node
     *
     * A new node is allocated for every node in the source subtree, so the
     * copy shares no memory with the original.
     *
     * @param  node - the subtree to copy, may be nullptr
     * @return Node* - pointer to the top of the newly copied subtree
     */
    Node* Clone(const Node* node) const;

    /**
     * @brief  Recursive helper checking the invariant within a bounded window
     *
     * Each key must lie strictly between the two bounds. A null bound means
     * that side is unbounded. The window narrows on the way down the tree.
     *
     * @param  node - the subtree to check, may be nullptr
     * @param  lo - exclusive lower bound, nullptr if unbounded
     * @param  hi - exclusive upper bound, nullptr if unbounded
     * @return bool - true if the subtree satisfies the invariant
     */
    bool CheckRI(const Node* node, const T* lo, const T* hi) const;
};

//---------------------------------------------------------------------------------
// Implementation. Templates require the definitions to be visible in the header.
//---------------------------------------------------------------------------------

template <class T>
BinarySearchTree<T>::BinarySearchTree() : m_root(nullptr) {}

template <class T>
BinarySearchTree<T>::~BinarySearchTree()
{
    DeleteTree(m_root);
}

template <class T>
BinarySearchTree<T>::BinarySearchTree(const BinarySearchTree<T>& other)
    : m_root(Clone(other.m_root)) {}

template <class T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(const BinarySearchTree<T>& other)
{
    if (this != &other)                 // guard against self-assignment
    {
        DeleteTree(m_root);             // free our own nodes first
        m_root = Clone(other.m_root);   // then deep-copy theirs
    }
    return *this;
}

template <class T>
void BinarySearchTree<T>::Insert(const T& value)
{
    Insert(m_root, value);
}

template <class T>
void BinarySearchTree<T>::Insert(Node*& node, const T& value)
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
bool BinarySearchTree<T>::Search(const T& value) const
{
    return Search(m_root, value);
}

template <class T>
bool BinarySearchTree<T>::Search(const Node* node, const T& value) const
{
    if (node == nullptr)
        return false;          // ran off the tree
    if (value < node->data)
        return Search(node->left,  value);
    if (node->data < value)
        return Search(node->right, value);
    return true;                                    // neither -> equal -> found
}

template <class T>
void BinarySearchTree<T>::InOrderTraversal(VisitFn visit) const
{
    InOrderTraversal(visit, m_root);
}

template <class T>
void BinarySearchTree<T>::PreOrderTraversal(VisitFn visit) const
{
    PreOrderTraversal(visit, m_root);
}

template <class T>
void BinarySearchTree<T>::PostOrderTraversal(VisitFn visit) const
{
    PostOrderTraversal(visit, m_root);
}

template <class T>
void BinarySearchTree<T>::InOrderTraversal(VisitFn visit, const Node* node) const
{
    if (node == nullptr)
        return;
    InOrderTraversal(visit, node->left);
    visit(node->data);                  // hand the key back to the client
    InOrderTraversal(visit, node->right);
}

template <class T>
void BinarySearchTree<T>::PreOrderTraversal(VisitFn visit, const Node* node) const
{
    if (node == nullptr)
        return;
    visit(node->data);
    PreOrderTraversal(visit, node->left);
    PreOrderTraversal(visit, node->right);
}

template <class T>
void BinarySearchTree<T>::PostOrderTraversal(VisitFn visit, const Node* node) const
{
    if (node == nullptr)
        return;
    PostOrderTraversal(visit, node->left);
    PostOrderTraversal(visit, node->right);
    visit(node->data);
}

template <class T>
void BinarySearchTree<T>::DeleteTree(Node* node)
{
    if (node == nullptr)
        return;
    DeleteTree(node->left);
    DeleteTree(node->right);
    delete node;
}

template <class T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::Clone(const Node* node) const
{
    if (node == nullptr)
        return nullptr;
    Node* copy = new Node{node->data, nullptr, nullptr};
    copy->left  = Clone(node->left);
    copy->right = Clone(node->right);
    return copy;
}

template <class T>
bool BinarySearchTree<T>::CheckRI() const
{
    return CheckRI(m_root, nullptr, nullptr);
}

template <class T>
bool BinarySearchTree<T>::CheckRI(const Node* node, const T* lo, const T* hi) const
{
    if (node == nullptr)
        return true;                       // empty is valid
    if (lo != nullptr && !(*lo < node->data))
        return false; // must be > lower bound
    if (hi != nullptr && !(node->data < *hi))
        return false; // must be < upper bound
    return CheckRI(node->left,  lo, &node->data) &&
           CheckRI(node->right, &node->data, hi);
}
