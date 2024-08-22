// StructuredDLLLoader.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "dll_loader.h"
#include "module_loader.h"
#include "file_loader.h"

typedef int(*tmp)();
typedef LPCSTR(*tmp2)(int);

int main()
{
    ModuleLoader::MetadataParser parser({ "A:1.2.3V" });
    ModuleLoader::ParsedEntry entry=parser.parse_kvp(parser.extract_kvp(0));
    std::cout << "Done.";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
