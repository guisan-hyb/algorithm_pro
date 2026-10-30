// 前缀信息模板：区间和 / 最长子数组和 / 子数组计数 / 取模最短移除 / 奇偶状态
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

// ---------- 1. 前缀和：区间和 O(1) ----------
struct PrefixSum {
    vector<long long> s;
    explicit PrefixSum(const vector<int>& a) : s(a.size() + 1, 0) {
        for (size_t i = 0; i < a.size(); i++) s[i + 1] = s[i] + a[i];
    }
    long long range(int l, int r) const { return s[r + 1] - s[l]; }
};

// ---------- 2. 和等于 k 的最长子数组（元素可正可负） ----------
int longestSubarraySumK(const vector<int>& a, int k) {
    unordered_map<long long, int> firstPos;
    firstPos[0] = -1;                                   // ★空前缀
    long long sum = 0;
    int ans = 0;
    for (int i = 0; i < (int)a.size(); i++) {
        sum += a[i];
        auto it = firstPos.find(sum - k);
        if (it != firstPos.end()) ans = max(ans, i - it->second);
        if (!firstPos.count(sum)) firstPos[sum] = i;    // 只记录第一次出现
    }
    return ans;
}

// ---------- 3. 和等于 k 的子数组个数 ----------
int countSubarraySumK(const vector<int>& a, int k) {
    unordered_map<long long, int> cnt;
    cnt[0] = 1;                                         // ★空前缀
    long long sum = 0;
    int ans = 0;
    for (int x : a) {
        sum += x;
        ans += cnt[sum - k];
        cnt[sum]++;
    }
    return ans;
}

// ---------- 4. 移除最短子数组，使剩余元素和能被 p 整除 ----------
int minRemovableSubarray(const vector<int>& a, int p) {
    long long total = 0;
    for (int x : a) total = (total + x) % p;
    if (total == 0) return 0;
    unordered_map<long long, int> lastPos;
    lastPos[0] = -1;
    long long cur = 0;
    int ans = (int)a.size();
    for (int i = 0; i < (int)a.size(); i++) {
        cur = (cur + a[i]) % p;
        long long need = (cur - total + p) % p;         // 想删掉的那段的余数
        if (lastPos.count(need)) ans = min(ans, i - lastPos[need]);
        lastPos[cur] = i;                               // 覆盖为最新位置
    }
    return ans == (int)a.size() ? -1 : ans;
}

// ---------- 5. 每个元音出现偶数次的最长子串 ----------
int findTheLongestSubstring(const string& s) {
    auto idx = [](char c) -> int {
        switch (c) {
            case 'a': return 0; case 'e': return 1; case 'i': return 2;
            case 'o': return 3; case 'u': return 4; default: return -1;
        }
    };
    vector<int> first(32, -2);                          // -2 表示还没出现过
    first[0] = -1;                                      // ★空前缀：全偶状态
    int status = 0, ans = 0;
    for (int i = 0; i < (int)s.size(); i++) {
        int m = idx(s[i]);
        if (m != -1) status ^= (1 << m);
        if (first[status] != -2) ans = max(ans, i - first[status]);
        else first[status] = i;
    }
    return ans;
}

int main() {
    vector<int> a = {1, 2, 3, -2, 5};
    std::cout << "和等于 3 的最长子数组长度 = " << longestSubarraySumK(a, 3) << "\n";
    std::cout << "和等于 3 的子数组个数 = " << countSubarraySumK(a, 3) << "\n";
    std::cout << "元音偶数次最长子串长度 = " << findTheLongestSubstring("eleetminicoworoep") << "\n";
    return 0;
}
