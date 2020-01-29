#pragma once

#include <string>
#include <map>

#include "mapper.h"

std::string copy_task(const std::string& line);
std::string first_letter_task(const std::string& line);

struct CalcLines {
    void operator()(std::string line) {
        ++m_[move(line)];
    }

    StrList get_res() {
        StrList res;
        res.reserve(m_.size());
        for (const auto& [k,v] : m_) {
            res.push_back(k + ' ' + std::to_string(v));
        }
        return res;
    }

    std::map<std::string, std::size_t> m_;

};