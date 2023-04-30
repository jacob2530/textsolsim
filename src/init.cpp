#include "init.h"
#include "randomSystem.h"
#include "home.h"

void init(int numPlanets, std::vector<Star>& stars, std::vector<Planet>& planets, int choice) {

    if (choice == 1) {
        std::vector<std::string> planetNames = readFile("planets.txt");
        std::vector<std::string> planetDescriptions = readFile("planetOrigins.txt");
        std::vector<std::string> starNames = readFile("stars.txt");
        std::vector<std::string> starDescriptions = readFile("starOrigins.txt");
        Star star = generateStar(starNames, starDescriptions);
        stars.push_back(star);
        float totalMass, avgMass;
        for (int i = 0; i < numPlanets; i++) {
            Planet planet = generatePlanet(star, i, planetNames, planetDescriptions);
            planets.push_back(planet);
            totalMass += planets[i].mass;
        }
    } else if (choice == 2) {
        Star star = generateSun();
        stars.push_back(star);

        for (int i = 0; i < numPlanets; i++) {
            Planet planet = generateSolarSystem(i);
            planets.push_back(planet);
        }
    }
    
}

std::vector<std::string> readFile(const std::string& filename) {
    std::vector<std::string> data;
    std::string newfilename = "datafiles/" + filename;
    std::ifstream inputFile(newfilename);

    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            data.push_back(line);
        }
        inputFile.close();
    }else {
        std::cerr << "Unable to open file: " << newfilename << std::endl;
    }
    return data;
}

int getRandomIndex(const std::vector<std::string>& names) {
    if (names.empty()) {
        return -1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, names.size() - 1);
    int randomIndex = dist(gen);

    return randomIndex;
}

int randomColorIndex(int minColor, int maxColor) {
    static std::random_device rd; //Obtain a random number from hardware
    static std::mt19937 gen(rd()); //seed generator
    std::uniform_int_distribution<> distrib(minColor, maxColor);

    return distrib(gen);
}