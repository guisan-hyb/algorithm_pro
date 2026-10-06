// 随机选择算法模板：O(n) 求第 K 大 / 第 K 小
// 期望时间 O(n)，最坏 O(n^2)（概率极低）；额外空间 O(1)
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

int firstIdx, lastIdx;

// 荷兰国旗划分：< x | == x | > x，带回等于区 [firstIdx, lastIdx]
void partition(vector<int>& a, int l, int r, int x) {
    firstIdx = l; lastIdx = r;
    int i = l;
    while (i <= lastIdx) {
        if (a[i] < x)      swap(a[firstIdx++], a[i++]);
        else if (a[i] > x) swap(a[lastIdx--], a[i]);
        else               i++;
    }
}

// 返回升序后下标为 i 的元素值（0-based）
int randomizedSelect(vector<int>& a, int i) {
    int n = (int)a.size();
    for (int l = 0, r = n - 1; l <= r; ) {           // 注意：循环里不自增
        partition(a, l, r, a[l + rand() % (r - l + 1)]);
        if (firstIdx > i)     r = firstIdx - 1;      // 目标在左侧
        else if (lastIdx < i) l = lastIdx + 1;       // 目标在右侧
        else                  return a[i];           // 落在等于区，直接命中
    }
    return -1;
}

int findKthLargest(vector<int>& a, int k) {          // 第 k 大 = 升序下标 n-k
    return randomizedSelect(a, (int)a.size() - k);
}
int findKthSmallest(vector<int>& a, int k) {         // 第 k 小 = 升序下标 k-1
    return randomizedSelect(a, k - 1);
}

int main() {
    srand((unsigned)time(nullptr));
    int n, k;
    if (scanf("%d %d", &n, &k) != 2) return 0;
    vector<int> a(n), backup;
    for (int& x : a) scanf("%d", &x);
    backup = a;
    printf("第 %d 大 = %d\n", k, findKthLargest(a, k));
    a = backup;
    printf("第 %d 小 = %d\n", k, findKthSmallest(a, k));
    return 0;
}
