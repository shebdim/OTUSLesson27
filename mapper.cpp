#include "mapper.h"

#include <fstream>

#include "file_utils.h"

using namespace std;

std::vector<Mapper::SplitInfo> Mapper::split(const std::string &fname, std::size_t thread_count) {
    vector<SplitInfo> res(thread_count);
    auto size = get_file_size(fname);
    if (size == 0) {
        throw invalid_argument("Empty input file");
    }
    size_t chunk_size = size / thread_count;
    ifstream f(fname);
    for (size_t i = 1; i < thread_count; ++i) {
        res[i-1].end = findEol(f, i*chunk_size);
        res[i].start = res[i-1].end;
    }
    res[thread_count-1].end = size;
    return res;
}
