// StructuredDLLLoader.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "main.h"

typedef int(*tmp)();
typedef LPCSTR(*tmp2)(int);

int main()
{
    std::vector<std::string> files = DLLLoader::get_files_by_extension("./",".cfg");
    std::cout << files.size() << "\n";
    DLLLoader::FileLoader loader("test.txt");
    DWORD existence = loader.check_file_existence();
    if (existence != NULL) {
        int tmpvar = loader.load_file();
        loader.run_sanity_check();
    }
    else if (existence == ERROR_FILE_NOT_FOUND) {
        std::cout << "Config file not found\n";
    }
    else {
        std::cout << "Error " << existence;
    }

    bool err;
    DLLLoader::LoadedDLL dll(L"Test.dll",&err,2);
    if (!err) {
        std::cout << "Loaded\n";
        tmp a = dll.collect_typed_funcptr<tmp>("a", &err);
        tmp2 b = dll.collect_typed_funcptr<tmp2>("b", &err);
        if (!err) {
            std::cout << "Running\n";
            std::cout << a() << "\n";
            std::cout << b(0) << "\n";
            std::cout << b(42) << "\n";
            
        }
    }
    if (err) {
        std::cout << "Error detected\n";
    }
    dll.~LoadedDLL();
    while (true) {

    }
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
