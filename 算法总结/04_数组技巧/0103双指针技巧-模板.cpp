// 双指针模板：接雨水 / 盛水容器 / 救生艇 / 寻找重复数 / 缺失第一个正数
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// ---------- 1. 接雨水：双指针 O(1) 空间 ----------
int trap(vector<int>& h) {
    int n = (int)h.size();
    if (n == 0) return 0;
    int lmax = h[0], rmax = h[n - 1], sum = 0;
    for (int l = 1, r = n - 2; l <= r; ) {
        if (lmax <= rmax) {
            sum += max(0, lmax - h[l]);
            lmax = max(lmax, h[l++]);
        } else {
            sum += max(0, rmax - h[r]);
            rmax = max(rmax, h[r--]);
        }
    }
    return sum;
}

// ---------- 2. 盛最多水的容器 ----------
int maxArea(vector<int>& h) {
    int ans = 0;
    for (int l = 0, r = (int)h.size() - 1; l < r; ) {
        ans = max(ans, min(h[l], h[r]) * (r - l));
        if (h[l] <= h[r]) l++; else r--;
    }
    return ans;
}

// ---------- 3. 救生艇（最多两人一船，贪心配对） ----------
int numRescueBoats(vector<int>& people, int limit) {
    sort(people.begin(), people.end());
    int boats = 0;
    for (int l = 0, r = (int)people.size() - 1; l <= r; ) {
        if (people[l] + people[r] > limit) { r--; boats++; }
        else { l++; r--; boats++; }
    }
    return boats;
}

// ---------- 4. 寻找重复数（数组当链表 + Floyd 判圈） ----------
int findDuplicate(vector<int>& nums) {
    int slow = nums[0], fast = nums[nums[0]];
    while (slow != fast) { slow = nums[slow]; fast = nums[nums[fast]]; }
    for (fast = 0; fast != slow; ) { fast = nums[fast]; slow = nums[slow]; }
    return slow;
}

// ---------- 5. 缺失的第一个正数（O(n) 时间 O(1) 空间） ----------
int firstMissingPositive(vector<int>& a) {
    int l = 0, r = (int)a.size();
    while (l < r) {
        if (a[l] == l + 1) {
            l++;
        } else if (a[l] <= l || a[l] > r || a[a[l] - 1] == a[l]) {
            swap(a[l], a[--r]);                  // 垃圾：扔进右侧垃圾区
        } else {
            swap(a[l], a[a[l] - 1]);             // 放到它该在的位置
        }
    }
    return l + 1;
}

// ---------- 6. 供暖器（同向双指针 + 排序） ----------
int findRadius(vector<int>& houses, vector<int>& heaters) {
    sort(houses.begin(), houses.end());
    sort(heaters.begin(), heaters.end());
    int ans = 0;
    for (int hi = 0, he = 0; hi < (int)houses.size(); hi++) {
        while (he < (int)heaters.size() - 1 &&
               abs(houses[hi] - heaters[he]) >= abs(houses[hi] - heaters[he + 1]))
            he++;                                // 走到离该房子最近的供暖器
        ans = max(ans, abs(houses[hi] - heaters[he]));
    }
    return ans;
}

int main() {
    vector<int> h = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    printf("接雨水 = %d\n", trap(h));
    vector<int> p = {3, 2, 2, 1};
    printf("救生艇 = %d\n", numRescueBoats(p, 3));
    vector<int> d = {1, 3, 4, 2, 2};
    printf("重复数 = %d\n", findDuplicate(d));
    return 0;
}
