// 单调队列模板：滑动窗口最值 / 和至少为 K 的最短子数组 / 绝对差不超过限制的最长子数组
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <climits>
#include <deque>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

// ================= 1. 滑动窗口最大值（数组模拟双端队列） =================
vector<int> maxSlidingWindow(const vector<int>& nums, int k) {
    int n = (int)nums.size();
    vector<int> dq(n), ans;
    int head = 0, tail = 0;                       // [head, tail)
    for (int i = 0; i < n; i++) {
        while (head < tail && nums[dq[tail - 1]] <= nums[i]) tail--;
        dq[tail++] = i;
        if (i >= k - 1) {
            ans.push_back(nums[dq[head]]);        // 队头即窗口最大值
            if (dq[head] == i - k + 1) head++;    // 队头过期
        }
    }
    return ans;
}

// ================= 2. 和至少为 K 的最短子数组（前缀和递增队列） =================
int shortestSubarray(const vector<int>& nums, int k) {
    int n = (int)nums.size();
    vector<long long> pre(n + 1, 0);
    for (int i = 1; i <= n; i++) pre[i] = pre[i - 1] + nums[i - 1];
    vector<int> dq(n + 1);
    int head = 0, tail = 0, ans = INT_MAX;
    for (int i = 0; i <= n; i++) {
        while (head < tail && pre[i] - pre[dq[head]] >= k) {
            ans = min(ans, i - dq[head]);
            head++;                               // 队头以后只会得到更长的答案
        }
        while (head < tail && pre[i] <= pre[dq[tail - 1]]) tail--;   // 队尾又旧又不优
        dq[tail++] = i;
    }
    return ans == INT_MAX ? -1 : ans;
}

// ================= 3. 绝对差不超过 limit 的最长子数组（双队列） =================
int longestSubarray(vector<int>& nums, int limit) {
    int n = (int)nums.size();
    vector<int> maxDq(n), minDq(n);
    int mh = 0, mt = 0, nh = 0, nt = 0, ans = 0;
    for (int l = 0, r = 0; r < n; r++) {
        while (mt > mh && nums[maxDq[mt - 1]] <= nums[r]) mt--;      // 最大值队列
        maxDq[mt++] = r;
        while (nt > nh && nums[minDq[nt - 1]] >= nums[r]) nt--;      // 最小值队列
        minDq[nt++] = r;
        while (nums[maxDq[mh]] - nums[minDq[nh]] > limit) {          // 超限则收左边界
            if (maxDq[mh] == l) mh++;
            if (minDq[nh] == l) nh++;
            l++;
        }
        ans = max(ans, r - l + 1);
    }
    return ans;
}

// ================= 4. 满足不等式的最大值（y_i + y_j + |x_i - x_j|） =================
int findMaxValueOfEquation(vector<vector<int>>& points, int k) {
    deque<pair<int,int>> dq;                      // {x, y - x}，按 y-x 从大到小
    int ans = INT_MIN;
    for (auto& p : points) {
        int x = p[0], y = p[1];
        while (!dq.empty() && x - dq.front().first > k) dq.pop_front();   // 1) 出过期队头
        if (!dq.empty()) ans = max(ans, dq.front().second + x + y);       // 2) 计算答案
        while (!dq.empty() && y - x >= dq.back().second) dq.pop_back();   // 3) 维护单调性
        dq.push_back({x, y - x});
    }
    return ans;
}

int main() {
    vector<int> a = {1, 3, -1, -3, 5, 3, 6, 7};
    for (int x : maxSlidingWindow(a, 3)) std::cout << x << " ";   // 3 3 5 5 6 7
    std::cout << "\n";
    vector<int> b = {2, -1, 2};
    std::cout << "和至少为 3 的最短子数组 = " << shortestSubarray(b, 3) << "\n";   // 3
    vector<int> c = {8, 2, 4, 7};
    std::cout << "绝对差 <=4 的最长子数组 = " << longestSubarray(c, 4) << "\n";    // 2
    return 0;
}
