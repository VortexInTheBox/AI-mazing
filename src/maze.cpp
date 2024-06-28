#include "maze.hpp"

Maze::Maze(int w, int h) : width(w), height(h)
{
    visited.assign(height, std::vector<bool>(width, false));
    vertical_walls.assign(height, std::vector<bool>(width - 1, true));
    horizontal_walls.assign(height - 1, std::vector<bool>(width, true));
    cellState["free"] = ' ';
    cellState["wall"] = '#';
    cellState["border"] = '@';
}

void Maze::generate(int x, int y)
{
    visited.assign(height, std::vector<bool>(width, false));
    vertical_walls.assign(height, std::vector<bool>(width - 1, true));
    horizontal_walls.assign(height - 1, std::vector<bool>(width, true));

    visited[x][y] = true;
    std::stack<Coordinate2D> stack;
    stack.push(Coordinate2D(x, y));
    Coordinate2D current;

    while (!stack.empty())
    {
        current = stack.top();
        stack.pop();

        std::vector<Coordinate2D> neighbors = getUnvisitedNeighbors(current.x, current.y);

        if (!neighbors.empty())
        {
            x = current.x;
            y = current.y;
            stack.push(current);
            int i = rand() % neighbors.size();

            int nx = neighbors[i].x, ny = neighbors[i].y;

            if (ny == y)
            {
                horizontal_walls[std::min(x, nx)][y] = false;
            }
            else
            {
                vertical_walls[current.x][std::min(y, ny)] = false;
            }

            visited[nx][ny] = true;
            stack.push(Coordinate2D(nx, ny));
        }
    }
    removeRandomWalls();
}

// Removes (width + height) / 10 walls
void Maze::removeRandomWalls()
{
    int wallsToRemove = (width + height) / 10;
    for (int i = 0; i < wallsToRemove; i++)
    {
        int xIndex = 0;
        int yIndex = 0;

        do
        {
            xIndex = rand() % height;
            yIndex = rand() % (width - 1);
        } while (vertical_walls[xIndex][yIndex] == false);

        vertical_walls[xIndex][yIndex] = false;
        printf("Removed vWall: %d-%d", xIndex, yIndex);

        do
        {
            xIndex = rand() % (height - 1);
            yIndex = rand() % width;
        } while (vertical_walls[xIndex][yIndex] == false);
        
        horizontal_walls[xIndex][yIndex] = false;
        printf("\t Removed hWall: %d-%d \n", xIndex, yIndex);
    }
}

std::vector<Coordinate2D> Maze::getNeighbors(int x, int y)
{
    std::vector<Coordinate2D> neighbors;
    std::vector<Coordinate2D> d = {Coordinate2D(-1, 0), Coordinate2D(0, -1), Coordinate2D(1, 0), Coordinate2D(0, 1)};
    Coordinate2D neighbor;
    for (unsigned int i = 0; i < 4; i++)
    {
        neighbor = Coordinate2D(x + d[i].x, y + d[i].y);

        if ((neighbor.x >= 0 && neighbor.x < width) && (neighbor.y >= 0 && neighbor.y < height))
        {
            neighbors.push_back(neighbor);
        }
    }

    return neighbors;
}

std::vector<Coordinate2D> Maze::getUnvisitedNeighbors(int x, int y)
{
    std::vector<Coordinate2D> neighbors = getNeighbors(x, y);
    std::vector<Coordinate2D> unvisitedNeighbors;
    for (unsigned int i = 0; i < neighbors.size(); i++)
    {
        if (!visited[neighbors[i].x][neighbors[i].y])
        {
            unvisitedNeighbors.push_back(neighbors[i]);
        }
    }
    return unvisitedNeighbors;
}

std::vector<Coordinate2D> Maze::getAccessibleNeighbors(int x, int y)
{
    std::vector<Coordinate2D> neighbors = getNeighbors(x, y);
    std::vector<Coordinate2D> accessibleNeighbors;
    for (unsigned int i = 0; i < neighbors.size(); i++)
    {
        int nx = neighbors[i].x, ny = neighbors[i].y;

        if (!(ny == y && horizontal_walls[std::min(x, nx)][y]) && !(nx == x && vertical_walls[x][std::min(y, ny)]))
        {
            accessibleNeighbors.push_back(neighbors[i]);
        }
    }
    return accessibleNeighbors;
}

