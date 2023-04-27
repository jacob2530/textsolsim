#include "display.h"

void displaySystem(const Star& star, const std::vector<Planet>& planets, int highlightedObjectIndex) {

    erase();

    // Calculate the scaling factor
    float scale = std::min(getmaxx(stdscr) / (DISTANCE_SCALE * AUtoKM), getmaxy(stdscr) / (DISTANCE_SCALE * AUtoKM));

    // Draw the planets and their trails
    for (size_t i = 0; i < planets.size(); i++) {
        const Planet& planet = planets[i];
        // Draw the trail
        for (const auto& [trailX, trailY] : planet.trail) {
            if (trailX >= 0 && trailX < getmaxx(stdscr) && trailY >= 0 && trailY < getmaxy(stdscr)) {
                attron(COLOR_PAIR(7));
                mvaddch(trailY, trailX, '~');
                attroff(COLOR_PAIR(7));
            }
        }
        //Draw the planet characters
        int planetX = static_cast<int>(planet.x * DISTANCE_SCALE + getmaxx(stdscr) / 2.0);
        int planetY = static_cast<int>(planet.y * DISTANCE_SCALE + getmaxy(stdscr) / 2.0);
        if (planetX >= 0 && planetX < getmaxx(stdscr) && planetY >= 0 && planetY < getmaxy(stdscr)) {
            mvaddch(planetY, planetX, planet.symbol | COLOR_PAIR(planet.color));
            if (highlightedObjectIndex == i) { // If this planet is selected
                mvaddch(planetY, planetX - 1, '(' | COLOR_PAIR(planet.color));
                mvaddch(planetY, planetX + 1, ')' | COLOR_PAIR(planet.color));
            }
        }
    }

    // Draw the star
    int starX = static_cast<int>(star.x * DISTANCE_SCALE + getmaxx(stdscr) / 2.0);
    int starY = static_cast<int>(star.y * DISTANCE_SCALE + getmaxy(stdscr) / 2.0);
    if (starX >= 0 && starX < getmaxx(stdscr) && starY >= 0 && starY < getmaxy(stdscr)) {
        attron(COLOR_PAIR(3));
        mvaddch(starY, starX, star.symbol);
        attroff(COLOR_PAIR(3));
        if (highlightedObjectIndex == -1) { // If the star is selected
            attron(COLOR_PAIR(3));
            mvaddch(starY, starX - 1, '(');
            mvaddch(starY, starX + 1, ')');
            attroff(COLOR_PAIR(3));
        }
    }

    // Clear the previous planet information
    int info_start_x = getmaxx(stdscr) - 30;
    int info_start_y = getmaxy(stdscr) - (2 * planets.size() + 1);
    for (int y = info_start_y; y < getmaxy(stdscr); ++y) {
        for (int x = info_start_x; x < getmaxx(stdscr); ++x) {
            mvaddch(y, x, ' ');
        }
    }
    
    displayHighlightedInfo(star, planets, highlightedObjectIndex);
    // Refresh the screen
    refresh();
}

