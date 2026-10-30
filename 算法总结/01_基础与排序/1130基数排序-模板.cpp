// 基数排序模板（LSD，低位优先，十进制）+ 负数处理
// 时间 O(n)，额外空间 O(n + BASE)，稳定
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

const int BASE = 10;

int bitsOf(long long x) {
    int ans = 0;
    while (x) { x /= 10; ans++; }
    return ans;
}

// arr 内必须全为非负数；bits 为最大值在 BASE 进制下的位数
void radixSort(vector<int>& arr, int bits) {
    int n = (int)arr.size();
    vector<int> cnt(BASE), help(n);
    for (long long offset = 1; bits > 0; offset *= BASE, bits--) {
        fill(cnt.begin(), cnt.end(), 0);
        for (int i = 0; i < n; i++)
            cnt[(arr[i] / offset) % BASE]++;
        for (int i = 1; i < BASE; i++)
            cnt[i] += cnt[i - 1];                                  // 前缀次数
        for (int i = n - 1; i >= 0; i--)                           // 从右往左 -> 稳定
            help[--cnt[(arr[i] / offset) % BASE]] = arr[i];
        for (int i = 0; i < n; i++)
            arr[i] = help[i];
    }
}

// 含负数版本：整体减去最小值平移，排完加回来
void sortWithNegative(vector<int>& a) {
    if (a.size() < 2) return;
    int mins = *min_element(a.begin(), a.end());
    int maxs = 0;
    for (int& x : a) { x -= mins; maxs = max(maxs, x); }
    radixSort(a, bitsOf(maxs));
    for (int& x : a) x += mins;
}

int main() {
    int n;
    if (!(std::cin >> n) || n <= 0) return 0;
    vector<int> a(n);
    for (int& x : a) (std::cin >> x);
    sortWithNegative(a);
    for (int i = 0; i < n; i++) std::cout << a[i] << " \n"[i == n - 1];
    return 0;
}
