#pragma once

#include <cstdlib>

const float starMassMin = 1.0; // in Solar Masses 
const float starMassMax = 5.0;
const float planetMassMin = 0.000000015;
const float planetMassMax = 0.0001;
const float AUtoKM = 149597870.7;
const float SMtoKG = 1.989e+30;
const float G = 6.67430e-11 * SMtoKG / (AUtoKM * AUtoKM * AUtoKM / 1000.0);
const size_t trail_length = 75; //Length of the trail

constexpr float DISTANCE_SCALE = 1.0f; // Adjust this value to zoom in/out on the system