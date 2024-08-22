#pragma once
#include <filesystem>
#include <fstream>
#include <regex>

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