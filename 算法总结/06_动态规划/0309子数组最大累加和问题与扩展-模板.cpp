// 子数组最大累加和模板：最大和/区间、打家劫舍、环形、乘积、子矩阵、余数状态、卷轴、三无重叠
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// ================= 1. 最大子数组和 + 区间 =================
int maxSubArray(const vector<int>& a) {
    int cur = a[0], ans = a[0];
    for (int i = 1; i < (int)a.size(); i++) {
        cur = max(a[i], cur + a[i]);
        ans = max(ans, cur);
    }
    return ans;
}
array<int,3> maxSubArrayRange(const vector<int>& a) {   // {left, right, sum}
    int n = (int)a.size();
    int left = 0, right = 0, sum = INT_MIN, pre = INT_MIN;
    for (int l = 0, r = 0; r < n; r++) {
        if (pre >= 0) pre += a[r];                       // 吸收左面有利可图
        else { pre = a[r]; l = r; }                      // 否则换开头
        if (pre > sum) { sum = pre; left = l; right = r; }
    }
    return {left, right, sum};
}

// ================= 2. 环形数组最大子数组和 =================
int maxSubarraySumCircular(const vector<int>& a) {
    int all = a[0], maxSum = a[0], minSum = a[0];
    int maxPre = a[0], minPre = a[0];
    for (int i = 1; i < (int)a.size(); i++) {
        maxPre = max(a[i], a[i] + maxPre);
        minPre = min(a[i], a[i] + minPre);
        maxSum = max(maxSum, maxPre);
        minSum = min(minSum, minPre);
        all += a[i];
    }
    return all == minSum ? maxSum : max(maxSum, all - minSum);   // 全负特判
}

// ================= 3. 打家劫舍（不能相邻） =================
int rob(const vector<int>& a) {
    int prepre = 0, pre = 0;
    for (int x : a) { int cur = max(pre, prepre + x); prepre = pre; pre = cur; }
    return pre;
}
int robCircular(const vector<int>& a) {                  // 环形版
    int n = (int)a.size();
    if (n == 1) return a[0];
    auto best = [&](int l, int r) {
        int prepre = 0, pre = 0;
        for (int i = l; i <= r; i++) { int cur = max(pre, prepre + a[i]); prepre = pre; pre = cur; }
        return pre;
    };
    return max(best(1, n - 1), a[0] + best(2, n - 2));
}

// ================= 4. 乘积最大子数组（同时维护 max/min） =================
int maxProduct(const vector<int>& a) {
    int premax = a[0], premin = a[0], ans = a[0];
    for (int i = 1; i < (int)a.size(); i++) {
        int curmax = max(a[i], max(premax * a[i], premin * a[i]));
        int curmin = min(a[i], min(premax * a[i], premin * a[i]));
        premax = curmax; premin = curmin;
        ans = max(ans, premax);
    }
    return ans;
}

// ================= 5. 子矩阵最大累加和（返回左上/右下坐标） =================
vector<int> getMaxMatrix(const vector<vector<int>>& m) {
    int rows = (int)m.size(), cols = (int)m[0].size();
    vector<int> nums(cols);
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0, sum = INT_MIN;
    for (int up = 0; up < rows; up++) {
        fill(nums.begin(), nums.end(), 0);
        for (int down = up; down < rows; down++) {
            int pre = INT_MIN, l = 0;
            for (int r = 0; r < cols; r++) {
                nums[r] += m[down][r];
                if (pre >= 0) pre += nums[r];
                else { pre = nums[r]; l = r; }
                if (pre > sum) { sum = pre; x1 = up; x2 = down; y1 = l; y2 = r; }
            }
        }
    }
    return {x1, y1, x2, y2};
}

// ================= 6. 子序列和被 7 整除的最大和（余数进状态） =================
int maxSumDivisibleBy7(const vector<int>& nums) {
    int n = (int)nums.size();
    vector<vector<int>> dp(n + 1, vector<int>(7, -1));   // -1 表示不存在
    dp[0][0] = 0;
    for (int i = 1; i <= n; i++) {
        int cur = nums[i - 1] % 7;
        for (int j = 0; j < 7; j++) {
            dp[i][j] = dp[i - 1][j];                     // 不要 nums[i-1]
            int need = (j - cur + 7) % 7;
            if (dp[i - 1][need] != -1)
                dp[i][j] = max(dp[i][j], dp[i - 1][need] + nums[i - 1]);
        }
    }
    return max(0, dp[n][0]);
}

// ================= 7. 魔法卷轴（2 个卷轴把连续一段变 0） =================
int magicScroll(const vector<int>& nums) {
    int n = (int)nums.size();
    int p1 = 0;
    for (int x : nums) p1 += x;                          // 0 个卷轴

    vector<int> prefix(n, 0);                            // 0~i 一定用 1 次
    int maxPreSum = max(0, nums[0]);
    for (int i = 1, curSum = nums[0]; i < n; i++) {
        prefix[i] = max(nums[i] + prefix[i - 1], maxPreSum);
        curSum += nums[i];
        maxPreSum = max(maxPreSum, curSum);
    }
    int p2 = prefix[n - 1];

    vector<int> suffix(n, 0);                            // i~n-1 一定用 1 次
    int maxSufSum = max(nums[n - 1], 0);
    for (int i = n - 2, curSum = nums[n - 1]; i >= 0; i--) {
        suffix[i] = max(nums[i] + suffix[i + 1], maxSufSum);
        curSum += nums[i];
        maxSufSum = max(maxSufSum, curSum);
    }
    int p3 = INT_MIN;
    for (int i = 1; i < n; i++) p3 = max(p3, prefix[i - 1] + suffix[i]);
    return max(max(p1, p2), p3);
}

// ================= 8. 三个无重叠子数组的最大和（k 固定） =================
vector<int> maxSumOfThreeSubarrays(const vector<int>& nums, int k) {
    int n = (int)nums.size();
    vector<int> sum(n, 0), prefix(n, 0), suffix(n, 0);
    for (int l = 0, r = 0, cur = 0; r < n; r++) {        // sum[i]：从 i 起长 k 的和
        cur += nums[r];
        if (r - l + 1 == k) { sum[l] = cur; cur -= nums[l]; l++; }
    }
    for (int i = k; i < n; i++)
        prefix[i] = sum[prefix[i - 1]] >= sum[i - k + 1] ? prefix[i - 1] : i - k + 1;
    suffix[n - k] = n - k;
    for (int i = n - k - 1; i >= 0; i--)
        suffix[i] = sum[i] >= sum[suffix[i + 1]] ? i : suffix[i + 1];

    int a = 0, b = 0, c = 0, best = 0;
    for (int i = k, j = 2 * k - 1; j <= n - k - 1; i++, j++) {
        int p = prefix[i - 1], s = suffix[j + 1];
        int cur = sum[p] + sum[i] + sum[s];
        if (cur > best) { best = cur; a = p; b = i; c = s; }
    }
    return {a, b, c};
}

int main() {
    vector<int> a = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    auto r = maxSubArrayRange(a);
    printf("最大子数组和 = %d (下标 %d~%d)\n", r[2], r[0], r[1]);   // 6 (3~6)
    printf("环形最大和 = %d\n", maxSubarraySumCircular({5,-3,5}));  // 10
    printf("打家劫舍 = %d\n", rob({2,7,9,3,1}));                    // 12
    printf("乘积最大 = %d\n", maxProduct({2,3,-2,4}));              // 6
    printf("被7整除最大和 = %d\n", maxSumDivisibleBy7({4,3,2,1}));  // 7
    return 0;
}
