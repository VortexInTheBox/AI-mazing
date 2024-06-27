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
    std::vector<std::pair<int, int>> getNeighbors(int x, int y);
    std::vector<std::pair<int, int>> getUnvisitedNeighbors(int x, int y);
    bool getVerticalWall(int x, int y);
    bool getHorizontalWall(int x, int y);
    void print();
    std::vector<std::pair<int, int>> solve(std::pair<int, int> start, std::pair<int, int> goal);
    std::vector<std::pair<int, int>> getAccessibleNeighbors(int x, int y);
};

#endif