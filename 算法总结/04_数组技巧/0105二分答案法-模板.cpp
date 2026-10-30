// 二分答案模板：珂珂吃香蕉 / 分割数组 / 第 K 小距离 / 等位时间 / 旅行时间
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <climits>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

// ================== 通用骨架 ==================
// 找"最小的满足 check 的值"：
//   while (lo <= hi) { mid = lo + (hi - lo) / 2;
//       if (check(mid)) { ans = mid; hi = mid - 1; } else lo = mid + 1; }
// 找"最大的满足 check 的值"：把上面两行互换（ans = mid; lo = mid + 1）

// ================== 1. 爱吃香蕉的珂珂 ==================
bool canEat(const vector<int>& piles, int h, int k) {
    long long time = 0;
    for (int p : piles) time += (p + k - 1) / k;      // 向上取整
    return time <= h;
}
int minEatingSpeed(vector<int>& piles, int h) {
    int lo = 1, hi = *max_element(piles.begin(), piles.end()), ans = 0;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (canEat(piles, h, mid)) { ans = mid; hi = mid - 1; }
        else lo = mid + 1;
    }
    return ans;
}

// ================== 2. 分割数组的最大值（画匠问题） ==================
int groupsNeeded(const vector<int>& a, long long cap) {
    int groups = 1;
    long long sum = 0;
    for (int x : a) {
        if (x > cap) return INT_MAX;                  // 单元素超上限，一定不可行
        if (sum + x > cap) { groups++; sum = x; }
        else sum += x;
    }
    return groups;
}
int splitArray(vector<int>& a, int k) {
    long long lo = *max_element(a.begin(), a.end());
    long long hi = accumulate(a.begin(), a.end(), 0LL);
    long long ans = 0;
    while (lo <= hi) {
        long long mid = lo + (hi - lo) / 2;
        if (groupsNeeded(a, mid) <= k) { ans = mid; hi = mid - 1; }
        else lo = mid + 1;
    }
    return (int)ans;
}

// ================== 3. 第 K 小的数对距离 ==================
int countPairsLE(const vector<int>& a, int x) {        // 距离 <= x 的数对个数
    int ans = 0;
    for (int l = 0, r = 1; l < (int)a.size(); l++) {
        while (r < (int)a.size() && a[r] - a[l] <= x) r++;
        ans += r - l - 1;
    }
    return ans;
}
int smallestDistancePair(vector<int>& a, int k) {
    sort(a.begin(), a.end());
    int lo = 0, hi = a.back() - a.front(), ans = 0;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (countPairsLE(a, mid) >= k) { ans = mid; hi = mid - 1; }
        else lo = mid + 1;
    }
    return ans;
}

// ================== 4. 等位时间 / 完成旅途的最少时间 ==================
// f(t) = Σ (t / arr[i] + 1)：到时刻 t 为止(含正在服务)一共能服务多少人
long long servedCount(const vector<int>& a, long long t) {
    long long cnt = 0;
    for (int x : a) cnt += t / x;
    return cnt;
}
long long minimumTime(vector<int>& time, int totalTrips) {   // LeetCode 2187
    long long lo = 1, hi = (long long)*min_element(time.begin(), time.end()) * totalTrips;
    long long ans = hi;
    while (lo <= hi) {
        long long mid = lo + (hi - lo) / 2;
        if (servedCount(time, mid) >= totalTrips) { ans = mid; hi = mid - 1; }
        else lo = mid + 1;
    }
    return ans;
}

// ================== 5. 机器人跳跃问题 ==================
bool canPass(const vector<int>& h, int n, int energy, int maxH) {
    for (int i = 1; i <= n; i++) {
        if (h[i] > energy) energy -= h[i] - energy;
        else energy += energy - h[i];
        if (energy >= maxH) return true;              // 剪枝：一定通关
        if (energy < 0) return false;
    }
    return true;
}

int main() {
    vector<int> piles = {3, 6, 7, 11};
    std::cout << "珂珂最小速度 = " << minEatingSpeed(piles, 8) << "\n";      // 4
    vector<int> a = {7, 2, 5, 10, 8};
    std::cout << "分割最大和最小 = " << splitArray(a, 2) << "\n";             // 18
    vector<int> d = {1, 3, 1};
    std::cout << "第 1 小数对距离 = " << smallestDistancePair(d, 1) << "\n";  // 0
    vector<int> t = {1, 2, 3};
    std::cout << "完成 5 趟最少时间 = " << minimumTime(t, 5) << "\n";       // 3
    return 0;
}
