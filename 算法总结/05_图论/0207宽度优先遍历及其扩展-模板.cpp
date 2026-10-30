// BFS 扩展模板：多源 BFS / 0-1 BFS / BFS + 小根堆
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

const std::array<int, 4> DX= {1, -1, 0, 0};
const std::array<int, 4> DY= {0, 0, 1, -1};
// ================= 1. 多源 BFS：地图分析 =================
int maxDistance(vector<vector<int>> grid) {
    int n = (int)grid.size(), m = (int)grid[0].size(), seas = 0;
    queue<pair<int,int>> q;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == 1) q.push({i, j});
            else seas++;
        }
    if (seas == 0 || seas == n * m) return -1;
    int level = 0;
    while (!q.empty()) {
        int sz = q.size();
        level++;
        while (sz--) {
            auto pr = q.front(); q.pop();
            int x = pr.first, y = pr.second;
            for (int d = 0; d < 4; d++) {
                int nx = x + DX[d], ny = y + DY[d];
                if (nx < 0 || nx >= n || ny < 0 || ny >= m || grid[nx][ny] != 0) continue;
                grid[nx][ny] = 1;                 // 入队即标记
                q.push({nx, ny});
            }
        }
    }
    return level - 1;
}

// ================= 2. 0-1 BFS：移除最少障碍物 =================
int minimumObstacles(vector<vector<int>> grid) {
    int n = (int)grid.size(), m = (int)grid[0].size();
    vector<vector<int>> dist(n, vector<int>(m, INT_MAX));
    deque<pair<int,int>> dq;
    dist[0][0] = 0;
    dq.push_front({0, 0});
    while (!dq.empty()) {
        auto pr = dq.front(); dq.pop_front();
        int x = pr.first, y = pr.second;
        if (x == n - 1 && y == m - 1) return dist[x][y];
        for (int d = 0; d < 4; d++) {
            int nx = x + DX[d], ny = y + DY[d];
            if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
            int w = grid[nx][ny];                 // 0 或 1
            if (dist[x][y] + w < dist[nx][ny]) {
                dist[nx][ny] = dist[x][y] + w;
                if (w == 0) dq.push_front({nx, ny});
                else        dq.push_back({nx, ny});
            }
        }
    }
    return -1;
}

// ================= 3. BFS + 小根堆：二维接雨水 =================
int trapRainWater(vector<vector<int>>& h) {
    int n = (int)h.size(), m = (int)h[0].size();
    if (n < 3 || m < 3) return 0;
    using State = array<int,3>;                   // {水线高度, x, y}
    priority_queue<State, vector<State>, greater<State>> pq;
    vector<vector<char>> vis(n, vector<char>(m, 0));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (i == 0 || i == n - 1 || j == 0 || j == m - 1) {
                pq.push({h[i][j], i, j});
                vis[i][j] = 1;
            }
    int ans = 0;
    while (!pq.empty()) {
        State cur = pq.top(); pq.pop();
        int water = cur[0], x = cur[1], y = cur[2];
        for (int d = 0; d < 4; d++) {
            int nx = x + DX[d], ny = y + DY[d];
            if (nx < 0 || nx >= n || ny < 0 || ny >= m || vis[nx][ny]) continue;
            vis[nx][ny] = 1;
            ans += max(0, water - h[nx][ny]);
            pq.push({max(water, h[nx][ny]), nx, ny});
        }
    }
    return ans;
}

int main() {
    vector<vector<int>> g = {{1,0,1},{0,0,0},{1,0,1}};
    std::cout << "地图分析 = " << maxDistance(g) << "\n";              // 2

    vector<vector<int>> grid = {{0,1,1},{1,1,0},{1,1,0}};
    std::cout << "最少移除障碍 = " << minimumObstacles(grid) << "\n";   // 2

    vector<vector<int>> height = {{1,4,3,1,3,2},{3,2,1,3,2,4},{2,3,3,2,3,1}};
    std::cout << "二维接雨水 = " << trapRainWater(height) << "\n";      // 4
    return 0;
}