void displayHighlightedInfo(const Star& star, const std::vector<Planet>& planets, int highlightedObjectIndex) {
    int info_y = 1;
    int info_x = getmaxx(stdscr) - 50;
    int maxWidth = 40;

    int bottom_info_y = getmaxy(stdscr) - 6;
    int bottom_info_x = info_x - 15;

    //Print controls
    mvprintw(0, 10, "---  Controls  ---");
    mvprintw(1, 1, "Press the Up/Down arrow keys to cycle through system objects.");
    mvprintw(2, 1, "Press (Q) to quit the simulation.");

    if (highlightedObjectIndex == -1) {
        // Star
        attron(COLOR_PAIR(star.color));
        mvprintw(0, info_x, "---  %s  ---", star.name.c_str());
        attroff(COLOR_PAIR(star.color));
        std::vector<std::string> descLines = breakStringIntoLines(star.desc, maxWidth);
        int lineNumber = 0;
        mvprintw(1, info_x, "Name Origin: ");
        for (const std::string &line : descLines) {
            mvprintw(info_y + lineNumber + 1, info_x, "%s", line.c_str());
            lineNumber++;
        }
        info_y = lineNumber;
        //bottom star info
        mvprintw(bottom_info_y++, bottom_info_x, "---  Mass: %.2e SM  ---  Speed: %6.2f km/s  ---", star.mass, std::sqrt(star.vx * star.vx + star.vy * star.vy + star.vz * star.vz) * AUtoKM / 1000);
        mvprintw(bottom_info_y++, bottom_info_x, "---  Distance from star: N/A AU  ---");
        mvprintw(bottom_info_y++, bottom_info_x, "--- X Position: %.2f AU  ---  X Velocity: %.2f km/s ---", star.x, star.vx * AUtoKM / 1000);
        mvprintw(bottom_info_y++, bottom_info_x, "--- Y Position: %.2f AU  ---  Y Velocity: %.2f km/s ---", star.y, star.vy * AUtoKM / 1000);
        mvprintw(bottom_info_y++, bottom_info_x, "--- Z Position: %.2f AU  ---  Z Velocity: %.2f km/s ---", star.z, star.vz * AUtoKM / 1000);
    } else {
        // Planet
        const Planet& planet = planets[highlightedObjectIndex];
        float dx = planet.x - star.x;
        float dy = planet.y - star.y;
        float dz = planet.z - star.z;
        float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
        float speed = std::sqrt(planet.vx * planet.vx + planet.vy * planet.vy + planet.vz * planet.vz);
        attron(COLOR_PAIR(planet.color));
        mvprintw(0, info_x, "---  %s  ---", planet.name.c_str());
        attroff(COLOR_PAIR(planet.color));
        std::vector<std::string> descLines = breakStringIntoLines(planet.desc, maxWidth);
        int lineNumber = 0;
        info_y = 1;
        mvprintw(1, info_x, "Name Origin: ");
        for (const std::string &line : descLines) {
            mvprintw(info_y + lineNumber + 1, info_x, "%s", line.c_str());
            lineNumber++;
        }
        info_y += lineNumber + 1;
        mvprintw(info_y, info_x, "---Planet Type: %s", planet.type.c_str());
        mvprintw(info_y++, info_x, "%s", planet.typedesc.c_str());
        //bottom planet info
        mvprintw(bottom_info_y++, bottom_info_x, "---  Mass: %.2e SM  ---  Speed: %6.2f km/s  ---", planet.mass, speed * AUtoKM / 1000);
        mvprintw(bottom_info_y++, bottom_info_x, "---  Distance from star: %3.2f AU  ---", distance);
        mvprintw(bottom_info_y++, bottom_info_x, "---X Position: %3.2f AU  ---  X Velocity: %6.2f km/s---", planet.x, planet.vx * AUtoKM / 1000);
        mvprintw(bottom_info_y++, bottom_info_x, "---Y Position: %3.2f AU  ---  Y Velocity: %6.2f km/s---", planet.y, planet.vy * AUtoKM / 1000);
        mvprintw(bottom_info_y++, bottom_info_x, "---Z Position: %3.2f AU  ---  Z Velocity: %6.2f km/s---", planet.z, planet.vz * AUtoKM / 1000);
    }
}

std::vector<std::string> breakStringIntoLines(const std::string &input, size_t max_width) {
    std::vector<std::string> lines;
    size_t start = 0;
    size_t end;

    while (start < input.length()) {
        end = input.find(' ', start + max_width - 1);
        if (end == std::string::npos || end >= input.length()) {
            end = input.length();
        } else {
            while (end > start && input[end] != ' ') {
                --end;
            }
        }
        lines.push_back(input.substr(start, end - start));
        start = end + 1;
    }

    return lines;
}
