// 根据数据量猜解法：示例（n<=10 用全排列枚举）+ 复杂度速查
// 核心：n * 复杂度 <= 1e8 才算安全
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

// ---------- 示例：打怪（n 个技能各用一次，最少几次打完） ----------
// 数据范围 1 <= n <= 10  ->  直接全排列枚举 O(n!)
int n, m;
vector<int> hurt(12);
vector<int> blood(12);

int dfs(vector<int>& a, int idx, int rest, int used) {
    if (rest <= 0) return used;                  // 已打死
    if (idx == n) return INT_MAX;                // 技能用完还没死
    int ans = INT_MAX;
    for (int j = idx; j < n; j++) {
        swap(a[idx], a[j]);
        int dmg = (rest <= blood[a[idx]]) ? 2 * hurt[a[idx]] : hurt[a[idx]];
        ans = min(ans, dfs(a, idx + 1, rest - dmg, used + 1));
        swap(a[idx], a[j]);
    }
    return ans;
}

int main() {
    int T;
    if (!(std::cin >> T)) return 0;
    while (T--) {
        std::cin >> n >> m;
        for (int i = 0; i < n; i++) (std::cin >> hurt[i] >> blood[i]);
        vector<int> order(n);
        iota(order.begin(), order.end(), 0);
        int ans = dfs(order, 0, m, 0);
        std::cout << (ans == INT_MAX ? -1 : ans) << "\n";
    }
    return 0;
}

// ---------- 附：复杂度速查（写代码前先过一遍） ----------
// n<=20        -> O(n!) / O(2^n)    暴力 / 状压 / 全排列
// n<=100       -> O(n^3)            三重循环 / Floyd / 区间 DP
// n<=1000      -> O(n^2)            双重循环 / 简单 DP
// n<=1e4       -> O(n*sqrt(n))      莫队 / 分块
// n<=1e5~1e6   -> O(n log n)        排序 / 二分 / 树状数组 / 堆
// n<=1e7       -> O(n)              双指针 / 前缀和 / 差分
// n>=1e9       -> O(log n) / O(1)   二分答案 / 数学 / 快速幂
