#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <map>
#include <string>
#include <SDL2/SDL.h>
#include "maze.hpp"
#include "mazeRenderer.hpp"

#define WIDTH 800
#define HEIGHT 800

using namespace std;

int main(int argv, char **args)
{
    //Sfrutta argomenti
    int width = 20, height = 20, size = 10;
    cout << "Enter width, height and cell size of the maze: ";
    cin >> width >> height >> size;

    srand(time(NULL));
    Maze maze(width, height);
    MazeRenderer renderer = MazeRenderer(0, 0, width, height, size, WIDTH, HEIGHT);

    renderer.drawMaze(maze);
    while (renderer.eventCatch(&maze));
    return 0;
}
