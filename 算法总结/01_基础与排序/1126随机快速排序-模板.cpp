// 随机快速排序模板：荷兰国旗三分区版（推荐）
// 期望时间 O(n log n)，额外空间 O(log n)（递归栈），不稳定
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

int firstIdx, lastIdx;          // 等于 x 的区间 [firstIdx, lastIdx]

// -------- 荷兰国旗划分：< x | == x | > x --------
void partition(vector<int>& a, int l, int r, int x) {
    firstIdx = l;
    lastIdx = r;
    int i = l;
    while (i <= lastIdx) {
        if (a[i] < x) {
            swap(a[firstIdx++], a[i++]);   // 换过来的是已看过的，i++
        } else if (a[i] > x) {
            swap(a[lastIdx--], a[i]);      // 换过来的是没看过的，i 不动
        } else {
            i++;                           // 等于区，直接跳过
        }
    }
}

void quickSort(vector<int>& a, int l, int r) {
    if (l >= r) return;
    int x = a[l + rand() % (r - l + 1)];   // 随机选取划分值
    partition(a, l, r, x);
    int left = firstIdx, right = lastIdx;  // 先存住，递归会覆盖全局变量
    quickSort(a, l, left - 1);
    quickSort(a, right + 1, r);
}

int main() {
    srand((unsigned)time(nullptr));
    int n;
    if (scanf("%d", &n) != 1 || n <= 0) return 0;
    vector<int> a(n);
    for (int& x : a) scanf("%d", &x);
    quickSort(a, 0, n - 1);
    for (int i = 0; i < n; i++) printf("%d%c", a[i], " \n"[i == n - 1]);
    return 0;
}
