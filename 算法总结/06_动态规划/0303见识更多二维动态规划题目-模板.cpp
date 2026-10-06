// 二维字符串 DP 模板：不同子序列 / 编辑距离 / 交错字符串 / 涂色方案 / 删除成子串
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// ================= 1. 不同的子序列（空间压缩） =================
int numDistinct(const string& s, const string& t) {
    int m = (int)s.size(), n = (int)t.size();
    vector<unsigned long long> dp(n + 1, 0);
    dp[0] = 1;                                   // t 为空 → 一种方案
    for (int i = 1; i <= m; i++)
        for (int j = n; j >= 1; j--)             // 依赖上方与左上 → 从右往左
            if (s[i - 1] == t[j - 1]) dp[j] += dp[j - 1];
    return (int)dp[n];
}

// ================= 2. 编辑距离（三种代价可不同 + 空间压缩） =================
int editDistance(const string& s1, const string& s2, int a, int b, int c) {
    int m = (int)s1.size(), n = (int)s2.size();
    vector<int> dp(n + 1, 0);
    for (int j = 0; j <= n; j++) dp[j] = j * a;   // 插入
    for (int i = 1; i <= m; i++) {
        int leftup = dp[0];
        dp[0] = i * b;                            // 删除
        for (int j = 1; j <= n; j++) {
            int backup = dp[j];
            if (s1[i - 1] == s2[j - 1]) dp[j] = leftup;
            else dp[j] = min(min(dp[j - 1] + a, dp[j] + b), leftup + c);
            leftup = backup;
        }
    }
    return dp[n];
}

// ================= 3. 交错字符串 =================
bool isInterleave(const string& s1, const string& s2, const string& s3) {
    int len1 = (int)s1.size(), len2 = (int)s2.size();
    if (len1 + len2 != (int)s3.size()) return false;
    vector<vector<bool>> dp(len1 + 1, vector<bool>(len2 + 1, false));
    dp[0][0] = true;
    for (int i = 1; i <= len1; i++) {
        if (s1[i - 1] != s3[i - 1]) break;        // 首列：连续匹配才为真
        dp[i][0] = true;
    }
    for (int j = 1; j <= len2; j++) {
        if (s2[j - 1] != s3[j - 1]) break;
        dp[0][j] = true;
    }
    for (int i = 1; i <= len1; i++)
        for (int j = 1; j <= len2; j++)
            dp[i][j] = (s1[i - 1] == s3[i + j - 1] && dp[i - 1][j])
                    || (s2[j - 1] == s3[i + j - 1] && dp[i][j - 1]);
    return dp[len1][len2];
}

// ================= 4. 有效涂色（n 个格子涂满 m 种颜色的方案数） =================
int coloringWays(int n, int m) {
    const long long MOD = 1000000007;
    vector<vector<long long>> dp(n + 1, vector<long long>(m + 1, 0));
    for (int i = 1; i <= n; i++) dp[i][1] = m;    // 只用一种颜色
    for (int i = 2; i <= n; i++)
        for (int j = 2; j <= m; j++)
            dp[i][j] = (dp[i - 1][j] * j + dp[i - 1][j - 1] * (m - (j - 1))) % MOD;
    return (int)dp[n][m];
}

// ================= 5. s1 至少删除多少字符能变成 s2 的子串 =================
int minDeleteToSubstring(const string& s1, const string& s2) {
    int m = (int)s1.size(), n = (int)s2.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (int i = 0; i <= m; i++) dp[i][0] = i;    // 目标是空串 → 全删
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            dp[i][j] = (s1[i - 1] != s2[j - 1]) ? 1 + dp[i - 1][j] : dp[i - 1][j - 1];
    int ans = INT_MAX;
    for (int j = 0; j <= n; j++) ans = min(ans, dp[m][j]);   // 取最后一行最小值
    return ans;
}

int main() {
    printf("不同子序列 = %d\n", numDistinct("rabbbit", "rabbit"));        // 3
    printf("编辑距离 = %d\n", editDistance("horse", "ros", 1, 1, 1));     // 3
    printf("交错字符串 = %d\n", (int)isInterleave("aab", "axy", "aaxaby"));// 1
    printf("涂色方案 = %d\n", coloringWays(3, 2));                        // 6
    printf("最少删除 = %d\n", minDeleteToSubstring("abcde", "bcd"));      // 2
    return 0;
}
