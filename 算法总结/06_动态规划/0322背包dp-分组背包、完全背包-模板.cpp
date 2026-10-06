// 分组背包 / 完全背包 / 正则匹配 / 通配符匹配 / 足量干草模板
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// ================= 1. 分组背包（每组最多取一件） =================
int groupKnapsack(int cap, const vector<vector<pair<int,int>>>& groups) {
    vector<long long> dp(cap + 1, 0);
    for (auto& g : groups) {
        for (int j = cap; j >= 0; j--)                 // ★容量倒序
            for (auto& it : g)
                if (j >= it.first)
                    dp[j] = max(dp[j], dp[j - it.first] + (long long)it.second);
    }
    return (int)dp[cap];
}

// ================= 2. 从栈中取 K 个硬币（分组背包 + 前缀和） =================
int maxValueOfCoins(const vector<vector<int>>& piles, int k) {
    int n = (int)piles.size();
    vector<vector<int>> dp(n + 1, vector<int>(k + 1, 0));
    for (int i = 1; i <= n; i++) {
        const vector<int>& nums = piles[i - 1];
        int t = min((int)nums.size(), k);
        vector<int> pre(t + 1, 0);
        for (int j = 1; j <= t; j++) pre[j] = pre[j - 1] + nums[j - 1];
        for (int j = 0; j <= k; j++) {
            dp[i][j] = dp[i - 1][j];
            for (int c = 0; c <= t && c <= j; c++)
                dp[i][j] = max(dp[i][j], dp[i - 1][j - c] + pre[c]);
        }
    }
    return dp[n][k];
}

// ================= 3. 完全背包（容量正序） =================
long long completeKnapsack(int cap, const vector<int>& cost, const vector<int>& val) {
    vector<long long> dp(cap + 1, 0);
    for (size_t i = 0; i < cost.size(); i++)
        for (int j = cost[i]; j <= cap; j++)           // ★容量正序
            dp[j] = max(dp[j], dp[j - cost[i]] + val[i]);
    return dp[cap];
}

// ================= 4. 正则表达式匹配 =================
bool isMatchRegex(const string& s, const string& p) {
    int m = (int)s.size(), n = (int)p.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    dp[m][n] = 1;
    for (int j = n - 1; j >= 0; j--)
        dp[m][j] = (j + 1 < n && p[j + 1] == '*' && dp[m][j + 2]);
    for (int i = m - 1; i >= 0; i--) {
        for (int j = n - 1; j >= 0; j--) {
            if (j + 1 == n || p[j + 1] != '*') {
                dp[i][j] = (p[j] == s[i] || p[j] == '.') && dp[i + 1][j + 1];
            } else {
                bool p1 = dp[i][j + 2];                                    // x* 匹配 0 次
                bool p2 = (p[j] == s[i] || p[j] == '.') && dp[i + 1][j];   // 匹配 1 次且继续
                dp[i][j] = p1 || p2;
            }
        }
    }
    return dp[0][0];
}

// ================= 5. 通配符匹配 =================
bool isMatchWildcard(const string& s, const string& p) {
    int m = (int)s.size(), n = (int)p.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    dp[m][n] = 1;
    for (int j = n - 1; j >= 0; j--) dp[m][j] = (p[j] == '*' && dp[m][j + 1]);
    for (int i = m - 1; i >= 0; i--) {
        for (int j = n - 1; j >= 0; j--) {
            if (p[j] != '*') dp[i][j] = (p[j] == s[i] || p[j] == '?') && dp[i + 1][j + 1];
            else             dp[i][j] = dp[i][j + 1] || dp[i + 1][j];   // 消掉 * 或吃一个字符
        }
    }
    return dp[0][0];
}

// ================= 6. 购买足量干草的最小花费（完全背包 + 数组扩充） =================
int minHayCost(int h, const vector<int>& cost, const vector<int>& amount) {
    int n = (int)cost.size(), extra = 0;
    for (int a : amount) extra = max(extra, a);        // 最多扩充"单份最大值"
    vector<vector<int>> dp(n + 1, vector<int>(h + extra + 1, INT_MAX));
    for (int i = 0; i <= n; i++) dp[i][0] = 0;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= h + extra; j++) {
            dp[i][j] = dp[i - 1][j];
            if (j >= amount[i - 1] && dp[i][j - amount[i - 1]] != INT_MAX)
                dp[i][j] = min(dp[i][j], dp[i][j - amount[i - 1]] + cost[i - 1]);
        }
    int ans = INT_MAX;
    for (int j = h; j <= h + extra; j++) ans = min(ans, dp[n][j]);   // 至少 h
    return ans;
}

int main() {
    vector<vector<pair<int,int>>> groups = {{{2,3},{3,5}}, {{1,4}}};
    printf("分组背包 = %d\n", groupKnapsack(4, groups));        // 7
    vector<vector<int>> piles = {{1,100,3},{7,8,9}};
    printf("取 K 个硬币 = %d\n", maxValueOfCoins(piles, 2));     // 101
    printf("完全背包 = %lld\n", completeKnapsack(10, {5,3}, {20,12}));  // 40
    printf("正则 = %d, 通配符 = %d\n",
           (int)isMatchRegex("aa", "a*"), (int)isMatchWildcard("cb", "?a"));  // 1 0
    printf("足量干草最小花费 = %d\n", minHayCost(7, {5}, {3}));  // 15
    return 0;
}
