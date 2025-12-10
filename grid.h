#ifndef GRID_H
#define GRID_H

#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

// -------------------------------------
// 1. GLOBAL CONSTANTS
// -------------------------------------

const int ROWS = 20;
const int COLS = 20;

const int MAX_BATTERY = 100;
const int MOVE_COST = 1;
const int RECHARGE_COST = 10;

// -------------------------------------
// 2. ENUMS
// -------------------------------------
enum CellType {
    EMPTY,
    OBSTACLE,
    START,
    GOAL,
    CHARGING_STATION
};

// -------------------------------------
// 3A. STRUCT: Point (Coordinate System)
// -------------------------------------
struct Point {
    int x, y;

    // Overload == so Member A can do comparisons easily
    bool operator==(const Point &other) const {
        return (x == other.x && y == other.y);
    }
};

// -------------------------------------
// 3B. STRUCT: Node (Each Grid Cell)
// -------------------------------------
struct Node {
    Point pos;
    CellType type = EMPTY;

    int g_cost = 0;
    int h_cost = 0;
    int f_cost = 0;

    Node* parent = nullptr;
    bool visited = false;
};

// -------------------------------------
// 4 & 5. CLASS: Grid Management (Member B)
// -------------------------------------
class Grid {
private:
    std::vector<std::vector<Node>> grid;    // 2D grid of Nodes

public:

    // Constructor → Initializes grid
    Grid() {
        grid.resize(ROWS, std::vector<Node>(COLS));
        initGrid();
    }

    // -------------------------------------
    // INITIALIZATION OF GRID
    // -------------------------------------
    void initGrid() {
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                grid[i][j].pos = {i, j};
                grid[i][j].type = EMPTY;
                grid[i][j].parent = nullptr;
                grid[i][j].visited = false;
            }
        }
    }

    // -------------------------------------
    // RANDOM OBSTACLE GENERATION
    // -------------------------------------
    void generateRandomObstacles(int count) {
        srand(time(NULL));

        while (count > 0) {
            int r = rand() % ROWS;
            int c = rand() % COLS;

            if (grid[r][c].type == EMPTY) {
                grid[r][c].type = OBSTACLE;
                count--;
            }
        }
    }

    // -------------------------------------
    // USER INPUT OBSTACLE PLACEMENT
    // -------------------------------------
    void setObstacle(int r, int c) {
        if (isValid(r, c)) {
            grid[r][c].type = OBSTACLE;
        }
    }

    void setStart(Point p) {
        grid[p.x][p.y].type = START;
    }

    void setGoal(Point p) {
        grid[p.x][p.y].type = GOAL;
    }

    void setChargingStation(Point p) {
        grid[p.x][p.y].type = CHARGING_STATION;
    }

    // -------------------------------------
    // REQUIRED FUNCTIONS FOR MEMBER A
    // -------------------------------------

    // Check boundaries
    bool isValid(int r, int c) const {
        return (r >= 0 && r < ROWS && c >= 0 && c < COLS);
    }

    // Check if blocked
    bool isBlocked(int r, int c) const {
        if (!isValid(r, c)) return true;
        return (grid[r][c].type == OBSTACLE);
    }

    // Allow Member A to access a Node
    Node& getNode(int r, int c) {
        return grid[r][c];
    }

    // Getter for full grid
    std::vector<std::vector<Node>>& getGrid() {
        return grid;
    }

    // -------------------------------------
    // OPTIONAL: PRINT GRID FOR TESTING
    // -------------------------------------
    void printGrid() {
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                switch (grid[i][j].type) {
                    case EMPTY: std::cout << "."; break;
                    case OBSTACLE: std::cout << "#"; break;
                    case START: std::cout << "S"; break;
                    case GOAL: std::cout << "G"; break;
                    case CHARGING_STATION: std::cout << "C"; break;
                }
            }
            std::cout << "\n";
        }
    }
};

#endif
