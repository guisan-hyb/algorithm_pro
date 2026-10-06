# A*、Floyd、Bellman-Ford 与 SPFA

> 来源笔记：`0223A星、Floyd、Bellman-Ford与SPFA.cpp`｜核心程度：必背（最短路四件套）｜分类：05 图论

## 一句话

四个算法各管一块：**A*** 用"预估函数"加速单源单目标；**Floyd** O(n³) 求全源；**Bellman-Ford** 处理负权边并能判负环；**SPFA** 是 Bellman-Ford 的队列优化常数版。

## 一、A* 算法

**与 Dijkstra 的唯一区别**：堆里排序的依据从"源点到当前点的距离"改成 **`g + h`**（`h` = 当前点到终点的预估距离）。

**预估函数的核心要求**（笔记原文）：`h(当前点, 终点) <= 真实最短距离`。

- 预估函数只影响"哪些节点先从堆中弹出"；
- 预估函数是一种"吸引力"：合适能加速；**过强（h > 真实值）会导致答案错误**；
- 常用预估函数：
  - 曼哈顿距离 `abs(x1-x2) + abs(y1-y2)`（只能上下左右走时最优）；
  - 欧氏距离（可走斜线）；
  - 对角线距离 `max(abs(x1-x2), abs(y1-y2))`。
- 更新 `distance` 时依据的仍是**真实距离**，只是入堆的排序键带上 `h`。

## 二、Floyd（多源最短路）

```
distance[i][j] = min(distance[i][j], distance[i][k] + distance[k][j]);
```

- 适用于**任何图**（有向/无向、边权可正可负），**但不能有负环**（否则最短路不存在）；
- O(n³) 时间、O(n²) 空间，常数小、好写；
- **跳板 k 必须放在最外层循环**（笔记连写三遍强调）；
- 初始化：`distance[i][i] = 0`，无边为 `INT_MAX`，转移前要判两个加数都不是 `INT_MAX`。

## 三、Bellman-Ford

**松弛操作**（专有名词）：若 `distance[P] + W < distance[S]`，就说"P 出发的边对 S 进行了松弛"。

**过程**：每一轮考察**所有边**，尝试松弛；某一轮没有任何松弛发生就停止。

**复杂度**：O(N·M)。理由（笔记原文）：最短路存在时，一次松弛至少让一个点的最短路"边数 +1"，而最短路最多经过 n 个点，所以轮数 ≤ n-1。

**重要推广**：如果**第 n 轮仍然有松弛发生**，说明从源点出发能到达负环。

**例题（LC 787 k 站中转内最便宜的航班）**：因为"中转次数 ≤ k"与"松弛轮数"语义不同，要用 `curDistance` 与 `nxtDistance` 两个数组交替（**本轮更新只能基于上一轮的结果**），循环 `k+1` 轮。

## 四、SPFA（Bellman-Ford + 队列优化）

**优化点**：只有"上一轮被松弛过的点"连出去的边才可能引起下一轮松弛 → 用队列维护这些点。

要点：

- 只有常数被优化，**最坏复杂度仍是 O(N·M)**，只适合小图；
- 没有负权边时应该用 Dijkstra；
- 可判断"从某点出发能否遇到负环"（要判断整张图需设虚拟源点）；
- `enter[]`（是否在队列中）与 `que[]` 必须同步更新；
- **判负环**：`updateCnt[v] > n - 1` 说明存在负环；
- 另一个重要用途是"费用流"（也可被 Primal-Dual 原始对偶算法代替）。

## 五、四种算法怎么选（速查）

| 场景 | 算法 |
| --- | --- |
| 单源、边权非负 | Dijkstra（`0212`） |
| 单源、有负权、要判负环 | Bellman-Ford / SPFA |
| 全源、n 小（≤ 400） | Floyd |
| 单源单目标、能设计出可采纳的 h | A* |
| 边权只有 0/1 | 0-1 BFS（`0207`） |

## 易错点

1. A* 的预估函数**必须不高于真实值**，否则答案错（宁可选弱一点的启发）；
2. Floyd 的**跳板 k 放最外层**，写成 `i, j, k` 的顺序就错了；
3. Floyd 转移前判断 `distance[i][k] != INF && distance[k][j] != INF`，防溢出；
4. Bellman-Ford 判负环看"第 n 轮是否还有松弛"；
5. LC 787 必须用两个数组交替（本轮不能用到本轮刚更新的值）；
6. SPFA 的 `enter` 与队列同步，`updateCnt` 在"真正入队"时才加；
7. SPFA 在"网格图 + 负权"这类题上会被卡成指数级，谨慎使用。

## 模板（默写骨架）

```cpp
#include <bits/stdc++.h>
using namespace std;

// ---------- 1. A*（网格，只能上下左右走，用曼哈顿距离） ----------
int aStar(vector<vector<int>>& grid, int sx, int sy, int tx, int ty) {
    int n = (int)grid.size(), m = (int)grid[0].size();
    if (grid[sx][sy] == 0 || grid[tx][ty] == 0) return -1;
    auto h = [&](int x, int y) { return abs(x - tx) + abs(y - ty); };
    int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
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
            int nx = x + dx[d], ny = y + dy[d];
            if (nx < 0 || nx >= n || ny < 0 || ny >= m || grid[nx][ny] == 0) continue;
            if (dist[x][y] + 1 < dist[nx][ny]) {
                dist[nx][ny] = dist[x][y] + 1;
                pq.push({dist[nx][ny] + h(nx, ny), nx, ny});
            }
        }
    }
    return -1;
}

// ---------- 2. Floyd（多源最短路，节点 1..n） ----------
void floyd(vector<vector<long long>>& d) {
    int n = (int)d.size() - 1;
    for (int k = 1; k <= n; k++)                    // ★跳板必须最外层
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                if (d[i][k] < LLONG_MAX / 4 && d[k][j] < LLONG_MAX / 4)
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
}

// ---------- 3. Bellman-Ford 变形：最多 k 次中转的最便宜航班 ----------
int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
    const int INF = INT_MAX;
    vector<int> cur(n, INF);
    cur[src] = 0;
    for (int i = 0; i <= k; i++) {
        vector<int> nxt = cur;                      // ★本轮只能基于上一轮
        for (auto& f : flights) {
            if (cur[f[0]] != INF && cur[f[0]] + f[2] < nxt[f[1]])
                nxt[f[1]] = cur[f[0]] + f[2];
        }
        cur = nxt;
    }
    return cur[dst] == INF ? -1 : cur[dst];
}
```

## 笔记里的题目

A* 网格模板（自测）、洛谷 P2910 Floyd 模板、LeetCode 787 k 站中转内最便宜的航班（Bellman-Ford 变形）、洛谷 P3385 负环（SPFA + `updateCnt` 判负环）。

## 延伸

- Dijkstra 与 0-1 BFS 见 `0212Dijkstra算法、分层图最短路` 与 `0207宽度优先遍历及其扩展`；
- 四种算法的选型对照见 `0223BFS与最短路算法的对比`；
- A* 与启发式搜索的直觉："预估函数是一种吸引力"。
