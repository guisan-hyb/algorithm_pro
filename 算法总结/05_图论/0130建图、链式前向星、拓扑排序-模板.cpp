// 建图 + 拓扑排序模板：邻接表 / 链式前向星 / 拓扑排序 / 字典序最小 / 火星词典
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
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
const int MAXN = 100005, MAXM = 200005;
int head[MAXN], nxt[MAXM], to[MAXN + MAXM], wt[MAXM], edgeCnt = 1;
void addEdge(int u, int v, int w = 0) {
    to[edgeCnt] = v;            // 这条边指向的点
    nxt[edgeCnt] = head[u];     // 接在该点上一条边之前
    wt[edgeCnt] = w;            // 边权
    head[u] = edgeCnt++;        // 更新该点的最新边
}
void traverse(int u) {
    for (int ei = head[u]; ei > 0; ei = nxt[ei]) {
        int v = to[ei];
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
    for (int x : topoSort(4, g)) printf("%d ", x);
    puts("");

    vector<string> words = {"wrt", "wrf", "er", "ett", "rftt"};
    printf("火星词典：%s\n", alienOrder(words).c_str());   // wertf
    return 0;
}
