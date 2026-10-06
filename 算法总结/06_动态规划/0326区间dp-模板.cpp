// 区间 DP 模板：回文插入 / 预测赢家 / 三角剖分 / 切棍子 / 戳气球 / 合并石头 / 不同回文子序列
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// ================= 1. 让字符串成为回文的最少插入次数 =================
int minInsertions(const string& s) {
    int n = (int)s.size();
    if (n < 2) return 0;
    vector<int> dp(n, 0);
    dp[n - 1] = (s[n - 2] == s[n - 1]) ? 0 : 1;
    for (int l = n - 3; l >= 0; l--) {
        int leftdown = 0, backup = 0;
        for (int r = l + 1; r < n; r++) {
            backup = dp[r];
            if (s[l] == s[r]) dp[r] = leftdown;
            else              dp[r] = min(dp[r - 1], dp[r]) + 1;
            leftdown = backup;
        }
    }
    return dp[n - 1];
}

// ================= 2. 预测赢家 =================
bool predictTheWinner(const vector<int>& nums) {
    int n = (int)nums.size(), sum = 0;
    for (int x : nums) sum += x;
    vector<vector<int>> dp(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) dp[i][i] = nums[i];
    for (int i = 0; i + 1 < n; i++) dp[i][i + 1] = max(nums[i], nums[i + 1]);
    for (int l = n - 3; l >= 0; l--)
        for (int r = l + 2; r < n; r++) {
            int p1 = nums[l] + min(dp[l + 2][r], dp[l + 1][r - 1]);
            int p2 = nums[r] + min(dp[l + 1][r - 1], dp[l][r - 2]);
            dp[l][r] = max(p1, p2);
        }
    return dp[0][n - 1] >= sum - dp[0][n - 1];
}

// ================= 3. 多边形三角剖分的最低得分 =================
int minScoreTriangulation(const vector<int>& values) {
    int n = (int)values.size();
    vector<vector<int>> dp(n, vector<int>(n, 0));
    for (int l = n - 3; l >= 0; l--)
        for (int r = l + 2; r < n; r++) {
            int best = INT_MAX;
            for (int k = l + 1; k < r; k++)
                best = min(best, values[l] * values[k] * values[r] + dp[l][k] + dp[k][r]);
            dp[l][r] = best;
        }
    return dp[0][n - 1];
}

// ================= 4. 切棍子的最小成本（离散化） =================
int minCostToCut(int n, vector<int> cuts) {
    sort(cuts.begin(), cuts.end());
    int m = (int)cuts.size();
    vector<int> pos(m + 2);
    pos[0] = 0;
    for (int i = 0; i < m; i++) pos[i + 1] = cuts[i];
    pos[m + 1] = n;
    vector<vector<int>> dp(m + 2, vector<int>(m + 2, 0));
    for (int i = 1; i <= m; i++) dp[i][i] = pos[i + 1] - pos[i - 1];
    for (int l = m - 1; l >= 1; l--)
        for (int r = l + 1; r <= m; r++) {
            dp[l][r] = INT_MAX;
            for (int k = l; k <= r; k++)
                dp[l][r] = min(dp[l][r], pos[r + 1] - pos[l - 1] + dp[l][k - 1] + dp[k + 1][r]);
        }
    return dp[1][m];
}

// ================= 5. 戳气球（k 最后打爆） =================
int maxCoins(const vector<int>& nums) {
    int n = (int)nums.size();
    vector<int> arr(n + 2, 1);
    for (int i = 0; i < n; i++) arr[i + 1] = nums[i];
    vector<vector<int>> dp(n + 2, vector<int>(n + 2, 0));
    for (int l = n; l >= 1; l--)
        for (int r = l; r <= n; r++)
            for (int k = l; k <= r; k++)
                dp[l][r] = max(dp[l][r],
                    arr[l - 1] * arr[k] * arr[r + 1] + dp[l][k - 1] + dp[k + 1][r]);
    return dp[1][n];
}

// ================= 6. 合并石头的最低成本 =================
int mergeStones(const vector<int>& stones, int k) {
    int n = (int)stones.size();
    if (k > 1 && (n - 1) % (k - 1) != 0) return -1;
    vector<int> pre(n + 1, 0);
    for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + stones[i];
    vector<vector<int>> dp(n, vector<int>(n, INT_MAX));
    for (int i = 0; i < n; i++) dp[i][i] = 0;
    for (int l = n - 2; l >= 0; l--)
        for (int r = l + 1; r < n; r++) {
            for (int m = l; m < r; m += k - 1)         // ★每次跳 k-1
                dp[l][r] = min(dp[l][r], dp[l][m] + dp[m + 1][r]);
            if ((r - l) % (k - 1) == 0) dp[l][r] += pre[r + 1] - pre[l];
        }
    return dp[0][n - 1];
}

// ================= 7. 统计不同回文子序列 =================
int countPalindromicSubsequences(const string& s) {
    const long long MOD = 1000000007;
    int n = (int)s.size();
    vector<int> prevPos(n), nextPos(n), last(256, -1);
    for (int i = 0; i < n; i++) { prevPos[i] = last[(unsigned char)s[i]]; last[(unsigned char)s[i]] = i; }
    fill(last.begin(), last.end(), n);
    for (int i = n - 1; i >= 0; i--) { nextPos[i] = last[(unsigned char)s[i]]; last[(unsigned char)s[i]] = i; }
    vector<vector<long long>> dp(n, vector<long long>(n, 0));
    for (int i = 0; i < n; i++) dp[i][i] = 1;
    for (int i = n - 2; i >= 0; i--)
        for (int j = i + 1; j < n; j++) {
            if (s[i] != s[j]) {
                dp[i][j] = dp[i][j - 1] + dp[i + 1][j] - dp[i + 1][j - 1] + MOD;
            } else {
                int l = nextPos[i], r = prevPos[j];
                if (l > r)       dp[i][j] = 2 * dp[i + 1][j - 1] + 2;
                else if (l == r) dp[i][j] = 2 * dp[i + 1][j - 1] + 1;
                else             dp[i][j] = 2 * dp[i + 1][j - 1] - dp[l + 1][r - 1] + MOD;
            }
            dp[i][j] %= MOD;
        }
    return (int)dp[0][n - 1];
}

int main() {
    printf("回文最少插入 = %d\n", minInsertions("mbadm"));               // 2
    printf("预测赢家 = %d\n", (int)predictTheWinner({1,5,2}));            // 0(输)
    printf("三角剖分最低分 = %d\n", minScoreTriangulation({1,3,1,4,1,5}));// 13
    printf("切棍最小成本 = %d\n", minCostToCut(7, {1,3,4,5}));            // 16
    printf("戳气球最大 = %d\n", maxCoins({3,1,5,8}));                     // 167
    printf("合并石头最低成本 = %d\n", mergeStones({3,2,4,1}, 2));         // 20
    printf("不同回文子序列 = %d\n", countPalindromicSubsequences("bccb"));// 6
    return 0;
}
