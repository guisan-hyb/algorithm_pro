// 优化枚举模板：股票系列 / DI 序列（前缀和）/ K 个逆序对（滑窗）/ 兼职工作（二分）
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <array>
#include <climits>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ================= 1. 一次交易 =================
int maxProfitOnce(const vector<int>& prices) {
    int ans = 0, minPrice = INT_MAX;
    for (int p : prices) {
        ans = max(ans, p - minPrice);
        minPrice = min(minPrice, p);
    }
    return ans;
}

// ================= 2. 无限次交易 =================
int maxProfitUnlimited(const vector<int>& prices) {
    int ans = 0;
    for (int i = 1; i < (int)prices.size(); i++)
        ans += max(0, prices[i] - prices[i - 1]);
    return ans;
}

// ================= 3. 最多两笔（预处理 best 消除枚举） =================
int maxProfitTwo(const vector<int>& prices) {
    int n = (int)prices.size();
    if (n < 2) return 0;
    int dp1 = 0, best = -prices[0], dp2 = 0, ans = 0;
    for (int i = 1, minPrice = prices[0]; i < n; i++) {
        minPrice = min(minPrice, prices[i]);
        dp1 = max(dp1, prices[i] - minPrice);
        best = max(best, dp1 - prices[i]);       // 滚动维护 max(dp1[p] - prices[p])
        dp2 = best + prices[i];
        ans = max(ans, dp2);
    }
    return ans;
}

// ================= 4. 最多 k 笔（O(kn)） =================
int maxProfitK(int k, const vector<int>& prices) {
    int n = (int)prices.size();
    if (k >= n / 2) return maxProfitUnlimited(prices);   // 剪枝
    vector<int> dp(n, 0);
    for (int t = 1; t <= k; t++) {
        int best = dp[0] - prices[0];
        for (int j = 1; j < n; j++) {
            int tmp = dp[j];
            dp[j] = max(dp[j - 1], best + prices[j]);
            best = max(best, tmp - prices[j]);
        }
    }
    return dp[n - 1];
}

// ================= 5. 含手续费 / 含冷冻期（记忆化） =================
int maxProfitWithFee(vector<int>& prices, int fee) {
    int n = (int)prices.size();
    vector<vector<int>> dp(n + 1, vector<int>(2, 0));
    for (int i = n - 1; i >= 0; i--) {
        dp[i][1] = max(dp[i + 1][1], dp[i + 1][0] + prices[i]);          // 持有
        dp[i][0] = max(dp[i + 1][0], dp[i + 1][1] - prices[i] - fee);    // 不持有
    }
    return dp[0][0];
}
int maxProfitWithCooldown(vector<int>& prices) {
    int n = (int)prices.size();
    vector<vector<int>> dp(n + 2, vector<int>(2, 0));
    for (int i = n - 1; i >= 0; i--) {
        dp[i][1] = max(dp[i + 1][1], dp[i + 2][0] + prices[i]);          // 卖完要跳过一天
        dp[i][0] = max(dp[i + 1][0], dp[i + 1][1] - prices[i]);
    }
    return dp[0][0];
}

// ================= 6. DI 序列有效排列（前缀和优化） =================
int numPermsDISequence(const string& s) {
    const long long MOD = 1000000007;
    int n = (int)s.size() + 1;
    vector<vector<long long>> dp(n + 1, vector<long long>(n + 1, 0));
    for (int less = 0; less <= n; less++) dp[n][less] = 1;
    for (int i = n - 1; i >= 0; i--) {
        if (i == 0 || s[i - 1] == 'D') {
            dp[i][1] = dp[i + 1][0];
            for (int less = 2; less <= n; less++)
                dp[i][less] = (dp[i][less - 1] + dp[i + 1][less - 1]) % MOD;
        } else {
            dp[i][n - i - 1] = dp[i + 1][n - i - 1];
            for (int less = n - i - 2; less >= 0; less--)
                dp[i][less] = (dp[i][less + 1] + dp[i + 1][less]) % MOD;
        }
    }
    return (int)dp[0][n];
}

// ================= 7. K 个逆序对（滑动窗口优化） =================
int kInversePairs(int n, int k) {
    const long long MOD = 1000000007;
    vector<vector<long long>> dp(n + 1, vector<long long>(k + 1, 0));
    for (int i = 0; i <= n; i++) dp[i][0] = 1;
    for (int i = 1; i <= n; i++) {
        long long window = 1;
        for (int j = 1; j <= k; j++) {
            window += dp[i - 1][j];
            if (j >= i) window -= dp[i - 1][j - i];
            window = (window % MOD + MOD) % MOD;
            dp[i][j] = window;
        }
    }
    return (int)dp[n][k];
}

// ================= 8. 规划兼职工作（排序 + 二分） =================
int jobScheduling(vector<int> startTime, vector<int> endTime, vector<int> profit) {
    int n = (int)startTime.size();
    vector<array<int,3>> info(n);
    for (int i = 0; i < n; i++) info[i] = {endTime[i], startTime[i], profit[i]};
    sort(info.begin(), info.end());
    vector<int> dp(n, 0), ends(n);
    for (int i = 0; i < n; i++) ends[i] = info[i][0];
    dp[0] = info[0][2];
    for (int i = 1; i < n; i++) {
        int s = info[i][1], p = info[i][2];
        int pos = (int)(lower_bound(ends.begin(), ends.begin() + i, s + 1) - ends.begin()) - 1;
        dp[i] = max(dp[i - 1], p + (pos >= 0 ? dp[pos] : 0));
    }
    return dp[n - 1];
}

int main() {
    std::cout << "一次交易 = " << maxProfitOnce({7,1,5,3,6,4}) << "\n";        // 5
    std::cout << "无限次 = " << maxProfitUnlimited({7,1,5,3,6,4}) << "\n";      // 7
    std::cout << "最多两笔 = " << maxProfitTwo({3,3,5,0,0,3,1,4}) << "\n";      // 6
    std::cout << "最多 k=2 笔 = " << maxProfitK(2, {3,3,5,0,0,3,1,4}) << "\n";   // 6
    std::cout << "DI 排列数 = " << numPermsDISequence("DID") << "\n";           // 5
    std::cout << "K 个逆序对 = " << kInversePairs(3, 1) << "\n";                // 2
    std::cout << "兼职最大报酬 = " << jobScheduling({1,2,3,3}, {3,4,5,6}, {50,10,40,70}) << "\n"; // 120
    return 0;
}
