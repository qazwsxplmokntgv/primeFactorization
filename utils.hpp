#pragma once

#include <string>
#include <print>

constexpr size_t panelWidth = 72;
constexpr size_t miniPanelWidth = panelWidth / 2;

void printDivider(std::string&& leftHeader = "", std::string&& rightHeader = "", FILE* outStream = stdout);
void printDivider(std::string&& leftHeader, FILE* outStream);
void printDivider(FILE* outStream);
