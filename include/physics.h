#pragma once

#include <cmath>
#include <vector>
#include <ncurses.h>
#include "structs.h"
#include "init.h"
#include "constants.h"

void applyGravity(Star& star, std::vector<Planet>& planets, float dt);
void updateStarPos(Star& star, float dt);
void updatePlanetPos(Planet& planet, float dt);
float calculateSpeed(Planet& planet);