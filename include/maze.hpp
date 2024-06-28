#ifndef MAZE_HPP
#define MAZE_HPP

#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <map>
#include <string>
#include <cmath>
#include <queue>
#include <limits>

struct Coordinate2D {
    int x;
    int y;

    Coordinate2D(int x = 0, int y = 0) : x(x), y(y) {}

    double distance(Coordinate2D& other) {
        return std::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
    }

    void print() {
        std::cout << "(" << x << ", " << y << ")";
    }

    bool operator==(Coordinate2D& other) {
        return x == other.x && y == other.y;
    }

    bool operator!=(Coordinate2D& other) {
        return !(*this == other);
    }
    
    bool operator<(const Coordinate2D& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }
};

struct Compare {
    bool operator()(const std::pair<int, Coordinate2D>& a, const std::pair<int, Coordinate2D>& b) const {
        return a.first > b.first; // Min-heap: change to < for max-heap
    }
};

class Maze
{
private:
    int width;
    int height;
    std::vector<std::vector<bool>> visited;
    std::vector<std::vector<bool>> vertical_walls;
    std::vector<std::vector<bool>> horizontal_walls;
    std::map<std::string, char> cellState;

public:
    Maze(int w, int h);
    void generate(int x, int y);
    std::vector<Coordinate2D> getNeighbors(int x, int y);
    std::vector<Coordinate2D> getUnvisitedNeighbors(int x, int y);
    std::vector<Coordinate2D> getAccessibleNeighbors(int x, int y);
    std::vector<Coordinate2D> getUnvisitedAccessibleNeighbors(int x, int y);
    bool getVerticalWall(int x, int y);
    bool getHorizontalWall(int x, int y);
    void print();
    std::vector<Coordinate2D> rebuildPath(std::map<Coordinate2D, Coordinate2D> cameFrom, Coordinate2D start, Coordinate2D end);
    std::vector<Coordinate2D> solve(Coordinate2D start, Coordinate2D goal);
    static float h(Coordinate2D current, Coordinate2D goal);
    void removeRandomWalls();
};

#endif