// 双向 BFS + 折半搜索模板
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// ================= 1. 双向 BFS：单词接龙（最短转换序列长度） =================
int ladderLength(string beginWord, string endWord, vector<string> wordList) {
    unordered_set<string> dict(wordList.begin(), wordList.end());
    if (!dict.count(endWord)) return 0;

    unordered_set<string> small{beginWord}, big{endWord}, next;
    int level = 2;
    while (!small.empty()) {
        for (auto& w : small) dict.erase(w);       // 已处理的串从词典删除
        next.clear();
        for (auto& w : small) {
            for (size_t i = 0; i < w.size(); i++) {
                string nw = w;
                for (char c = 'a'; c <= 'z'; c++) {
                    nw[i] = c;
                    if (nw == w) continue;
                    if (big.count(nw)) return level;      // 两侧相遇
                    if (dict.count(nw)) next.insert(nw);
                }
            }
        }
        if (next.size() >= big.size()) { small = big; big = next; }  // 总是扩小的一侧
        else                            { small = next; }
        level++;
    }
    return 0;
}

// ================= 2. 折半搜索：背包方案数（n <= 40） =================
vector<long long> lsum, rsum;

void dfsHalf(vector<long long>& out, const vector<long long>& a, int i, int end, long long sum, long long limit) {
    if (sum > limit) return;
    if (i == end) { out.push_back(sum); return; }
    dfsHalf(out, a, i + 1, end, sum, limit);              // 不要 i
    dfsHalf(out, a, i + 1, end, sum + a[i], limit);       // 要 i
}

long long countWays(const vector<long long>& a, long long limit) {
    lsum.clear(); rsum.clear();
    int n = (int)a.size();
    dfsHalf(lsum, a, 0, n / 2, 0, limit);
    dfsHalf(rsum, a, n / 2, n, 0, limit);
    sort(lsum.begin(), lsum.end());
    sort(rsum.begin(), rsum.end());

    long long ans = 0;
    for (int i = (int)lsum.size() - 1, j = 0; i >= 0; i--) {
        while (j < (int)rsum.size() && lsum[i] + rsum[j] <= limit) j++;
        ans += j;
    }
    return ans;
}

// ================= 3. 折半搜索：最接近目标值的子序列和 =================
int minAbsDifference(vector<int>& nums, int goal) {
    lsum.clear(); rsum.clear();
    int n = (int)nums.size();

    long long maxs = 0, mins = 0;                      // 剪枝
    for (int x : nums) { if (x > 0) maxs += x; if (x < 0) mins += x; }
    if (maxs < goal) return (int)abs(goal - maxs);
    if (mins > goal) return (int)abs(goal - mins);

    function<void(vector<long long>&, int, int, long long)> dfs =
        [&](vector<long long>& out, int i, int end, long long sum) {
            if (i == end) { out.push_back(sum); return; }
            dfs(out, i + 1, end, sum);
            dfs(out, i + 1, end, sum + nums[i]);
        };
    dfs(lsum, 0, n / 2, 0);
    dfs(rsum, n / 2, n, 0);
    sort(lsum.begin(), lsum.end());
    sort(rsum.begin(), rsum.end());

    long long ans = llabs(goal);
    for (int i = 0, j = (int)rsum.size() - 1; i < (int)lsum.size(); i++) {
        while (j > 0 && llabs(goal - lsum[i] - rsum[j]) >= llabs(goal - lsum[i] - rsum[j - 1]))
            j--;                                        // 相等时也要移动
        ans = min(ans, llabs(goal - lsum[i] - rsum[j]));
    }
    return (int)ans;
}

int main() {
    vector<string> dict = {"hot", "dot", "dog", "lot", "log", "cog"};
    printf("单词接龙最短长度 = %d\n", ladderLength("hit", "cog", dict));   // 5

    vector<long long> a = {1, 2, 3, 4};
    printf("体积不超过 5 的方案数 = %lld\n", countWays(a, 5));              // 5

    vector<int> nums = {5, -7, 3, 5};
    printf("最接近 6 的子序列和差 = %d\n", minAbsDifference(nums, 6));      // 0
    return 0;
}
