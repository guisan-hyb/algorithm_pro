// 建图 + 拓扑排序模板：邻接表 / 链式前向星 / 拓扑排序 / 字典序最小 / 火星词典
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>
using namespace std;

// ================= 1. 邻接表建图（带权） =================
vector<vector<pair<int,int>>> adj;
void buildAdjList(int n, const vector<array<int,3>>& edges, bool directed) {
    adj.assign(n + 1, {});
    for (auto& e : edges) {
        adj[e[0]].push_back({e[1], e[2]});
        if (!directed) adj[e[1]].push_back({e[0], e[2]});
    }
}

// ================= 2. 链式前向星 =================
const int MAX_NODE = 100005;
const int MAX_EDGE = 200005;
vector<int> head(MAX_NODE, 0);
vector<int> nextEdge(MAX_EDGE);
vector<int> to(MAX_EDGE);
vector<int> weightEdge(MAX_EDGE);
int edgeCount = 1;
void addEdge(int u, int v, int w = 0) {
    to[edgeCount] = v;
    nextEdge[edgeCount] = head[u];
    weightEdge[edgeCount] = w;
    head[u] = edgeCount++;
}
void traverse(int u) {
    for (int edgeIndex = head[u]; edgeIndex > 0; edgeIndex = nextEdge[edgeIndex]) {
        int v = to[edgeIndex];
        (void)v;                // 处理边 u -> v
    }
}

// ================= 3. 拓扑排序（无环返回拓扑序，有环返回空） =================
vector<int> topoSort(int n, vector<vector<int>>& g) {
    vector<int> indegree(n, 0);
    for (int u = 0; u < n; u++)
        for (int v : g[u]) indegree[v]++;

    vector<int> q(n), ans;
    int l = 0, r = 0;
    for (int i = 0; i < n; i++) if (!indegree[i]) q[r++] = i;
    while (l < r) {
        int u = q[l++];
        ans.push_back(u);
        for (int v : g[u]) if (--indegree[v] == 0) q[r++] = v;
    }
    return (int)ans.size() == n ? ans : vector<int>{};
}

// ================= 4. 字典序最小的拓扑排序（小根堆） =================
vector<int> topoSortSmallest(int n, vector<vector<int>>& g) {
    vector<int> indegree(n, 0);
    for (int u = 0; u < n; u++)
        for (int v : g[u]) indegree[v]++;
    priority_queue<int, vector<int>, greater<int>> pq;
    for (int i = 0; i < n; i++) if (!indegree[i]) pq.push(i);
    vector<int> ans;
    while (!pq.empty()) {
        int u = pq.top(); pq.pop();
        ans.push_back(u);
        for (int v : g[u]) if (--indegree[v] == 0) pq.push(v);
    }
    return (int)ans.size() == n ? ans : vector<int>{};
}

// ================= 5. 火星词典 =================
string alienOrder(vector<string>& words) {
    vector<int> indegree(26, -1);
    for (auto& w : words) for (char c : w) indegree[c - 'a'] = 0;

    vector<vector<int>> g(26);
    for (size_t i = 0; i + 1 < words.size(); i++) {
        const string& a = words[i];
        const string& b = words[i + 1];
        size_t len = min(a.size(), b.size()), p = 0;
        for (; p < len; p++) {
            if (a[p] != b[p]) {
                g[a[p] - 'a'].push_back(b[p] - 'a');
                indegree[b[p] - 'a']++;
                break;
            }
            if (p + 1 == b.size() && a.size() > b.size()) return "";   // 非法前缀
        }
    }

    int kind = 0;
    for (int i = 0; i < 26; i++) if (indegree[i] != -1) kind++;

    queue<int> q;
    for (int i = 0; i < 26; i++) if (indegree[i] == 0) q.push(i);
    string ans;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        ans.push_back(char(u + 'a'));
        for (int v : g[u]) if (--indegree[v] == 0) q.push(v);
    }
    return (int)ans.size() == kind ? ans : "";
}

int main() {
    // 课程表 II：0 -> 1，0 -> 2，1 -> 3
    vector<vector<int>> g(4);
    g[0].push_back(1); g[0].push_back(2); g[1].push_back(3);
    for (int x : topoSort(4, g)) std::cout << x << " ";
    std::cout << "\n";

    vector<string> words = {"wrt", "wrf", "er", "ett", "rftt"};
    std::cout << "火星词典：" << alienOrder(words) << "\n";   // wertf
    return 0;
}
