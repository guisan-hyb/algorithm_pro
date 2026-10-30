// 最短路四件套模板：A* / Floyd / Bellman-Ford / SPFA 判负环
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <array>
#include <climits>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

// ================= 1. A*（网格，四方向，曼哈顿启发） =================
int aStar(vector<vector<int>>& grid, int sx, int sy, int tx, int ty) {
    int n = (int)grid.size(), m = (int)grid[0].size();
    if (grid[sx][sy] == 0 || grid[tx][ty] == 0) return -1;
    auto h = [&](int x, int y) { return abs(x - tx) + abs(y - ty); };
    const std::array<int, 4> rowOffset = {1, -1, 0, 0};
    const std::array<int, 4> columnOffset = {0, 0, 1, -1};
    vector<vector<int>> dist(n, vector<int>(m, INT_MAX));
    vector<vector<char>> done(n, vector<char>(m, 0));
    using State = array<int,3>;                     // {f = g + h, x, y}
    priority_queue<State, vector<State>, greater<State>> pq;
    dist[sx][sy] = 0;
    pq.push({h(sx, sy), sx, sy});
    while (!pq.empty()) {
        State cur = pq.top(); pq.pop();
        int x = cur[1], y = cur[2];
        if (done[x][y]) continue;
        done[x][y] = 1;
        if (x == tx && y == ty) return dist[x][y];
        for (int d = 0; d < 4; d++) {
            int nx = x + rowOffset[d], ny = y + columnOffset[d];
            if (nx < 0 || nx >= n || ny < 0 || ny >= m || grid[nx][ny] == 0) continue;
            if (dist[x][y] + 1 < dist[nx][ny]) {
                dist[nx][ny] = dist[x][y] + 1;
                pq.push({dist[nx][ny] + h(nx, ny), nx, ny});   // 用 f 排序，用 g 更新
            }
        }
    }
    return -1;
}

// ================= 2. Floyd（节点 1..n） =================
void floyd(vector<vector<long long>>& d) {
    int n = (int)d.size() - 1;
    for (int k = 1; k <= n; k++)                    // ★跳板必须放最外层
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                if (d[i][k] < LLONG_MAX / 4 && d[k][j] < LLONG_MAX / 4)
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
}

// ================= 3. Bellman-Ford 变形：最多 k 次中转的最便宜航班 =================
int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
    const int INF = INT_MAX;
    vector<int> cur(n, INF);
    cur[src] = 0;
    for (int i = 0; i <= k; i++) {
        vector<int> nxt = cur;                      // ★本轮只基于上一轮的结果
        for (auto& f : flights)
            if (cur[f[0]] != INF && cur[f[0]] + f[2] < nxt[f[1]])
                nxt[f[1]] = cur[f[0]] + f[2];
        cur = nxt;
    }
    return cur[dst] == INF ? -1 : cur[dst];
}

// ================= 4. SPFA 判负环（从 1 号点出发） =================
bool spfaHasNegativeCycle(int n, vector<vector<pair<int,int>>>& adj) {
    const long long INF = LLONG_MAX / 4;
    vector<long long> dist(n, INF);
    vector<char> inQueue(n, 0);
    vector<int> updateCnt(n, 0);
    queue<int> q;
    dist[1] = 0;
    q.push(1);
    inQueue[1] = 1;
    updateCnt[1] = 1;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        inQueue[u] = 0;
        for (auto& e : adj[u]) {
            int v = e.first, w = e.second;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (!inQueue[v]) {
                    if (++updateCnt[v] > n - 1) return true;   // 松弛超过 n-1 次 ⇒ 负环
                    q.push(v);
                    inQueue[v] = 1;
                }
            }
        }
    }
    return false;
}

int main() {
    vector<vector<int>> grid = {
        {1,1,1,1},
        {1,0,1,1},
        {1,1,1,1}
    };
    std::cout << "A* 最短步数 = " << aStar(grid, 0, 0, 2, 3) << "\n";   // 5

    int n = 3;
    vector<vector<long long>> d(n + 1, vector<long long>(n + 1, LLONG_MAX / 4));
    for (int i = 1; i <= n; i++) d[i][i] = 0;
    d[1][2] = 1; d[2][3] = 2; d[1][3] = 5;
    floyd(d);
    std::cout << "Floyd d[1][3] = " << d[1][3] << "\n";               // 3

    vector<vector<int>> flights = {{0,1,100},{1,2,100},{0,2,500}};
    std::cout << "最便宜航班 = " << findCheapestPrice(3, flights, 0, 2, 1) << "\n";  // 200

    vector<vector<pair<int,int>>> adj(4);
    adj[1].push_back({2, -1});
    adj[2].push_back({3, -1});
    adj[3].push_back({1, -1});
    std::cout << "存在负环 = " << (int)spfaHasNegativeCycle(4, adj) << "\n";         // 1
    return 0;
}
