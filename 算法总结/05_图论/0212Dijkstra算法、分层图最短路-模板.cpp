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
