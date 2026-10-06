// 二维动态规划模板：最小路径和 / LCS / 最长回文子序列 / 二叉树结构数 / 矩阵最长递增路径
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// ================= 1. 最小路径和（二维 DP + 空间压缩） =================
int minPathSum(const vector<vector<int>>& g) {
    int m = (int)g.size(), n = (int)g[0].size();
    vector<int> dp(n, 0);
    dp[0] = g[0][0];
    for (int j = 1; j < n; j++) dp[j] = dp[j - 1] + g[0][j];
    for (int i = 1; i < m; i++) {
        dp[0] += g[i][0];
        for (int j = 1; j < n; j++)
            dp[j] = min(dp[j - 1], dp[j]) + g[i][j];
    }
    return dp[n - 1];
}

// ================= 2. 最长公共子序列（长度定义 + 空间压缩） =================
int longestCommonSubsequence(string s1, string s2) {
    if (s1.size() > s2.size()) swap(s1, s2);       // 让 s1 更短
    int m = (int)s1.size(), n = (int)s2.size();
    vector<int> dp(m + 1, 0);
    for (int len2 = 1; len2 <= n; len2++) {         // 外层遍历长串
        int leftup = 0;                             // dp[i-1][j-1]
        for (int len1 = 1; len1 <= m; len1++) {
            int backup = dp[len1];
            if (s1[len1 - 1] == s2[len2 - 1]) dp[len1] = 1 + leftup;
            else                              dp[len1] = max(dp[len1], dp[len1 - 1]);
            leftup = backup;
        }
    }
    return dp[m];
}

// ================= 3. 最长回文子序列（区间 DP + 空间压缩） =================
int longestPalindromeSubseq(const string& s) {
    int n = (int)s.size();
    vector<int> dp(n, 0);
    for (int l = n - 1; l >= 0; l--) {
        int leftdown = 0, backup = 0;
        for (int r = l; r < n; r++) {
            backup = dp[r];
            if (l == r)            dp[r] = 1;
            else if (s[l] == s[r]) dp[r] = 2 + leftdown;
            else                   dp[r] = max(dp[r - 1], dp[r]);
            leftdown = backup;
        }
    }
    return dp[n - 1];
}

// ================= 4. 编辑距离（二维 DP + 空间压缩，三操作可不同代价） =================
int editDistance(const string& s1, const string& s2, int a, int b, int c) {
    int m = (int)s1.size(), n = (int)s2.size();
    vector<int> dp(n + 1, 0);
    for (int j = 0; j <= n; j++) dp[j] = j * a;      // s1 为空：全是插入
    for (int i = 1; i <= m; i++) {
        int leftup = dp[0];
        dp[0] = i * b;                               // s2 为空：全是删除
        for (int j = 1; j <= n; j++) {
            int backup = dp[j];
            if (s1[i - 1] == s2[j - 1]) dp[j] = leftup;
            else dp[j] = min(min(dp[j - 1] + a, dp[j] + b), leftup + c);
            leftup = backup;
        }
    }
    return dp[n];
}

// ================= 5. 矩阵中的最长递增路径（只能记忆化搜索） =================
class LongestIncreasingPath {
    int m, n;
    vector<vector<int>> dp;
    int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
    int f(const vector<vector<int>>& g, int i, int j) {
        if (dp[i][j] != -1) return dp[i][j];
        int nxt = 0;
        for (int d = 0; d < 4; d++) {
            int x = i + dx[d], y = j + dy[d];
            if (x >= 0 && x < m && y >= 0 && y < n && g[x][y] > g[i][j])
                nxt = max(nxt, f(g, x, y));
        }
        return dp[i][j] = 1 + nxt;
    }
public:
    int longestIncreasingPath(const vector<vector<int>>& g) {
        m = (int)g.size(); n = (int)g[0].size();
        dp.assign(m, vector<int>(n, -1));
        int ans = 0;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                ans = max(ans, f(g, i, j));
        return ans;
    }
};

int main() {
    vector<vector<int>> g = {{1,3,1},{1,5,1},{4,2,1}};
    printf("最小路径和 = %d\n", minPathSum(g));                        // 7
    printf("LCS = %d\n", longestCommonSubsequence("abcde", "ace"));    // 3
    printf("最长回文子序列 = %d\n", longestPalindromeSubseq("bbbab")); // 4
    printf("编辑距离 = %d\n", editDistance("horse", "ros", 1, 1, 1));  // 3
    LongestIncreasingPath lip;
    printf("最长递增路径 = %d\n", lip.longestIncreasingPath(g));       // 4
    return 0;
}
