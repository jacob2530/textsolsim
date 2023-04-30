#include "physics.h"

void applyGravity(Star& star, std::vector<Planet>& planets, float dt) {
    // Compute the gravitational force between the star and each planet
    for (Planet& planet : planets) {
        float dx = planet.x - star.x;
        float dy = planet.y - star.y;
        float dz = planet.z - star.z;

        float distanceSquared = dx * dx + dy * dy + dz * dz;
        float distance = std::sqrt(std::fabs(distanceSquared));
        float force = G * star.mass * planet.mass / distanceSquared;


        float forceX = force * dx / distance;
        float forceY = force * dy / distance;
        float forceZ = force * dz / distance;

        star.vx += forceX * dt / star.mass;
        star.vy += forceY * dt / star.mass;
        star.vz += forceZ * dt / star.mass;

        planet.vx -= forceX * dt / planet.mass;
        planet.vy -= forceY * dt / planet.mass;
        planet.vz -= forceZ * dt / planet.mass;

        // Compute the gravitational force between each pair of planets
        for (Planet& otherPlanet : planets) {
            if (&planet != &otherPlanet) {
                float dx = otherPlanet.x - planet.x;
                float dy = otherPlanet.y - planet.y;
                float dz = otherPlanet.z - planet.z;

                float distanceSquared = dx * dx + dy * dy + dz * dz;
                float distance = std::sqrt(std::fabs(distanceSquared));
                float force = G * planet.mass * otherPlanet.mass / distanceSquared;

                float forceX = force * dx / distance;
                float forceY = force * dy / distance;
                float forceZ = force * dz / distance;

                // Update the velocities of the planets based on the gravitational force
                planet.vx += forceX * dt / planet.mass;
                planet.vy += forceY * dt / planet.mass;
                planet.vz += forceZ * dt / planet.mass;

                otherPlanet.vx -= forceX * dt / otherPlanet.mass;
                otherPlanet.vy -= forceY * dt / otherPlanet.mass;
                otherPlanet.vz -= forceZ * dt / otherPlanet.mass;
            }
        }
    }
}

void updateStarPos(Star& star, float dt) {
    star.x += star.vx * dt;
    star.y += star.vy * dt;
    star.z += star.vz * dt;
}

void updatePlanetPos(Planet& planet, float dt) {
    planet.x += planet.vx * dt;
    planet.y += planet.vy * dt;
    planet.z += planet.vz * dt;

    // Store the trail position
    int trailX = static_cast<int>(planet.x * DISTANCE_SCALE + getmaxx(stdscr) / 2.0);
    int trailY = static_cast<int>(planet.y * DISTANCE_SCALE + getmaxy(stdscr) / 2.0);
    planet.trail.push_front({trailX, trailY});

    // Limit the trail length
    if (planet.trail.size() > trail_length) {
        planet.trail.pop_back();
    }
}

float calculateSpeed(Planet& planet) {
    float speed;
    speed = std::sqrt(planet.vx * planet.vx + planet.vy * planet.vy + planet.vz * planet.vz);
    return speed;
}

