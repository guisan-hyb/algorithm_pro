// 多重背包模板：二进制分组 / 混合背包 / 可行性背包
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// ================= 1. 二进制分组：多重背包 -> 01 背包 =================
long long multiKnapsackBinary(int cap, const vector<int>& weight,
                              const vector<int>& val, const vector<int>& cnt) {
    vector<long long> w, v;
    for (size_t i = 0; i < weight.size(); i++) {
        int c = cnt[i];
        for (int k = 1; k <= c; k <<= 1) {           // ★1,2,4,8,...
            w.push_back((long long)weight[i] * k);
            v.push_back((long long)val[i] * k);
            c -= k;
        }
        if (c > 0) {                                 // 余数单独一组
            w.push_back((long long)weight[i] * c);
            v.push_back((long long)val[i] * c);
        }
    }
    vector<long long> dp(cap + 1, 0);
    for (size_t i = 0; i < w.size(); i++)
        for (int j = cap; j >= w[i]; j--)            // 01 背包：容量倒序
            dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
    return dp[cap];
}

// ================= 2. 混合背包（cnt == 0 表示无限件） =================
long long mixedKnapsack(int cap, const vector<int>& weight,
                        const vector<int>& val, const vector<int>& cnt) {
    vector<int> fixed = cnt;
    for (size_t i = 0; i < fixed.size(); i++)
        if (fixed[i] == 0) fixed[i] = cap / weight[i];   // 完全背包转多重
    return multiKnapsackBinary(cap, weight, val, fixed);
}

// ================= 3. 可行性多重背包 + used 窗口优化 =================
// 返回 1..m 中有多少金额能被凑出
int countableAmounts(int m, const vector<int>& val, const vector<int>& cnt) {
    vector<char> dp(m + 1, 0);
    vector<int> used(m + 1, 0);
    dp[0] = 1;
    for (size_t i = 0; i < val.size(); i++) {
        fill(used.begin(), used.end(), 0);           // 每种货币重新计数
        for (int j = val[i]; j <= m; j++)
            if (!dp[j] && dp[j - val[i]] && used[j - val[i]] < cnt[i]) {
                dp[j] = 1;
                used[j] = used[j - val[i]] + 1;
            }
    }
    int ans = 0;
    for (int j = 1; j <= m; j++) ans += dp[j];
    return ans;
}

// ================= 4. 多重背包（不优化，便于对照与对拍） =================
int multiKnapsackNaive(int cap, const vector<int>& weight,
                       const vector<int>& val, const vector<int>& cnt) {
    int n = (int)weight.size();
    vector<vector<int>> dp(n + 1, vector<int>(cap + 1, 0));
    for (int i = 1; i <= n; i++)
        for (int j = 0; j <= cap; j++) {
            dp[i][j] = dp[i - 1][j];
            for (int k = 1; k <= cnt[i - 1] && k * weight[i - 1] <= j; k++)
                dp[i][j] = max(dp[i][j], dp[i - 1][j - k * weight[i - 1]] + k * val[i - 1]);
        }
    return dp[n][cap];
}

int main() {
    // 洛谷 P1776 样例：1 种货物，价值 5、重量 3、数量 13，容量 10
    vector<int> w = {3}, v = {5}, c = {13};
    printf("多重背包(二进制) = %lld\n", multiKnapsackBinary(10, w, v, c));   // 15
    printf("多重背包(朴素)   = %d\n", multiKnapsackNaive(10, w, v, c));      // 15

    // 混合背包：货物 1 无限件、货物 2 限 2 件
    vector<int> w2 = {2, 3}, v2 = {3, 4}, c2 = {0, 2};
    printf("混合背包 = %lld\n", mixedKnapsack(10, w2, v2, c2));              // 15

    // POJ 1742 样例：面值 3 的货币 5 张，问 1~10 能凑出几种
    printf("可凑出金额种数 = %d\n", countableAmounts(10, {3}, {5}));          // 3
    return 0;
}
