/**
 * @file testTree.cpp
 * @brief Driver for Lab 10 exercise d: function-pointer traversals of the template BST
 *
 * test1 - print call-back, proves the call-back mechanism works.
 * test2 - collector call-back, the pattern Assignment 2 uses: the tree hands
 *         each key back to client code, which stores it.
 * test3 - passes the tree by value, by const reference and by reference, and
 *         checks the data is correct on pass and on return.
 *
 * @author Jun Rong
 * @version 01
 * @date 19/07/2026 Jun Rong, Started
 */

#include "BinarySearchTree.h"
#include "Date.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

/// Shorter name for the tree, as suggested in the supplied sample driver
template <class T>
using Bst = BinarySearchTree<T>;

//---------------------------------------------------------------------------------
// Client-side call-backs. The tree does not know what these do.
//---------------------------------------------------------------------------------

/**
 * @brief  Print call-back matching Bst<Date>::VisitFn
 *
 * @param  d - the key handed back by the traversal
 * @return void
 */
void PrintDate(const Date& d)
{
    std::cout << d << ' ';
}

/**
 * @class DateCollector
 * @brief Client-side collector that keeps the keys handed back by a traversal
 *
 * The call-back must be a plain function pointer, so Collect is a static
 * member function and the storage it writes to is static as well.
 */
class DateCollector
{
public:

    /**
     * @brief  Call-back that stores one key
     *
     * @param  d - the key handed back by the traversal
     * @return void
     */
    static void Collect(const Date& d)
    {
        if (m_count < CAPACITY)
            m_store[m_count++] = d;
    }

    /**
     * @brief  Empties the collector so it can be reused
     *
     * @return void
     */
    static void Reset()
    {
        m_count = 0;
    }

    /**
     * @brief  Reports how many keys have been collected
     *
     * @return int - the number of stored keys
     */
    int Size() const
    {
        return m_count;
    }

    /**
     * @brief  Reads back one collected key
     *
     * @param  k - index of the key, 0 to Size() - 1
     * @return const Date& - the key at that index
     */
    const Date& operator[](int k) const
    {
        return m_store[k];
    }

private:

    /// Largest number of keys the collector can hold
    static const int CAPACITY = 1000;

    /// Number of keys collected so far
    static int  m_count;

    /// The collected keys, in the order the traversal handed them over
    static Date m_store[CAPACITY];
};

int  DateCollector::m_count = 0;
Date DateCollector::m_store[DateCollector::CAPACITY];

//---------------------------------------------------------------------------------
// Loading
//---------------------------------------------------------------------------------

/**
 * @brief  Builds a Date from a "d/m/yyyy" string
 *
 * @param  text - the date text, anything after the year is ignored
 * @return Date - the date it represents
 */
Date ParseDate(const std::string& text)
{
    std::stringstream ss(text);
    int day, month, year;
    char slash;
    ss >> day >> slash >> month >> slash >> year;
    return Date(day, month, year);
}

/**
 * @brief  Reads one date per line into the tree
 *
 * The tree is taken by reference so the caller's own tree is filled.
 *
 * @param  fileName - the text file to read
 * @param  tree - the tree to insert into
 * @return void
 */
void LoadDates(const std::string& fileName, Bst<Date>& tree)
{
    std::ifstream in(fileName);
    if (!in)
    {
        std::cout << "Could not open " << fileName << '\n';
        return;
    }

    std::string line;
    while (std::getline(in, line))
    {
        if (line.empty())
            continue;
        tree.Insert(ParseDate(line));
    }
}

//---------------------------------------------------------------------------------
// Routines used by the pass-by tests
//---------------------------------------------------------------------------------

/**
 * @brief  Takes the tree BY VALUE, so it works on its own deep copy
 *
 * A key is inserted into the copy to show the caller's tree is untouched.
 *
 * @param  tree - a copy of the caller's tree
 * @return void
 */
