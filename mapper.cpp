#include "mapper.h"

#include <utility>
#include <optional>

#include "file_utils.h"
#include "iterator_range.h"

using namespace std;

std::vector<Mapper::SplitInfo> Mapper::split(const std::string &fname, std::size_t thread_count) {
    auto size = get_file_size(fname);
    if (size == 0) {
        throw invalid_argument("Empty input file");
    }
    ifstream f(fname);
    return split(f, thread_count, size);
}

vector<Mapper::SplitInfo> Mapper::split(std::istream &file, std::size_t thread_count,
        std::size_t stream_size) {
    vector<SplitInfo> res(thread_count);
    size_t chunk_size = stream_size / thread_count;
    for (size_t i = 1; i < thread_count; ++i) {
        res[i-1].end = findEol(file, i*chunk_size);
        res[i].start = res[i-1].end;
    }
    res[thread_count-1].end = stream_size;
    return res;
}


StrList Mapper::mergeLists(vector<StrList>& lists) {
    StrList res;
    using Iterator = StrList::iterator;
    // init
    vector<IteratorRange<Iterator>> iterators;
    for (auto& list : lists) {
        iterators.emplace_back(list.begin(), list.end());
    }

    // merge cycle
    while (!isAllEmpty(iterators)) {
        optional<size_t> min_it_idx = nullopt;
        optional<string> min_value = nullopt;

        // find min add it to res and move fwrd min_it
        // TODO: remove copy
        for (size_t i = 0; i < iterators.size(); ++i) {
            if (iterators[i].empty()) continue;
            if (!min_value || *min_value > *(iterators[i].begin()) ) {
                min_it_idx = i;
                min_value = *(iterators[i].begin());
            }
        }
        ++(iterators[*min_it_idx].begin());
        res.push_back(move(*min_value));
    }

    return res;
}
