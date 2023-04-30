#pragma once

#include <cstdio>
#include "init.h"

void displaySystem(const Star& star, const std::vector<Planet>& planets, int highlightedObjectIndex);
void displayHighlightedInfo(const Star& star, const std::vector<Planet>& planets, int highlightedObjectIndex);
std::vector<std::string> breakStringIntoLines(const std::string &input, size_t max_width);
