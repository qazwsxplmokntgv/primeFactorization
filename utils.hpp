#pragma once

#include <print>

constexpr size_t panelWidth = 64;
constexpr size_t miniPanelWidth = panelWidth / 2;

void printDivider(std::string&& leftHeader = "", std::string&& rightHeader = "");