/*
std::vector<Coordinate2D> Maze::getUnvisitedAccessibleNeighbors(int x, int y)
{
    std::vector<Coordinate2D> neighbors = getNeighbors(x, y);
    std::vector<Coordinate2D> unvisitedAccessibleNeighbors;
    for (unsigned int i = 0; i < neighbors.size(); i++)
    {
        int nx = neighbors[i].x, ny = neighbors[i].y;

        bool isUnvisited = !visited[nx][ny];
        bool isAccessible = !(ny == y && horizontal_walls[std::min(x, nx)][y]) && !(nx == x && vertical_walls[x][std::min(y, ny)]);

        if (isUnvisited && isAccessible)
        {
            unvisitedAccessibleNeighbors.push_back(neighbors[i]);
        }
    }
    return unvisitedAccessibleNeighbors;
}
*/

bool Maze::getVerticalWall(int x, int y)
{
    return vertical_walls[x][y];
}

bool Maze::getHorizontalWall(int x, int y)
{
    return horizontal_walls[x][y];
}

void Maze::print()
{
    std::cout << cellState["wall"];
    for (int i = 0; i < width; ++i)
    {
        std::cout << cellState["wall"] << cellState["wall"];
    }
    std::cout << std::endl;

    for (int x = 0; x < height; ++x)
    {
        std::cout << cellState["wall"];
        for (int y = 0; y < width; ++y)
        {
            std::cout << " "; // cell
            if (y < width - 1)
            {
                if (vertical_walls[x][y])
                    std::cout << cellState["wall"]; // wall
                else
                    std::cout << cellState["free"]; // free passage
            }
        }
        std::cout << cellState["wall"] << std::endl;

        if (x < height - 1)
        {
            std::cout << cellState["wall"];
            for (int y = 0; y < width; ++y)
            {
                if (horizontal_walls[x][y])
                    std::cout << cellState["wall"] << cellState["wall"];
                else
                    std::cout << " " << cellState["wall"];
            }
            std::cout << std::endl;
            // cout << cellState["border"] << endl;
        }
    }

    std::cout << cellState["wall"];
    for (int i = 0; i < width; ++i)
    {
        std::cout << cellState["wall"] << cellState["wall"];
    }
    std::cout << std::endl
              << std::endl;
}

std::vector<Coordinate2D> Maze::rebuildPath(std::map<Coordinate2D, Coordinate2D> cameFrom, Coordinate2D start, Coordinate2D end)
{
    std::vector<Coordinate2D> path;

    Coordinate2D current = end;

    while (current != start)
    {
        path.push_back(current);
        current = cameFrom[current];
    }
    path.push_back(start);
    return path;
}

std::vector<Coordinate2D> Maze::solve(Coordinate2D start, Coordinate2D goal)
{
    std::priority_queue<std::pair<int, Coordinate2D>, std::vector<std::pair<int, Coordinate2D>>, Compare> frontier;
    frontier.push(std::make_pair(0, start));

    // Creiamo le matrici di f e di g grandi height x width, inizializzate ad infinito, ovvero non conosciamo la distanza tra lo start e ogni singolo nodo
    std::vector<std::vector<float>> f(height, std::vector<float>(width, std::numeric_limits<float>::infinity()));
    std::vector<std::vector<float>> g(height, std::vector<float>(width, std::numeric_limits<float>::infinity()));

    // Mappa utilizzata per ricostruire il percorso seguito
    std::map<Coordinate2D, Coordinate2D> cameFrom;

    f[start.x][start.y] = h(start, goal);
    g[start.x][start.y] = 0;

    Coordinate2D current;

    while (!frontier.empty())
    {
        current = frontier.top().second;
        frontier.pop();

        if (current == goal)
            return rebuildPath(cameFrom, start, current);

        std::vector<Coordinate2D> neighbors = getAccessibleNeighbors(current.x, current.y);
        for (unsigned int i = 0; i < neighbors.size(); i++)
        {

            int x = current.x, y = current.y;
            int nx = neighbors[i].x, ny = neighbors[i].y;

            float new_g = g[x][y] + 1;
            if (new_g < g[nx][ny])
            {
                g[nx][ny] = new_g;
                f[nx][ny] = g[nx][ny] + h(current, goal);
                frontier.push(std::make_pair(f[nx][ny], neighbors[i]));
                cameFrom[neighbors[i]] = current;
                // std::cout << neighbors[i].x << " - " << neighbors[i].y << " = " << current.x << " - " << current.y << std::endl;
            }
        }
    }
    return rebuildPath(cameFrom, start, current);
}

float Maze::h(Coordinate2D current, Coordinate2D goal)
{
    return sqrt(pow((goal.x - current.x), 2) + pow((goal.y - current.y), 2));
}
