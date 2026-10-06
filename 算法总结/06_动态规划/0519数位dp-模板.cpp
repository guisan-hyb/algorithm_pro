// 数位 DP 模板：不含连续1 / 数位和区间计数 / 无重复数字 / windy数 / 位数码计数
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// ================= 通用要点 =================
// f(i, isLimit, isNum, ...) ：构造从左往右第 i 位及之后的方案数
// up    = isLimit ? num[i]-'0' : 9
// start = isNum ? 0 : 1
// 只有 (!isLimit && isNum) 的状态才能写进 dp（通用子问题）

// ================= 1. 不含连续 1 的非负整数 =================
class NoConsecutiveOnes {
    vector<vector<int>> dp;
    string num;
    int f(int i, bool isPre, bool isLimit) {
        if (i == (int)num.size()) return 1;
        if (!isLimit && dp[i][isPre] != -1) return dp[i][isPre];
        int ans = 0, cur = num[i] - '0', up = isLimit ? cur : 1;
        for (int pick = 0; pick <= up; pick++) {
            if (isPre && pick) continue;                       // 不能连续两个 1
            ans += f(i + 1, pick == 1, isLimit && pick == up);
        }
        if (!isLimit) dp[i][isPre] = ans;
        return ans;
    }
public:
    int findIntegers(int n) {
        num = bitset<32>(n).to_string();
        dp.assign(num.size(), vector<int>(2, -1));
        return f(0, false, true);
    }
};

// ================= 2. 数位和在 [mins, maxs] 的整数个数（两个剪枝） =================
class CountOfIntegers {
    vector<vector<vector<int>>> dp;
    int mins = 0, maxs = 0;
    static const int MOD = 1000000007;
    int f(const string& num, int i, int sum, bool isLimit) {
        if (sum > maxs) return 0;
        if (sum + (int)(num.size() - i) * 9 < mins) return 0;  // ★神之一剪
        if (i == (int)num.size()) return 1;
        if (dp[i][sum][isLimit] != -1) return dp[i][sum][isLimit];
        long long ans = 0;
        int cur = num[i] - '0';
        if (isLimit) {
            ans += f(num, i + 1, sum + cur, true);
            for (int pick = 0; pick < cur; pick++) ans += f(num, i + 1, sum + pick, false);
        } else {
            for (int pick = 0; pick <= 9; pick++) ans += f(num, i + 1, sum + pick, false);
        }
        return dp[i][sum][isLimit] = (int)(ans % MOD);
    }
    bool check(const string& s) {
        int sum = 0;
        for (char c : s) sum += c - '0';
        return sum >= mins && sum <= maxs;
    }
    void reset() { dp.assign(23, vector<vector<int>>(401, vector<int>(2, -1))); }
public:
    int count(const string& num1, const string& num2, int min_sum, int max_sum) {
        mins = min_sum; maxs = max_sum;
        reset();
        long long ans = f(num2, 0, 0, true);
        reset();
        ans -= f(num1, 0, 0, true);
        if (check(num1)) ans++;                                // 补回左端点
        return (int)((ans % MOD + MOD) % MOD);
    }
};

// ================= 3. 完全没有重复的数字（status 位集） =================
class SpecialNumbers {
    vector<vector<int>> dp;
    string num;
    int f(int i, bool isLimit, bool isNum, int status) {
        if (i == (int)num.size()) return isNum ? 1 : 0;
        if (!isLimit && isNum && dp[i][status] != -1) return dp[i][status];
        int ans = 0;
        if (!isNum) ans += f(i + 1, false, false, status);
        int up = isLimit ? num[i] - '0' : 9;
        int start = isNum ? 0 : 1;
        for (int pick = start; pick <= up; pick++)
            if (!(status & (1 << pick)))
                ans += f(i + 1, isLimit && pick == up, true, status ^ (1 << pick));
        if (!isLimit && isNum) dp[i][status] = ans;
        return ans;
    }
public:
    int countSpecialNumbers(int n) {
        num = to_string(n);
        dp.assign(num.size(), vector<int>(1 << 10, -1));
        return f(0, true, false, 0);
    }
    int numDupDigitsAtMostN(int n) { return n - countSpecialNumbers(n); }  // 至少有 1 位重复
};

// ================= 4. windy 数（相邻差 ≥ 2） =================
class WindyNumbers {
    vector<vector<int>> dp;
    int f(const string& num, int i, bool isLimit, bool isNum, int prev) {
        if (i == (int)num.size()) return isNum ? 1 : 0;
        if (!isLimit && isNum && dp[i][prev] != -1) return dp[i][prev];
        int ans = 0;
        if (!isNum) ans += f(num, i + 1, false, false, 0);
        int up = isLimit ? num[i] - '0' : 9;
        int start = isNum ? 0 : 1;
        for (int pick = start; pick <= up; pick++) {
            if (!isNum || pick <= prev - 2 || pick >= prev + 2)   // 首位不判
                ans += f(num, i + 1, isLimit && pick == up, true, pick);
        }
        if (!isLimit && isNum) dp[i][prev] = ans;
        return ans;
    }
public:
    int countUpTo(int x) {
        if (x < 0) return 0;
        string s = to_string(x);
        dp.assign(11, vector<int>(10, -1));
        return f(s, 0, true, false, 0);
    }
    int inRange(int a, int b) { return countUpTo(b) - countUpTo(a - 1); }
};

// ================= 5. 数字 1 的个数（状态带计数） =================
class DigitOne {
    vector<vector<long long>> dp;
    string num;
    long long f(int i, int cnt, bool isLimit) {
        if (i == (int)num.size()) return cnt;
        if (!isLimit && dp[i][cnt] != -1) return dp[i][cnt];
        long long ans = 0;
        int up = isLimit ? num[i] - '0' : 9;
        for (int pick = 0; pick <= up; pick++)
            ans += f(i + 1, cnt + (pick == 1), isLimit && pick == up);
        if (!isLimit) dp[i][cnt] = ans;
        return ans;
    }
public:
    long long countDigitOne(long long n) {
        num = to_string(n);
        dp.assign(num.size(), vector<long long>(num.size() + 1, -1));
        return f(0, 0, true);
    }
};

int main() {
    NoConsecutiveOnes a;
    printf("不含连续1(5) = %d\n", a.findIntegers(5));          // 5
    CountOfIntegers b;
    printf("数位和[1,8]内的数(1..20) = %d\n", b.count("1", "20", 1, 8));  // 18
    SpecialNumbers c;
    printf("无重复数字(20) = %d\n", c.countSpecialNumbers(20));  // 19
    WindyNumbers d;
    printf("windy数[1,10] = %d\n", d.inRange(1, 10));            // 9
    DigitOne e;
    printf("数字1的个数(13) = %lld\n", e.countDigitOne(13));     // 6
    return 0;
}
