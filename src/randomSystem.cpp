#include "randomSystem.h"

Star generateStar(std::vector<std::string>& names, std::vector<std::string>& descriptions) {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_real_distribution<> mass_distrib(starMassMin, starMassMax);
    std::uniform_real_distribution<> pos_distrib(-1, 1);
    std::uniform_real_distribution<> vel_distrib(-1, 1);

    int randomIndex = getRandomIndex(names);

    Star star;
    star.mass = mass_distrib(gen);
    star.x = 0;
    star.y = 0;
    star.z = 0;
    star.vx = 0;
    star.vy = 0;
    star.vz = 0;
    star.color = 3;
    star.symbol = '*';
    star.name = names[randomIndex];
    star.desc = descriptions[randomIndex];

    return star;
}

Planet generatePlanet(const Star& star, int i, std::vector<std::string>& names, std::vector<std::string>& descriptions, std::vector<std::string>& types, std::vector<std::string>& typeDescriptions) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> mass_distrib(planetMassMin, planetMassMax);
    std::uniform_real_distribution<> distance_distrib(5, 25);//Mean distrib for distance of 5, with std deviation of 25
    std::uniform_real_distribution<> angle_distrib(0, 2 * M_PI);
    std::uniform_real_distribution<> inclination_distrib(-0.175, 0.175);
    std::uniform_real_distribution<> random_float_distrib(-1, 1);
    std::uniform_real_distribution<> perturb_distrib(-0.0005, 0.0005);

    Planet planet;
    planet.mass = mass_distrib(gen);

    // Generate a distance from the star
    float distance;
    do {
        distance = std::abs(distance_distrib(gen));
    } while (distance < 0.25 || distance > 40);
    // Choose random angles for the position in the orbit
    float angle = angle_distrib(gen);
    float inclination = inclination_distrib(gen);

    // Set the position of the planet using spherical coordinates
    planet.x = star.x + distance * std::cos(angle);
    planet.y = star.y + distance * std::sin(angle);
    planet.z = star.z + distance * std::sin(inclination);

    // Calculate the radial vector from the star to the planet
    float rx = planet.x - star.x;
    float ry = planet.y - star.y;
    float rz = planet.z - star.z;

    // Compute the cross product of the radial vector and the random vector
    float tx = -ry;
    float ty = rx;
    float tz = 0;

    // Normalize the tangent vector
    float magnitude = std::sqrt(tx * tx + ty * ty + tz * tz);
    tx /= magnitude;
    ty /= magnitude;
    tz /= magnitude;

    // Compute the orbital speed
    float orbital_speed = std::sqrt(G * star.mass / distance);

    // Set the initial velocity of the planet
    planet.vx = (orbital_speed * tx) + perturb_distrib(gen);
    planet.vy = (orbital_speed * ty) + perturb_distrib(gen);
    planet.vz = distance * std::cos(inclination) * orbital_speed * std::sin(inclination) / std::sqrt(rx * rx + ry * ry + rz * rz) + perturb_distrib(gen);

    //Choose character for planet
    if (planet.mass < ((planetMassMax - planetMassMin) / 5)) {
        planet.symbol = '.';
    } else if (planet.mass < (2 * (planetMassMax - planetMassMin) / 5)) {
        planet.symbol = 'o';
    } else if (planet.mass < (3 * (planetMassMax - planetMassMin) / 5)) {
        planet.symbol = 'O';
    } else if (planet.mass < (4 * (planetMassMax - planetMassMin) / 5)) {
        planet.symbol = '0';
    } else {
        planet.symbol = '@';
    }

    //Choose planet color
    planet.color = randomColorIndex(1, 7);

    //Choose planet name
    int randomIndex = getRandomIndex(names);
    planet.name = names[randomIndex];
    planet.desc = descriptions[randomIndex];



    return planet;
}