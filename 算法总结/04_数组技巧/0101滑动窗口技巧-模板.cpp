// 滑动窗口模板：最短 / 最长 / 覆盖 / 种类数
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// ---------- 1. 和 >= target 的最短子数组（元素为正） ----------
int minSubArrayLen(int target, vector<int>& nums) {
    int ans = INT_MAX;
    for (int l = 0, r = 0, sum = 0; r < (int)nums.size(); r++) {
        sum += nums[r];
        while (sum - nums[l] >= target) sum -= nums[l++];   // 能扔就扔
        if (sum >= target) ans = min(ans, r - l + 1);
    }
    return ans == INT_MAX ? 0 : ans;
}

// ---------- 2. 无重复字符的最长子串 ----------
int lengthOfLongestSubstring(const string& s) {
    vector<int> last(256, -1);
    int ans = 0;
    for (int l = 0, r = 0; r < (int)s.size(); r++) {
        l = max(l, last[(unsigned char)s[r]] + 1);   // 左边界跳到不冲突处
        ans = max(ans, r - l + 1);
        last[(unsigned char)s[r]] = r;
    }
    return ans;
}

// ---------- 3. 最小覆盖子串（负债表） ----------
string minWindow(const string& s, const string& t) {
    unordered_map<char,int> debt;
    for (char c : t) debt[c]--;
    int total = -(int)t.size(), cnt = INT_MAX;
    string ans;
    for (int l = 0, r = 0; r < (int)s.size(); r++) {
        if (debt[s[r]]++ < 0) total++;          // 注意后置 ++
        if (total == 0) {
            while (debt[s[l]] > 0) debt[s[l++]]--;
            if (r - l + 1 < cnt) { cnt = r - l + 1; ans = s.substr(l, cnt); }
        }
    }
    return ans;
}

// ---------- 4. 至多 k 种不同整数（容斥的基础） ----------
int numsOfMostKinds(const vector<int>& a, int k) {
    unordered_map<int,int> mp;
    int ans = 0;
    for (int l = 0, r = 0, kinds = 0; r < (int)a.size(); r++) {
        if (++mp[a[r]] == 1) kinds++;
        while (kinds > k) if (--mp[a[l++]] == 0) kinds--;
        ans += r - l + 1;                       // 以 r 结尾的合法子数组个数
    }
    return ans;
}
int subarraysWithKDistinct(vector<int>& a, int k) {
    return numsOfMostKinds(a, k) - numsOfMostKinds(a, k - 1);
}

// ---------- 5. 至少有 K 个重复字符的最长子串（外层枚举种类数） ----------
int longestSubstring(const string& s, int k) {
    int ans = 0;
    for (int require = 1; require <= 26; require++) {
        int cnt[256] = {0};
        for (int l = 0, r = 0, kinds = 0, aim = 0; r < (int)s.size(); r++) {
            if (cnt[(unsigned char)s[r]] == 0) kinds++;
            if (cnt[(unsigned char)s[r]] == k - 1) aim++;
            cnt[(unsigned char)s[r]]++;
            while (kinds > require) {
                if (cnt[(unsigned char)s[l]] == 1) kinds--;
                if (cnt[(unsigned char)s[l]] == k) aim--;
                cnt[(unsigned char)s[l++]]--;
            }
            if (require == aim) ans = max(ans, r - l + 1);
        }
    }
    return ans;
}

int main() {
    vector<int> a = {2, 3, 1, 2, 4, 3};
    printf("最短和>=7 : %d\n", minSubArrayLen(7, a));
    printf("最长无重复子串 : %d\n", lengthOfLongestSubstring("abcabcbb"));
    printf("最小覆盖子串 : %s\n", minWindow("ADOBECODEBANC", "ABC").c_str());
    printf("至少 k 次重复最长子串 : %d\n", longestSubstring("aaabb", 3));
    return 0;
}
