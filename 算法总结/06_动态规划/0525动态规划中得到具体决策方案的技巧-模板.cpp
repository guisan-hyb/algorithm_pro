// 方案反推模板：LCS 路径 / 状压反推队伍 / 路径收集结构
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// ================= 1. 由 DP 表反推 LCS 的具体方案 =================
string lcsPath(const string& a, const string& b) {
    int m = (int)a.size(), n = (int)b.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            dp[i][j] = (a[i - 1] == b[j - 1])
                     ? dp[i - 1][j - 1] + 1
                     : max(dp[i - 1][j], dp[i][j - 1]);
    int len = dp[m][n];
    if (len == 0) return "-1";
    string ans(len, ' ');
    for (int i = m, j = n; len > 0; ) {
        if (a[i - 1] == b[j - 1]) { ans[--len] = a[i - 1]; i--; j--; }
        else if (dp[i - 1][j] >= dp[i][j - 1]) i--;
        else j--;
    }
    return ans;
}

// ================= 2. 状压 + 反推方案：最小的必要团队 =================
vector<int> smallestSufficientTeam(vector<string>& req_skills,
                                   vector<vector<string>>& people) {
    unordered_map<string,int> id;
    for (auto& s : req_skills) id[s] = (int)id.size();
    int num = (int)req_skills.size(), n = (int)people.size();
    vector<int> arr(n, 0);
    for (int i = 0; i < n; i++)
        for (auto& s : people[i])
            if (id.count(s)) arr[i] |= 1 << id[s];

    const int INF = INT_MAX / 2;
    vector<vector<int>> dp(n + 1, vector<int>(1 << num, INF));
    dp[n][(1 << num) - 1] = 0;
    for (int i = n - 1; i >= 0; i--)
        for (int s = 0; s < (1 << num); s++)
            dp[i][s] = min(dp[i + 1][s], 1 + dp[i + 1][s | arr[i]]);

    vector<int> ans;                                       // 反推方案
    for (int i = 0, s = 0; s != (1 << num) - 1 && i < n; i++)
        if (i == n - 1 || dp[i][s] != dp[i + 1][s]) {
            ans.push_back(i);
            s |= arr[i];
        }
    return ans;
}

// ================= 3. 路径收集结构：二维费用背包 + 字典序最小方案 =================
pair<int,string> knapsackWithPath(int m, int v,
                                  const vector<int>& a, const vector<int>& b,
                                  const vector<int>& c) {
    int n = (int)a.size();
    vector<vector<int>> dp(m + 1, vector<int>(v + 1, 0));
    vector<vector<string>> path(m + 1, vector<string>(v + 1, ""));
    for (int i = 1; i <= n; i++) {
        for (int j = m; j >= a[i - 1]; j--) {
            for (int k = v; k >= b[i - 1]; k--) {
                string p = path[j - a[i - 1]][k - b[i - 1]];
                p = p.empty() ? to_string(i) : p + " " + to_string(i);
                int val = dp[j - a[i - 1]][k - b[i - 1]] + c[i - 1];
                if (val > dp[j][k]) { dp[j][k] = val; path[j][k] = p; }
                else if (val == dp[j][k] && p < path[j][k]) path[j][k] = p;
            }
        }
    }
    return {dp[m][v], path[m][v]};
}

// ================= 4. LIS 字典序最小方案（倒着求 dp：以 i 开头） =================
// dp[i] = 以 i 开头往右延伸的最长递增子序列长度（从右往左、用"不上升"二分）
vector<int> lisLexSmallest(const vector<int>& nums) {
    int n = (int)nums.size();
    vector<int> dp(n, 0), help(n, 0);
    int len = 0;
    auto findLe = [&](int x) {                             // 找第一个 <= x 的位置
        int l = 0, r = len - 1, pos = -1;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (help[mid] <= x) { pos = mid; r = mid - 1; }
            else l = mid + 1;
        }
        return pos;
    };
    for (int i = n - 1; i >= 0; i--) {
        int pos = findLe(nums[i]);
        if (pos == -1) { help[len++] = nums[i]; dp[i] = len; }
        else { help[pos] = nums[i]; dp[i] = pos + 1; }
    }
    vector<int> ans(len, INT_MAX);
    for (int i = 0; i < n; i++) {
        if (dp[i] == len) ans[0] = nums[i];                // 越靠右且越小 -> 覆盖
        else if (ans[len - 1 - dp[i]] < nums[i]) ans[len - dp[i]] = nums[i];
    }
    return ans;
}

int main() {
    printf("LCS 方案 = %s\n", lcsPath("abcde", "ace").c_str());   // ace

    vector<string> req = {"java","nodejs","reactjs"};
    vector<vector<string>> people = {{"java"},{"nodejs"},{"nodejs","reactjs"}};
    for (int i : smallestSufficientTeam(req, people)) printf("%d ", i);
    puts("");                                                     // 0 2

    vector<int> lis = lisLexSmallest({1, 3, 2, 4});
    for (int x : lis) printf("%d ", x);
    puts("");                                                     // 1 2 4

    auto pr = knapsackWithPath(10, 10, {5,4}, {6,5}, {10,8});
    printf("最大价值 = %d, 方案 = %s\n", pr.first, pr.second.c_str());
    return 0;
}
