#include "grid.h"
#include "pathfinder.h"
#include <iostream>
#include <vector>

// -------------------------------------
// DISPLAY GRID WITH PATH OVERLAY
// -------------------------------------
void displayGridWithPath(Grid& gridManager, const std::vector<Point>& path) {
    // Create a temporary display grid
    char display[ROWS][COLS];

    // Initialize display grid from cell types
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            CellType type = gridManager.getNode(i, j).type;
            switch (type) {
                case EMPTY: display[i][j] = '.'; break;
                case OBSTACLE: display[i][j] = '#'; break;
                case START: display[i][j] = 'S'; break;
                case GOAL: display[i][j] = 'G'; break;
                case CHARGING_STATION: display[i][j] = 'C'; break;
            }
        }
    }

    // Overlay path (do NOT overwrite start/goal/charging)
    for (const auto& p : path) {
        if (display[p.x][p.y] == '.') {
            display[p.x][p.y] = '*';
        }
    }

    // Print grid
    std::cout << "\n--- GRID VIEW ---\n";
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            std::cout << display[i][j] << " ";
        }
        std::cout << "\n";
    }
}

// -------------------------------------
// MENU DISPLAY
// -------------------------------------
void showMenu() {
    std::cout << "\n===== PATHFINDING SIMULATOR =====\n";
    std::cout << "1. Display Grid\n";
    std::cout << "2. Run BFS\n";
    std::cout << "3. Run A*\n";
    std::cout << "4. Compare BFS vs A*\n";
    std::cout << "5. Exit\n";
    std::cout << "Enter choice: ";
}

// -------------------------------------
// MAIN FUNCTION 
// -------------------------------------
int main() {
    Grid gridManager;
    Pathfinder pathfinder;

    // Define start & goal
    Point start = {0, 0};
    Point goal  = {ROWS - 1, COLS - 1};

    gridManager.setStart(start);
    gridManager.setGoal(goal);

    // Generate obstacles for testing
    gridManager.generateRandomObstacles(60);

    int choice;
    do {
        showMenu();
        std::cin >> choice;

        if (choice == 1) {
            gridManager.printGrid();
        }

        else if (choice == 2) {
            std::cout << "\nRunning BFS...\n";
            bool found = pathfinder.runBFS(start, goal, gridManager);

            if (found) {
                auto path = pathfinder.getPath(goal, gridManager);
                std::cout << "BFS Path Length: " << path.size() << "\n";
                displayGridWithPath(gridManager, path);
            } else {
                std::cout << "No path found using BFS.\n";
            }
        }

        else if (choice == 3) {
            std::cout << "\nRunning A*...\n";
            bool found = pathfinder.runAStar(start, goal, gridManager);

            if (found) {
                auto path = pathfinder.getPath(goal, gridManager);
                std::cout << "A* Path Length: " << path.size() << "\n";
                displayGridWithPath(gridManager, path);
            } else {
                std::cout << "No path found using A*.\n";
            }
        }

        else if (choice == 4) {
            std::cout << "\n--- COMPARISON MODE ---\n";

            bool bfsFound = pathfinder.runBFS(start, goal, gridManager);
            auto bfsPath = pathfinder.getPath(goal, gridManager);

            bool astarFound = pathfinder.runAStar(start, goal, gridManager);
            auto astarPath = pathfinder.getPath(goal, gridManager);

            std::cout << "\nAlgorithm | Path Found | Steps\n";
            std::cout << "--------------------------------\n";
            std::cout << "BFS       | " 
                      << (bfsFound ? "Yes" : "No") 
                      << "        | " 
                      << bfsPath.size() << "\n";

            std::cout << "A*        | " 
                      << (astarFound ? "Yes" : "No") 
                      << "        | " 
                      << astarPath.size() << "\n";
        }

        else if (choice == 5) {
            std::cout << "Exiting program.\n";
        }

        else {
            std::cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 5);

    return 0;
}
