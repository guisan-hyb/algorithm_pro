// 拓扑排序扩展模板：食物链计数 / 并行课程 / 喧闹和富有 / 参加会议的最多员工
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// ================= 1. 最大食物链计数 =================
long long countFoodChains(int n, vector<vector<int>>& adj) {
    const long long MOD = 80112002;
    vector<int> indegree(n + 1, 0);
    for (int u = 1; u <= n; u++)
        for (int v : adj[u]) indegree[v]++;

    vector<long long> lines(n + 1, 0);
    vector<int> q(n + 1);
    int l = 0, r = 0;
    for (int i = 1; i <= n; i++)
        if (!indegree[i]) { q[r++] = i; lines[i] = 1; }   // 入度为 0 的是链的起点

    long long ans = 0;
    while (l < r) {
        int cur = q[l++];
        if (adj[cur].empty()) {                            // 到达最顶端
            ans = (ans + lines[cur]) % MOD;
        } else {
            for (int nxt : adj[cur]) {
                lines[nxt] = (lines[nxt] + lines[cur]) % MOD;   // 上游推给下游
                if (--indegree[nxt] == 0) q[r++] = nxt;
            }
        }
    }
    return ans;
}

// ================= 2. 并行课程 III =================
int parallelCoursesIII(int n, vector<vector<int>>& relations, vector<int>& time) {
    vector<vector<int>> adj(n + 1);
    vector<int> indegree(n + 1, 0);
    for (auto& e : relations) { adj[e[0]].push_back(e[1]); indegree[e[1]]++; }

    vector<int> cost(n + 1, 0), q(n + 1);
    int l = 0, r = 0, ans = 0;
    for (int i = 1; i <= n; i++)
        if (!indegree[i]) { q[r++] = i; cost[i] = time[i - 1]; }
    while (l < r) {
        int cur = q[l++];
        ans = max(ans, cost[cur]);
        for (int nxt : adj[cur]) {
            cost[nxt] = max(cost[nxt], cost[cur]);        // 等所有先修课完成
            if (--indegree[nxt] == 0) {
                cost[nxt] += time[nxt - 1];               // 入度清零后才加自己的耗时
                q[r++] = nxt;
            }
        }
    }
    return ans;
}

// ================= 3. 喧闹和富有 =================
vector<int> loudAndRich(vector<vector<int>>& richer, vector<int>& quiet) {
    int n = (int)quiet.size();
    vector<vector<int>> adj(n);
    vector<int> indegree(n, 0), ans(n);
    for (int i = 0; i < n; i++) ans[i] = i;                // 初始认为自己是答案
    for (auto& e : richer) {                               // 有钱 -> 没钱
        adj[e[0]].push_back(e[1]);
        indegree[e[1]]++;
    }
    vector<int> q(n);
    int l = 0, r = 0;
    for (int i = 0; i < n; i++) if (!indegree[i]) q[r++] = i;
    while (l < r) {
        int cur = q[l++];
        for (int nxt : adj[cur]) {
            if (quiet[ans[nxt]] > quiet[ans[cur]]) ans[nxt] = ans[cur];   // 传播更安静的人
            if (--indegree[nxt] == 0) q[r++] = nxt;
        }
    }
    return ans;
}

// ================= 4. 参加会议的最多员工数（基环树） =================
int maximumInvitations(vector<int>& favorite) {
    int n = (int)favorite.size();
    vector<int> indegree(n, 0);
    for (int i = 0; i < n; i++) indegree[favorite[i]]++;

    vector<int> deep(n, 0), q(n);
    int l = 0, r = 0;
    for (int i = 0; i < n; i++) if (!indegree[i]) q[r++] = i;
    while (l < r) {                                        // 剥掉所有非环节点
        int cur = q[l++];
        int nxt = favorite[cur];
        deep[nxt] = max(deep[nxt], deep[cur] + 1);         // 记录朝向环的最长链
        if (--indegree[nxt] == 0) q[r++] = nxt;
    }

    int sumSmallRings = 0, bigRings = 0;
    for (int i = 0; i < n; i++) {
        if (indegree[i] > 0) {                             // 只在环上
            int ringSize = 1;
            indegree[i] = 0;
            for (int e = favorite[i]; e != i; e = favorite[e]) {
                ringSize++;
                indegree[e] = 0;
            }
            if (ringSize == 2) sumSmallRings += deep[i] + deep[favorite[i]] + 2;
            else               bigRings = max(bigRings, ringSize);
        }
    }
    return max(sumSmallRings, bigRings);
}

int main() {
    // 食物链：1->2, 2->3
    vector<vector<int>> adj(4);
    adj[1].push_back(2); adj[2].push_back(3);
    printf("食物链条数 = %lld\n", countFoodChains(3, adj));   // 1

    vector<vector<int>> rel = {{1,3},{2,3}};
    vector<int> t = {3, 2, 5};
    printf("并行课程最少月数 = %d\n", parallelCoursesIII(3, rel, t));  // 8

    vector<int> fav = {2, 2, 1, 2};
    printf("会议最多员工数 = %d\n", maximumInvitations(fav));  // 3
    return 0;
}
