#pragma once
#include <iostream>
#include <cstdlib>
#include <random>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <sstream>
#include "structs.h"
#include "constants.h"
#include "physics.h"
#include "randomSystem.h"
#include "home.h"
#include "display.h"

void init(int numPlanets, std::vector<Star>& stars, std::vector<Planet>& planets, int choice);

std::vector<std::string> readFile(const std::string& fileName);
int getRandomIndex(const std::vector<std::string>& names);
int randomColorIndex(int minColor, int maxColor);