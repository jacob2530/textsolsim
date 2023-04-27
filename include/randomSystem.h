#pragma once

#include "init.h"

Star generateStar(std::vector<std::string>& names, std::vector<std::string>& descriptions);
Planet generatePlanet(const Star& star, int i, std::vector<std::string>& names, std::vector<std::string>& descriptions, std::vector<std::string>& types, std::vector<std::string>& typeDescriptions);
