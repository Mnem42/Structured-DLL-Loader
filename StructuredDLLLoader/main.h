#pragma once
#include <vector>
#include <filesystem>
#include <fstream>
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

    class FileLoader {
    private:
        std::string data = "";
        std::string filetoload = "";
        bool fileloaded    = false;
    public: 
        FileLoader(std::string name) {
            filetoload = name;
        }
        DWORD check_file_existence() {
            std::ifstream file;
            file.open(filetoload);
            if (!file.good()) {
                return 0;
            }
            else {
                return 1;
            }
        } //returns 1 if it exists
        DWORD load_file() {
            std::ifstream file;
            char line[128];
            file.open(filetoload);
            if (file.is_open()) {
                file.getline(line, 128);
                file.close();

                data = std::string(line);
                fileloaded = true;
                return NULL;
            }
            else {
                return 1;
            }
        }  // This only uses WIN32 because I forgot STL exists
        bool is_file_loaded() {
            return fileloaded;
        }
        std::string get_raw_data() {
            return data;
        };
        uint8_t run_sanity_check() {
            if (fileloaded = true) {
                if (data == "" || filetoload == "") {
                    return 1;
                }
            }
            return 0;
        }
    };
    std::vector<std::string> get_files_by_extension(std::string dir,std::string extension) {
        std::vector<std::string> tmp = {};
        for (auto& p : std::filesystem::recursive_directory_iterator(dir))
        {
            if (p.path().extension() == extension) {
                tmp.push_back(p.path().stem().string());
            }
        }
        return tmp;
    }
    
    namespace Parser {
        enum EntryType {
            String,
            Int,
            Float,
            Version,
            Invalid
        };
        struct ParsedEntry {
            std::string key;
            std::string value;
            EntryType type;
        };
        class MetadataParser {
        private:
            char delim;
            std::vector<std::string> lines; //It's easier to parse a vector of
            //strings than a single string with 
            //newlines
        public:
            MetadataParser(std::vector<std::string> data, char delimiter = ':') {
                lines = data;
                delim = delimiter;
            };
            std::pair<std::string, std::string> extract_kvp(size_t index, char delim = ':') {
                if (index < lines.size()) {
                    size_t delimpos = lines[index].find(':');
                    std::string key = lines[index].substr(0, delimpos);
                    std::string value = lines[index].substr(delimpos + 1, lines[index].length());
                    return { key, value };
                }
            }
            ParsedEntry parse_kvp(std::pair<std::string,std::string> kvp) {
                ParsedEntry tmp;
                tmp.key   = kvp.first;
                tmp.value = kvp.second;
                switch(kvp.second.back()) {
                case '"': {
                    if (kvp.second.front() == '"') {
                        tmp.type = EntryType::String;
                        if (kvp.second.size() == 2) {
                            tmp.value = "";
                            break;
                        }
                        else {
                            //Take everything except the first and last chars
                            tmp.value = kvp.second.substr(1, kvp.second.length() - 2);
                            break;
                        }
                    }
                    else tmp.type = EntryType::Invalid;
                    break;
                }
                case 'I': {
                    bool invalid=false;
                    for (char c : kvp.second.substr(0,kvp.second.length()-1)) {
                        if (!(c >= '0' && c <= '9')) {
                            tmp.type = EntryType::Invalid;
                            invalid = true;
                        }
                    }
                    if (!invalid) {
                        tmp.type = Int;
                        tmp.value = kvp.second.substr(0, kvp.second.length() - 1);
                    }
                    break;
                }
                case 'F': {
                    bool invalid = false;
                    int decimalnum = 
                        std::count(kvp.second.begin(), kvp.second.end(), '.');
                    if (decimalnum == 1) {
                        for (char c : kvp.second.substr(0, kvp.second.length() - 1)) {
                            if (!(c >= '0' && c <= '9') && c!='.') {
                                tmp.type = EntryType::Invalid;
                                invalid = true;
                            }
                        }
                        if (!invalid) {
                            tmp.type = Float;
                            tmp.value = kvp.second.substr(0, kvp.second.length() - 1);
                        }
                    }
                    else {
                        tmp.type = EntryType::Invalid;
                    }
                    break;
                }
                default:  {
                    tmp.type = EntryType::Invalid;
                }
                }
                return tmp;
            }
        };
    }

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