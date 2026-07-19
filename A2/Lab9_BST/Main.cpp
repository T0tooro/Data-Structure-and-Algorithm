//include "intBst.h"
#include "Bst.h"
#include "Date.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
//typedef intBst Tree;
//typedef Bst<int> Tree;
typedef Bst<Date> Tree;   // Project 2: the tree stores Dates

/*// Reads whitespace-separated integers from a file and inserts each one.
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

// Passed BY VALUE — gets its own deep copy; editing it must NOT touch the original.
void ModifyCopy(Tree treeCopy)
{
    treeCopy.Insert(999);
    std::cout << "Inside the copy (999 added): "; treeCopy.InOrder();
}

*/
void ModifyCopy(Tree treeCopy)
{
    treeCopy.Insert(Date(31, 12, 2099));   // a date unlikely to be in the data
    std::cout << "Inside the copy (extra date added): "; treeCopy.InOrder();
}

// Passed BY REFERENCE (const: read-only) — works on the caller's real tree.

void ShowTree(const Tree& tree)
{
    std::cout << "InOrder   : "; tree.InOrder();
    std::cout << "PreOrder  : "; tree.PreOrder();
    std::cout << "PostOrder : "; tree.PostOrder();
}


// Reuses Assignment 1 extraction, but keeps only day/month/year.
Date ParseDate(const std::string& wast)
{

    std::stringstream ss(wast);
    int day, month, year;
    char slash;
    ss >> day >> slash >> month >> slash >> year;   // stops after the year; ignores the time
    return Date(day, month, year);

}


// Tree passed BY REFERENCE so it fills the caller's actual tree.
/*
void LoadDatesFromFile(const std::string& configPath, Tree& tree)
{
    std::ifstream config(configPath);
    if (!config)
    {
        std::cout << "Could not open " << configPath << '\n';
        return;
    }

    std::string csvName;
    while (std::getline(config, csvName))            // one CSV filename per line
    {
        if (csvName.empty()) continue;

        std::ifstream csv("data/" + csvName);        // prepend the data/ folder
        if (!csv)
        {
            std::cout << "Could not open data/" << csvName << '\n';
            continue;                                 // skip and keep going
        }

        std::string line;
        std::getline(csv, line);                     // throw away the header row

        while (std::getline(csv, line))              // each data row
        {
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string wast;
            std::getline(ss, wast, ',');             // WAST is the first column

            if (!wast.empty())
                tree.Insert(ParseDate(wast));        // duplicate dates are ignored by the BST
        }
    }
} */

// Project 3: reads one date per line from a plain text file.
void LoadDatesFromTextFile(const std::string& fileName, Tree& tree)
{
    std::ifstream in(fileName);
    if (!in)
    {
        std::cout << "Could not open " << fileName << '\n';
        return;
    }

    std::string line;
    while (std::getline(in, line))     // one date per line, e.g. "15/6/2015"
    {
        if (line.empty()) continue;
        tree.Insert(ParseDate(line));  // same ParseDate as Project 2
    }
}

int main()
{

/*
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


    Tree dateTree;

    LoadDatesFromFile("data/data_source.txt", dateTree);

    std::cout << "--- traversals (by reference) ---\n";
    ShowTree(dateTree);

    std::cout << "\n--- search ---\n";
    Date probe(1, 3, 2015);
    std::cout << "Search 1/3/2015: " << (dateTree.Search(probe) ? "found" : "not found") << '\n';

    std::cout << "\n--- invariant check ---\n";
    std::cout << "CheckRI: " << (dateTree.CheckRI() ? "holds" : "BROKEN") << '\n';  */

    Tree dateTree;

    LoadDatesFromTextFile("date.txt", dateTree);

    std::cout << "--- traversals (by reference) ---\n";
    ShowTree(dateTree);

    std::cout << "\n--- search ---\n";
    Date probe(7, 4, 2014);
    std::cout << "Search 7/4/2014: " << (dateTree.Search(probe) ? "found" : "not found") << '\n';

    std::cout << "\n--- invariant check ---\n";
    std::cout << "CheckRI: " << (dateTree.CheckRI() ? "holds" : "BROKEN") << '\n';

    std::cout << "\n--- pass by value (deep-copy test) ---\n";
    ModifyCopy(dateTree);                            // the required by-value test
    std::cout << "Original after copy : "; dateTree.InOrder();

    return 0;



}
