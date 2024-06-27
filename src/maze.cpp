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
    std::stack<std::pair<int, int>> stack;
    stack.push(std::make_pair(x, y));
    std::pair<int, int> current;

    while (!stack.empty())
    {
        current = stack.top();
        stack.pop();

        std::vector<std::pair<int, int>> neighbors = getUnvisitedNeighbors(current.first, current.second);

        if (!neighbors.empty())
        {
            x = current.first;
            y = current.second;
            stack.push(current);
            int index = rand() % neighbors.size();

            int nx = neighbors[index].first;
            int ny = neighbors[index].second;

            if (ny == y)
            { // Same column
                horizontal_walls[std::min(x, nx)][y] = false;
            }
            else
            { // Same row
                vertical_walls[current.first][std::min(y, ny)] = false;
            }

            visited[nx][ny] = true;
            stack.push(std::make_pair(nx, ny));
        }
    }
}

std::vector<std::pair<int, int>> Maze::getNeighbors(int x, int y)
{
    std::vector<std::pair<int, int>> neighbors;
    std::vector<std::pair<int, int>> d = {std::make_pair(-1, 0), std::make_pair(0, -1), std::make_pair(1, 0), std::make_pair(0, 1)};
    std::pair<int, int> neighbor;
    for (unsigned int i = 0; i < 4; i++)
    {
        neighbor = std::make_pair(x + d[i].first, y + d[i].second);

        if ((neighbor.first >= 0 && neighbor.first < width) && (neighbor.second >= 0 && neighbor.second < height))
        {
            neighbors.push_back(neighbor);
        }
    }

    return neighbors;
}

std::vector<std::pair<int, int>> Maze::getUnvisitedNeighbors(int x, int y)
{
    std::vector<std::pair<int, int>> neighbors = getNeighbors(x, y);
    std::vector<std::pair<int, int>> unvisitedNeighbors;
    for (unsigned int i = 0; i < neighbors.size(); i++)
    {
        if (!visited[neighbors[i].first][neighbors[i].second])
        {
            unvisitedNeighbors.push_back(neighbors[i]);
        }
    }
    return unvisitedNeighbors;
}

std::vector<std::pair<int, int>> Maze::getAccessibleNeighbors(int x, int y)
{

    std::vector<std::pair<int, int>> neighbors = getNeighbors(x, y);
    std::vector<std::pair<int, int>> accessibleNeighbors;
    for (unsigned int i = 0; i < neighbors.size(); i++)
    {
        int nx = neighbors[i].first;
        int ny = neighbors[i].second;

        if (ny == y && horizontal_walls[std::min(x, nx)][y])
        {
            continue;
        }
        else if (nx == x && vertical_walls[x][std::min(y, ny)])
        {
            continue;
        }

        accessibleNeighbors.push_back(neighbors[i]);

    }
    return accessibleNeighbors;
}

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

std::vector<std::pair<int, int>> Maze::rebuildPath(std::map<std::pair<int, int>, std::pair<int, int>> cameFrom, std::pair<int, int> start, std::pair<int, int> end)
{
    std::vector<std::pair<int, int>> path;

    std::pair<int, int> current = end;

    while (current != start)
    {
        path.push_back(current);
        current = cameFrom[current];
    }
    path.push_back(start);
    return path;
}

std::vector<std::pair<int, int>> Maze::solve(std::pair<int, int> start, std::pair<int, int> goal)
{
    std::priority_queue<std::pair<int, std::pair<int, int>>> frontier;
    frontier.push(make_pair(0, start));

    // Creiamo le matrici di f e di g grandi height x width, inizializzate ad infinito, ovvero non conosciamo la distanza tra lo start e ogni singolo nodo
    std::vector<std::vector<float>> f(height, std::vector<float>(width, std::numeric_limits<float>::infinity()));
    std::vector<std::vector<float>> g(height, std::vector<float>(width, std::numeric_limits<float>::infinity()));

    // Mappa utilizzata per ricostruire il percorso seguito
    std::map<std::pair<int, int>, std::pair<int, int>> cameFrom;

    f[start.first][start.second] = h(start, goal);
    g[start.first][start.second] = 0;

    std::pair<int, int> current;

    while (!frontier.empty())
    {
        current = frontier.top().second;
        frontier.pop();

        if (current == goal)
            return rebuildPath(cameFrom, start, current);

        std::vector<std::pair<int, int>> neighbors = getAccessibleNeighbors(current.first, current.second);
        for (unsigned int i = 0; i < neighbors.size(); i++)
        {

            int x = current.first;
            int y = current.second;

            int nx = neighbors[i].first;
            int ny = neighbors[i].second;

            float new_g = g[x][y] + 1;
            if (new_g < g[nx][ny])
            {
                g[nx][ny] = new_g;
                f[nx][ny] = g[nx][ny] + h(current, goal);
                frontier.push(make_pair(f[nx][ny], neighbors[i]));
                cameFrom[neighbors[i]] = current;
                // std::cout << neighbors[i].first << " - " << neighbors[i].second << " = " << current.first << " - " << current.second << std::endl;
            }
        }
    }
    return rebuildPath(cameFrom, start, current);
}

float Maze::h(std::pair<int, int> current, std::pair<int, int> goal)
{
    return sqrt(pow((goal.first - current.first), 2) + pow((goal.second - current.second), 2));
}
