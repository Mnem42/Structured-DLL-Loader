#pragma once
#include <vector>
#include <windows.h>

namespace DLLLoader {
    typedef std::vector < std::pair<HANDLE, bool>> LoadedFuncSet;
    typedef LPCSTR(*GetInfoLineFunc)(int, LPCWSTR, LPCWSTR);

    //The plain data for a module
    struct ModuleData {
        LPCWSTR modulename = NULL;
        int modulebuildnum = -1;
        int loaderversion = -1;
        int moduleminver = -1;
        int modulemaxver = -1;
    };

    //A loaded DLL
    class LoadedDLL {
    private:
        HINSTANCE dll = NULL;
        LoadedFuncSet functionsloaded;
        ModuleData data;
        LPCWSTR unparsedmetadata=NULL;
        bool loadedfile    = false;
        bool metadataloaded= false;
    public:
        LoadedDLL(LPCWSTR name, bool* error,int loader_version) {
            dll = LoadLibrary(name);
            *error = (dll == NULL); //sets flag to false if dll load failed 
                                    //(LoadLibrary returns NULL if there
                                    // was an error)
            data.loaderversion = loader_version;
        }
        template<typename T> T collect_typed_funcptr(LPCSTR name, bool* err)
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
        ModuleData get_module_data() {
            return data;
        }
        ~LoadedDLL() {
            FreeLibrary(dll);
            dll = NULL;
        }
    };
}