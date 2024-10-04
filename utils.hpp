#pragma once

#include <print>

constexpr size_t panelWidth = 64;
constexpr size_t miniPanelWidth = panelWidth / 2;

void printDivider(const std::string&& leftHeader = "", const std::string&& rightHeader = "");

