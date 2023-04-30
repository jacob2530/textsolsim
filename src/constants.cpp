#include "constants.h"

float kToF(float kelvin) {
    float f = (kelvin - 273.15) * (9.0 / 5.0) + 32.0;
    return f;
}