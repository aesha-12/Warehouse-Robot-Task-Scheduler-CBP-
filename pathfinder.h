#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "grid.h"      // Include Member B's code
#include <vector>
#include <queue>       // For BFS and A* (priority_queue)
#include <stack>       // For DFS
#include <cmath>       // For abs() in Manhattan distance
#include <algorithm>   // For std::reverse
#include <iostream>

class Pathfinder {
private:
    // -------------------------------------
    // DIRECTION ARRAYS (Up, Down, Left, Right)
    // -------------------------------------
    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};

    // Helper: Reset grid state before running a new algorithm
    // We need to access the raw grid from Member B to reset visited/parents
    void resetGridState(Grid& gridManager) {
        auto& grid = gridManager.getGrid(); // Access Member B's vector
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                grid[i][j].visited = false;
                grid[i][j].parent = nullptr;
                grid[i][j].g_cost = 0;
                grid[i][j].h_cost = 0;
                grid[i][j].f_cost = 0;
            }
        }
    }

    // Helper: Heuristic for A* (Manhattan Distance)
    int calculateHeuristic(Point a, Point b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    }

public:
    // -------------------------------------
    // 1. BREADTH-FIRST SEARCH (BFS)
    // -------------------------------------
    bool runBFS(Point startPos, Point goalPos, Grid& gridManager) {
        resetGridState(gridManager);

        std::queue<Node*> q;
        Node& startNode = gridManager.getNode(startPos.x, startPos.y);
        
        startNode.visited = true;
        q.push(&startNode);

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();

            // Check if we reached the goal
            if (current->pos == goalPos) {
                return true; // Path found
            }

            // Explore neighbors
            for (int i = 0; i < 4; ++i) {
                int newX = current->pos.x + dx[i];
                int newY = current->pos.y + dy[i];

                // Check Bounds (Member B function) & Obstacles (Member B function)
                if (gridManager.isValid(newX, newY) && 
                   !gridManager.isBlocked(newX, newY)) {
                    
                    Node& neighbor = gridManager.getNode(newX, newY);

                    if (!neighbor.visited) {
                        neighbor.visited = true;
                        neighbor.parent = current; // Link for path reconstruction
                        q.push(&neighbor);
                    }
                }
            }
        }
        return false; // No path found
    }

    // -------------------------------------
    // 2. DEPTH-FIRST SEARCH (DFS)
    // -------------------------------------
    bool runDFS(Point startPos, Point goalPos, Grid& gridManager) {
        resetGridState(gridManager);

        std::stack<Node*> s;
        Node& startNode = gridManager.getNode(startPos.x, startPos.y);

        startNode.visited = true;
        s.push(&startNode);

        while (!s.empty()) {
            Node* current = s.top();
            s.pop();

            if (current->pos == goalPos) {
                return true;
            }

            // Explore neighbors
            for (int i = 0; i < 4; ++i) {
                int newX = current->pos.x + dx[i];
                int newY = current->pos.y + dy[i];

                if (gridManager.isValid(newX, newY) && 
                   !gridManager.isBlocked(newX, newY)) {
                    
                    Node& neighbor = gridManager.getNode(newX, newY);

                    if (!neighbor.visited) {
                        neighbor.visited = true;
                        neighbor.parent = current;
                        s.push(&neighbor);
                    }
                }
            }
        }
        return false;
    }

    // -------------------------------------
    // 3. A* ALGORITHM (Smart Search)
    // -------------------------------------
    bool runAStar(Point startPos, Point goalPos, Grid& gridManager) {
        resetGridState(gridManager);

        // Comparator for Priority Queue (Min-Heap based on f_cost)
        auto cmp = [](Node* left, Node* right) {
            return left->f_cost > right->f_cost; 
        };
        std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> openSet(cmp);

        Node& startNode = gridManager.getNode(startPos.x, startPos.y);
        startNode.g_cost = 0;
        startNode.h_cost = calculateHeuristic(startNode.pos, goalPos);
        startNode.f_cost = startNode.g_cost + startNode.h_cost;
        
        openSet.push(&startNode);

        // Note: In A*, we don't mark 'visited' immediately upon pushing.
        // We often use a 'closed set', but here we can check visited when popping
        // or ensure we update costs if we find a better path.
        // For this simple grid, simple visited logic often suffices if g_cost check is strict.

        while (!openSet.empty()) {
            Node* current = openSet.top();
            openSet.pop();

            // If already visited with a lower cost, skip (simplified logic)
            if (current->visited) continue;
            current->visited = true;

            if (current->pos == goalPos) {
                return true;
            }

            for (int i = 0; i < 4; ++i) {
                int newX = current->pos.x + dx[i];
                int newY = current->pos.y + dy[i];

                if (gridManager.isValid(newX, newY) && 
                   !gridManager.isBlocked(newX, newY)) {

                    Node& neighbor = gridManager.getNode(newX, newY);
                    
                    // BATTERY CONSTRAINT CHECK (Optional but requested)
                    // If moving here exceeds MAX_BATTERY, we can't go here.
                    if (current->g_cost + 1 > MAX_BATTERY) {
                        continue; 
                    }

                    int newGCost = current->g_cost + 1; // Assumes 1 cost per move

                    // If neighbor not visited or we found a shorter path to it
                    if (!neighbor.visited || newGCost < neighbor.g_cost) {
                        neighbor.g_cost = newGCost;
                        neighbor.h_cost = calculateHeuristic(neighbor.pos, goalPos);
                        neighbor.f_cost = neighbor.g_cost + neighbor.h_cost;
                        neighbor.parent = current;
                        
                        openSet.push(&neighbor);
                    }
                }
            }
        }
        return false;
    }

    // -------------------------------------
    // 4. RETRIEVE PATH
    // -------------------------------------
    // Backtracks from Goal -> Start using parent pointers
    std::vector<Point> getPath(Point goalPos, Grid& gridManager) {
        std::vector<Point> path;
        Node* current = &gridManager.getNode(goalPos.x, goalPos.y);

        // If goal wasn't reached or visited, return empty
        if (!current->visited && current->type != START) {
            return path; 
        }

        while (current != nullptr) {
            path.push_back(current->pos);
            current = current->parent;
        }

        // The path is currently Goal -> Start, so we reverse it
        std::reverse(path.begin(), path.end());
        return path;
    }
};

#endif
