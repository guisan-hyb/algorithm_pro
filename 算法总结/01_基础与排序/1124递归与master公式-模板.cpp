// 递归与 master 公式：模板 + 自测
// 记住结论，别每次现场画递归树
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

// T(n) = 2T(n/2) + O(1) -> O(n)：区间求最大值
int maxValue(const vector<int>& a, int l, int r) {
    if (l == r) return a[l];
    int mid = l + ((r - l) >> 1);
    return max(maxValue(a, l, mid), maxValue(a, mid + 1, r));
}

// T(n) = 2T(n/2) + O(n) -> O(n log n)：归并排序的复杂度来源（见 1125 归并排序）
// T(n) = T(n/2) + O(1)  -> O(log n)：二分（见 1120 二分搜索）

// 递归改非递归的两个方向（此处只示范区间求最大值的显式栈写法）
int maxValueIterative(const vector<int>& a) {
    if (a.empty()) return INT_MIN;
    // 自底向上合并：把相邻区间两两合并，直到只剩一个区间
    vector<int> cur = a;
    while (cur.size() > 1) {
        vector<int> nxt;
        for (size_t i = 0; i < cur.size(); i += 2) {
            if (i + 1 < cur.size()) nxt.push_back(max(cur[i], cur[i + 1]));
            else nxt.push_back(cur[i]);
        }
        cur.swap(nxt);
    }
    return cur[0];
}

int main() {
    int n;
    if (!(std::cin >> n) || n <= 0) return 0;
    vector<int> a(n);
    for (int& x : a) (std::cin >> x);
    std::cout << maxValue(a, 0, n - 1) << " " << maxValueIterative(a) << "\n";
    return 0;
}
