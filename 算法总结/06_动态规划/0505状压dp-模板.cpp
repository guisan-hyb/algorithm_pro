// 状压 DP 模板：TSP / 等和子集 / 帽子分配 / 子集枚举 trick
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// ================= 1. TSP：dp[status][i] =================
int tsp(int n, vector<vector<int>>& d) {
    vector<vector<int>> dp(1 << n, vector<int>(n, -1));
    function<int(int,int)> f = [&](int status, int i) -> int {
        if (status == (1 << n) - 1) return d[i][0];
        if (dp[status][i] != -1) return dp[status][i];
        int ans = INT_MAX;
        for (int k = 1; k < n; k++)
            if (!(status & (1 << k)))
                ans = min(ans, f(status | (1 << k), k) + d[i][k]);
        return dp[status][i] = ans;
    };
    return f(1, 0);
}

// ================= 2. 我能赢吗（只缓存 status） =================
class CanIWin {
    vector<char> dp;
    int n;
    bool f(int status, int rest) {
        if (rest <= 0) return false;
        if (dp[status] != -1) return dp[status];
        char ans = 0;
        for (int i = 1; i <= n; i++)
            if ((status & (1 << i)) && !f(status ^ (1 << i), rest - i)) { ans = 1; break; }
        return dp[status] = ans;
    }
public:
    bool canIWin(int maxChoosableInteger, int desiredTotal) {
        n = maxChoosableInteger;
        if (desiredTotal == 0) return true;
        if ((1 + n) * n / 2 < desiredTotal) return false;
        dp.assign(1 << (n + 1), -1);
        return f((1 << (n + 1)) - 1, desiredTotal);
    }
};

// ================= 3. 划分为 k 个等和子集（暴力 + 剪枝） =================
bool canPartitionKSubsets(vector<int> nums, int k) {
    int sum = 0;
    for (int x : nums) sum += x;
    if (sum % k) return false;
    int aim = sum / k;
    sort(nums.begin(), nums.end());
    if (nums.back() > aim) return false;
    vector<int> group(k, 0);
    function<bool(int)> f = [&](int idx) -> bool {
        if (idx < 0) return true;
        for (int i = 0; i < k; i++) {
            if (group[i] + nums[idx] <= aim) {
                group[i] += nums[idx];
                if (f(idx - 1)) return true;
                group[i] -= nums[idx];
                while (i + 1 < k && group[i] == group[i + 1]) i++;   // ★跳过同和值的桶
            }
        }
        return false;
    };
    return f((int)nums.size() - 1);
}

// ================= 4. 每个人戴不同帽子（帽子选人 + Brian 算法） =================
int numberWays(vector<vector<int>>& hats) {
    const long long MOD = 1000000007;
    int n = (int)hats.size(), maxHat = 0;
    for (auto& h : hats) for (int x : h) maxHat = max(maxHat, x);
    vector<int> hat(maxHat + 1, 0);                      // 每顶帽子被哪些人喜欢
    for (int i = 0; i < n; i++)
        for (int x : hats[i]) hat[x] |= (1 << i);
    vector<vector<int>> dp(1 << n, vector<int>(maxHat + 1, -1));
    function<int(int,int)> f = [&](int status, int hatIdx) -> int {
        if (status == (1 << n) - 1) return 1;
        if (hatIdx > maxHat) return 0;
        if (dp[status][hatIdx] != -1) return dp[status][hatIdx];
        long long ans = f(status, hatIdx + 1);           // 这顶帽子不给任何人
        int info = hat[hatIdx];
        while (info) {                                   // Brian 算法逐个取位
            int one = info & (-info);
            if (!(status & one)) ans += f(status | one, hatIdx + 1);
            info ^= one;
        }
        return dp[status][hatIdx] = (int)(ans % MOD);
    };
    return f(0, 1);
}

// ================= 5. 分配重复整数（子集需求 + 子集枚举） =================
bool canDistribute(vector<int> nums, vector<int> quantity) {
    sort(nums.begin(), nums.end());
    vector<int> cnt;
    for (size_t i = 0; i < nums.size(); ) {
        size_t j = i;
        while (j < nums.size() && nums[j] == nums[i]) j++;
        cnt.push_back((int)(j - i));
        i = j;
    }
    int m = (int)quantity.size();
    vector<int> need(1 << m, 0);
    for (int i = 0; i < m; i++) {
        int bit = 1 << i;
        for (int j = 0; j < bit; j++) need[bit | j] = quantity[i] + need[j];
    }
    vector<vector<char>> dp(1 << m, vector<char>(cnt.size(), -1));
    function<bool(int,int)> f = [&](int status, int i) -> bool {
        if (status == 0) return true;
        if (i == (int)cnt.size()) return false;
        if (dp[status][i] != -1) return dp[status][i];
        bool ans = false;
        for (int j = status; j > 0 && !ans; j = (j - 1) & status)   // ★枚举所有子集
            if (need[j] <= cnt[i]) ans = f(status ^ j, i + 1);
        if (!ans) ans = f(status, i + 1);
        return dp[status][i] = (char)ans;
    };
    return f((1 << m) - 1, 0);
}

int main() {
    int n = 4;
    vector<vector<int>> d = {{0,2,9,10},{1,0,6,4},{15,7,0,8},{6,3,12,0}};
    printf("TSP 最短路 = %d\n", tsp(n, d));                     // 21
    printf("划分为 4 个等和子集 = %d\n", (int)canPartitionKSubsets({1,1,2,2,2}, 4));  // 0
    vector<vector<int>> hats = {{3,4},{4,5},{5}};
    printf("戴帽子方案数 = %d\n", numberWays(hats));            // 1
    return 0;
}
