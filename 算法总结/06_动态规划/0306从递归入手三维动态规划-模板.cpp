// 三维动态规划模板：一和零 / 盈利计划 / 骑士概率 / 同余路径计数
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ================= 1. 一和零（多维费用背包，压缩成二维） =================
int findMaxForm(const vector<string>& strs, int m, int n) {
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (const string& s : strs) {
        int zeros = 0, ones = 0;
        for (char c : s) (c == '0' ? zeros : ones)++;
        for (int z = m; z >= zeros; z--)              // ★两个维度都倒序
            for (int o = n; o >= ones; o--)
                dp[z][o] = max(dp[z][o], 1 + dp[z - zeros][o - ones]);
    }
    return dp[m][n];
}

// ================= 2. 盈利计划（记忆化 + 超额截断） =================
class ProfitableSchemes {
    static const int MOD = 1000000007;
    vector<int> group, profit;
    int minProfit = 0;
    vector<vector<vector<long long>>> dp;
    int f(int i, int n, int sum) {
        if (i < 0 || n <= 0) return sum >= minProfit ? 1 : 0;
        if (sum > minProfit) sum = minProfit;         // 关键优化：超额部分不再区分
        if (dp[i][n][sum] != -1) return (int)dp[i][n][sum];
        long long ans = f(i - 1, n, sum);
        if (n >= group[i]) ans += f(i - 1, n - group[i], sum + profit[i]);
        return (int)(dp[i][n][sum] = ans % MOD);
    }
public:
    int profitableSchemes(int n, int mp, vector<int>& g, vector<int>& p) {
        group = g; profit = p; minProfit = mp;
        int m = (int)p.size();
        dp.assign(m, vector<vector<long long>>(n + 1, vector<long long>(mp + 1, -1)));
        return f(m - 1, n, 0);
    }
};

// ================= 3. 骑士在棋盘上的概率（平移 + 逐层迭代） =================
double knightProbability(int n, int k, int row, int column) {
    const std::vector<std::array<int, 2>> moves = {
        {-1, -2}, {-1, 2}, {-2, -1}, {-2, 1},
        {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };
    vector<vector<vector<double>>> dp(k + 1,
        vector<vector<double>>(n + 4, vector<double>(n + 4, 0)));
    for (int i = 2; i <= n + 1; i++)
        for (int j = 2; j <= n + 1; j++) dp[0][i][j] = 1;
    for (int step = 1; step <= k; step++)
        for (int i = 2; i <= n + 1; i++)
            for (int j = 2; j <= n + 1; j++) {
                double tmp = 0;
                for (const auto& move : moves) {
                    tmp += dp[step - 1][i + move[0]][j + move[1]] / 8;
                }
                dp[step][i][j] = tmp;
            }
    return dp[k][row + 2][column + 2];
}

// ================= 4. 矩阵中和能被 K 整除的路径（同余做第三维） =================
int numberOfPaths(vector<vector<int>>& grid, int k) {
    const int MOD = 1000000007;
    int m = (int)grid.size(), n = (int)grid[0].size();
    vector<vector<vector<int>>> dp(m, vector<vector<int>>(n, vector<int>(k, 0)));
    dp[m - 1][n - 1][grid[m - 1][n - 1] % k] = 1;
    for (int i = m - 2; i >= 0; i--)
        for (int r = 0; r < k; r++)
            dp[i][n - 1][r] = dp[i + 1][n - 1][(r - grid[i][n - 1] % k + k) % k];
    for (int j = n - 2; j >= 0; j--)
        for (int r = 0; r < k; r++)
            dp[m - 1][j][r] = dp[m - 1][j + 1][(r - grid[m - 1][j] % k + k) % k];
    for (int i = m - 2; i >= 0; i--)
        for (int j = n - 2; j >= 0; j--)
            for (int r = 0; r < k; r++) {
                int need = (r - grid[i][j] % k + k) % k;
                dp[i][j][r] = (dp[i + 1][j][need] + dp[i][j + 1][need]) % MOD;
            }
    return dp[0][0][0];
}

int main() {
    vector<string> strs = {"10", "0001", "111001", "1", "0"};
    std::cout << "一和零 = " << findMaxForm(strs, 5, 3) << "\n";            // 4

    vector<int> g = {2,2,3,3}, p = {2,3,1,3};
    ProfitableSchemes ps;
    std::cout << "盈利计划 = " << ps.profitableSchemes(5, 3, g, p) << "\n";  // 3

    std::cout << "骑士概率 = " << std::fixed << std::setprecision(5) << knightProbability(3, 2, 0, 0) << std::defaultfloat << std::setprecision(6) << "\n";   // 0.0625

    vector<vector<int>> grid = {{5,2,4},{3,0,5},{0,7,2}};
    std::cout << "整除路径数 = " << numberOfPaths(grid, 3) << "\n";          // 2
    return 0;
}
