// 旋转数组二分模板：搜索目标值 + 寻找最小值
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// ---------- 1. 搜索旋转排序数组（元素互不相同） ----------
int search(vector<int>& nums, int target) {
    int n = (int)nums.size();
    int ans = -1;
    for (int l = 0, r = n - 1; l <= r;) {
        int m = l + (r - l) / 2;
        if (nums[m] == target) { ans = m; break; }
        if (nums[0] <= nums[m]) {                 // m 在大的递增区间
            if (target >= nums[0] && target < nums[m]) r = m - 1;
            else l = m + 1;
        } else {                                  // m 在小的递增区间
            if (target > nums[m] && target <= nums[n - 1]) l = m + 1;
            else r = m - 1;
        }
    }
    return ans;
}

// ---------- 2. 寻找旋转排序数组中的最小值 ----------
int findMin(vector<int>& nums) {
    int l = 0, r = (int)nums.size() - 1;
    while (l < r) {
        int m = l + (r - l) / 2;
        if (nums[m] < nums[r]) r = m;             // 右段有序，最小值在 m 或左侧
        else l = m + 1;                           // 有断崖，最小值在右侧
    }
    return nums[l];
}

// ---------- 3. 变体：允许重复元素（LeetCode 154 思路） ----------
int findMinWithDuplicates(vector<int>& nums) {
    int l = 0, r = (int)nums.size() - 1;
    while (l < r) {
        int m = l + (r - l) / 2;
        if (nums[m] < nums[r])      r = m;
        else if (nums[m] > nums[r]) l = m + 1;
        else                        r--;          // 相等无法判断，安全缩小区间
    }
    return nums[l];
}

int main() {
    vector<int> a = {4, 5, 6, 7, 0, 1, 2};
    printf("search(0) = %d, search(3) = %d\n", search(a, 0), search(a, 3));
    printf("findMin = %d\n", findMin(a));                       // 0
    vector<int> b = {2, 2, 2, 0, 1};
    printf("findMin(含重复) = %d\n", findMinWithDuplicates(b)); // 0
    return 0;
}
