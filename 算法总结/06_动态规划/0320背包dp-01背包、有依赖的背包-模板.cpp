// 01 背包模板：基础 / 目标和转化 / 石头 / 前 k 小子序列和 / 第 K 大子序列和
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// ================= 1. 01 背包（空间压缩） =================
int knapsack01(int cap, const vector<int>& cost, const vector<int>& val) {
    vector<int> dp(cap + 1, 0);
    for (size_t i = 0; i < cost.size(); i++)
        for (int j = cap; j >= cost[i]; j--)              // ★容量倒序
            dp[j] = max(dp[j], dp[j - cost[i]] + val[i]);
    return dp[cap];
}

// ================= 2. 目标和 -> 子集和方案数（01 背包） =================
int findTargetSumWays(const vector<int>& nums, int target) {
    int sum = 0;
    for (int x : nums) sum += abs(x);
    if (target > sum || target < -sum || ((target + sum) & 1)) return 0;
    int aim = (target + sum) / 2;
    vector<int> dp(aim + 1, 0);
    dp[0] = 1;
    for (int x : nums) {
        int w = abs(x);
        for (int j = aim; j >= w; j--) dp[j] += dp[j - w];
    }
    return dp[aim];
}

// ================= 3. 最后一块石头的重量 II =================
int lastStoneWeightII(const vector<int>& stones) {
    int sum = 0;
    for (int s : stones) sum += s;
    int aim = sum / 2;
    vector<int> dp(aim + 1, 0);
    for (int s : stones)
        for (int j = aim; j >= s; j--)
            dp[j] = max(dp[j], dp[j - s] + s);
    return sum - 2 * dp[aim];
}

// ================= 4. 有依赖的背包（主件 + 最多 2 个附件） =================
// cost[i] / val[i] 为第 i 件的花费与收益；king[i] 表示 i 是主件
// follow[i][0..1] 为主件 i 的附件下标，fans[i] 为附件个数
int dependentKnapsack(int money, const vector<int>& cost, const vector<int>& val,
                      const vector<char>& king,
                      const vector<array<int,2>>& follow,
                      const vector<int>& fans) {
    int m = (int)cost.size() - 1;                        // 商品编号 1..m
    vector<vector<int>> dp(m + 1, vector<int>(money + 1, 0));
    int preMain = 0;
    for (int i = 1; i <= m; i++) {
        if (!king[i]) continue;
        int p1 = fans[i] >= 1 ? follow[i][0] : -1;
        int p2 = fans[i] >= 2 ? follow[i][1] : -1;
        for (int j = 0; j <= money; j++) {
            dp[i][j] = dp[preMain][j];                                  // 不要主件
            if (j >= cost[i])
                dp[i][j] = max(dp[i][j], dp[preMain][j - cost[i]] + val[i]);   // 只买主件
            if (p1 != -1 && j >= cost[i] + cost[p1])
                dp[i][j] = max(dp[i][j], dp[preMain][j - cost[i] - cost[p1]] + val[i] + val[p1]);
            if (p2 != -1 && j >= cost[i] + cost[p2])
                dp[i][j] = max(dp[i][j], dp[preMain][j - cost[i] - cost[p2]] + val[i] + val[p2]);
            if (p1 != -1 && p2 != -1 && j >= cost[i] + cost[p1] + cost[p2])
                dp[i][j] = max(dp[i][j], dp[preMain][j - cost[i] - cost[p1] - cost[p2]]
                                          + val[i] + val[p1] + val[p2]);
        }
        preMain = i;                                          // ★更新上一件主件
    }
    return dp[preMain][money];
}

// ================= 5. 非负数组前 k 小子序列和（排序 + 小根堆） =================
vector<long long> topKSmallestSubseqSum(vector<int> nums, int k) {
    sort(nums.begin(), nums.end());
    vector<long long> ans(k, 0);                              // ans[0] = 0（空集）
    if (k == 1) return ans;
    using State = array<long long,2>;                         // {最右下标, 当前和}
    priority_queue<State, vector<State>, greater<State>> pq;
    pq.push({0, nums[0]});
    for (int i = 1; i < k; i++) {
        State cur = pq.top(); pq.pop();
        ans[i] = cur[1];
        long long idx = cur[0], sum = cur[1];
        if (idx + 1 < (long long)nums.size()) {
            pq.push({idx + 1, sum - nums[idx] + nums[idx + 1]});   // 不换最右，接下一个
            pq.push({idx + 1, sum + nums[idx + 1]});               // 加上下一个
        }
    }
    return ans;
}

// ================= 6. 第 K 大子序列和 =================
long long kSum(vector<int> nums, int k) {
    long long maxSum = 0;
    for (int& x : nums) {
        if (x > 0) maxSum += x;
        else x = -x;                                          // 取绝对值
    }
    sort(nums.begin(), nums.end());
    if (k == 1) return maxSum;
    using State = array<long long,2>;
    priority_queue<State, vector<State>, greater<State>> pq;
    pq.push({0, nums[0]});
    long long aim = 0;                                        // 第 k 小的绝对值和
    for (int i = 1; i < k; i++) {
        State cur = pq.top(); pq.pop();
        aim = cur[1];
        long long idx = cur[0], sum = cur[1];
        if (idx + 1 < (long long)nums.size()) {
            pq.push({idx + 1, sum - nums[idx] + nums[idx + 1]});
            pq.push({idx + 1, sum + nums[idx + 1]});
        }
    }
    return maxSum - aim;
}

int main() {
    vector<int> cost = {1, 2, 3}, val = {1, 5, 8};
    printf("01 背包 = %d\n", knapsack01(4, cost, val));        // 9
    printf("目标和方案数 = %d\n", findTargetSumWays({1,1,1,1,1}, 3));  // 5
    printf("最后石头重量 = %d\n", lastStoneWeightII({2,7,4,1,8,1}));   // 1
    printf("第 3 小子序列和 = %lld\n", topKSmallestSubseqSum({1,2,3}, 3)[2]);  // 3
    printf("第 2 大子序列和 = %lld\n", kSum({1,2,3,4,5}, 2));  // 14
    return 0;
}
