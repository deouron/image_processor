#include "argument_parser.h"

namespace ArgumentParser {
void Argument::ClearArgument() {
    Name.clear();
    Parameters.clear();
}

bool Argument::EmptyArgument() const {
    return Name.empty() && Parameters.empty();
}

std::vector<Argument> ParseArguments(int argc, char *argv[]) {
    std::vector<Argument> result;
    Argument temp;
    for (size_t i = 1; i < static_cast<size_t>(argc); ++i) {
        std::string str(argv[i]);
        if (i <= 2) {
            temp.Name = str;
            result.push_back(temp);
            temp.ClearArgument();
        } else {
            if (str.front() == '-') {
                if (!temp.EmptyArgument()) {
                    result.push_back(temp);
                    temp.ClearArgument();
                }
                temp.Name = str;
            } else {
                temp.Parameters.push_back(str);
            }
        }
    }
    if (!temp.EmptyArgument()) {
        result.push_back(temp);
    }
    return result;
}
}
