// 二分搜索模板：找最左/最右边界 + 无序数组找峰值
// 时间 O(log n)，额外空间 O(1)
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
using namespace std;

// ---------- 1. 有序数组中 >= value 的最左位置 ----------
int findLeft(const vector<int>& a, int value) {
    int lo = 0, hi = (int)a.size() - 1, pos = -1;
    while (lo <= hi) {
        int mid = lo + ((hi - lo) >> 1);
        if (a[mid] >= value) { pos = mid; hi = mid - 1; }   // 记录后继续往左找
        else lo = mid + 1;
    }
    return pos;                                              // 不存在返回 -1
}

// ---------- 2. 有序数组中 <= value 的最右位置 ----------
int findRight(const vector<int>& a, int value) {
    int lo = 0, hi = (int)a.size() - 1, pos = -1;
    while (lo <= hi) {
        int mid = lo + ((hi - lo) >> 1);
        if (a[mid] <= value) { pos = mid; lo = mid + 1; }   // 记录后继续往右找
        else hi = mid - 1;
    }
    return pos;
}

// ---------- 3. 无序数组找任一峰值下标（相邻元素互不相等） ----------
int findPeak(const vector<int>& a) {
    int n = (int)a.size();
    if (n == 1) return 0;
    if (a[0] > a[1]) return 0;                 // 先单独判两端
    if (a[n - 1] > a[n - 2]) return n - 1;
    int l = 1, r = n - 2;                      // 中间区间必定存在峰值
    while (l <= r) {
        int mid = l + ((r - l) >> 1);
        if (a[mid - 1] > a[mid]) r = mid - 1;      // 上坡向左，峰在左
        else if (a[mid] < a[mid + 1]) l = mid + 1; // 上坡向右，峰在右
        else return mid;
    }
    return -1;
}

// ---------- 自测：读入 n 个升序数 + 查询值 ----------
int main() {
    int n, q;
    if (!(std::cin >> n >> q)) return 0;
    vector<int> a(n);
    for (int& x : a) (std::cin >> x);
    while (q--) {
        int x;
        (std::cin >> x);
        std::cout << ">=(" << x << ") left=" << findLeft(a, x) << "  <=(" << x << ") right=" << findRight(a, x) << "\n";
    }
    return 0;
}
