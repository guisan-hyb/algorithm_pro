// 二维前缀和 + 二维差分 + 离散化模板
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// ================= 1. 二维前缀和（补 0 行列） =================
struct MatSum {
    vector<vector<long long>> s;
    explicit MatSum(const vector<vector<int>>& a) {
        int n = (int)a.size(), m = n ? (int)a[0].size() : 0;
        s.assign(n + 1, vector<long long>(m + 1, 0));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                s[i + 1][j + 1] = a[i][j] + s[i][j + 1] + s[i + 1][j] - s[i][j];
    }
    long long query(int r1, int c1, int r2, int c2) const {   // 0-based 闭区间
        return s[r2 + 1][c2 + 1] - s[r1][c2 + 1] - s[r2 + 1][c1] + s[r1][c1];
    }
};

// ================= 2. 二维差分 =================
vector<vector<long long>> diff;

void diffAdd(int a, int b, int c, int d, long long v) {        // 子矩阵整体加 v
    diff[a][b]         += v;
    diff[c + 1][b]     -= v;
    diff[a][d + 1]     -= v;
    diff[c + 1][d + 1] += v;
}

void diffBuild(int n, int m) {                                 // 还原每个位置的值
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            diff[i][j] += diff[i][j - 1] + diff[i - 1][j] - diff[i - 1][j - 1];
}

// ================= 3. 离散化 + 二维差分（力场题骨架） =================
int fieldOfGreatestBlessing(vector<vector<int>>& forceField) {
    int n = (int)forceField.size();
    if (n == 0) return 0;
    vector<long long> px, py;
    for (auto& f : forceField) {
        long long x = f[0], y = f[1], side = f[2];
        px.push_back(2 * x - side); px.push_back(2 * x + side);   // 乘 2 避免小数
        py.push_back(2 * y - side); py.push_back(2 * y + side);
    }
    sort(px.begin(), px.end());
    px.erase(unique(px.begin(), px.end()), px.end());
    sort(py.begin(), py.end());
    py.erase(unique(py.begin(), py.end()), py.end());

    int X = (int)px.size(), Y = (int)py.size();
    vector<vector<int>> d(X + 2, vector<int>(Y + 2, 0));
    auto add = [&](int a, int b, int c, int e) {
        d[a][b] += 1; d[c + 1][b] -= 1;
        d[a][e + 1] -= 1; d[c + 1][e + 1] += 1;
    };

    for (auto& f : forceField) {
        long long x = f[0], y = f[1], side = f[2];
        int x1 = (int)(lower_bound(px.begin(), px.end(), 2 * x - side) - px.begin());
        int x2 = (int)(lower_bound(px.begin(), px.end(), 2 * x + side) - px.begin());
        int y1 = (int)(lower_bound(py.begin(), py.end(), 2 * y - side) - py.begin());
        int y2 = (int)(lower_bound(py.begin(), py.end(), 2 * y + side) - py.begin());
        add(x1, y1, x2, y2);
    }

    int best = 0;
    for (int i = 0; i < X; i++) {
        for (int j = 0; j < Y; j++) {
            if (i) d[i][j] += d[i - 1][j];
            if (j) d[i][j] += d[i][j - 1];
            if (i && j) d[i][j] -= d[i - 1][j - 1];
            best = max(best, d[i][j]);
        }
    }
    return best;
}

int main() {
    vector<vector<int>> mat = {
        {3, 0, 1, 4, 2},
        {5, 6, 3, 2, 1},
        {1, 2, 0, 1, 5},
        {4, 1, 0, 1, 7},
        {1, 0, 3, 0, 5}
    };
    MatSum ms(mat);
    std::cout << "子矩阵和 = " << ms.query(2, 1, 4, 3) << "\n";   // 8

    // 3x3 全 0 矩阵，给 [1..2] x [1..2] 加 1
    diff.assign(5, vector<long long>(5, 0));
    diffAdd(1, 1, 2, 2, 1);
    diffBuild(3, 3);
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= 3; j++) std::cout << diff[i][j] << " ";
        std::cout << "\n";
    }
    return 0;
}
