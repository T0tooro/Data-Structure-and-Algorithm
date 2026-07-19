#include "Bst.h"
#include "Date.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

typedef Bst<Date> Tree;     // Project 3: the tree stores Dates

// Pulls a date out of one line, e.g. "15/6/2015" -> Date(15,6,2015).
Date ParseDate(const std::string& text)
{
    std::stringstream ss(text);
    int day, month, year;
    char slash;
    ss >> day >> slash >> month >> slash >> year;
    return Date(day, month, year);
}

// Reads one date per line from a plain text file.
// Tree passed BY REFERENCE so it fills the caller's actual tree.
void LoadDatesFromTextFile(const std::string& fileName, Tree& tree)
{
    std::ifstream in(fileName);
    if (!in)
    {
        std::cout << "Could not open " << fileName << '\n';
        return;
    }

    std::string line;
    while (std::getline(in, line))                   // one date per line
    {
        if (line.empty()) continue;
        tree.Insert(ParseDate(line));
    }
}

// Passed BY REFERENCE (const: read-only) - works on the caller's real tree.
void ShowTree(const Tree& tree)
{
    std::cout << "InOrder   : "; tree.InOrder();
    std::cout << "PreOrder  : "; tree.PreOrder();
    std::cout << "PostOrder : "; tree.PostOrder();
}

// Passed BY VALUE - gets its own deep copy; editing it must NOT touch the original.
void ModifyCopy(Tree treeCopy)
{
    treeCopy.Insert(Date(31, 12, 2099));             // a date not present in date.txt
    std::cout << "Inside the copy (31/12/2099 added): "; treeCopy.InOrder();
}

int main()
{
    Tree dateTree;

    LoadDatesFromTextFile("date.txt", dateTree);     // dates in date.txt are NOT in order

    std::cout << "--- traversals (by reference) ---\n";
    ShowTree(dateTree);                              // InOrder comes out chronological

    std::cout << "\n--- search ---\n";
    Date probe(7, 4, 2014);                          // this date IS in date.txt
    std::cout << "Search 7/4/2014: " << (dateTree.Search(probe) ? "found" : "not found") << '\n';

    std::cout << "\n--- invariant check ---\n";
    std::cout << "CheckRI: " << (dateTree.CheckRI() ? "holds" : "BROKEN") << '\n';

    std::cout << "\n--- pass by value (deep-copy test) ---\n";
    ModifyCopy(dateTree);
    std::cout << "Original after copy : "; dateTree.InOrder();   // 31/12/2099 must be ABSENT

    return 0;
}
