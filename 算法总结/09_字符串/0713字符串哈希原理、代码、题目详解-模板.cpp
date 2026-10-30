// 字符串哈希模板：子串哈希 O(1) / 替代 KMP / 统计不同字符串
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
using namespace std;

const unsigned long long BASE = 499;                 // 建议用质数（433/499/599/1e9+7）

struct StringHash {
    vector<unsigned long long> pw, hs;
    explicit StringHash(const string& s) {
        int n = (int)s.size();
        pw.assign(max(n, 1), 1);
        hs.assign(max(n, 1), 0);
        for (int i = 1; i < n; i++) pw[i] = pw[i - 1] * BASE;
        if (n > 0) {
            hs[0] = s[0] - 'a' + 1;                  // ★字符值从 1 开始
            for (int i = 1; i < n; i++)
                hs[i] = hs[i - 1] * BASE + (s[i] - 'a' + 1);
        }
    }
    // 子串 s[l..r] 的哈希（自然溢出）
    unsigned long long get(int l, int r) const {
        unsigned long long ans = hs[r];
        if (l > 0) ans -= hs[l - 1] * pw[r - l + 1];
        return ans;
    }
    // 整串哈希
    static unsigned long long hashOf(const string& s) {
        unsigned long long h = 0;
        for (char c : s) h = h * BASE + (c - 'a' + 1);
        return h;
    }
};

// ---------- 用字符串哈希替代 KMP ----------
int strStr(const string& haystack, const string& needle) {
    int n = (int)haystack.size(), m = (int)needle.size();
    if (m == 0) return 0;
    if (n < m) return -1;
    StringHash h(haystack);
    unsigned long long need = StringHash::hashOf(needle);
    for (int l = 0, r = m - 1; r < n; l++, r++)
        if (h.get(l, r) == need) return l;
    return -1;
}

// ---------- 统计不同字符串（洛谷 P3370 型） ----------
int distinctStrings(vector<string> strs) {
    vector<unsigned long long> v;
    v.reserve(strs.size());
    for (auto& s : strs) v.push_back(StringHash::hashOf(s));
    sort(v.begin(), v.end());
    return (int)(unique(v.begin(), v.end()) - v.begin());
}

// ---------- 独特子串数量（每个数字出现频率相同） ----------
int equalDigitFrequency(const string& str) {
    int n = (int)str.size(), ans = 0;
    unordered_set<unsigned long long> st;
    for (int i = 0; i < n; i++) {
        unsigned long long hashCode = 0;
        int maxCnt = 0, maxCntKind = 0, allKind = 0;
        std::array<int, 10> cnt= {0};
        for (int j = i; j < n; j++) {
            int cur = str[j] - '0';
            if (++cnt[cur] == 1) allKind++;
            hashCode = hashCode * BASE + (cur + 1);
            if (cnt[cur] > maxCnt) { maxCnt = cnt[cur]; maxCntKind = 1; }
            else if (cnt[cur] == maxCnt) maxCntKind++;
            if (maxCntKind == allKind) st.insert(hashCode);
        }
    }
    return (int)st.size();
}

// ---------- 重复叠加字符串匹配（LeetCode 686） ----------
int repeatedStringMatch(const string& a, const string& b) {
    int n = (int)a.size(), m = (int)b.size();
    int k = (m + n - 1) / n;
    string t = a;
    for (int i = 0; i < k + 1; i++) t += a;
    StringHash h(t);
    unsigned long long need = StringHash::hashOf(b);
    for (int l = 0, r = m - 1; r < (int)t.size(); l++, r++)
        if (h.get(l, r) == need) return r < k * n ? k : k + 1;
    return -1;
}

int main() {
    std::cout << "strStr(abcabcabc, abc) = " << strStr("abcabcabc", "abc") << "\n";   // 0
    std::cout << "不同字符串个数 = " << distinctStrings({"ab","ab","ba"}) << "\n";     // 2
    std::cout << "独特子串数量 = " << equalDigitFrequency("1212") << "\n";             // 5
    std::cout << "重复叠加 = " << repeatedStringMatch("abcd", "cdabcdab") << "\n";     // 3
    StringHash h("cabef");
    std::cout << "子串 be 的哈希 = " << h.get(2, 3) << "\n";
    return 0;
}
