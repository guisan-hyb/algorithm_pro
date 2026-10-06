// 三傻排序模板：选择 / 冒泡 / 插入
// 用途：小数据量排序、对数器暴力解、手写热身
// 复杂度：时间 O(n^2)，额外空间 O(1)
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// ---------- 选择排序：不稳定 ----------
void selectionSort(int* a, int n) {
    if (n < 2) return;
    for (int i = 0; i < n - 1; i++) {
        int idx = i;                          // 先记下标，循环结束再交换
        for (int j = i + 1; j < n; j++)
            if (a[j] < a[idx]) idx = j;
        swap(a[i], a[idx]);
    }
}

// ---------- 冒泡排序：稳定 ----------
void bubbleSort(int* a, int n) {
    if (n < 2) return;
    for (int i = 0; i < n - 1; i++) {
        bool sorted = true;
        for (int j = 0; j < n - i - 1; j++) {
            if (a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]);
                sorted = false;
            }
        }
        if (sorted) break;
    }
}

// ---------- 插入排序：稳定，近乎有序时接近 O(n) ----------
void insertionSort(int* a, int n) {
    if (n < 2) return;
    for (int i = 1; i < n; i++)
        for (int j = i - 1; j >= 0 && a[j] > a[j + 1]; j--)
            swap(a[j], a[j + 1]);
}

// ---------- 自测 ----------
int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    vector<int> a(n), b(n), c(n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
        b[i] = c[i] = a[i];
    }
    selectionSort(a.data(), n);
    bubbleSort(b.data(), n);
    insertionSort(c.data(), n);
    for (int i = 0; i < n; i++)
        printf("%d%c", a[i], " \n"[i == n - 1]);
    return 0;
}
