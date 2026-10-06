// 贪心专题4模板：最小偏移量 / 兔子 / 相似数组 / 知识竞赛 / 递增分组 / 最少加油
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// ---------- 1. 数组的最小偏移量 ----------
int minimumDeviation(vector<int> nums) {
    set<int> s;
    for (int x : nums) s.insert(x % 2 == 0 ? x : x * 2);    // 奇数先翻倍
    int ans = *s.rbegin() - *s.begin();
    while (ans > 0 && *s.rbegin() % 2 == 0) {
        int mx = *s.rbegin();
        s.erase(mx);
        s.insert(mx / 2);
        ans = min(ans, *s.rbegin() - *s.begin());
    }
    return ans;
}

// ---------- 2. 森林中的兔子 ----------
int numRabbits(vector<int> answers) {
    unordered_map<int,int> cnt;
    for (int v : answers) cnt[v]++;
    int ans = 0;
    for (auto& kv : cnt) {
        int v = kv.first, f = kv.second;
        ans += (f + v) / (v + 1) * (v + 1);
    }
    return ans;
}

// ---------- 3. 使数组相似的最少操作次数 ----------
long long makeSimilar(vector<int> nums, vector<int> target) {
    auto splitAndSort = [](vector<int>& a) {
        int odd = 0;
        for (int i = 0; i < (int)a.size(); i++)
            if (a[i] & 1) swap(a[odd++], a[i]);
        sort(a.begin(), a.begin() + odd);
        sort(a.begin() + odd, a.end());
    };
    splitAndSort(nums);
    splitAndSort(target);
    long long sum = 0;
    for (int i = 0; i < (int)nums.size(); i++) sum += abs(nums[i] - target[i]);
    return sum / 4;
}

// ---------- 4. 知识竞赛（按 |a-b| 排序 + 前缀最值；返回 2*答案） ----------
int knowledgeContest(vector<vector<int>> a) {
    sort(a.begin(), a.end(), [](const vector<int>& x, const vector<int>& y) {
        return abs(x[0] - x[1]) < abs(y[0] - y[1]);
    });
    int ans = 0, maxA = 0, maxB = 0;
    for (auto& p : a) {
        if (p[0] < p[1]) ans = max(ans, p[0] + maxA);
        else             ans = max(ans, p[1] + maxB);
        maxA = max(maxA, p[0]);
        maxB = max(maxB, p[1]);
    }
    return ans;                                         // 真实答案 = ans / 2.0
}

// ---------- 5. 将数组分成几个递增序列 ----------
bool canDivideIntoSubsequences(vector<int> nums, int k) {
    sort(nums.begin(), nums.end());
    int n = (int)nums.size(), maxCnt = 1, cur = 1;
    for (int i = 1; i < n; i++) {
        if (nums[i] == nums[i - 1]) cur++;
        else { maxCnt = max(maxCnt, cur); cur = 1; }
    }
    maxCnt = max(maxCnt, cur);                          // 别忘最后一次
    return n / maxCnt >= k;
}

// ---------- 6. 最低加油次数 ----------
int minRefuelStops(int target, int startFuel, vector<vector<int>>& stations) {
    if (startFuel >= target) return 0;
    priority_queue<int> heap;                           // 路过的油量（大根堆）
    long long cur = startFuel;
    int cnt = 0;
    for (auto& st : stations) {
        while (!heap.empty() && cur < st[0]) { cur += heap.top(); heap.pop(); cnt++; }
        if (cur < st[0]) return -1;
        heap.push(st[1]);
    }
    while (!heap.empty() && cur < target) { cur += heap.top(); heap.pop(); cnt++; }
    return cur >= target ? cnt : -1;
}

int main() {
    printf("最小偏移量 = %d\n", minimumDeviation({1,2,3,4}));        // 1
    printf("最少兔子数 = %d\n", numRabbits({1,1,2}));                 // 5
    printf("相似最少操作 = %lld\n", makeSimilar({1,2,3}, {2,1,3}));   // 0
    printf("递增分组 = %d\n", (int)canDivideIntoSubsequences({1,2,2,3,3,4}, 3));  // 1
    vector<vector<int>> st = {{10,60},{20,30},{30,30},{60,40}};
    printf("最少加油次数 = %d\n", minRefuelStops(100, 10, st));       // 2
    return 0;
}
