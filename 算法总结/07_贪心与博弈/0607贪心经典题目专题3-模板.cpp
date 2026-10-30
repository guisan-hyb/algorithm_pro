// 贪心专题3模板：最短无序子数组 / 最小区间 / 组团买票 / 平均值和 / 初始电量 / 01区间
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <set>
#include <vector>
using namespace std;

// ---------- 1. 最短无序连续子数组（O(n) 两遍扫描） ----------
int findUnsortedSubarray(const vector<int>& nums) {
    int n = (int)nums.size();
    int l = n, r = -1, maxs = INT_MIN;
    for (int i = 0; i < n; i++) {                 // 从左往右找最右的"不达标"
        if (maxs > nums[i]) r = i;
        maxs = max(maxs, nums[i]);
    }
    int mins = INT_MAX;
    for (int i = n - 1; i >= 0; i--) {            // 从右往左找最左的"不达标"
        if (mins < nums[i]) l = i;
        mins = min(mins, nums[i]);
    }
    return max(0, r - l + 1);
}

// ---------- 2. 最小区间（k 个有序列表，有序集合 + 推进最小值） ----------
vector<int> smallestRange(vector<vector<int>>& nums) {
    int k = (int)nums.size();
    struct Node { int v, i, j; };
    auto cmp = [](const Node& a, const Node& b) {
        return a.v != b.v ? a.v < b.v : (a.i != b.i ? a.i < b.i : a.j < b.j);
    };
    set<Node, decltype(cmp)> s(cmp);
    for (int i = 0; i < k; i++) s.insert({nums[i][0], i, 0});
    int bestL = 0, bestR = INT_MAX;
    while ((int)s.size() == k) {
        Node lo = *s.begin(), hi = *s.rbegin();
        if (hi.v - lo.v < bestR - bestL) { bestL = lo.v; bestR = hi.v; }
        s.erase(s.begin());
        if (lo.j + 1 < (int)nums[lo.i].size())
            s.insert({nums[lo.i][lo.j + 1], lo.i, lo.j + 1});
    }
    return {bestL, bestR};
}

// ---------- 3. 组团买票（大根堆取边际收益） ----------
long long groupTickets(int n, vector<vector<int>>& games) {   // games[i] = {Ki, Bi}
    struct Info { long long bi, ki, people; };
    auto profit = [](const Info& x) { return max(0LL, x.bi - x.ki * (2 * x.people + 1)); };
    auto cmp = [&](const Info& a, const Info& b) { return profit(a) < profit(b); };
    priority_queue<Info, vector<Info>, decltype(cmp)> heap(cmp);
    for (auto& g : games) heap.push({g[1], g[0], 0});
    long long ans = 0;
    for (int i = 0; i < n && !heap.empty(); i++) {
        Info cur = heap.top();
        long long p = profit(cur);
        if (p <= 0) break;
        heap.pop();
        ans += p;
        cur.people++;
        heap.push(cur);
    }
    return ans;
}

// ---------- 4. 平均值最小累加和 ----------
int minAverageSum(vector<int> arr, int k) {
    sort(arr.begin(), arr.end());
    int ans = 0;
    for (int i = 0; i <= k - 2; i++) ans += arr[i];       // 最小的 k-1 个各自成组
    int sum = 0;
    for (int i = k - 1; i < (int)arr.size(); i++) sum += arr[i];
    ans += sum / ((int)arr.size() - k + 1);
    return ans;
}

// ---------- 5. 执行所有任务的最少初始电量（按 消耗-需求 降序） ----------
int minimumEffort(vector<vector<int>> tasks) {
    sort(tasks.begin(), tasks.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[0] - a[1] > b[0] - b[1]; });
    int ans = 0;
    for (auto& t : tasks) ans = max(ans + t[0], t[1]);
    return ans;
}

// ---------- 6. 两个 0/1 数量相等区间的最大长度 ----------
int maxEqualZerosOnes(const vector<int>& arr) {
    int n = (int)arr.size();
    int lz = -1, lo = -1, rz = -1, ro = -1;
    for (int i = 0; i < n; i++) {
        if (arr[i] == 0 && lz == -1) lz = i;
        if (arr[i] == 1 && lo == -1) lo = i;
    }
    for (int i = n - 1; i >= 0; i--) {
        if (arr[i] == 0 && rz == -1) rz = i;
        if (arr[i] == 1 && ro == -1) ro = i;
    }
    return max(rz - lz, ro - lo);
}

int main() {
    std::cout << "最短无序子数组 = " << findUnsortedSubarray({2,6,4,8,10,9,15}) << "\n";   // 5
    vector<vector<int>> lists = {{4,10,15,24,26},{0,9,12,20},{5,18,22,30}};
    auto r = smallestRange(lists);
    std::cout << "最小区间 = [" << r[0] << ", " << r[1] << "]\n";                                 // [20,24]
    std::cout << "最小初始电量 = " << minimumEffort({{1,2},{2,4},{4,8}}) << "\n";           // 8
    std::cout << "平均值和 = " << minAverageSum({1,2,3,4,5}, 2) << "\n";                    // 4
    std::cout << "01 最大长度 = " << maxEqualZerosOnes({0,1,1,0,0,1}) << "\n";              // 5
    return 0;
}
