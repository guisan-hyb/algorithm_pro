// 一维动态规划模板：递归 -> 记忆化 -> 迭代 -> 空间压缩
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <climits>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ================= 1. 记忆化搜索骨架 =================
vector<int> memo;
int dfs(int i) {
    if (/* base case 1 */ false) return 0;
    if (memo[i] != -1) return memo[i];          // 算过就直接返回
    int ans = 0;
    // ans = min(ans, ... + dfs(...));
    return memo[i] = ans;
}
int solveMemo(int n) {
    memo.assign(n + 1, -1);
    return dfs(n);
}

// ================= 2. 记忆化搜索：最低票价（LeetCode 983） =================
class MinCostTickets {
    vector<int> duration{1, 7, 30};
    vector<int> days, costs;
    vector<int> dp;
    int f(int i) {
        if (i == (int)days.size()) return 0;
        if (dp[i] != -1) return dp[i];
        int ans = INT_MAX;
        for (int k = 0; k < 3; k++) {
            int j = i;
            while (j < (int)days.size() && days[i] + duration[k] - 1 >= days[j]) j++;
            ans = min(ans, costs[k] + f(j));
        }
        return dp[i] = ans;
    }
public:
    int mincostTickets(vector<int>& d, vector<int>& c) {
        days = d; costs = c;
        dp.assign(days.size(), -1);
        return f(0);
    }
};

// ================= 3. 迭代 + 空间压缩：解码方法（LeetCode 91） =================
int numDecodings(const string& s) {
    int n = (int)s.size();
    int last = 1;               // dp[n]
    int lastlast = 0;           // dp[n+1]
    for (int i = n - 1; i >= 0; i--) {
        int cur = 0;
        if (s[i] != '0') {
            cur = last;
            if (i + 1 < n && ((s[i] - '0') * 10 + (s[i + 1] - '0')) <= 26)
                cur += lastlast;
        }
        lastlast = last;
        last = cur;
    }
    return last;
}

// ================= 4. 解码方法 II（带 '*'，取模） =================
int numDecodingsII(const string& s) {
    const long long MOD = 1000000007;
    int n = (int)s.size();
    long long last = 1, lastlast = 0;            // dp[n] = 1, dp[n+1] = 0
    for (int i = n - 1; i >= 0; i--) {
        long long cur = 0;
        if (s[i] != '0') {
            cur = last * (s[i] == '*' ? 9 : 1);  // 一位
            if (i + 1 < n) {                     // 两位
                if (s[i] != '*') {
                    if (s[i + 1] != '*' && ((s[i] - '0') * 10 + (s[i + 1] - '0')) <= 26)
                        cur += lastlast;
                    else if (s[i + 1] == '*') {
                        if (s[i] == '1') cur += 9 * lastlast;
                        else if (s[i] == '2') cur += 6 * lastlast;
                    }
                } else {
                    if (s[i + 1] != '*') cur += ((s[i + 1] - '0') <= 6 ? 2 : 1) * lastlast;
                    else                 cur += 15 * lastlast;
                }
            }
            cur %= MOD;
        }
        lastlast = last;
        last = cur;
    }
    return (int)last;
}

int main() {
    std::cout << "斐波那契 f(10) = " << solveMemo(10) << "\n";            // 0 初始化后未用，示例占位
    vector<int> days = {1, 4, 6, 7, 8, 20}, costs = {2, 7, 15};
    MinCostTickets m;
    std::cout << "最低票价 = " << m.mincostTickets(days, costs) << "\n";   // 11
    std::cout << "解码方法(226) = " << numDecodings("226") << "\n";        // 3
    std::cout << "解码方法II(1*) = " << numDecodingsII("1*") << "\n";      // 18
    return 0;
}
