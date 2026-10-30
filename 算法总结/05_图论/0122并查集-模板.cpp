// 并查集模板：路径压缩 + 小挂大 + 集合计数
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct DSU {
    vector<int> fa, sz;
    int sets;
    explicit DSU(int n) : fa(n), sz(n, 1), sets(n) {
        for (int i = 0; i < n; i++) fa[i] = i;
    }
    int find(int i) { return i == fa[i] ? i : fa[i] = find(fa[i]); }
    bool same(int a, int b) { return find(a) == find(b); }
    bool unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) return false;
        if (sz[ra] < sz[rb]) swap(ra, rb);    // 小挂大
        fa[rb] = ra;
        sz[ra] += sz[rb];
        sets--;
        return true;
    }
};

// ---------- 例题 1：岛屿数量（二维点编号 + 只查上/左邻居） ----------
int numIslands(vector<vector<char>>& grid) {
    int n = (int)grid.size(), m = n ? (int)grid[0].size() : 0;
    DSU dsu(n * m);
    auto idx = [&](int i, int j) { return i * m + j; };
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] != '1') continue;
            if (i > 0 && grid[i - 1][j] == '1') dsu.unite(idx(i - 1, j), idx(i, j));
            if (j > 0 && grid[i][j - 1] == '1') dsu.unite(idx(i, j - 1), idx(i, j));
        }
    }
    int blocks = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (grid[i][j] == '1' && dsu.find(idx(i, j)) == idx(i, j)) blocks++;
    return blocks;
}

// ---------- 例题 2：情侣牵手（答案 = n/2 - 集合数） ----------
int minSwapsCouples(vector<int>& row) {
    int n = (int)row.size() / 2;
    DSU dsu(n);
    for (int i = 0; i + 1 < (int)row.size(); i += 2)
        dsu.unite(row[i] / 2, row[i + 1] / 2);   // 情侣编号 = 人编号 / 2
    return n - dsu.sets;
}

// ---------- 例题 3：相似字符串组 ----------
bool isSimilar(const string& a, const string& b) {
    int diff = 0;
    for (size_t i = 0; i < a.size(); i++)
        if (a[i] != b[i] && ++diff > 2) return false;
    return diff == 0 || diff == 2;
}
int numSimilarGroups(vector<string>& strs) {
    int n = (int)strs.size();
    DSU dsu(n);
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (!dsu.same(i, j) && isSimilar(strs[i], strs[j])) dsu.unite(i, j);
    return dsu.sets;
}

int main() {
    vector<vector<char>> g = {{'1','1','0'},{'0','1','0'},{'0','0','1'}};
    std::cout << "岛屿数量 = " << numIslands(g) << "\n";                  // 2
    vector<int> row = {0, 2, 1, 3};
    std::cout << "情侣牵手最少交换 = " << minSwapsCouples(row) << "\n";    // 1
    vector<string> strs = {"tars", "rats", "arts", "star"};
    std::cout << "相似字符串组 = " << numSimilarGroups(strs) << "\n";      // 2
    return 0;
}
