#pragma once
#include <filesystem>
#include <fstream>
#include <regex>

class FileLoader {
private:
    std::vector<std::string> data = {};
    std::string filetoload = "";
    bool fileloaded = false;
public:
    FileLoader(std::string name) {
        filetoload = name;
    }
    bool check_file_existence() {
        std::ifstream file;
        file.open(filetoload);
        return file.good();
    } //returns 1 if it exists
    bool load_file() {
        std::ifstream file;
        char line[128];
        file.open(filetoload);
        if (file.is_open()) {
            while (file.getline(line, 128)) {
                data.push_back(line);
            }
            file.close();
            fileloaded = true;
            return false;
        }
        else {
            return true;
        }
    }
    bool is_file_loaded() {
        return fileloaded;
    }
    std::vector<std::string> get_data() {
        return data;
    };
    uint8_t run_sanity_check() {
        if (fileloaded = true) {
            for (int i = 0; i < data.size(); i++) {
                if (data[i] == "" || filetoload == "") {
                    return 1;
                }
            }
        }
        return 0;
    }
};
std::vector<std::string> get_files_by_extension(std::string dir, std::string extension) {
    std::vector<std::string> tmp = {};
    for (auto& p : std::filesystem::recursive_directory_iterator(dir))
    {
        if (p.path().extension() == extension) {
            tmp.push_back(p.path().stem().string());
        }
    }
    return tmp;
}

namespace ModuleLoader {
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
        //It's easier to parse a vector of strings than a
        //single string with newlines, and no harder to load
        std::vector<std::string> lines;
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
        ParsedEntry parse_kvp(std::pair<std::string, std::string> kvp) {
            ParsedEntry tmp;
            tmp.key = kvp.first;
            tmp.value = kvp.second;
            tmp.value.erase(std::remove(
                tmp.value.begin(),
                tmp.value.end(), ' '),
                tmp.value.end()
            );
            switch (kvp.second.back()) {
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
                bool invalid = false;
                for (char c : kvp.second.substr(0, kvp.second.length() - 1)) {
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
                        if (!(c >= '0' && c <= '9') && c != '.') {
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
            case 'V': {
                std::regex regex("/^(([0-9]){1,3}\.([0-9]){1,3}\.([0-9]){1,3}V)/m");
                bool valid = std::regex_match(tmp.value, regex);
                if (valid) {
                    tmp.value.substr(0, tmp.value.length() - 2);
                    tmp.type = EntryType::Version;
                    return tmp;
                }
                else {
                    tmp.type = EntryType::Invalid;
                    break;
                }
            }
            default: {
                tmp.type = EntryType::Invalid;
            }
            }
            return tmp;
        }
    };
}