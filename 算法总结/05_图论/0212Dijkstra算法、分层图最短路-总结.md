# Dijkstra 算法与分层图最短路

> 来源笔记：`0212Dijkstra算法、分层图最短路.cpp`｜核心程度：必背｜分类：05 图论

## 一句话

单源带权最短路的正权标准解：每次从"未确定"的点里取距离最小的弹出，弹出即定稿（边权非负才成立）；**分层图**则把"位置 + 状态"打包成新点，再跑 Dijkstra。

## 一、三种实现

| 实现 | 复杂度 | 说明 |
| --- | --- | --- |
| 暴力（笔记不讲） | 太差 | 无意义 |
| **普通堆（最常用）** | O(m log m) | 堆里存 `{节点, 距离}`，允许重复入堆，弹出时判 visited |
| **反向索引堆（最极致）** | O(m log n) | 维护 `where[v]`（v 在堆中的位置），支持修改堆中某点的值 |

**普通堆版流程**：

1. `distance[源点] = 0`，`(源点, 0)` 入小根堆；
2. 弹出 `(u, d)`：`visited[u]` 就跳过；否则标记；
3. 遍历 u 的每条边 `(u -> v, w)`：`!visited[v] && distance[u] + w < distance[v]` 就更新并入堆；
4. 堆空结束；**遇到终点可直接结算（剪枝）**。

**反向索引堆的三种状态（笔记原文）**：

- `where[v] == -1`：从未进过堆；
- `where[v] == -2`：已经弹出过（忽略）；
- `where[v] >= 0`：在堆中的位置就是 `where[v]`。

## 二、distance 数组的重新定义

笔记的关键洞察：**`distance` 里存的未必是"距离"**，可以是任何"能贪心累加的指标"。

| 题目 | distance 的含义 | 转移 |
| --- | --- | --- |
| 最小体力消耗路径（LC 1631） | 到该点的"路径上高度差绝对值的最大值" | `w = max(当前值, abs(h1-h2))` |
| 水位上升的泳池中游泳（LC 778） | 到该点"必须达到的最低水位" | `w = max(当前值, grid[dx][dy])` |

这两题还能用并查集、二分答案 + BFS 做（笔记提到三种解法均可）。

## 三、分层图最短路（扩点最短路）

**核心**：图上的点不是"位置"，而是"**位置 + 状态**"的组合；状态一变就是一个新点，之后 BFS / Dijkstra 的过程完全不变。

| 题目 | 新点信息 | 两种转移 |
| --- | --- | --- |
| 获取所有钥匙的最短路径（LC 864） | `(x, y, 钥匙状态 bitmask)` | 遇钥匙 `ns |= bit`；遇锁没钥匙就跳过 |
| 电动车游城市（LCP 28） | `(城市, 当前电量)` | 充一格电（花费 `charge[i]`）或开往邻城（花费距离、电量减少） |
| 免费航班（洛谷 P4568） | `(城市, 已用免单次数)` | 免单（次数 +1、花费 0）或正常付费 |

**笔记的提醒**：处理"电量减少"这类转移时**先判断合法性再访问数组**，否则会越界。

## 适用特征（看到这些信号就选它）

- 单源最短路、边权**非负**（有负权改用 Bellman-Ford / SPFA，见 `0223A星、Floyd、Bellman-Ford与SPFA`）；
- 网格上"带权移动代价"（代价可以取 max 或求和）；
- 状态需要扩展：钥匙集合、电量、免单次数、剩余体力 → 分层图；
- 边权只有 0/1 → 用 0-1 BFS 更快（见 `0207宽度优先遍历及其扩展`）。

## 复杂度

- 普通堆 O(m log m)；反向索引堆 O(m log n)；
- 分层图：点数乘状态数，按 `n × 状态数` 计算。

## 易错点

1. 弹出时判 visited 并跳过（同一点可能多次入堆）；
2. 只更新"未弹出过"的点；
3. 反向索引堆的 `heapInsert / heapify / pop` 都要**同步交换 `where`**；
4. 分层图的状态维度别搞反，转移前先做合法性判断；
5. 起点/终点的状态要初始化正确（如 LC 864 起点状态是"没有钥匙"）；
6. 用 `INT_MAX` 时注意加法溢出（先判 `distance[u] != INT_MAX`）。

## 模板（默写骨架）

```cpp
#include <bits/stdc++.h>
using namespace std;

// 普通堆版 Dijkstra（邻接表，节点 0..n-1）
vector<long long> dijkstra(int n, vector<vector<pair<int,int>>>& adj, int src) {
    const long long INF = LLONG_MAX / 4;
    vector<long long> dist(n, INF);
    vector<char> done(n, 0);
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
    dist[src] = 0;
    pq.push({0, src});
    while (!pq.empty()) {
        auto pr = pq.top(); pq.pop();
        long long d = pr.first;
        int u = pr.second;
        if (done[u]) continue;                 // 已定稿
        done[u] = 1;
        for (auto& e : adj[u]) {
            int v = e.first, w = e.second;
            if (!done[v] && d + w < dist[v]) {
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

// 网格上"最小值最大化"（最小体力消耗 / 泳池）：distance 存"路径上的最大代价"
int minEffort(vector<vector<int>>& h) {
    int n = (int)h.size(), m = (int)h[0].size();
    int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
    vector<vector<int>> dist(n, vector<int>(m, INT_MAX));
    vector<vector<char>> done(n, vector<char>(m, 0));
    using State = array<int,3>;                // {代价, x, y}
    priority_queue<State, vector<State>, greater<State>> pq;
    dist[0][0] = 0;
    pq.push({0, 0, 0});
    while (!pq.empty()) {
        State cur = pq.top(); pq.pop();
        int d = cur[0], x = cur[1], y = cur[2];
        if (done[x][y]) continue;
        done[x][y] = 1;
        if (x == n - 1 && y == m - 1) return d;
        for (int k = 0; k < 4; k++) {
            int nx = x + dx[k], ny = y + dy[k];
            if (nx < 0 || nx >= n || ny < 0 || ny >= m || done[nx][ny]) continue;
            int nd = max(d, abs(h[nx][ny] - h[x][y]));
            if (nd < dist[nx][ny]) { dist[nx][ny] = nd; pq.push({nd, nx, ny}); }
        }
    }
    return -1;
}
```

## 笔记里的题目

LeetCode 743 网络延迟时间（普通堆模板）、洛谷 P4779 单源最短路径（链式前向星 + 反向索引堆）、LC 1631 最小体力消耗路径、LC 778 水位上升的泳池中游泳、LC 864 获取所有钥匙的最短路径、LCP 28 电动车游城市、洛谷 P4568 飞行路线（免费 k 条边）。

## 延伸

- 0-1 BFS（Dijkstra 的特例）见 `0207宽度优先遍历及其扩展`；
- 负权边与全源最短路见 `0223A星、Floyd、Bellman-Ford与SPFA`；
- 算法选型见 `0223BFS与最短路算法的对比`。
