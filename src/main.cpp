//textsolsim
//Project Start Date: 23.04.2023

#include "constants.h"
#include "structs.h"
#include "init.h"
#include "physics.h"
#include "display.h"

int main() {

    srand((time(0)));
    std::vector<Star> stars;
    std::vector<Planet> planets;
       
    //add sim loop
    float dt = 1; // Time step in seconds

    // Set up the ncurses window
    initscr();
    cbreak();
    curs_set(0);
    noecho();
    nodelay(stdscr, FALSE);
    keypad(stdscr, TRUE);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_BLUE, COLOR_BLACK);
        init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(6, COLOR_CYAN, COLOR_BLACK);
        init_pair(7, COLOR_WHITE, COLOR_BLACK);

    }

    // Get the screen size
    int screen_rows, screen_cols;
    getmaxyx(stdscr, screen_rows, screen_cols);

    // Add menu items to the window
    mvprintw(screen_rows / 2 - 5, screen_cols / 2 - 13, "--- Solar System Simulator ---");
    mvprintw(screen_rows / 2 - 3, screen_cols / 2 - 13, "Enter system type --");
    mvprintw(screen_rows / 2 - 1, screen_cols / 2 - 13, "(1) Random -- Control how many planets are simulated");
    mvprintw(screen_rows / 2, screen_cols / 2 - 13, "(2) The Solar System -- Simulate our own solar system");
    mvprintw(screen_rows / 2 + 1, screen_cols / 2 - 13, "Choice: ");
    refresh();

    
    bool valid = false;
    int numPlanets;

    while (!valid) {
        int choice;
        std::string input;
        while (true) {
            choice = getch();
            if (choice == '\n') {
                break;
            } else if (isdigit(choice)) {
                echochar(choice);
                input.push_back(choice);
            } else if (choice == KEY_BACKSPACE || choice == 127) {
                if (!input.empty()) {
                    input.pop_back();
                    int y, x;
                    getyx(stdscr, y, x);
                    mvdelch(y, x - 1);
                }
            }
        }

    int finalChoice = std::stoi(input);
    choice = finalChoice;
        switch (choice) {
            case 1: {
                mvprintw(screen_rows / 2 + 3, screen_cols / 2 - 13, "Enter number of planets to generate: ");
                refresh();
                bool validInput = false;
                std::string input;
                int numPlanets;
                while (!validInput) {
                    int choice;
                    while (true) {
                        choice = getch();
                        if (choice == '\n') {
                            break;
                        } else if (isdigit(choice)) {
                            echochar(choice);
                            input.push_back(choice);
                        } else if (choice == KEY_BACKSPACE || choice == 127) {
                            if (!input.empty()) {
                                input.pop_back();
                                int y, x;
                                getyx(stdscr, y, x);
                                mvdelch(y, x - 1);
                            }
                        }
                    }

                    std::istringstream inputConverter(input);
                    if (inputConverter >> numPlanets) {
                        validInput = true;
                    } else {
                        input.clear();
                        mvprintw(screen_rows / 2 + 4, screen_cols / 2 - 13, "Invalid input. Please enter a valid number: ");
                        refresh();
                    }
                }

                // Initialize the solar system
                init(numPlanets, stars, planets, choice);
                valid = true;
                break;
            }
            case 2: {
                init(9, stars, planets, choice);
                valid = true;
                break;
            }
            default: {
                mvprintw(screen_rows / 2 + 3, screen_cols / 2 - 13, "Invalid Input. Try again: ");
                refresh();
            }
        }
    }

    clear();
    refresh();
    curs_set(0);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    int ch;
    int highlightedObjectIndex = -1; // -1 for the star, 0 to planets.size() - 1 for planets

    while ((ch = getch()) != static_cast<int>('q')) {
        if (ch != ERR) {
            if (ch == KEY_DOWN) {
                highlightedObjectIndex++;
                if (highlightedObjectIndex >= static_cast<int>(planets.size())) {
                    highlightedObjectIndex = -1; // Loop back to the star
                }
            } else if (ch == KEY_UP) {
                highlightedObjectIndex--;
                if (highlightedObjectIndex < -1) {
                    highlightedObjectIndex = static_cast<int>(planets.size() - 1); // Loop back to the last planet
                }
            }
            flushinp();
        }

        applyGravity(stars[0], planets, dt);

        for (Star& star : stars) {
            updateStarPos(star, dt);
        }
        for (Planet& planet : planets) {
            updatePlanetPos(planet, dt);
        }
        displaySystem(stars[0], planets, highlightedObjectIndex);

        napms(15);
        
    } 
    endwin();
    
    return 0;
}