#include "file_utils.h"

#include <algorithm>
#include <filesystem>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

void make_full_paths(std::vector<std::string>& files) {
    fs::path cur_path = fs::current_path();
    transform(files.begin(), files.end(), files.begin(), [cur_path](string& fname){
        return cur_path.string() + "/" + fname;
    });
}

void remove_non_valid_paths(std::vector<std::string>& files) {
    files.erase(
            remove_if(files.begin(), files.end(), [](const string& f){
                return !fs::exists(f);}),
            files.end()
    );
}

void sort_names_and_remove_duplic(std::vector<std::string>& files) {
    sort(files.begin(), files.end());
    files.erase(unique(files.begin(), files.end()), files.end());
}

std::vector<std::string> getFileListRecursive(const std::string& path_name) {
    fs::path parent_path;
    if (!path_name.empty()) {
        if (!fs::exists(path_name)) return {};
        parent_path = fs::path(path_name);
    } else {
        parent_path = fs::current_path();
    }
    vector<string> res;
    try {
        for (const auto& p : fs::recursive_directory_iterator(parent_path)) {
            if (fs::is_regular_file(p.path())) {
                res.push_back(p.path().string());
            }
        }
    } catch (const exception& e) {
        cerr << e.what() << " " << res.back() << endl;
        return res;
    }
    return res;
}

std::size_t get_file_size(const std::string& filename) {
    return fs::file_size(fs::path(filename));
}

std::size_t findEol(std::istream& f, std::size_t start_pos) {
    f.seekg(start_pos, ios_base::beg);
    char c;
    while (f.get(c)) {
        ++start_pos;
        if (c == '\n') break;
    }
    return start_pos;
}