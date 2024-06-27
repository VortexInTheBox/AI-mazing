#include "mazeRenderer.hpp"

const SDL_Color white = {255, 255, 255, 255};
const SDL_Color black = {0, 0, 0, 255};
const SDL_Color red = {255, 0, 0, 255};
const SDL_Color blue = {0, 0, 255, 255};
const SDL_Color green = {0, 255, 0, 255};

MazeRenderer::MazeRenderer(int x, int y, int x_cells, int y_cells, int cell_size, int windowWidth, int windowHeight)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("Maze", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_ALLOW_HIGHDPI);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    mazeGrid = {x, x, x_cells, y_cells, cell_size, white, black};
    start = {0, 0};
    goal = {0, 0};
}

bool MazeRenderer::eventCatch(Maze *maze)
{
    // SDL_RenderClear(renderer);
    if (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            return false;
            break;

        case SDL_MOUSEBUTTONDOWN:
        {
            drawMaze(*maze);
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
            {
                start = {event.button.x, event.button.y};
                break;
            }
            case SDL_BUTTON_RIGHT:
            {
                goal = {event.button.x, event.button.y};
                break;
            }
            default:
                break;
            }

            drawRect(start, red);
            drawRect(goal, blue);

            break;
        }

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_SPACE:
            {
                maze->generate(0, 0);
                drawMaze(*maze);
                break;
            }
            case SDLK_r:
            {
                std::pair<int, int> normalizedStart = std::make_pair(((start.y - mazeGrid.y) / mazeGrid.cell_size), ((start.x - mazeGrid.x) / mazeGrid.cell_size));
                std::pair<int, int> normalizedGoal = std::make_pair(((goal.y - mazeGrid.y) / mazeGrid.cell_size), ((goal.x - mazeGrid.x) / mazeGrid.cell_size));
                
                std::vector<std::pair<int, int>> path = maze->solve(normalizedStart, normalizedGoal);
                printf("start: %d - %d \t goal: %d - %d \n", normalizedStart.first, normalizedStart.second, normalizedGoal.first, normalizedGoal.second);
                printf("path lenght: %d\n", path.size());

                for (unsigned int i = 0; i < path.size(); i++)
                {
                    //printf("%d - %d\n", path[i].first, path[i].second);
                    drawRect({mazeGrid.y + (path[i].second * mazeGrid.cell_size), mazeGrid.x + (path[i].first * mazeGrid.cell_size)}, green);
                }

                drawRect(start, red);
                drawRect(goal, blue);
                
            }

            default:
                break;
            }

            break;

        default:
            break;
        }
    }

    SDL_RenderPresent(renderer);
    return true;
}

void MazeRenderer::drawMaze(Maze maze)
{
    int width = (mazeGrid.x_cells * mazeGrid.cell_size);
    int height = (mazeGrid.y_cells * mazeGrid.cell_size);

    // Draw grid background.
    SDL_SetRenderDrawColor(renderer, mazeGrid.background_color.r, mazeGrid.background_color.g, mazeGrid.background_color.b, mazeGrid.background_color.a);
    SDL_RenderClear(renderer);

    // Draw grid lines.
    SDL_SetRenderDrawColor(renderer, mazeGrid.line_color.r, mazeGrid.line_color.g, mazeGrid.line_color.b, mazeGrid.line_color.a);

    SDL_RenderDrawLine(renderer, mazeGrid.x, mazeGrid.y, mazeGrid.x, mazeGrid.y + height);
    SDL_RenderDrawLine(renderer, mazeGrid.x, mazeGrid.y, mazeGrid.x + width, mazeGrid.y);

    SDL_RenderDrawLine(renderer, mazeGrid.x + width, mazeGrid.y, mazeGrid.x + width, mazeGrid.y + height);
    SDL_RenderDrawLine(renderer, mazeGrid.x, mazeGrid.y + height, mazeGrid.x + width, mazeGrid.y + height);

    // SDL_RenderDrawLine(renderer, 100, 100, 150, 100);

    for (int y = 0; y < mazeGrid.y_cells; y++)
    {
        for (int x = 0; x < mazeGrid.x_cells; x++)
        {
            if (y < mazeGrid.y_cells - 1)
            {
                if (maze.getVerticalWall(x, y))
                {
                    SDL_RenderDrawLine(renderer, (y * mazeGrid.cell_size) + mazeGrid.y + mazeGrid.cell_size, (x * mazeGrid.cell_size) + mazeGrid.x, (y * mazeGrid.cell_size) + mazeGrid.y + mazeGrid.cell_size, (x * mazeGrid.cell_size) + mazeGrid.x + mazeGrid.cell_size);
                }
            }
            if (x < mazeGrid.x_cells - 1)
            {
                if (maze.getHorizontalWall(x, y))
                {
                    SDL_RenderDrawLine(renderer, (y * mazeGrid.cell_size) + mazeGrid.y, (x * mazeGrid.cell_size) + mazeGrid.x + mazeGrid.cell_size, (y * mazeGrid.cell_size) + mazeGrid.y + mazeGrid.cell_size, (x * mazeGrid.cell_size) + mazeGrid.x + mazeGrid.cell_size);
                }
            }
        }
    }
}

void MazeRenderer::drawRect(SDL_Point screenPoint, SDL_Color color)
{
    SDL_Point point;
    point.x = ((screenPoint.x - mazeGrid.x) / mazeGrid.cell_size) * mazeGrid.cell_size;
    point.y = ((screenPoint.y - mazeGrid.y) / mazeGrid.cell_size) * mazeGrid.cell_size;

    SDL_Rect startRect = {mazeGrid.x + point.x + 1, mazeGrid.y + point.y + 1, mazeGrid.cell_size - 1, mazeGrid.cell_size - 1};

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &startRect);
}

MazeRenderer::~MazeRenderer()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    window = nullptr;
    renderer = nullptr;

    SDL_Quit();
}