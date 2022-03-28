#pragma once

#include <string>
#include <vector>

namespace ArgumentParser {
    struct Argument {
        std::string Name;
        std::vector<std::string> Parameters;
        void ClearArgument();
        bool EmptyArgument() const;
    };

    std::vector<Argument> ParseArguments(int argc, char* argv[]);
}
