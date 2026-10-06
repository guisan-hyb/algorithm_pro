// 两有序数组中位数模板：二分分割线 O(log min(m,n))
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// ---------- 写法一：在短数组上二分"切分点下标" ----------
double findMedianSortedArrays(vector<int> nums1, vector<int> nums2) {
    if (nums1.size() > nums2.size())
        return findMedianSortedArrays(nums2, nums1);      // 保证 nums1 更短

    int m = (int)nums1.size(), n = (int)nums2.size();
    int half = (m + n + 1) / 2;                           // 左半部分元素个数（奇数时左多一个）
    int left = 0, right = m;                              // 在 nums1 上二分
    while (left < right) {
        int i = left + (right - left) / 2;
        int j = half - i;
        if (nums2[j - 1] > nums1[i]) left = i + 1;        // i 太小，往右
        else                         right = i;           // 可能已经是答案，保留
    }
    int i = left, j = half - i;

    int l1 = (i == 0) ? INT_MIN : nums1[i - 1];
    int l2 = (j == 0) ? INT_MIN : nums2[j - 1];
    int r1 = (i == m) ? INT_MAX : nums1[i];
    int r2 = (j == n) ? INT_MAX : nums2[j];

    int leftMax  = max(l1, l2);
    int rightMin = min(r1, r2);
    if ((m + n) % 2 == 1) return (double)leftMax;
    return (leftMax + rightMin) / 2.0;
}

// ---------- 写法二（笔记作者版）：i, j 表示"切过去的个数" ----------
double findMedianSortedArrays2(vector<int>& nums1, vector<int>& nums2) {
    if (nums1.size() > nums2.size()) return findMedianSortedArrays2(nums2, nums1);
    int m = (int)nums1.size(), n = (int)nums2.size();
    int half = (m + n + 1) / 2;
    int i = -1;
    for (int l = 0, r = m - 1; l <= r;) {
        int mid = l + (r - l) / 2;
        if (nums1[mid] <= nums2[half - mid - 1]) { i = mid; l = mid + 1; }
        else r = mid - 1;
    }
    i += 1;
    int j = half - i;
    int l1 = (i == 0) ? INT_MIN : nums1[i - 1];
    int l2 = (j == 0) ? INT_MIN : nums2[j - 1];
    int r1 = (i == m) ? INT_MAX : nums1[i];
    int r2 = (j == n) ? INT_MAX : nums2[j];
    int leftMax = max(l1, l2), rightMin = min(r1, r2);
    return (m + n) % 2 ? (double)leftMax : (leftMax + rightMin) / 2.0;
}

int main() {
    vector<int> a = {1, 3}, b = {2};
    printf("%.1f\n", findMedianSortedArrays(a, b));    // 2.0
    vector<int> c = {1, 2}, d = {3, 4};
    printf("%.1f\n", findMedianSortedArrays2(c, d));   // 2.5
    return 0;
}
