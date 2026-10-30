// 单调栈模板：左右最近更小 / 柱状图最大矩形 / 每日温度 / 最大宽度坡 / 去重字母
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ================= 1. 经典：每个位置左右最近的严格更小位置 =================
void nearestSmaller(const vector<int>& a, vector<int>& L, vector<int>& R) {
    int n = (int)a.size();
    L.assign(n, -1);
    R.assign(n, -1);
    vector<int> stk;
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && a[stk.back()] >= a[i]) {   // 相等也要弹（配合修正阶段）
            int idx = stk.back(); stk.pop_back();
            L[idx] = stk.empty() ? -1 : stk.back();
            R[idx] = i;
        }
        stk.push_back(i);
    }
    while (!stk.empty()) {
        int idx = stk.back(); stk.pop_back();
        L[idx] = stk.empty() ? -1 : stk.back();
        R[idx] = -1;
    }
    // 修正：右边第一个"更小"其实相等时要继续往右找
    for (int i = n - 2; i >= 0; i--)
        if (R[i] != -1 && a[R[i]] == a[i]) R[i] = R[R[i]];
}

// ================= 2. 柱状图中最大的矩形 =================
int largestRectangleArea(const vector<int>& h) {
    long long ans = 0;
    vector<int> stk;
    int n = (int)h.size();
    for (int i = 0; i <= n; i++) {
        int cur = (i == n) ? -1 : h[i];                   // 末尾虚拟高度 -1，强制清算
        while (!stk.empty() && h[stk.back()] >= cur) {
            int idx = stk.back(); stk.pop_back();
            int pre = stk.empty() ? -1 : stk.back();
            ans = max(ans, (long long)h[idx] * (i - pre - 1));
        }
        if (i < n) stk.push_back(i);
    }
    return (int)ans;
}

// ================= 3. 每日温度 =================
vector<int> dailyTemperatures(const vector<int>& t) {
    int n = (int)t.size();
    vector<int> ans(n, 0), stk;
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && t[i] > t[stk.back()]) {
            int idx = stk.back(); stk.pop_back();
            ans[idx] = i - idx;
        }
        stk.push_back(i);
    }
    return ans;
}

// ================= 4. 最大宽度坡 =================
int maxWidthRamp(const vector<int>& a) {
    int n = (int)a.size(), ans = 0;
    vector<int> stk;
    for (int i = 0; i < n; i++)                          // 只把"可能是左端"的入栈
        if (stk.empty() || a[i] < a[stk.back()]) stk.push_back(i);
    for (int i = n - 1; i >= 0; i--)                     // 从右往左找答案
        while (!stk.empty() && a[stk.back()] <= a[i]) {
            ans = max(ans, i - stk.back());
            stk.pop_back();
        }
    return ans;
}

// ================= 5. 去除重复字母，字典序最小 =================
string removeDuplicateLetters(const string& s) {
    std::array<int, 26> cnt= {0};
    std::array<bool, 26> inStack= {false};
    for (char c : s) cnt[c - 'a']++;
    string stk;
    for (char c : s) {
        if (!inStack[c - 'a']) {
            while (!stk.empty() && c < stk.back() && cnt[stk.back() - 'a'] > 0) {
                inStack[stk.back() - 'a'] = false;
                stk.pop_back();
            }
            inStack[c - 'a'] = true;
            stk.push_back(c);
        }
        cnt[c - 'a']--;
    }
    return stk;
}

int main() {
    vector<int> a = {2, 1, 5, 6, 2, 3};
    std::cout << "柱状图最大矩形 = " << largestRectangleArea(a) << "\n";   // 10
    vector<int> t = {73, 74, 75, 71, 69, 72, 76, 73};
    for (int x : dailyTemperatures(t)) std::cout << x << " ";        // 1 1 4 2 1 1 0 0
    std::cout << "\n";
    vector<int> r = {6, 0, 8, 2, 1, 5};
    std::cout << "最大宽度坡 = " << maxWidthRamp(r) << "\n";               // 4
    std::cout << "去重字母 = " << removeDuplicateLetters("bcabc") << "\n";  // abc
    return 0;
}
