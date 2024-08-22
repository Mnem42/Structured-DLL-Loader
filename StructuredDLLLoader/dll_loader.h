#pragma once
#include <vector>
#include <windows.h>

namespace DLLLoader {
    typedef std::vector < std::pair<LPCWSTR, bool>> LoadedFuncSet;

    //A loaded DLL
    class LoadedDLL {
    private:
        HINSTANCE dll = NULL;
        LoadedFuncSet functionsloaded;
        LPCWSTR unparsedmetadata=NULL;
        bool loadedfile    = false;
        bool metadataloaded= false;
    public:
        LoadedDLL(LPCWSTR name, bool* error) {
            dll = LoadLibrary(name);
            *error = (dll == NULL); //sets flag to true if dll load failed 
                                    //(LoadLibrary returns NULL if there
                                    // was an error)
        }
        template<typename T> T collect_typed_funcptr(LPCSTR name, bool* err)
        {
            HANDLE function = GetProcAddress(dll, name);

            if (!function) { //GetProcAddress returns NULL if a function doesn't exist
                *err = true;
                functionsloaded.push_back({ name,false });
                return NULL;
            }
            else {
                functionsloaded.push_back({ name,true });
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