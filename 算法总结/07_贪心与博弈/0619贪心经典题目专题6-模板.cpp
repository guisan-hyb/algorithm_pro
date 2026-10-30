// 贪心专题6模板：消灭怪物 / 最大回文数字 / 最大平均通过率 / 雇佣工人 / 砍树
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <array>
#include <functional>
#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>
using namespace std;

// ---------- 1. 消灭怪物的最大数量 ----------
int eliminateMaximum(vector<int>& dist, vector<int>& speed) {
    int n = (int)dist.size();
    vector<int> t(n);
    for (int i = 0; i < n; i++) t[i] = (dist[i] + speed[i] - 1) / speed[i];   // 向上取整
    sort(t.begin(), t.end());
    for (int i = 0; i < n; i++) if (t[i] <= i) return i;
    return n;
}

// ---------- 2. 最大回文数字 ----------
string largestPalindromic(const string& num) {
    std::array<int, 10> cnt= {0};
    for (char c : num) cnt[c - '0']++;
    string half;
    char mid = 0;
    for (int d = 9; d >= 1; d--) {
        if (mid == 0 && (cnt[d] & 1)) mid = char('0' + d);
        half.append(cnt[d] / 2, char('0' + d));
    }
    if (half.empty() && mid == 0) return "0";
    if (half.empty()) return string(1, mid);
    half.append(cnt[0] / 2, '0');
    if (mid == 0 && (cnt[0] & 1)) mid = '0';
    string ans = half;
    if (mid) ans += mid;
    for (int i = (int)half.size() - 1; i >= 0; i--) ans += half[i];
    return ans;
}

// ---------- 3. 最大平均通过率（大根堆按边际收益） ----------
double maxAverageRatio(vector<vector<int>>& classes, int extra) {
    auto gain = [](double p, double t) { return (p + 1) / (t + 1) - p / t; };
    auto cmp = [&](const vector<double>& a, const vector<double>& b) {
        return gain(a[0], a[1]) < gain(b[0], b[1]);
    };
    priority_queue<vector<double>, vector<vector<double>>, decltype(cmp)> heap(cmp);
    for (auto& c : classes) heap.push({(double)c[0], (double)c[1]});
    while (extra--) {
        vector<double> cur = heap.top(); heap.pop();
        cur[0] += 1; cur[1] += 1;
        heap.push(cur);
    }
    double sum = 0;
    while (!heap.empty()) { sum += heap.top()[0] / heap.top()[1]; heap.pop(); }
    return sum / classes.size();
}

// ---------- 4. 雇佣 K 名工人的最低成本 ----------
double mincostToHireWorkers(vector<int>& quality, vector<int>& wage, int k) {
    int n = (int)quality.size();
    vector<pair<double,int>> arr(n);                    // {ratio, quality}
    for (int i = 0; i < n; i++) arr[i] = {(double)wage[i] / quality[i], quality[i]};
    sort(arr.begin(), arr.end());
    priority_queue<int> heap;                           // 质量大根堆
    long long sumQ = 0;
    double ans = 1e18;                                  // 用大数代替 DBL_MAX，避免依赖 <cfloat>
    for (auto& pr : arr) {
        double ratio = pr.first;
        int q = pr.second;
        heap.push(q);
        sumQ += q;
        if ((int)heap.size() > k) { sumQ -= heap.top(); heap.pop(); }
        if ((int)heap.size() == k) ans = min(ans, sumQ * ratio);
    }
    return ans;
}

// ---------- 5. 砍树（按增长率排序 + DP） ----------
long long cutTrees(vector<vector<int>> tree, int day) {  // tree[i] = {初始重量, 每日增长}
    sort(tree.begin(), tree.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[1] < b[1]; });
    int n = (int)tree.size();
    vector<vector<long long>> dp(n + 1, vector<long long>(day + 1, 0));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= day; j++)
            dp[i][j] = max(dp[i - 1][j],
                           dp[i - 1][j - 1] + tree[i - 1][0] + (long long)tree[i - 1][1] * (j - 1));
    return dp[n][day];
}
// 暴力版（作对数器）：枚举每天的砍树选择
long long cutTreesBrute(vector<vector<int>> tree, int day) {
    int n = (int)tree.size();
    vector<char> used(n, 0);
    function<long long(int)> f = [&](int remaining) -> long long {
        if (remaining == 0) return 0;
        long long ans = f(remaining - 1);                       // 今天不砍
        for (int i = 0; i < n; i++)
            if (!used[i]) {
                used[i] = 1;
                ans = max(ans, f(remaining - 1) + tree[i][0] + (long long)tree[i][1] * (day - remaining));
                used[i] = 0;
            }
        return ans;
    };
    return f(day);
}

int main() {
    vector<int> dist = {1,3,4}, speed = {1,1,1};
    std::cout << "消灭怪物 = " << eliminateMaximum(dist, speed) << "\n";       // 3
    std::cout << "最大回文数字 = " << largestPalindromic("444947137") << "\n";
    vector<vector<int>> cls = {{1,2},{3,5},{2,2}};
    std::cout << "最大平均通过率 = " << std::fixed << std::setprecision(5) << maxAverageRatio(cls, 2) << std::defaultfloat << std::setprecision(6) << "\n";
    vector<int> q = {10,20,5}, w = {70,50,30};
    std::cout << "雇佣最低成本 = " << std::fixed << std::setprecision(5) << mincostToHireWorkers(q, w, 2) << std::defaultfloat << std::setprecision(6) << "\n";  // 105.00000
    vector<vector<int>> tree = {{1,2},{2,1}};
    std::cout << "砍树最大收益 = " << cutTrees(tree, 2) << " (暴力 = " << cutTreesBrute(tree, 2) << ")\n";
    return 0;
}
