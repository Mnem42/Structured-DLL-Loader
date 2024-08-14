// StructuredDLLLoader.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <map>
#include <vector>
#include <windows.h>

typedef std::vector < std::pair<HANDLE, bool>> LoadedFuncSet;

//Mainly for safety
class LoadedDLL {
private:
    HINSTANCE dll = NULL;
    LoadedFuncSet functionsloaded;
public:
    LoadedDLL(LPCWSTR name, bool* error) {
        dll = LoadLibrary(name);
        *error = (dll == NULL); //sets flag to false if dll load failed 
                                //(LoadLibrary returns NULL if there
                                // was an error)
    }
    template<typename T> T collect_typed_funcptr(
        LPCSTR name, uint8_t flags, bool* err)
    {
        HANDLE function = GetProcAddress(dll, name);

        if (!function) { //GetProcAddress returns NULL if a function doesn't exist
            *err = true;
            return NULL;
        }
        else {
            functionsloaded.push_back({ dll,name });
            return (T)function;
        }
    }
    LoadedFuncSet get_loaded_functions() {
        return functionsloaded;
    }
    ~LoadedDLL() {
        FreeLibrary(dll);
        dll = NULL; 
    }
};
typedef int(*tmp)();
typedef LPCSTR(*tmp2)(int);
int main()
{
    bool err;
    LoadedDLL dll(L"Test.dll",&err);
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
    std::cout << "Hello World!\n";
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
