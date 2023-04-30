#pragma once

#include <string>
#include <deque>

typedef struct Star {
    float x, y, z;
    float vx, vy, vz;
    float mass;
    float radius;
    float luminosity;
    char symbol;
    std::string name;
    std::string desc;
    int color;
}Star;

typedef struct Planet {
    float x, y, z;    //3D coordinate system for position
    float vx, vy, vz; //velocity variables
    float mass;
    float radius;
    float albedo;
    float Te;
    char symbol;
    std::deque<std::pair<int, int>> trail;
    int color;
    std::string name;
    std::string desc;
    std::string type;
    std::string subtype;
}Planet;