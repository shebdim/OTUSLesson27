#include "reducer.h"

#include <fstream>

using namespace std;

namespace Yamr {

struct StringHasher {
    static constexpr size_t coef = 127;
    static constexpr size_t module = 1'000'000'007;

    std::size_t operator()(const std::string &str) const {
        size_t hash_res = 0;
        size_t cur_coeff = coef;
        for (auto c : str) {
            cur_coeff *= coef;
            cur_coeff %= module;
            hash_res += cur_coeff * c;
        }
        return hash_res % module;
    }
};

void saveStrListToFile(const StrList &lst, const std::string &fname) {
    ofstream f(fname);
    for (const auto &line : lst) {
        f << line << '\n';
    }
}

std::vector<StrList> Reducer::split(StrList &string_list, std::size_t thread_count) {
    std::vector<StrList> res(thread_count);
    StringHasher h; //    std::hash<string> h;
    for (auto &line : string_list) {
        size_t idx = h(line) % thread_count;
        res[idx].push_back(move(line));
    }
    return res;
}

std::vector<StrList> Reducer::split(const std::string &filename, std::size_t thread_count) {
    std::vector<StrList> res(thread_count);
    StringHasher h; //    std::hash<string> h;
    ifstream f(filename);
    for (string line; getline(f, line); ) {
        size_t idx = h(line) % thread_count;
        res[idx].push_back(move(line));
    }
    return res;
}

}