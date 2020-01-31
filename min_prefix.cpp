#include "min_prefix.h"

#include <string_view>

#include "str_utils.h"

using namespace std;

// mappers
std::vector<std::string> copy_task(std::string line) {
    return {move(line)};
}

std::vector<std::string> first_letter_task(const std::string& line) {
    return {string{line[0]}};
}

std::vector<std::string> NLetter::operator()(const std::string& line) const {
    return {line.substr(0, min(letter_count_, line.size()))};
}

std::vector<std::string> get_first_word(std::string line) {
    return {line.substr(0, line.find(' '))};
}
std::vector<std::string> get_all_words(std::string line) {
    string_view data{line};
    vector<string> res;
    for (string_view word = read_token(data, " ");
         !word.empty();
         word = read_token(data, " ")
        )
    {
        res.emplace_back(word);
    }
    return res;
}

std::vector<std::string> all_prefixes(std::string line) {
    if (line.empty()) return {};
    std::vector<std::string> res(line.size());
    for (size_t i = 0; i+1 < line.size(); ++i) {
        res[i] = line.substr(0, i+1);
    }
    res.back() = move(line);
    return res;
}

// reducers
void CalcLines::operator()(std::string line) {
    if (line == prev_) {
        ++counter_;
    } else {
        if (!prev_.empty()) {
            res_.push_back(std::to_string(counter_) + ' ' + prev_);
        }
        counter_ = 1;
        prev_ = move(line);
    }
}

Yamr::StrList CalcLines::get_res() {
    if (counter_ > 0 && !prev_.empty()) {
        res_.push_back(std::to_string(counter_) + ' ' + prev_);
    }
    return std::move(res_);
}

void CheckPrefixIsDuplicate::operator()(std::string line) {
    if (line == prev_) {
        ++counter_;
    } else {
        if (!prev_.empty()) {
            if (auto it = size_to_isduplicate_.find(prev_.size());
                it != size_to_isduplicate_.end())
            {
                if (counter_ > 1 && !it->second) {
                    it->second = true;
                }
            } else {
                size_to_isduplicate_[prev_.size()] = counter_ > 1;
            }
        }
        counter_ = 1;
        prev_ = std::move(line);
    }
}

Yamr::StrList CheckPrefixIsDuplicate::get_res() {
    if (counter_ > 0 && !prev_.empty()) {
        if (auto it = size_to_isduplicate_.find(prev_.size());
                it != size_to_isduplicate_.end())
        {
            if (counter_ > 1 && !it->second) {
                it->second = true;
            }
        } else {
            size_to_isduplicate_[prev_.size()] = counter_ > 1;
        }
    }
    Yamr::StrList res;
    res.reserve(size_to_isduplicate_.size());
    for (const auto& [k,v] : size_to_isduplicate_) {
        res.push_back(std::to_string(k) + ' ' + std::string(v ? "true" : "false"));
    }
    return res;
}


