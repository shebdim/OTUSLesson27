#pragma once

#include <string>
#include <vector>
#include <map>

#include "mapper.h"

// mapper func. objects
// interface:
// std::vector<std::string> (*) (std::string)

std::vector<std::string> copy_task(std::string line);

std::vector<const std::string&> first_letter_task(std::string line);

struct NLetter {
    explicit NLetter(std::size_t letter_count) : letter_count_(letter_count) {}

    std::vector<std::string> operator()(const std::string& line) const;

    std::size_t letter_count_ = 1;
};

std::vector<std::string> get_first_word(std::string line);

std::vector<std::string> get_all_words(std::string line);

std::vector<std::string> all_prefixes(std::string line);


// reducer func. objects
// interface:
// void (*)(std::string)
// StrList get_res()

struct CalcLines {
    void operator()(std::string line);

    Yamr::StrList get_res();

    std::string prev_;
    std::size_t counter_ = 0;
    Yamr::StrList res_;
};

struct CheckPrefixIsDuplicate {
    void operator()(std::string line);

    Yamr::StrList get_res();

    std::map<std::size_t, bool> size_to_isduplicate_;
    std::string prev_;
    std::size_t counter_ = 0;
};

struct FindMaxPrefix {
    void operator()(std::string line);

    Yamr::StrList get_res();

    std::string prev_;
    std::size_t counter_ = 0;

    std::size_t max_duplicate_size_ = 0;
    std::size_t max_size_ = 0;
};

