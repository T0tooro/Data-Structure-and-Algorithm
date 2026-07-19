#include "Bst.h"
#include "Date.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

typedef Bst<Date> Tree;     // Project 2: the tree stores Dates

// Pulls the date out of one WAST field, e.g. "1/3/2015 0:00" -> Date(1,3,2015).
Date ParseDate(const std::string& wast)
{
    std::stringstream ss(wast);
    int day, month, year;
    char slash;
    ss >> day >> slash >> month >> slash >> year;   // stops after the year; ignores the time
    return Date(day, month, year);
}

// Reads data_source.txt, then loads the WAST date from every row of each listed CSV.
// Tree passed BY REFERENCE so it fills the caller's actual tree.
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
        if (csvName.empty())
            continue;

        std::ifstream csv("data/" + csvName);        // prepend the data/ folder
        if (!csv)
        {
            std::cout << "Could not open data/" << csvName << '\n';
            continue;                                // skip and keep going
        }

        std::string line;
        std::getline(csv, line);                     // discard the header row

        while (std::getline(csv, line))              // each data row
        {
            if (line.empty())
                continue;

            std::stringstream ss(line);
            std::string wast;
            std::getline(ss, wast, ',');             // WAST is the first column

            if (!wast.empty())
                tree.Insert(ParseDate(wast));        // duplicate dates are ignored by the BST
        }
    }
}

// Passed BY REFERENCE (const: read-only) - works on the caller's real tree.
void ShowTree(const Tree& tree)
{
    std::cout << "InOrder   : ";
    tree.InOrder();
    std::cout << "PreOrder  : ";
    tree.PreOrder();
    std::cout << "PostOrder : ";
    tree.PostOrder();
}

// Passed BY VALUE - gets its own deep copy; editing it must NOT touch the original.
void ModifyCopy(Tree treeCopy)
{
    treeCopy.Insert(Date(31, 12, 2099));             // a date not present in the data
    std::cout << "Inside the copy (31/12/2099 added): ";
    treeCopy.InOrder();
}

int main()
{
    Tree dateTree;

    LoadDatesFromFile("data/data_source.txt", dateTree);

    std::cout << "--- traversals (by reference) ---\n";
    ShowTree(dateTree);

    std::cout << "\n--- search ---\n";
    Date probe(1, 3, 2015);                          // this date IS in the CSV data
    std::cout << "Search 1/3/2015: " << (dateTree.Search(probe) ? "found" : "not found") << '\n';

    std::cout << "\n--- invariant check ---\n";
    std::cout << "CheckRI: " << (dateTree.CheckRI() ? "holds" : "BROKEN") << '\n';

    std::cout << "\n--- pass by value (deep-copy test) ---\n";
    ModifyCopy(dateTree);
    std::cout << "Original after copy : ";
    dateTree.InOrder();   // 31/12/2099 must be ABSENT

    return 0;
}
