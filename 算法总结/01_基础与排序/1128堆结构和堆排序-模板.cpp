// 堆结构 + 堆排序模板（大根堆）
// heapInsert / heapify 均 O(log n)；从底到顶建堆 O(n)；堆排序 O(n log n)，额外空间 O(1)
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// -------- 向上调整：i 位置新插入的值 --------
void heapInsert(vector<int>& h, int i) {
    while (i > 0 && h[i] > h[(i - 1) / 2]) {
        swap(h[i], h[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// -------- 向下调整：i 位置的值变小了，堆大小为 size --------
void heapify(vector<int>& h, int i, int size) {
    int l = 2 * i + 1;
    while (l < size) {                                   // 有左孩子
        int r = l + 1;
        int best = (r < size && h[r] > h[l]) ? r : l;    // 较大的孩子
        if (h[best] <= h[i]) break;
        swap(h[i], h[best]);
        i = best;
        l = 2 * i + 1;
    }
}

// -------- 堆排序 --------
void heapSort(vector<int>& h) {
    int n = (int)h.size();
    for (int i = n / 2 - 1; i >= 0; i--) heapify(h, i, n);   // O(n) 建堆
    int size = n;
    while (size > 1) {
        swap(h[0], h[--size]);
        heapify(h, 0, size);
    }
}

int main() {
    int n;
    if (scanf("%d", &n) != 1 || n <= 0) return 0;
    vector<int> h(n);
    for (int& x : h) scanf("%d", &x);
    heapSort(h);
    for (int i = 0; i < n; i++) printf("%d%c", h[i], " \n"[i == n - 1]);
    return 0;
}
