#include "intBst.h"
#include <iostream>
#include <climits>   // INT_MIN / INT_MAX — the starting bounds for CheckRI

// ---------- construction / destruction ----------

intBst::intBst() : m_root(nullptr)
{
}

intBst::~intBst()
{
    DeleteTree(m_root);          // frees every node when the tree dies
}

// ---------- Insert ----------

void intBst::Insert(int value)   // public: the client's only entry point
{
    Insert(m_root, value);       // start the recursion at the root
}

void intBst::Insert(Node*& node, int value)
{
    if (node == nullptr)                                 // empty spot found
        node = new Node{value, nullptr, nullptr};        // create leaf here
    else if (value < node->data)
        Insert(node->left, value);                       // walk left
    else if (value > node->data)
        Insert(node->right, value);                      // walk right
    // value == node->data -> duplicate, ignored (keys stay unique)
}

// ---------- Search ----------

bool intBst::Search(int value) const
{
    return Search(m_root, value);
}

bool intBst::Search(const Node* node, int value) const
{
    if (node == nullptr)
        return false;          // ran off the tree
    if (value == node->data)
        return true;           // found it
    if (value < node->data)
        return Search(node->left,  value);
    return Search(node->right, value);
}

// ---------- Traversals ----------

void intBst::InOrder() const
{
    InOrder(m_root);
    std::cout << '\n';
}
void intBst::PreOrder() const
{
    PreOrder(m_root);
    std::cout << '\n';
}
void intBst::PostOrder() const
{
    PostOrder(m_root);
    std::cout << '\n';
}

void intBst::InOrder(const Node* node) const
{
    if (node == nullptr)
        return;
    InOrder(node->left);                 // left
    std::cout << node->data << ' ';      // node
    InOrder(node->right);                // right
}

void intBst::PreOrder(const Node* node) const
{
    if (node == nullptr)
        return;
    std::cout << node->data << ' ';      // node
    PreOrder(node->left);                // left
    PreOrder(node->right);               // right
}

void intBst::PostOrder(const Node* node) const
{
    if (node == nullptr)
        return;
    PostOrder(node->left);               // left
    PostOrder(node->right);              // right
    std::cout << node->data << ' ';      // node
}

// ---------- DeleteTree (memory cleanup) ----------

void intBst::DeleteTree(Node* node)
{
    if (node == nullptr)
        return;
    DeleteTree(node->left);              // free left subtree
    DeleteTree(node->right);             // free right subtree
    delete node;                         // free this node last
}

// ---------- CheckRI (debug: verify the invariant) ----------

bool intBst::CheckRI() const
{
    return CheckRI(m_root, INT_MIN, INT_MAX);
}

bool intBst::CheckRI(const Node* node, int lo, int hi) const
{
    if (node == nullptr)
        return true;                    // empty is always valid
    if (node->data <= lo || node->data >= hi)
        return false;   // outside allowed window
    return CheckRI(node->left, lo, node->data) &&    // left window shrinks up
           CheckRI(node->right, node->data, hi);              // right window shrinks down
}

// ---------- Please survive the copy ----------
intBst::intBst(const intBst& other) : m_root(Clone(other.m_root))
{
}

intBst& intBst::operator=(const intBst& other)
{
    if (this != &other)                 // guard against self-assignment
    {
        DeleteTree(m_root);             // free our own nodes first
        m_root = Clone(other.m_root);   // then deep-copy theirs
    }
    return *this;
}

intBst::Node* intBst::Clone(const Node* node) const
{
    if (node == nullptr) return nullptr;
    Node* copy = new Node{node->data, nullptr, nullptr};
    copy->left  = Clone(node->left);    // copy the left subtree
    copy->right = Clone(node->right);   // copy the right subtree
    return copy;
}
