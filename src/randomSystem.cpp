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
    star.radius = pow(star.mass, 0.8);
    star.luminosity = (pow(star.mass, 3.5)); //Given in Sun Luminosities.
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

Planet generatePlanet(const Star& star, int i, std::vector<std::string>& names, std::vector<std::string>& descriptions) {
    std::random_device rd;
    std::mt19937 gen(rd());

    const float earthMass = 3.003e-6;
    const float uranusMass = 4.366e-5;
    const float saturnMass = 2.857e-4;
    const float jupiterMass = 0.0009543;

    const float halfwayEarthMass = planetMassMin + 0.5 * (earthMass - planetMassMin);
    const float doubleEarthMass = 2 * earthMass;
    const float tenTimesEarthMass = 10 * earthMass;

    std::vector<float> intervals {
        planetMassMin,
        halfwayEarthMass,
        doubleEarthMass,
        tenTimesEarthMass,
        uranusMass,
        saturnMass,
        jupiterMass,
        planetMassMax
    };

    std::vector<float> probabilities {5.0, 2.0, 1.0, 0.5, 0.25, 0.1, 0.1};
    // Create the piecewise linear distribution for mass
    std::piecewise_linear_distribution<float> mass_distrib(intervals.begin(), intervals.end(), probabilities.begin());

    std::uniform_real_distribution<> distance_distrib(1, 35);//(Mean distrib for distance, std deviation)
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

    //Choose Planet type and display information
    assignProps(planet, distance);

    //Calculate planet effective Temperature. Given in Kelvin
    float distanceM = distance * AUtoM;
    planet.Te = (pow((star.luminosity * SLtoW * (1 - planet.albedo)) / (16 * M_PI * stefanBoltzmanConst * distanceM * distanceM), 0.25));

    //Choose planet color
    planet.color = randomColorIndex(1, 7);

    //Choose planet name
    int randomIndex = getRandomIndex(names);
    planet.name = names[randomIndex];
    planet.desc = descriptions[randomIndex];

    return planet;
}

void assignProps (Planet& planet, float distance) {
    // Define mass thresholds and distance thresholds for different planet types and subtypes
    const float massThreshold1 = (planetMassMax - planetMassMin) / 5;
    const float massThreshold2 = 2 * massThreshold1;
    const float massThreshold3 = 3 * massThreshold1;
    const float massThreshold4 = 4 * massThreshold1;

    const float earthDistanceThreshold = 5.0; // Example threshold, adjust as needed
    const float earthLikeMassThreshold = 2.0 / SMtoEM; //Earth-like mass threshold
    const float superEarthMassThreshold = 10.0 / SMtoEM; // Example threshold, adjust as needed
    const float superEarthDistanceThreshold = 10.0; //Super-Earth Distance Threshold
    const float dwarfPlanetDistanceThresholdMin = 30.0; // Example threshold, adjust as needed
    const float dwarfPlanetDistanceThresholdMax = 40.0; // Example threshold, adjust as needed
    const float gasGiantDistanceThresholdMin = 5.0; // Example threshold, adjust as needed
    const float gasGiantDistanceThresholdMax = 15.0; // Example threshold, adjust as needed
    const float hotJupiterDistanceThreshold = 5.0; // Example threshold, adjust as needed
    const float iceGiantMassThreshold = 25.0 / SMtoEM; // Example threshold, adjust as needed
    const float iceGiantDistanceThreshold = 20.0; // Example threshold, adjust as needed

    const float constantFactor = 1.0; // Earth Radii / Earth Mass^0.55
    const float gasGiantRadiusMin = 6.0; // Earth Radii
    const float gasGiantRadiusMax = 15.0; // Earth Radii
    float tempmass = planet.mass * SMtoEM;

    if (planet.mass < massThreshold1) {
        planet.symbol = '.';
        // Calculate radius based on mass-radius relationship for small-mass planets
        planet.radius = constantFactor * pow(tempmass, 0.55);
        planet.type = "Terrestrial";
        // Assign subtypes based on distance from the star
        if (distance < earthDistanceThreshold && planet.mass <= earthLikeMassThreshold) {
            planet.subtype = "Earth-like";
            planet.albedo = 0.3;
        } else if (planet.mass < superEarthMassThreshold && distance <= superEarthDistanceThreshold) {
            planet.subtype = "Super-Earth";
            planet.albedo = 0.35;
        } else if (distance >= dwarfPlanetDistanceThresholdMin && distance <= dwarfPlanetDistanceThresholdMax) {
            planet.subtype = "Dwarf planet";
            planet.albedo = 0.09;
        } else {
            planet.subtype = "Rocky planet";
            planet.albedo = 0.07;
        }
    } else {
        planet.symbol = 'O';
        float massNormalized = (planet.mass - massThreshold1) / (planetMassMax - massThreshold1);
        planet.radius = gasGiantRadiusMin + massNormalized * (gasGiantRadiusMax - gasGiantRadiusMin);
        planet.type = "Gas Giant";
        // Assign subtypes based on distance from the star
        if (distance >= gasGiantDistanceThresholdMin && distance <= gasGiantDistanceThresholdMax) {
            planet.subtype = "Jupiter-like";
            planet.albedo = 0.5;
        } else if (distance < hotJupiterDistanceThreshold) {
            planet.subtype = "Hot Jupiter";
            planet.albedo = 0.9;
        } else if (planet.mass >= iceGiantMassThreshold && distance > iceGiantDistanceThreshold) {
            planet.subtype = "Ice Giant";
            planet.albedo = 0.5;
        }
        else {
            planet.subtype = "Neptune-like";
            planet.albedo = 0.4;
        }
    }
}