#include "intBst.h"
//#include "Bst.h"
#include <iostream>
#include <fstream>
#include <string>
typedef intBst Tree;
//typedef Bst<int> Tree;

// Reads whitespace-separated integers from a file and inserts each one.
// Tree passed BY REFERENCE so it fills the caller's actual tree.

void LoadFromFile(const std::string& fileName, Tree& tree)
{
    std::ifstream in(fileName);
    if (!in)
    {
        std::cout << "Could not open " << fileName << '\n';
        return;
    }
    int value;
    while (in >> value)
        tree.Insert(value);
}

// Passed BY REFERENCE (const: read-only) — works on the caller's real tree.

void ShowTree(const Tree& tree)
{
    std::cout << "InOrder   : "; tree.InOrder();
    std::cout << "PreOrder  : "; tree.PreOrder();
    std::cout << "PostOrder : "; tree.PostOrder();
}

// Passed BY VALUE — gets its own deep copy; editing it must NOT touch the original.

void ModifyCopy(Tree treeCopy)
{
    treeCopy.Insert(999);
    std::cout << "Inside the copy (999 added): "; treeCopy.InOrder();
}

int main()
{

    Tree intTree;

    LoadFromFile("ints.txt", intTree);     // open file, insert in a loop

    std::cout << "--- traversals (by reference) ---\n";
    ShowTree(intTree);

    std::cout << "\n--- search ---\n";
    std::cout << "Search 40: " << (intTree.Search(40) ? "found" : "not found") << '\n';
    std::cout << "Search 41: " << (intTree.Search(41) ? "found" : "not found") << '\n';

    std::cout << "\n--- invariant check ---\n";
    std::cout << "CheckRI: " << (intTree.CheckRI() ? "holds" : "BROKEN") << '\n';

    std::cout << "\n--- pass by value (deep-copy test) ---\n";
    ModifyCopy(intTree);
    std::cout << "Original after copy        : "; intTree.InOrder();  // 999 must be ABSENT

    return 0;
}
