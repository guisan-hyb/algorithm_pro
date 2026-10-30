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

## 模板（可直接编译运行）

```cpp
// Dijkstra 模板：普通堆 / 反向索引堆 / 网格变形 / 分层图（钥匙）
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <array>
#include <climits>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>
using namespace std;

// ================= 1. 普通堆版 Dijkstra =================
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
        if (done[u]) continue;                 // 弹出过就忽略
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

// ================= 2. 反向索引堆版 Dijkstra（O(m log n)） =================
struct IndexedHeap {
    vector<int> heap, where;                   // where[v]: -1 没进过 / -2 弹出过 / >=0 堆中位置
    vector<long long> dist;
    int sz = 0;
    explicit IndexedHeap(int n) : heap(n), where(n, -1), dist(n, LLONG_MAX / 4) {}
    void swapNode(int i, int j) {
        swap(heap[i], heap[j]);
        swap(where[heap[i]], where[heap[j]]);
    }
    void heapInsert(int i) {
        while (i > 0 && dist[heap[i]] < dist[heap[(i - 1) / 2]]) {
            swapNode(i, (i - 1) / 2);
            i = (i - 1) / 2;
        }
    }
    void heapify(int i) {
        int l = 2 * i + 1;
        while (l < sz) {
            int r = l + 1, best = l;
            if (r < sz && dist[heap[r]] < dist[heap[l]]) best = r;
            if (dist[heap[i]] <= dist[heap[best]]) return;
            swapNode(i, best);
            i = best;
            l = 2 * i + 1;
        }
    }
    void push(int v, long long d) {
        if (where[v] == -2) return;            // 已弹出，忽略
        if (where[v] == -1) { heap[sz] = v; where[v] = sz++; dist[v] = d; heapInsert(where[v]); }
        else if (d < dist[v]) { dist[v] = d; heapInsert(where[v]); }
    }
    int pop() {
        int v = heap[0];
        swapNode(0, --sz);
        where[v] = -2;
        heapify(0);
        return v;
    }
    bool empty() const { return sz == 0; }
};
vector<long long> dijkstraIndexed(int n, vector<vector<pair<int,int>>>& adj, int src) {
    IndexedHeap h(n);
    h.push(src, 0);
    while (!h.empty()) {
        int u = h.pop();
        for (auto& e : adj[u]) h.push(e.first, h.dist[u] + e.second);
    }
    return h.dist;
}

// ================= 3. 分层图：获取所有钥匙的最短路径 =================
int shortestPathAllKeys(vector<string>& grid) {
    int n = (int)grid.size(), m = (int)grid[0].size();
    const std::array<int, 4> rowOffset = {1, -1, 0, 0};
    const std::array<int, 4> columnOffset = {0, 0, 1, -1};
    int all = 0, sx = 0, sy = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == '@') { sx = i; sy = j; }
            if (islower((unsigned char)grid[i][j])) all |= 1 << (grid[i][j] - 'a');
        }
    vector<vector<vector<char>>> vis(n, vector<vector<char>>(m, vector<char>(1 << 6, 0)));
    queue<array<int,3>> q;                     // {x, y, 钥匙状态}
    q.push({sx, sy, 0});
    vis[sx][sy][0] = 1;
    int level = 0;
    while (!q.empty()) {
        int sz = q.size();
        level++;
        while (sz--) {
            array<int,3> cur = q.front(); q.pop();
            int x = cur[0], y = cur[1], s = cur[2];
            if (s == all) return level - 1;
            for (int d = 0; d < 4; d++) {
                int nx = x + rowOffset[d], ny = y + columnOffset[d], ns = s;
                if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;   // 越界先判
                char c = grid[nx][ny];
                if (c == '#') continue;
                if (isupper((unsigned char)c) && !(ns & (1 << (c - 'A')))) continue;
                if (islower((unsigned char)c)) ns |= 1 << (c - 'a');
                if (!vis[nx][ny][ns]) {
                    vis[nx][ny][ns] = 1;
                    q.push({nx, ny, ns});
                }
            }
        }
    }
    return -1;
}

int main() {
    // 3 个点：0->1 权 1，1->2 权 2，0->2 权 5
    vector<vector<pair<int,int>>> adj(3);
    adj[0].push_back({1, 1});
    adj[1].push_back({2, 2});
    adj[0].push_back({2, 5});
    auto d1 = dijkstra(3, adj, 0);
    auto d2 = dijkstraIndexed(3, adj, 0);
    std::cout << "普通堆: " << d1[0] << " " << d1[1] << " " << d1[2] << "\n";   // 0 1 3
    std::cout << "索引堆: " << d2[0] << " " << d2[1] << " " << d2[2] << "\n";   // 0 1 3

    vector<string> g = {"@.a.#", "###.#", "b.A.B"};
    std::cout << "拿齐钥匙最短步数 = " << shortestPathAllKeys(g) << "\n"; // 8
    return 0;
}
```

## 笔记里的题目

LeetCode 743 网络延迟时间（普通堆模板）、洛谷 P4779 单源最短路径（链式前向星 + 反向索引堆）、LC 1631 最小体力消耗路径、LC 778 水位上升的泳池中游泳、LC 864 获取所有钥匙的最短路径、LCP 28 电动车游城市、洛谷 P4568 飞行路线（免费 k 条边）。

## 延伸

- 0-1 BFS（Dijkstra 的特例）见 `0207宽度优先遍历及其扩展`；
- 负权边与全源最短路见 `0223A星、Floyd、Bellman-Ford与SPFA`；
- 算法选型见 `0223BFS与最短路算法的对比`。

## 专题深化：为什么成立

Dijkstra 每次确定当前未访问中距离最小的点；非负边权保证以后不可能通过更长边把它变得更短。分层图把状态扩展为 `(点,层)`，用额外边表示钥匙、优惠或换层。

## 专题深化：手算与边界

边 `0-1` 权 1、`1-2` 权 2、`0-2` 权 5：先确定 1（dist=1），再用 1 松弛 2 得到 3，最后确定 2，故最短路 3。

### 对照与取舍

| 条件 | 算法 |
| --- | --- |
| 非负权 | Dijkstra |
| 有负边 | Bellman-Ford/SPFA |
| 边权相同 | BFS |
| 0/1 边 | 01 BFS |

### 易错点与反例

1. 不能处理负边
2. 懒删除堆要跳过旧距离
3. 状态图可能爆炸
4. 拿钥匙等状态要一起入 dist 维

## 专题深化：进阶拓展

索引堆可减少重复入堆；A* 用启发函数加速单目标；分层图也用于多费用约束。

## 专题深化：面试追问

1. 为什么 Dijkstra 需要非负边？
2. 懒删除和索引堆各怎么做？
3. 分层图的边如何定义？

## 专题深化：自测（闭卷）

1. 手跑三节点最短路
2. 构造负边反例
3. 设计拿钥匙状态
