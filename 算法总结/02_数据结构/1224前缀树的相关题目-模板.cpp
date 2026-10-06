// 前缀树进阶模板：二进制 Trie 求最大异或 + 哈希贪心解法
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// ================= 1. 二进制 Trie（求最大异或对） =================
static const int MAXN = 3000001;      // 约 31 * n
int trie[MAXN][2];
int nodeCnt = 1, highBit = 0;

void buildXorTrie(const vector<int>& nums) {
    int maxs = 0;
    for (int x : nums) maxs = max(maxs, x);
    highBit = 31;
    while (highBit > 0 && !((maxs >> highBit) & 1)) highBit--;

    for (int x : nums) {
        int cur = 1;
        for (int i = highBit; i >= 0; i--) {
            int p = (x >> i) & 1;
            if (!trie[cur][p]) trie[cur][p] = ++nodeCnt;
            cur = trie[cur][p];
        }
    }
}

// x 与树中某个数的最大异或值：贪心走相反位
int queryMaxXor(int x) {
    int ans = 0, cur = 1;
    for (int i = highBit; i >= 0; i--) {
        int st = (x >> i) & 1;
        int want = st ^ 1;                    // 希望走相反位，让这一位异或成 1
        if (!trie[cur][want]) want ^= 1;      // 没有相反位，只能走同一位
        ans |= (st ^ want) << i;
        cur = trie[cur][want];
    }
    return ans;
}

// ================= 2. 哈希表 + 贪心（更好写，空间更省） =================
int findMaximumXOR(vector<int>& nums) {
    int maxs = *max_element(nums.begin(), nums.end());
    int high = 31;
    while (high > 0 && !((maxs >> high) & 1)) high--;

    int ans = 0;
    unordered_set<int> st;
    for (int i = high; i >= 0; i--) {
        int want = ans | (1 << i);            // 希望第 i 位为 1
        st.clear();                           // 只用一个表，省空间
        for (int x : nums) {
            int highBits = (x >> i) << i;     // 只保留高 i 位
            st.insert(highBits);
            if (st.count(highBits ^ want)) {  // 存在两个前缀异或得到 want
                ans = want;
                break;
            }
        }
    }
    return ans;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    vector<int> nums(n);
    for (int& x : nums) scanf("%d", &x);

    printf("哈希贪心: %d\n", findMaximumXOR(nums));
    buildXorTrie(nums);
    int ans = 0;
    for (int x : nums) ans = max(ans, queryMaxXor(x));
    printf("二进制 Trie: %d\n", ans);
    return 0;
}
