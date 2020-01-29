#pragma once

#include <vector>
#include <stdexcept>
#include <future>

#include "mapper.h"

void saveStrListToFile(const StrList& lst, const std::string& fname);

class Reducer {
    friend struct TestReducer;
public:
    Reducer(StrList& string_list, std::size_t thread_count)
        : string_list_(string_list), thread_count_(thread_count)
    {
        if (thread_count_ == 0) {
            throw std::invalid_argument("threads count can't be zero");
        }
        splitted_lists_ = split(string_list_, thread_count_);
    }

    template <typename Func>
    void run(Func func, const std::string& fname_prefix);

private:
    StrList& string_list_;
    std::size_t thread_count_ = 0;
    std::vector<StrList> splitted_lists_;

    static std::vector<StrList>
        split(StrList& string_list, std::size_t thread_count);

    template <typename Func>
    static StrList thread_work(const StrList & str_list, Func func);
};

template <typename Func>
void Reducer::run(Func func, const std::string& fname_prefix) {
    std::vector<std::future<StrList>> futures;
    futures.reserve(thread_count_);
    for (std::size_t i = 0; i < thread_count_; ++i) {
        futures.emplace_back(std::async(thread_work<Func>, splitted_lists_[i], func));
    }
    std::size_t i = 0;
    for (auto& task : futures) {
        saveStrListToFile(task.get(), fname_prefix + std::to_string(i++));
    }
}

template <typename Func>
StrList Reducer::thread_work(const StrList & str_list, Func func) {
    //TODO: make move from copy
    for (const auto& line : str_list) {
        func(line);
    }
    return func.get_res();
}

struct TestReducer {
    static std::vector<StrList> split(StrList& string_list, std::size_t thread_count) {
        return Reducer::split(string_list, thread_count);
    }
};