// BFS 选型模板：连通块 / 单点弹出距离 / 分层 / 多源 / 0-1 BFS / 优先队列
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <array>
#include <climits>
#include <deque>
#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

constexpr array<int, 4> ROW_OFFSET = {1, -1, 0, 0};
constexpr array<int, 4> COLUMN_OFFSET = {0, 0, 1, -1};

struct GridPosition {
    int row;
    int column;
};

struct WeightedState {
    int row;
    int column;
    int distance;
};

struct WaterState {
    int waterLevel;
    int row;
    int column;
};

struct WaterStateGreater {
    bool operator()(const WaterState& left, const WaterState& right) const {
        return left.waterLevel > right.waterLevel;
    }
};

bool isInside(int row, int column, int rowCount, int columnCount) {
    return 0 <= row && row < rowCount &&
           0 <= column && column < columnCount;
}

// 只问连通性：单点弹出，不需要 layerSize 或 distance。
int countReachableVertices(const vector<vector<int>>& graph, int source) {
    if (source < 0 || source >= static_cast<int>(graph.size())) {
        return 0;
    }

    vector<char> visited(graph.size(), false);
    queue<int> pending;
    visited[source] = true;
    pending.push(source);

    int reachedCount = 0;
    while (!pending.empty()) {
        int current = pending.front();
        pending.pop();
        ++reachedCount;

        for (int next : graph[current]) {
            if (visited[next]) {
                continue;
            }
            visited[next] = true;
            pending.push(next);
        }
    }
    return reachedCount;
}

// 需要距离但不需要按层分组：仍然是单点弹出，用 dist 保存距离。
vector<int> shortestDistanceFrom(const vector<vector<int>>& graph,
                                 int source) {
    vector<int> distance(graph.size(), -1);
    if (source < 0 || source >= static_cast<int>(graph.size())) {
        return distance;
    }

    queue<int> pending;
    distance[source] = 0;
    pending.push(source);

    while (!pending.empty()) {
        int current = pending.front();
        pending.pop();

        for (int next : graph[current]) {
            if (distance[next] != -1) {
                continue;
            }
            distance[next] = distance[current] + 1;
            pending.push(next);
        }
    }
    return distance;
}

// 服务器通信：同一行/列串成连通块，最后统计大小大于 1 的块。
int countCommunicatingServers(const vector<vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) {
        return 0;
    }

    int rowCount = static_cast<int>(grid.size());
    int columnCount = static_cast<int>(grid[0].size());
    int cellCount = rowCount * columnCount;
    vector<vector<int>> adjacency(cellCount);

    for (int row = 0; row < rowCount; ++row) {
        int previousServer = -1;
        for (int column = 0; column < columnCount; ++column) {
            if (grid[row][column] == 0) {
                continue;
            }
            int currentServer = row * columnCount + column;
            if (previousServer != -1) {
                adjacency[previousServer].push_back(currentServer);
                adjacency[currentServer].push_back(previousServer);
            }
            previousServer = currentServer;
        }
    }

    for (int column = 0; column < columnCount; ++column) {
        int previousServer = -1;
        for (int row = 0; row < rowCount; ++row) {
            if (grid[row][column] == 0) {
                continue;
            }
            int currentServer = row * columnCount + column;
            if (previousServer != -1) {
                adjacency[previousServer].push_back(currentServer);
                adjacency[currentServer].push_back(previousServer);
            }
            previousServer = currentServer;
        }
    }

    vector<char> visited(cellCount, false);
    int communicatingCount = 0;
    for (int row = 0; row < rowCount; ++row) {
        for (int column = 0; column < columnCount; ++column) {
            int source = row * columnCount + column;
            if (grid[row][column] == 0 || visited[source]) {
                continue;
            }

            int componentSize = 0;
            queue<int> pending;
            visited[source] = true;
            pending.push(source);

            while (!pending.empty()) {
                int current = pending.front();
                pending.pop();
                ++componentSize;

                for (int next : adjacency[current]) {
                    if (visited[next]) {
                        continue;
                    }
                    visited[next] = true;
                    pending.push(next);
                }
            }

            if (componentSize > 1) {
                communicatingCount += componentSize;
            }
        }
    }
    return communicatingCount;
}

// 需要明确的层次语义：固定当前层大小，第一次弹出目标时返回层号。
int shortestDistanceLayered(const vector<vector<int>>& graph,
                            int source,
                            int target) {
    int vertexCount = static_cast<int>(graph.size());
    if (source < 0 || source >= vertexCount ||
        target < 0 || target >= vertexCount) {
        return -1;
    }
    if (source == target) {
        return 0;
    }

    vector<char> visited(vertexCount, false);
    queue<int> pending;
    visited[source] = true;
    pending.push(source);

    int level = 0;
    while (!pending.empty()) {
        int layerSize = static_cast<int>(pending.size());
        for (int index = 0; index < layerSize; ++index) {
            int current = pending.front();
            pending.pop();
            if (current == target) {
                return level;
            }

            for (int next : graph[current]) {
                if (visited[next]) {
                    continue;
                }
                visited[next] = true;
                pending.push(next);
            }
        }
        ++level;
    }
    return -1;
}

