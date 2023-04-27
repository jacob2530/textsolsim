#include "home.h"

Star generateSun() {
    Star star;
    star.mass = 1.0;
    star.x = 0;
    star.y = 0;
    star.z = 0;
    star.vx = 0;
    star.vy = 0;
    star.vz = 0;
    star.symbol = '*';
    star.name = "Sun";
    star.desc = "The name Sun comes from the Old English word sunne, which is derived from the Proto-Germanic word sunnōn. The Sun has been called different names in various cultures and languages, but they all essentially refer to the same celestial body.";

    return star;
}

Planet generateSolarSystem(int pos) {
    Planet planet;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> angle_distrib(0, 2 * M_PI);
    std::uniform_real_distribution<> random_float_distrib(-1, 1);
    
    const float mass[] = {1.66013e-07, 2.4478383e-06, 3.00273e-06, 3.2271495e-07, 0.0009543, 0.0002857, 4.366244e-05, 5.15083e-5, 6.55e-9};
    const float d[] = {0.387, 0.723, 1, 1.524, 5.203, 9.537, 19.18, 30.07, 39.48}; //distance from the sun in AU
    const float i[] = {7.005, 3.3947, 0.00005, 1.8506, 1.303, 2.4891, 0.7734, 1.7692, 17.16}; //inclination angle, in degrees
    const char sym[] = {'m', 'v', 'e', 'M', 'J', 'S', 'U', 'N', 'p'};
    const int colors[] = {1, 6, 4, 1, 1, 3, 4, 4, 7};
    const std::string name[] = {"Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune", "Pluto"};
    const std::string descriptions[] = {
        "Mercury is named after the Roman god of commerce, travel, and thievery, who was also the messenger of the gods. The planet was named for this god because of its swift movement across the sky.",
        "Venus is named after the Roman goddess of love and beauty. The planet was named for this goddess because it is the brightest and most beautiful object in the night sky after the Moon.",
        "The name Earth comes from the Old English word eorþe, which is derived from the Proto-Germanic word erthō. The name has been used to refer to our planet in various forms across many languages.",
        "Mars is named after the Roman god of war. The planet was named for this god because of its reddish appearance, which was reminiscent of the color of blood.",
        "Jupiter is named after the Roman king of the gods, also known as Jove. The planet was named for this god because it is the largest and most massive planet in our solar system.",
        "Saturn is named after the Roman god of agriculture and time. The planet was named for this god because it is the slowest-moving of the visible planets, reflecting the god's association with time.",
        "Uranus is named after the ancient Greek god of the sky, Ouranos. The planet was named for this god because it is a giant gas planet with a sky-blue appearance.",
        "Neptune is named after the Roman god of the sea. The planet was named for this god because of its deep blue color, which is reminiscent of the color of the ocean.",
        "Pluto, now considered a dwarf planet, is named after the Roman god of the underworld. The planet was named for this god because it is far away from the Sun and was thought to be a dark and cold place."
    };

    //convert angle of inclination to radians
    float iRad = i[pos] * M_PI / 180.0;
    float angle = angle_distrib(gen);
    planet.mass = mass[pos];

    // Set the position of the planet using spherical coordinates
    planet.x = d[pos] * std::cos(angle);
    planet.y = d[pos] * std::sin(angle);
    planet.z = d[pos] * std::sin(i[pos]);

    // Calculate the radial vector from the star to the planet
    float rx = planet.x;
    float ry = planet.y;
    float rz = planet.z;

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
    float orbital_speed = std::sqrt(G * 1.0 / d[pos]);

    // Set the initial velocity of the planet in the orbital plane
    float vx_plane = (orbital_speed * tx);
    float vy_plane = (orbital_speed * ty);
    float vz_plane = (orbital_speed * tz);

    // Rotate the velocity vector according to the inclination angle
    planet.vx = vx_plane;
    planet.vy = vy_plane * std::cos(iRad) - vz_plane * std::sin(iRad);
    planet.vz = vy_plane * std::sin(iRad) + vz_plane * std::cos(iRad);

    planet.symbol = sym[pos];
    planet.color = colors[pos];
    planet.name = name[pos];
    planet.desc = descriptions[pos];
    
    return planet;
}