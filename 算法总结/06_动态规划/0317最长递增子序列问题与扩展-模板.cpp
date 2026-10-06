// LIS 模板：O(n log n) 二分 + 套娃信封 + K 递增 + 数对链 + 一次修改机会
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// ================= 1. LIS：ends + 二分（O(n log n)） =================
// strict = true：严格递增；false：最长不下降
int LIS(const vector<int>& a, bool strict = true) {
    int n = (int)a.size();
    vector<int> ends(n);
    int len = 0;
    for (int x : a) {
        int l = 0, r = len - 1, pos = -1;
        while (l <= r) {                       // 找第一个满足条件的位置
            int m = l + (r - l) / 2;
            bool ok = strict ? (ends[m] >= x) : (ends[m] > x);
            if (ok) { pos = m; r = m - 1; }
            else l = m + 1;
        }
        if (pos == -1) ends[len++] = x;        // 追加（所有值都 < x）
        else ends[pos] = x;                    // 替换（让结尾更小）
    }
    return len;
}

// ================= 2. 俄罗斯套娃信封 =================
int maxEnvelopes(vector<vector<int>> env) {
    sort(env.begin(), env.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[0] != b[0] ? a[0] < b[0] : a[1] > b[1];   // 宽升序、高降序
    });
    vector<int> heights;
    for (auto& e : env) heights.push_back(e[1]);
    return LIS(heights, true);
}

// ================= 3. 使数组 K 递增的最少操作次数 =================
int kIncreasing(const vector<int>& arr, int k) {
    int n = (int)arr.size(), ans = 0;
    for (int i = 0; i < k; i++) {
        vector<int> group;
        for (int j = i; j < n; j += k) group.push_back(arr[j]);
        ans += (int)group.size() - LIS(group, false);      // 每组要不下降
    }
    return ans;
}

// ================= 4. 最长数对链 =================
// 法一：LIS 思路（按 left 升序，ends 存 right，替换时取 min）
int findLongestChainLIS(vector<vector<int>> pairs) {
    sort(pairs.begin(), pairs.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[0] < b[0]; });
    vector<int> ends(pairs.size());
    int len = 0;
    for (auto& p : pairs) {
        int l = 0, r = len - 1, pos = -1;
        while (l <= r) {
            int m = l + (r - l) / 2;
            if (ends[m] >= p[0]) { pos = m; r = m - 1; }
            else l = m + 1;
        }
        if (pos == -1) ends[len++] = p[1];
        else ends[pos] = min(ends[pos], p[1]);
    }
    return len;
}
// 法二：贪心（按 right 升序，更优）
int findLongestChainGreedy(vector<vector<int>> pairs) {
    sort(pairs.begin(), pairs.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[1] < b[1]; });
    int ans = 0, pre = INT_MIN;
    for (auto& p : pairs)
        if (p[0] > pre) { ans++; pre = p[1]; }
    return ans;
}

// ================= 5. 有一次修改机会的最长不下降子序列 =================
// 思路：把要修改的连续 k 个数放在中间，左右各接一段最长不下降子序列
int longestAfterChange(const vector<int>& nums, int k) {
    int n = (int)nums.size();
    vector<int> rightLen(n, 0);                 // 以 i 开头的最长不下降长度
    vector<int> ends(n);
    int len = 0;
    for (int i = n - 1; i >= 0; i--) {          // 等价于从右往左的最长不上升
        int l = 0, r = len - 1, pos = -1;
        while (l <= r) {
            int m = l + (r - l) / 2;
            if (ends[m] < nums[i]) { pos = m; r = m - 1; }
            else l = m + 1;
        }
        if (pos == -1) { ends[len++] = nums[i]; rightLen[i] = len; }
        else { ends[pos] = nums[i]; rightLen[i] = pos + 1; }
    }

    int ans = 0;
    len = 0;
    for (int i = 0, j = k; j < n; i++, j++) {
        int l = 0, r = len - 1, pos = -1;       // 找左侧第一个 > nums[j] 的位置
        while (l <= r) {
            int m = l + (r - l) / 2;
            if (ends[m] > nums[j]) { pos = m; r = m - 1; }
            else l = m + 1;
        }
        int lans = (pos == -1) ? len : pos;
        ans = max(ans, lans + k + rightLen[j]);

        l = 0; r = len - 1; pos = -1;           // 把 nums[i] 并入左侧
        while (l <= r) {
            int m = l + (r - l) / 2;
            if (ends[m] > nums[i]) { pos = m; r = m - 1; }
            else l = m + 1;
        }
        if (pos == -1) ends[len++] = nums[i];
        else ends[pos] = nums[i];
    }
    ans = max(ans, len + k);                    // 修改区间落到数组末尾
    return ans;
}

int main() {
    vector<int> a = {10, 9, 2, 5, 3, 7, 101, 18};
    printf("LIS(严格) = %d\n", LIS(a, true));                    // 4
    printf("LIS(不下降) = %d\n", LIS({2,2,2}, false));           // 3
    vector<vector<int>> env = {{5,4},{6,4},{6,7},{2,3}};
    printf("套娃信封 = %d\n", maxEnvelopes(env));                // 3
    printf("K 递增最少修改 = %d\n", kIncreasing({5,4,3,2,1}, 1));// 4
    vector<vector<int>> pairs = {{1,2},{2,3},{3,4}};
    printf("最长数对链 = %d\n", findLongestChainGreedy(pairs));  // 2
    printf("一次修改后最长不下降 = %d\n", longestAfterChange({1,2,3,4,5}, 1)); // 5
    return 0;
}
