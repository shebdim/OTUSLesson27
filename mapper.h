#pragma once

#include <string>
#include <utility>
#include <vector>
#include <stdexcept>
#include <filesystem>
#include <thread>

class Mapper {
    static constexpr char SIZEOF_EOL = 1;

    struct SplitInfo {
        std::size_t start = 0;
        std::size_t end = 0;
    };
public:
    Mapper (std::string fname, std::size_t thread_count)
        : fname_(std::move(fname)), thread_count_(thread_count)
    {
        if (thread_count_ == 0) {
            throw std::invalid_argument("threads count can't be zero");
        }
        if (!std::filesystem::exists(fname_)) {
            throw std::invalid_argument("Cannot find file " + fname_ + " for mapping");
        }

        split_info_ = split(fname_, thread_count_);
    }

    template <typename F>
    void run(F f) {
        std::vector<std::thread> threads;
        threads.reserve(thread_count_);
        for (std::size_t i = 0; i < thread_count_; ++i) {
            threads.emplace_back(start_thread(split_info_[i], f));
        }
        // join threads
    }

private:
    std::string fname_;
    std::size_t thread_count_ = 0;
    std::vector<SplitInfo> split_info_;

    static std::vector<SplitInfo> split(const std::string& fname, std::size_t thread_count);
    template <typename F>
    std::thread start_thread(const SplitInfo split_info, F f) {
        
    }
};