// 多源 BFS：所有陆地距离为 0，静止时已入队；每成功扩展一轮，距离加一。
int maxDistanceToNearestLand(vector<vector<int>> grid) {
    if (grid.empty() || grid[0].empty()) {
        return -1;
    }

    int rowCount = static_cast<int>(grid.size());
    int columnCount = static_cast<int>(grid[0].size());
    int seaCount = 0;
    queue<GridPosition> pending;

    for (int row = 0; row < rowCount; ++row) {
        for (int column = 0; column < columnCount; ++column) {
            if (grid[row][column] == 1) {
                pending.push({row, column});
            } else {
                ++seaCount;
            }
        }
    }
    if (seaCount == 0 || seaCount == rowCount * columnCount) {
        return -1;
    }

    int maximumDistance = 0;
    while (!pending.empty()) {
        int layerSize = static_cast<int>(pending.size());
        bool expanded = false;

        for (int index = 0; index < layerSize; ++index) {
            GridPosition current = pending.front();
            pending.pop();

            for (int direction = 0; direction < 4; ++direction) {
                int nextRow = current.row + ROW_OFFSET[direction];
                int nextColumn = current.column + COLUMN_OFFSET[direction];
                if (!isInside(nextRow, nextColumn, rowCount, columnCount) ||
                    grid[nextRow][nextColumn] != 0) {
                    continue;
                }

                grid[nextRow][nextColumn] = 1;
                pending.push({nextRow, nextColumn});
                expanded = true;
            }
        }

        if (expanded) {
            ++maximumDistance;
        }
    }
    return maximumDistance;
}

// 0-1 BFS：0 边放队头，1 边放队尾。
int minimumObstacles(const vector<vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) {
        return -1;
    }

    int rowCount = static_cast<int>(grid.size());
    int columnCount = static_cast<int>(grid[0].size());
    vector<vector<int>> distance(
        rowCount, vector<int>(columnCount, INT_MAX));
    deque<WeightedState> pending;

    distance[0][0] = 0;
    pending.push_front({0, 0, 0});

    while (!pending.empty()) {
        WeightedState current = pending.front();
        pending.pop_front();
        if (current.distance != distance[current.row][current.column]) {
            continue;
        }

        for (int direction = 0; direction < 4; ++direction) {
            int nextRow = current.row + ROW_OFFSET[direction];
            int nextColumn = current.column + COLUMN_OFFSET[direction];
            if (!isInside(nextRow, nextColumn, rowCount, columnCount)) {
                continue;
            }

            int edgeCost = grid[nextRow][nextColumn];
            int nextDistance = current.distance + edgeCost;
            if (nextDistance >= distance[nextRow][nextColumn]) {
                continue;
            }

            distance[nextRow][nextColumn] = nextDistance;
            WeightedState nextState = {
                nextRow, nextColumn, nextDistance
            };
            if (edgeCost == 0) {
                pending.push_front(nextState);
            } else {
                pending.push_back(nextState);
            }
        }
    }
    return distance[rowCount - 1][columnCount - 1];
}

// 非负权时需要小根堆，这时是 Dijkstra 思路，不再属于普通 BFS。
int trapRainWater(const vector<vector<int>>& heights) {
    if (heights.size() < 3 || heights[0].size() < 3) {
        return 0;
    }

    int rowCount = static_cast<int>(heights.size());
    int columnCount = static_cast<int>(heights[0].size());
    vector<vector<char>> visited(
        rowCount, vector<char>(columnCount, false));
    priority_queue<
        WaterState,
        vector<WaterState>,
        WaterStateGreater
    > pending;

    for (int row = 0; row < rowCount; ++row) {
        for (int column = 0; column < columnCount; ++column) {
            bool isBoundary =
                row == 0 || row == rowCount - 1 ||
                column == 0 || column == columnCount - 1;
            if (!isBoundary) {
                continue;
            }
            pending.push({heights[row][column], row, column});
            visited[row][column] = true;
        }
    }

    int trappedWater = 0;
    while (!pending.empty()) {
        WaterState current = pending.top();
        pending.pop();

        for (int direction = 0; direction < 4; ++direction) {
            int nextRow = current.row + ROW_OFFSET[direction];
            int nextColumn = current.column + COLUMN_OFFSET[direction];
            if (!isInside(nextRow, nextColumn, rowCount, columnCount) ||
                visited[nextRow][nextColumn]) {
                continue;
            }

            visited[nextRow][nextColumn] = true;
            trappedWater += max(
                0, current.waterLevel - heights[nextRow][nextColumn]);
            pending.push({
                max(current.waterLevel, heights[nextRow][nextColumn]),
                nextRow,
                nextColumn
            });
        }
    }
    return trappedWater;
}

int main() {
    vector<vector<int>> graph(6);
    graph[0] = {1, 2};
    graph[1] = {0, 3};
    graph[2] = {0};
    graph[3] = {1};
    graph[4] = {5};
    graph[5] = {4};

    cout << "0 的连通块大小 = " << countReachableVertices(graph, 0)
         << "\n";
    cout << "0 到 3 的距离（dist） = "
         << shortestDistanceFrom(graph, 0)[3] << "\n";
    cout << "0 到 4 的距离（分层） = "
         << shortestDistanceLayered(graph, 0, 4) << "\n";

    vector<vector<int>> serverGrid = {
        {1, 0},
        {1, 1}
    };
    cout << "可通信服务器数 = "
         << countCommunicatingServers(serverGrid) << "\n";

    vector<vector<int>> landGrid = {
        {1, 0, 1},
        {0, 0, 0},
        {1, 0, 1}
    };
    cout << "最近陆地的最大距离 = "
         << maxDistanceToNearestLand(landGrid) << "\n";

    vector<vector<int>> obstacleGrid = {
        {0, 1, 1},
        {1, 1, 0},
        {1, 1, 0}
    };
    cout << "最少移除障碍 = "
         << minimumObstacles(obstacleGrid) << "\n";

    vector<vector<int>> heightGrid = {
        {1, 4, 3, 1, 3, 2},
        {3, 2, 1, 3, 2, 4},
        {2, 3, 3, 2, 3, 1}
    };
    cout << "二维接雨水 = " << trapRainWater(heightGrid) << "\n";
    return 0;
}
