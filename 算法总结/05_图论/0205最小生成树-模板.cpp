// 最小生成树模板：Kruskal + Prim
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

struct DSU {
    vector<int> fa;
    explicit DSU(int n) : fa(n) { for (int i = 0; i < n; i++) fa[i] = i; }
    int find(int i) { return i == fa[i] ? i : fa[i] = find(fa[i]); }
    bool unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) return false;
        fa[rb] = ra;
        return true;
    }
};

// ================= Kruskal（最常用，不用建图） =================
long long kruskal(int n, vector<array<int,3>> edges) {
    sort(edges.begin(), edges.end(),
         [](const array<int,3>& a, const array<int,3>& b) { return a[2] < b[2]; });
    DSU dsu(n + 1);
    long long ans = 0;
    int cnt = 0;
    for (auto& e : edges) {
        if (dsu.unite(e[0], e[1])) { ans += e[2]; cnt++; }
        if (cnt == n - 1) break;
    }
    return cnt == n - 1 ? ans : -1;              // -1 表示图不连通
}

// ================= Prim（需要建图） =================
long long prim(int n, vector<vector<pair<int,int>>>& adj) {
    vector<char> inSet(n + 1, 0);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;   // {w, v}
    inSet[1] = 1;
    int cnt = 1;
    long long ans = 0;
    for (auto& e : adj[1]) pq.push({e.second, e.first});
    while (!pq.empty()) {
        auto pr = pq.top(); pq.pop();
        int w = pr.first, v = pr.second;
        if (inSet[v]) continue;                  // 已在集合里，丢弃这条边
        inSet[v] = 1;
        ans += w;
        cnt++;
        for (auto& e : adj[v])
            if (!inSet[e.first]) pq.push({e.second, e.first});
    }
    return cnt == n ? ans : -1;
}

// ================= 应用：水资源分配优化（虚拟节点） =================
int minCostToSupplyWater(int n, vector<int>& wells, vector<vector<int>>& pipes) {
    vector<array<int,3>> edges;
    for (int i = 1; i <= n; i++) edges.push_back({0, i, wells[i - 1]});  // 虚拟水源点 0
    for (auto& p : pipes) edges.push_back({p[0], p[1], p[2]});
    return (int)kruskal(n + 1, edges);           // 注意节点数是 n+1
}

int main() {
    // 洛谷 P3366 样例：4 个点 5 条边
    vector<array<int,3>> edges = {{1,2,2},{1,3,2},{1,4,3},{2,3,4},{3,4,3}};
    std::cout << "Kruskal 最小总权 = " << kruskal(4, edges) << "\n";      // 7

    vector<vector<pair<int,int>>> adj(5);
    for (auto& e : edges) {
        adj[e[0]].push_back({e[1], e[2]});
        adj[e[1]].push_back({e[0], e[2]});
    }
    std::cout << "Prim 最小总权 = " << prim(4, adj) << "\n";              // 7

    vector<int> wells = {1, 2, 2};
    vector<vector<int>> pipes = {{1,2,1},{2,3,1}};
    std::cout << "供水最低成本 = " << minCostToSupplyWater(3, wells, pipes) << "\n";   // 3
    return 0;
}
