#define BOOST_TEST_MODULE yamr_test_module
#include <boost/test/unit_test.hpp>

#include <sstream>
#include <cmath>
#include <iostream>
#include <set>

#include "mapper.h"
#include "reducer.h"
#include "min_prefix.h"

using namespace std;
using namespace Yamr;

BOOST_AUTO_TEST_SUITE(mapper_test_suite)

	BOOST_AUTO_TEST_CASE(test_split) {
        stringstream file {
            "one\n"
            "two\n"
            "three\n"
            "four\n"
        };
        {
            auto res = TestMapper::split(file, 2u, file.str().size());
            BOOST_CHECK(res.size() == 2u);
            BOOST_CHECK(res[0].start == 0);
            BOOST_CHECK(res[0].end == 14u); // 19 // 2 = 9 -> 14 (to \n)
            BOOST_CHECK(res[1].start == 14u);
            BOOST_CHECK(res[1].end == 19u);
        }
        {
            auto res = TestMapper::split(file, 3u, file.str().size());
            BOOST_CHECK(res.size() == 3u);
            BOOST_CHECK(res[0].start == 0);
            BOOST_CHECK(res[0].end == 8u); // 19 // 2 = 6
            BOOST_CHECK(res[1].start == 8u);
            BOOST_CHECK(res[1].end == 14u);
            BOOST_CHECK(res[2].start == 14u);
            BOOST_CHECK(res[2].end == 19u);
        }
	}

    BOOST_AUTO_TEST_CASE(test_merge) {
        vector<StrList> splitted {
                {"abc", "adf", "bcd", "dft"},
                {"abs", "acf", "gcd", "zft"},
                {"aas", "bcf", "lcf", "mft"},
        };
        auto merged = TestMapper::mergeLists(splitted);
        StrList expected {
                "aas", "abc", "abs",
                "acf", "adf", "bcd",
                "bcf", "dft", "gcd",
                "lcf", "mft", "zft",
        };
        BOOST_CHECK(merged == expected);
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(reducer_test_suite)

    BOOST_AUTO_TEST_CASE(test_split) {
        {
            StrList input {
                    "aas", "abc", "abs",
                    "acf", "aas", "bcd",
                    "bcf", "dft", "gcd",
                    "lcf", "gcd", "zft",
                    "lcf", "gcd", "zft",
                    "abs", "acf", "zft",
            };

            auto res = TestReducer::split(input, 2);
            BOOST_CHECK(res.size() == 2u);
            BOOST_CHECK(abs((int)res[0].size() - (int)res[1].size()) <= input.size() / 2);
            set<string> s1{res[0].begin(), res[0].end()};
            set<string> s2{res[1].begin(), res[1].end()};
            vector<string> uniq;
            set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), back_inserter(uniq));
            BOOST_CHECK(uniq.empty());
        }
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(func_test_suite)

    BOOST_AUTO_TEST_CASE(test_all_prefixes) {
        {
            string line{"abcdefg"};
            auto res = all_prefixes(line);
            BOOST_CHECK(res.size() == line.size());

            vector<string> expected{"a", "ab", "abc", "abcd", "abcde", "abcdef",
                                    "abcdefg"};
            BOOST_CHECK(res == expected);
        }
        {
            string line{""};
            auto res = all_prefixes(line);
            BOOST_CHECK(res.size() == line.size());

            vector<string> expected{};
            BOOST_CHECK(res == expected);
        }
        {
            string line{"a"};
            auto res = all_prefixes(line);
            BOOST_CHECK(res.size() == line.size());

            vector<string> expected{"a"};
            BOOST_CHECK(res == expected);
        }
    }

    BOOST_AUTO_TEST_CASE(test_calc_lines) {
        {
            vector<string> lines{
                    "abc", "abc", "abc",
                    "abs", "abs",
                    "bca",
                    "adb",
                    "adba",
            };
            CalcLines func;
            for (const auto &line : lines) {
                func(line);
            }
            auto res = func.get_res();
            vector<string> expected{
                    "3 abc",
                    "2 abs",
                    "1 bca",
                    "1 adb",
                    "1 adba",
            };
            BOOST_CHECK(res == expected);
        }
        {
            vector<string> lines{
                    "abc", "abc", "abc",
                    "abs", "abs",
                    "bca",
                    "adb", "adb",
            };
            CalcLines func;
            for (const auto &line : lines) {
                func(line);
            }
            auto res = func.get_res();
            vector<string> expected{
                    "3 abc",
                    "2 abs",
                    "1 bca",
                    "2 adb",
            };
            BOOST_CHECK(res == expected);
        }
    }

    BOOST_AUTO_TEST_CASE(test_check_duplicate) {
        {
            vector<string> lines{
                    "abc", "abc", "abc",
                    "abs", "abs",
                    "bca",
                    "adb",
                    "adba",
            };
            CheckPrefixIsDuplicate func;
            for (const auto &line : lines) {
                func(line);
            }
            auto res = func.get_res();
            vector<string> expected{
                    "3 true",
                    "4 false",
            };
            BOOST_CHECK(res == expected);
        }
        {
            vector<string> lines{
                    "a", "a", "a", "a",
                    "ab", "ab", "ab",
                    "abc",
                    "abs", "abs",
                    "abcd", "abcd",
                    "abcf",
                    "abcdef",
            };
            CheckPrefixIsDuplicate func;
            for (const auto &line : lines) {
                func(line);
            }
            auto res = func.get_res();
            vector<string> expected{
                    "1 true",
                    "2 true",
                    "3 true",
                    "4 true",
                    "6 false",
            };
            BOOST_CHECK(res == expected);
        }
        {
            vector<string> lines{
                    "a", "a", "a", "a",
                    "ab", "ab", "ab",
                    "abc",
                    "abs", "abs",
                    "abcd", "abcd",
                    "abcf",
                    "abcdef", "abcdef",
            };
            CheckPrefixIsDuplicate func;
            for (const auto &line : lines) {
                func(line);
            }
            auto res = func.get_res();
            vector<string> expected{
                    "1 true",
                    "2 true",
                    "3 true",
                    "4 true",
                    "6 true",
            };
            BOOST_CHECK(res == expected);
        }
    }

    BOOST_AUTO_TEST_CASE(test_find_max_prefix) {
        {
            vector<string> lines{
                    "abc", "abc", "abc",
                    "abs", "abs",
                    "bca",
                    "adb",
                    "adba",
            };
            FindMaxPrefix func;
            for (const auto &line : lines) {
                func(line);
            }
            auto res = func.get_res();
            vector<string> expected{
                    "4",
            };
            BOOST_CHECK(res == expected);
        }
        {
            vector<string> lines{
                    "a", "a", "a", "a",
                    "ab", "ab", "ab",
                    "abc",
                    "abs", "abs",
                    "abcd", "abcd",
                    "abcf",
                    "abcdef",
            };
            FindMaxPrefix func;
            for (const auto &line : lines) {
                func(line);
            }
            auto res = func.get_res();
            vector<string> expected{
                    "5",
            };
            BOOST_CHECK(res == expected);
        }
        {
            vector<string> lines{
                    "a", "a", "a", "a",
                    "ab", "ab", "ab",
                    "abc",
                    "abs", "abs",
                    "abcd", "abcd",
                    "abcf",
                    "abcdef", "abcdef",
            };
            FindMaxPrefix func;
            for (const auto &line : lines) {
                func(line);
            }
            auto res = func.get_res();
            vector<string> expected{
                    "6",
            };
            BOOST_CHECK(res == expected);
        }
    }

BOOST_AUTO_TEST_SUITE_END()