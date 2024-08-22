#pragma once
#include <vector>
#include <string>
#include <fstream>

namespace FileLoader{
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
}