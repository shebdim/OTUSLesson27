#include <iostream>
#include <map>
#include <fstream>

#include "mapper.h"
#include "reducer.h"
#include "min_prefix.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "# yamr <src> <mnum> <rnum>\n"
             << "src -- path to input file\n"
             << "mnum -- threads number for mapper\n"
             << "rnum -- threads number for reducer"
             << endl;
        return 0;
    }

    Yamr::Mapper m("test.in", 2u);
    auto res = m.run(all_prefixes);

    ofstream out("mapper_output.out");
    for (const auto& line : res) {
        out << line << '\n';
    }

    Yamr::Reducer r(move(res), 2u);
//    Yamr::Reducer r("mapper_output.out", 2u);
    r.run(CheckPrefixIsDuplicate(), "ouput_");
	return 0;
}
