#include "utils.hpp"

void printDivider(const std::string&& leftHeader, const std::string&& rightHeader) {
    static constexpr size_t indent = 3;
    std::println("\033[1;4;97;53m{:-<{}}{:-<{}}{:-<{}}\033[0m", "", indent, leftHeader, panelWidth, rightHeader, panelWidth - indent);
}
