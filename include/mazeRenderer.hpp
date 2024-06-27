#ifndef MAZE_RENDERER_HPP
#define MAZE_RENDERER_HPP

#include <iostream>
#include <cstdlib>
#include <SDL2/SDL.h>
#include "maze.hpp"

typedef struct
{
    int x;
    int y;
    int x_cells;
    int y_cells;
    int cell_size;
    SDL_Color background_color;
    SDL_Color line_color;
} grid;

class MazeRenderer
{
private:
    grid mazeGrid;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    SDL_Point start;
    SDL_Point goal;
    int windowHeight;
    int windowWidth;

public:
    MazeRenderer(int x, int y, int x_cells, int y_cells, int cell_size, int windowWidth, int windowHeight);
    bool eventCatch(Maze *maze);
    void drawGrid();
    void drawMaze(Maze maze);
    void drawRect(SDL_Point screenPoint, SDL_Color color);
    ~MazeRenderer();
};

#endif