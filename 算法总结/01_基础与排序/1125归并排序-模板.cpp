// 归并排序模板：递归版 + 迭代版
// 时间 O(n log n)，额外空间 O(n)，稳定排序
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

vector<int> a, help;

// -------- merge：谁小拷贝谁，<= 保证稳定 --------
void merge(int l, int mid, int r) {
    int i = l, p = l, q = mid + 1;
    while (p <= mid && q <= r)
        help[i++] = (a[p] <= a[q]) ? a[p++] : a[q++];
    while (p <= mid) help[i++] = a[p++];
    while (q <= r)   help[i++] = a[q++];
    for (int k = l; k <= r; k++) a[k] = help[k];
}

// -------- 递归版 --------
void mergeSort(int l, int r) {
    if (l >= r) return;
    int mid = l + ((r - l) >> 1);
    mergeSort(l, mid);
    mergeSort(mid + 1, r);
    merge(l, mid, r);
}

// -------- 迭代版：步长 1,2,4,... 自底向上 --------
void mergeSortIterative(int n) {
    for (int step = 1; step < n; step <<= 1) {
        for (int l = 0; l < n; l += 2 * step) {
            int m = l + step - 1;
            if (m + 1 > n - 1) break;              // 只剩左段，无需 merge
            int r = min(l + 2 * step - 1, n - 1);
            merge(l, m, r);
        }
    }
}

int main() {
    int n;
    if (scanf("%d", &n) != 1 || n <= 0) return 0;
    a.resize(n);
    help.resize(n);
    for (int& x : a) scanf("%d", &x);
    mergeSort(0, n - 1);
    // mergeSortIterative(n);   // 二选一
    for (int i = 0; i < n; i++) printf("%d%c", a[i], " \n"[i == n - 1]);
    return 0;
}
