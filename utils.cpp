#include "utils.hpp"

void printDivider(std::string&& leftHeader, std::string&& rightHeader, FILE* outStream) {
    static constexpr size_t indent = 3;
    //bold, underline, overline, bright-white series of fill dashes with header text inserted left aligned to each panel, 
    //indented according to indent
    std::println(outStream, "\033[1;4;53;97m{:-<{}}{:-<{}}{:-<{}}\033[0m", "", indent, leftHeader, panelWidth, rightHeader, panelWidth - indent);
}

void printDivider(std::string&& leftHeader, FILE* outStream) {
    printDivider(std::move(leftHeader), "", outStream);
}

void printDivider(FILE* outStream) {
    printDivider("", "", outStream);
}
