#pragma once
#include <iostream>



	/**
	 * @class Bst
	 * @brief  Templated Binary Search Tree storing unique keys of type T
	 *
	 * All tree operations are implemented recursively.
	 * Duplicate keys are ignored, so every key held in the tree is unique.
	 *
	 *
	 * Type T must supply operator< so that keys can be ordered.\n
	 * Type T must supply operator<< so that the traversals can print each key.\n
	 * The tree manages its own memory: the destructor frees every node, and
	 * copying a tree performs a deep copy.\n
	 *
	 * @author Jun Rong
	 * @version 01
	 * @date 08/07/2026 Jun Rong, Started
	 *
	 *
	 * @todo Compare iterative traversals against the recursive ones for timing.
	 */

template <class T>
class Bst
{
public:

		/**
		 * @brief  Builds an empty tree
		 *
		 * The root is set to nullptr, so the tree initially holds no nodes.
		 *
		 * @return void
		 */
	Bst();

		/**
		 * @brief  Frees every node in the tree
		 *
		 * Calls the private DeleteTree helper on the root, which recursively frees
		 * both subtrees before freeing each node, so no memory is leaked.
		 *
		 * @return void
		 */
	~Bst();

		/**
		 * @brief  Deep-copies another tree
		 *
		 * Every node of the source tree is duplicated, so this tree owns its own
		 * nodes and can be destroyed independently of the original.
		 *
		 * @param  other - the tree to copy from
		 * @return void
		 */
	Bst(const Bst<T>& other);

		/**
		 * @brief  Deep-copy assignment
		 *
		 * Frees this tree's existing nodes, then deep-copies the source tree.
		 *
		 * @param  other - the tree to copy from
		 * @return Bst<T>& - reference to this tree
		 * @pre self-assignment is guarded against
		 */
	Bst<T>& operator=(const Bst<T>& other);

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
		 * @brief  Prints every key in ascending order
		 *
		 * Visits the left subtree, then the node, then the right subtree.
		 *
		 * @return void
		 */
	void InOrder() const;

		/**
		 * @brief  Prints each node before its subtrees
		 *
		 * Visits the node, then the left subtree, then the right subtree.
		 *
		 * @return void
		 */
	void PreOrder() const;

		/**
		 * @brief  Prints each node after its subtrees
		 *
		 * Visits the left subtree, then the right subtree, then the node.
		 *
		 * @return void
		 */
	void PostOrder() const;

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
		 * @brief  Recursive helper printing the subtree in ascending order
		 *
		 * @param  node - the subtree to print, may be nullptr
		 * @return void
		 */
	void InOrder(const Node* node) const;

		/**
		 * @brief  Recursive helper printing each node before its subtrees
		 *
		 * @param  node - the subtree to print, may be nullptr
		 * @return void
		 */
	void PreOrder(const Node* node) const;

		/**
		 * @brief  Recursive helper printing each node after its subtrees
		 *
		 * @param  node - the subtree to print, may be nullptr
		 * @return void
		 */
	void PostOrder(const Node* node) const;

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
    return true;                                    // neither -> equal -> found
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