void ByValue(Bst<Date> tree)
{
    std::cout << "  by value, on entry   : ";
    tree.InOrderTraversal(PrintDate);
    tree.Insert(Date(31, 12, 2099));
    std::cout << "\n  by value, after insert: ";
    tree.InOrderTraversal(PrintDate);
    std::cout << "\n  invariant on the copy : " << (tree.CheckRI() ? "holds" : "BROKEN") << '\n';
}

/**
 * @brief  Takes the tree BY CONST REFERENCE, so it can only read
 *
 * @param  tree - the caller's tree, read-only
 * @return void
 */
void ByConstRef(const Bst<Date>& tree)
{
    std::cout << "  by const ref          : ";
    tree.InOrderTraversal(PrintDate);
    std::cout << '\n';
}

/**
 * @brief  Takes the tree BY REFERENCE, so a change is seen by the caller
 *
 * @param  tree - the caller's actual tree
 * @return void
 */
void ByRef(Bst<Date>& tree)
{
    tree.Insert(Date(1, 1, 2000));
    std::cout << "  by ref, after insert  : ";
    tree.InOrderTraversal(PrintDate);
    std::cout << '\n';
}

//---------------------------------------------------------------------------------
// Tests
//---------------------------------------------------------------------------------

/**
 * @brief  test1 - print call-back on a Bst<int>
 *
 * @return void
 */
void PrintInt(const int& n)
{
    std::cout << n << ' ';
}

void test1()
{
    std::cout << "--- test1: print call-back ---\n";

    Bst<int> intTree;
    intTree.Insert(50);
    intTree.Insert(30);
    intTree.Insert(70);
    intTree.Insert(20);
    intTree.Insert(40);

    std::cout << "InOrder   : ";
    intTree.InOrderTraversal(PrintInt);
    std::cout << '\n';
    std::cout << "PreOrder  : ";
    intTree.PreOrderTraversal(PrintInt);
    std::cout << '\n';
    std::cout << "PostOrder : ";
    intTree.PostOrderTraversal(PrintInt);
    std::cout << '\n';
    std::cout << "Search 40 : " << (intTree.Search(40) ? "found" : "not found") << '\n';
    std::cout << "Search 99 : " << (intTree.Search(99) ? "found" : "not found") << '\n';
    std::cout << "Invariant : " << (intTree.CheckRI() ? "holds" : "BROKEN") << "\n\n";
}

/**
 * @brief  test2 - collector call-back on a Bst<Date>
 *
 * The traversal hands each date to client code, which stores it. The tree
 * never prints, so it stays ignorant of what a Date is for.
 *
 * @param  tree - the tree to collect from
 * @return void
 */
void test2(const Bst<Date>& tree)
{
    std::cout << "--- test2: collector call-back ---\n";

    DateCollector::Reset();
    tree.InOrderTraversal(DateCollector::Collect);

    DateCollector c;
    std::cout << "collected " << c.Size() << " dates:\n";
    for (int i = 0; i < c.Size(); ++i)
        std::cout << c[i] << ' ';
    std::cout << "\n\n";
}

/**
 * @brief  test3 - pass the tree by value, const reference and reference
 *
 * @param  tree - the caller's tree
 * @return void
 */
void test3(Bst<Date>& tree)
{
    std::cout << "--- test3: pass by value / const ref / ref ---\n";

    std::cout << "  before any call       : ";
    tree.InOrderTraversal(PrintDate);
    std::cout << '\n';

    ByValue(tree);
    std::cout << "  caller after by value : ";
    tree.InOrderTraversal(PrintDate);
    std::cout << "   (unchanged, deep copy worked)\n";

    ByConstRef(tree);

    ByRef(tree);
    std::cout << "  caller after by ref   : ";
    tree.InOrderTraversal(PrintDate);
    std::cout << "   (changed, as expected)\n";

    std::cout << "  invariant on caller   : " << (tree.CheckRI() ? "holds" : "BROKEN") << "\n\n";
}

//---------------------------------------------------------------------------------

int main()
{
    test1();

    Bst<Date> dateTree;
    LoadDates("date.txt", dateTree);

    test2(dateTree);
    test3(dateTree);

    return 0;
}
