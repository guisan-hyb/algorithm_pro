// 数据量猜解法模板：同一题的两种 DP / 排列转 LIS / 值域太大用哈希表
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <climits>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

// ================= 1. 贿赂怪兽：a 大 b 小 -> 按"钱"做容量 =================
// dp[j] = 花不超过 j 的钱能获得的最大能力（NEG 表示不可达）
int bribeByMoney(int n, const vector<int>& a, const vector<int>& b) {
    int sumB = 0;
    for (int i = 1; i <= n; i++) sumB += b[i];
    const int NEG = INT_MIN / 2;
    vector<int> dp(sumB + 1, NEG);
    dp[0] = 0;
    for (int i = 1; i <= n; i++) {
        vector<int> ndp = dp;
        for (int j = 0; j <= sumB; j++) {
            if (j >= b[i] && dp[j - b[i]] != NEG)
                ndp[j] = max(ndp[j], dp[j - b[i]] + a[i]);      // 贿赂
        }
        // "不贿赂"要求当前能力 >= a[i]，所以在能力维度上不可直接用一维滚动，
        // 这里给出明确的二维写法便于理解（实际按数据量可二选一）
        dp.swap(ndp);
        for (int j = 0; j <= sumB; j++) if (dp[j] < a[i] && dp[j] != NEG) dp[j] = NEG;
    }
    for (int j = 0; j <= sumB; j++) if (dp[j] != NEG) return j;
    return -1;
}

// ================= 2. 贿赂怪兽：a 小 b 大 -> 按"能力"做容量 =================
// dp[j] = 能力恰好为 j 时花的最少钱（INF 表示不可达）
long long bribeByAbility(int n, const vector<int>& a, const vector<int>& b) {
    int total = 0;
    for (int i = 1; i <= n; i++) total += a[i];
    const long long INF = LLONG_MAX / 4;
    vector<long long> dp(total + 1, INF);
    dp[0] = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = total; j >= 0; j--) {
            long long cur = INF;
            if (j >= a[i] && dp[j] != INF) cur = dp[j];          // 不贿赂
            if (j >= a[i] && dp[j - a[i]] != INF)
                cur = min(cur, dp[j - a[i]] + b[i]);             // 贿赂
            dp[j] = cur;
        }
    }
    long long ans = INF;
    for (int j = 0; j <= total; j++) ans = min(ans, dp[j]);
    return ans == INF ? -1 : ans;
}

// ================= 3. 两个排列的 LCS -> LIS（n <= 1e5） =================
int lcsOfPermutations(const vector<int>& A, const vector<int>& B) {
    int n = (int)A.size();
    vector<int> where(n + 1, 0);
    for (int i = 0; i < n; i++) where[A[i]] = i;
    vector<int> ends(n);
    int len = 0;
    for (int i = 0; i < n; i++) {
        int x = where[B[i]];
        int l = 0, r = len - 1, pos = -1;
        while (l <= r) {
            int m = l + (r - l) / 2;
            if (ends[m] >= x) { pos = m; r = m - 1; }
            else l = m + 1;
        }
        if (pos == -1) ends[len++] = x;
        else ends[pos] = x;
    }
    return len;
}

// ================= 4. 值域太大：用哈希表当 DP 表 =================
// f(pre, i) 中 pre 可达 1e9，用 long long 把 (pre, i) 压成 key
unordered_map<long long,int> memo;
int makeArrayIncreasing(const vector<int>& arr1, vector<int> arr2, int i, int pre) {
    if (i == (int)arr1.size()) return 0;
    long long key = ((long long)pre << 32) | (unsigned)i;        // pre 为负时也能用（哈希键）
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;
    const int INF = INT_MAX / 2;
    int ans = INF;
    if (arr1[i] > pre) ans = min(ans, makeArrayIncreasing(arr1, arr2, i + 1, arr1[i]));
    auto pos = upper_bound(arr2.begin(), arr2.end(), pre);
    if (pos != arr2.end()) ans = min(ans, 1 + makeArrayIncreasing(arr1, arr2, i + 1, *pos));
    return memo[key] = ans;
}

int main() {
    vector<int> a = {0, 8, 2, 5}, b = {0, 1, 4, 2};     // 1-based 使用
    std::cout << "按钱做容量 = " << bribeByMoney(3, a, b) << "\n";
    std::cout << "按能力做容量 = " << bribeByAbility(3, a, b) << "\n";

    vector<int> A = {3, 2, 1, 4, 5}, B = {1, 2, 3, 4, 5};
    std::cout << "排列的 LCS = " << lcsOfPermutations(A, B) << "\n";   // 3

    vector<int> arr1 = {1, 5, 3, 6, 7};
    vector<int> arr2 = {1, 3, 2, 4};
    sort(arr2.begin(), arr2.end());
    arr2.erase(unique(arr2.begin(), arr2.end()), arr2.end());
    memo.clear();
    std::cout << "最小替换次数 = " << makeArrayIncreasing(arr1, arr2, 0, INT_MIN / 2) << "\n";
    return 0;
}
