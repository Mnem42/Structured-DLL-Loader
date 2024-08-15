#pragma once
#include <vector>
#include <windows.h>

namespace DLLLoader {
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
}