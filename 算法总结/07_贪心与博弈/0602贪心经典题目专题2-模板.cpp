// 贪心专题2模板：砍竹子 / 分k份 / 会议两类 / IPO / 差值收敛
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <numeric>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;
const long long MOD = 1000000007;

long long quickPow(long long x, long long power) {
    long long ans = 1;
    x %= MOD;
    while (power > 0) {
        if (power & 1) ans = ans * x % MOD;
        x = x * x % MOD;
        power >>= 1;
    }
    return ans;
}

// ---------- 1. 砍竹子 II（尽量拆 3） ----------
int cuttingBamboo(int n) {
    if (n <= 2) return 1;
    if (n == 3) return 2;
    int tail = n % 3 == 0 ? 1 : (n % 3 == 1 ? 4 : 2);
    int power = (tail == 1 ? n : n - tail) / 3;
    return (int)(quickPow(3, power) * tail % MOD);
}

// ---------- 2. 分成 k 份的最大乘积（均分） ----------
int maxProductOfKParts(long long n, long long k) {
    long long a = n / k, b = n % k;
    return (int)(quickPow(a + 1, b) * quickPow(a, k - b) % MOD);
}

// ---------- 3. 会议独占时间段：按结束时间排序 ----------
int maxEventsExclusive(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[1] < b[1]; });
    int ans = 0, cur = INT_MIN;
    for (auto& it : intervals)
        if (it[0] >= cur) { ans++; cur = it[1]; }
    return ans;
}

// ---------- 4. 会议只占一天：按开始时间 + 小根堆 ----------
int maxEvents(vector<vector<int>> events) {
    sort(events.begin(), events.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[0] < b[0]; });
    int minDay = events[0][0], maxDay = 0;
    for (auto& e : events) maxDay = max(maxDay, e[1]);
    priority_queue<int, vector<int>, greater<>> heap;
    int ans = 0, idx = 0;
    for (int day = minDay; day <= maxDay; day++) {
        while (idx < (int)events.size() && events[idx][0] == day) heap.push(events[idx++][1]);
        while (!heap.empty() && heap.top() < day) heap.pop();
        if (!heap.empty()) { ans++; heap.pop(); }
    }
    return ans;
}

// ---------- 5. IPO（门槛小根堆 + 利润大根堆） ----------
int findMaximizedCapital(int k, int w, vector<int> profits, vector<int> capital) {
    int n = (int)profits.size();
    vector<int> idx(n);
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [&](int a, int b) { return capital[a] < capital[b]; });
    priority_queue<int> maxPro;
    int i = 0;
    while (k-- > 0) {
        while (i < n && capital[idx[i]] <= w) { maxPro.push(profits[idx[i]]); i++; }
        if (maxPro.empty()) break;
        w += maxPro.top(); maxPro.pop();
    }
    return w;
}

// ---------- 6. 差值绝对值直到长度固定（裴蜀定理） ----------
int finalLength(vector<int> arr) {
    if (arr.size() < 2) return (int)arr.size();
    unordered_map<int,int> cnt;
    int g = 0, maxs = 0;
    for (int x : arr) { maxs = max(maxs, x); g = std::gcd(g, x); cnt[x]++; }
    if (g == 0) return (int)arr.size();
    int distinct = (int)cnt.size();
    bool hasZero = cnt.count(0) > 0 || distinct < (int)arr.size();
    int seqLen = maxs / g + (hasZero ? 1 : 0);
    return (int)arr.size() + (seqLen - distinct);
}

// ---------- 7. 暴力模拟版（作对数器） ----------
int finalLengthBrute(vector<int> arr) {
    unordered_set<int> seen(arr.begin(), arr.end());
    bool changed = true;
    while (changed) {
        changed = false;
        vector<int> add;
        for (size_t i = 0; i < arr.size(); i++)
            for (size_t j = i + 1; j < arr.size(); j++) {
                int d = abs(arr[i] - arr[j]);
                if (!seen.count(d)) { seen.insert(d); add.push_back(d); }
            }
        for (int d : add) { arr.push_back(d); changed = true; }
    }
    return (int)arr.size();
}

int main() {
    std::cout << "砍竹子(10) = " << cuttingBamboo(10) << "\n";                   // 36
    std::cout << "分k份 = " << maxProductOfKParts(6, 3) << "\n";                  // 8
    std::cout << "独占会议数 = " << maxEventsExclusive({{1,2},{2,3},{3,4}}) << "\n";  // 3
    std::cout << "只占一天 = " << maxEvents({{1,2},{2,3},{3,4}}) << "\n";              // 3
    std::cout << "IPO = " << findMaximizedCapital(2, 0, {1,2,3}, {0,1,1}) << "\n";     // 4
    std::cout << "最终长度 = " << finalLength({6,10}) << " (暴力 = " << finalLengthBrute({6,10}) << ")\n";
    return 0;
}
