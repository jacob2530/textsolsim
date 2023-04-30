#pragma once

#include <cstdlib>
#include "init.h"

const float starMassMin = 0.75; // in Solar Masses 
const float starMassMax = 5.0;
const float planetMassMin = 2.372157e-6;
const float planetMassMax = 0.001;

const float AUtoM = 149597870691;
const float AUtoKM = 149597870.691;
const float SMtoKG = 1.989e+30;
const float SMtoEM = 333030.0; // Multiply a solar mass by this number to convert it to Earth Masses.
const float SLtoW = 3.827e+26; //Sun Luminosities to Watts

const float G = 6.67430e-11 * SMtoKG / (AUtoKM * AUtoKM * AUtoKM / 1000.0);

const float stefanBoltzmanConst = 5.67e-8;

const size_t trail_length = 75; //Length of the trail
constexpr float DISTANCE_SCALE = 1.0f; // Adjust this value to zoom in/out on the system

float kToF(float kelvin);