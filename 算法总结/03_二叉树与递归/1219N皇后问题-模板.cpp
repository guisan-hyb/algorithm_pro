// N 皇后模板：路径数组法 + 位运算法（位运算版常数更小）
#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

// ================= 法一：路径数组（好懂） =================
class SolutionArray {
    int n, ans = 0;
    vector<int> path;                              // path[i] = 第 i 行皇后所在列
    bool check(int i, int j) {
        for (int k = 0; k < i; k++)
            if (j == path[k] || abs(i - k) == abs(j - path[k]))
                return false;                      // 同列 或 同对角线
        return true;
    }
    void dfs(int i) {
        if (i == n) { ++ans; return; }
        for (int j = 0; j < n; j++) {
            if (check(i, j)) {
                path[i] = j;
                dfs(i + 1);
                path[i] = -1;                      // 回溯
            }
        }
    }
public:
    int totalNQueens(int n_) {
        n = n_; path.assign(n, -1); ans = 0;
        dfs(0);
        return ans;
    }
};

// ================= 法二：位运算（推荐） =================
class SolutionBit {
    int dfs(int limit, int col, int left, int right) {
        if (col == limit) return 1;
        int ans = 0;
        int candidate = limit & ~(col | left | right);
        while (candidate) {
            int place = candidate & (-candidate);   // 取出最低位的 1
            candidate ^= place;                     // 该位置已使用
            ans += dfs(limit, col | place,
                       (left  | place) >> 1,
                       (right | place) << 1);
        }
        return ans;
    }
public:
    int totalNQueens(int n) {
        return n < 1 ? 0 : dfs((1 << n) - 1, 0, 0, 0);
    }
};

int main() {
    for (int n = 1; n <= 10; n++)
        std::cout << "n=" << std::setw(2) << n << "  数组法=" << SolutionArray().totalNQueens(n) << "  位运算法=" << SolutionBit().totalNQueens(n) << "\n";
    return 0;
}
