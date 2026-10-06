// 贪心专题5模板：跳跃游戏 / 水龙头 / 字符串转化 / 过河 / 超级洗衣机
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// ---------- 1. 跳跃游戏 II（区间覆盖） ----------
int jump(vector<int>& nums) {
    int ans = 0, curEnd = 0, nextEnd = 0;
    for (int i = 0; i < (int)nums.size(); i++) {
        if (curEnd < i) { ans++; curEnd = nextEnd; }
        nextEnd = max(nextEnd, i + nums[i]);
    }
    return ans;
}

// ---------- 2. 灌溉花园的最少水龙头数目 ----------
int minTaps(int n, vector<int>& ranges) {
    vector<int> right(n + 1, 0);
    for (int i = 0; i <= n; i++) {
        int l = max(0, i - ranges[i]);
        int r = min(n, i + ranges[i]);
        right[l] = max(right[l], r);
    }
    int ans = 0, curEnd = 0, nextEnd = 0;
    for (int i = 0; i < n; i++) {
        nextEnd = max(nextEnd, right[i]);
        if (curEnd == i) {
            if (i < nextEnd) { ans++; curEnd = nextEnd; }
            else return -1;
        }
    }
    return ans;
}

// ---------- 3. 字符串转化 ----------
bool canConvert(const string& s1, const string& s2) {
    if (s1 == s2) return true;
    vector<int> cnt(26, 0);
    int kind = 0;
    for (char c : s2) if (cnt[c - 'a']++ == 0) kind++;
    if (kind == 26) return false;
    vector<int> pos(26, -1);
    for (int i = 0; i < (int)s1.size(); i++) {
        int c = s1[i] - 'a';
        if (pos[c] != -1 && s2[pos[c]] != s2[i]) return false;
        pos[c] = i;
    }
    return true;
}

// ---------- 4. 过河问题 ----------
long long crossRiver(vector<int> t) {
    sort(t.begin(), t.end());
    int n = (int)t.size();
    if (n == 1) return t[0];
    if (n == 2) return t[1];
    vector<long long> dp(n, 0);
    dp[0] = t[0];
    dp[1] = t[1];
    dp[2] = t[0] + t[1] + t[2];
    for (int i = 3; i < n; i++)
        dp[i] = min(t[i] + t[0] + dp[i - 1],
                    t[1] + t[0] + t[i] + t[1] + dp[i - 2]);
    return dp[n - 1];
}

// ---------- 5. 超级洗衣机 ----------
int findMinMoves(vector<int>& m) {
    int n = (int)m.size(), sum = 0;
    for (int x : m) sum += x;
    if (sum % n) return -1;
    int avg = sum / n, ans = 0, lSum = 0, rSum = sum;
    for (int i = 0; i < n; i++) {
        rSum -= m[i];
        int lneed = i * avg - lSum;
        int rneed = (n - i - 1) * avg - rSum;
        if (lneed > 0 && rneed > 0) ans = max(ans, lneed + rneed);
        else ans = max(ans, max(abs(lneed), abs(rneed)));
        lSum += m[i];
    }
    return ans;
}

int main() {
    vector<int> a = {2,3,1,1,4};
    printf("最少跳跃 = %d\n", jump(a));                              // 2
    vector<int> r = {3,4,1,1,0,0};
    printf("水龙头 = %d\n", minTaps(5, r));                          // 1
    printf("字符串转化 = %d\n", (int)canConvert("aabcc", "ccdee"));    // 1
    printf("过河时间 = %lld\n", crossRiver({1,2,5,10}));              // 17
    vector<int> m = {1,0,5};
    printf("超级洗衣机 = %d\n", findMinMoves(m));                     // 3
    return 0;
}
