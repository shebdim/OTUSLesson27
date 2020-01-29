#include <iostream>
#include <map>
#include <utility>

#include "mapper.h"
#include "reducer.h"
#include "min_prefix.h"

using namespace std;

int main(int argc, char* argv[]) {
    Mapper m("test.in", 2u);
    auto res = m.run(first_letter_task);
    cout << "Maping: " << endl;
    for (const auto& line : res) {
        cout << line << endl;
    }

    Reducer r(res, 2u);
    r.run(CalcLines(), "ouput_");
	return 0;
}
