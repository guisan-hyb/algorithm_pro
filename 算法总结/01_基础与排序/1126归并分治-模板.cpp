// 归并分治模板：小和 + 翻转对
// 时间 O(n log n)，额外空间 O(n)
// 判别三步法：① 大范围 = 左 + 右 + 跨左右？ ② 左右有序后跨越部分能否滑窗？ ③ 都成立即可用
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

vector<int> a, help;

// -------- 小和：每个数左边比它小的数之和 --------
long long mergeSmall(int l, int m, int r) {
    long long ans = 0;
    for (int i = l, j = m + 1, sum = 0; j <= r; j++) {
        while (i <= m && a[i] <= a[j]) sum += a[i++];   // 左半有序，i 不回头
        ans += sum;
    }
    int i = l, p = l, q = m + 1;                        // 先统计，后排序
    while (p <= m && q <= r) help[i++] = a[p] <= a[q] ? a[p++] : a[q++];
    while (p <= m) help[i++] = a[p++];
    while (q <= r)   help[i++] = a[q++];
    for (int k = l; k <= r; k++) a[k] = help[k];
    return ans;
}
long long smallSum(int l, int r) {
    if (l == r) return 0;
    int m = (l + r) >> 1;
    return smallSum(l, m) + smallSum(m + 1, r) + mergeSmall(l, m, r);
}

// -------- 翻转对：i < j 且 a[i] > 2 * a[j] --------
long long mergeReverse(int l, int m, int r) {
    long long ans = 0;
    int j = m + 1;                                      // 必须在循环外初始化
    for (int i = l; i <= m; i++) {
        while (j <= r && (long long)a[i] > (long long)a[j] * 2) j++;
        ans += j - m - 1;                               // j 是第一个不满足的位置
    }
    int i = l, p = l, q = m + 1;
    while (p <= m && q <= r) help[i++] = a[p] <= a[q] ? a[p++] : a[q++];
    while (p <= m) help[i++] = a[p++];
    while (q <= r)   help[i++] = a[q++];
    for (int k = l; k <= r; k++) a[k] = help[k];
    return ans;
}
long long reversePairs(int l, int r) {
    if (l == r) return 0;
    int m = (l + r) >> 1;
    return reversePairs(l, m) + reversePairs(m + 1, r) + mergeReverse(l, m, r);
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    a.resize(n);
    help.resize(n);
    for (int& x : a) scanf("%d", &x);

    vector<int> origin = a;                 // 两个函数都会打乱数组，各跑一份
    printf("smallSum = %lld\n", smallSum(0, n - 1));
    a = origin;
    printf("reversePairs = %lld\n", reversePairs(0, n - 1));
    return 0;
}
