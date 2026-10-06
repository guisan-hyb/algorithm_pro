// 快速幂 + 矩阵快速幂模板
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;
const long long MOD = 1000000007;

// ---------- 快速幂 ----------
long long quickPower(long long a, long long b, long long p) {
    long long ans = 1;
    a %= p;
    while (b > 0) {
        if (b & 1) ans = ans * a % p;
        a = a * a % p;
        b >>= 1;
    }
    return ans;
}

// ---------- 矩阵乘法与矩阵快速幂 ----------
using Mat = vector<vector<long long>>;
Mat multiply(const Mat& a, const Mat& b, long long mod = MOD) {
    int ma = (int)a.size(), na = (int)a[0].size(), nb = (int)b[0].size();
    Mat ret(ma, vector<long long>(nb, 0));
    for (int i = 0; i < ma; i++)
        for (int k = 0; k < na; k++) {
            if (!a[i][k]) continue;
            for (int j = 0; j < nb; j++)
                ret[i][j] = (ret[i][j] + a[i][k] * b[k][j]) % mod;
        }
    return ret;
}
Mat quickPowerMat(Mat m, long long p, long long mod = MOD) {
    int n = (int)m.size();
    Mat ans(n, vector<long long>(n, 0));
    for (int i = 0; i < n; i++) ans[i][i] = 1;            // 单位矩阵
    for (; p > 0; p >>= 1) {
        if (p & 1) ans = multiply(ans, m, mod);
        m = multiply(m, m, mod);
    }
    return ans;
}

// ---------- 一维 k 阶递推：斐波那契 ----------
long long fib(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    Mat start = {{1, 0}};                                 // {f(1), f(0)}
    Mat base = {{1, 1}, {1, 0}};                          // 关系矩阵，第 0 列是系数
    Mat ret = multiply(start, quickPowerMat(base, n - 1));
    return ret[0][0];
}

// ---------- 一维三阶递推：泰波那契 ----------
long long tribonacci(int n) {
    if (n == 0) return 0;
    if (n == 1 || n == 2) return 1;
    Mat start = {{1, 1, 0}};
    Mat base = {{1, 1, 0}, {1, 0, 1}, {1, 0, 0}};
    Mat ret = multiply(start, quickPowerMat(base, n - 2));
    return ret[0][0];
}

// ---------- 打表找规律得到的递推：f(n) = 2f(n-1) + f(n-3) ----------
long long numTilings(long long n) {
    auto g = [&](long long m) -> long long {              // g(m) = f(m+1)
        if (m == 0) return 1;
        if (m == 1) return 2;
        if (m == 2) return 5;
        Mat start = {{5, 2, 1}};
        Mat base = {{2, 1, 0}, {0, 0, 1}, {1, 0, 0}};
        return multiply(start, quickPowerMat(base, m - 2))[0][0];
    };
    return g(n - 1);
}

// ---------- k 维一阶递推：统计元音字母序列（5 个状态） ----------
int countVowelPermutation(int n) {
    Mat start = {{1, 1, 1, 1, 1}};
    Mat base = {
        {0, 1, 0, 0, 0},        // a 后面只能接 e
        {1, 0, 1, 0, 0},        // e 后面接 a 或 i
        {1, 1, 0, 1, 1},        // i 后面不能接 i
        {0, 0, 1, 0, 1},        // o 后面接 i 或 u
        {1, 0, 0, 0, 0}         // u 后面接 a
    };
    Mat ret = multiply(start, quickPowerMat(base, n - 1));
    long long ans = 0;
    for (long long x : ret[0]) ans = (ans + x) % MOD;
    return (int)ans;
}

// ---------- k 维一阶递推：学生出勤记录 II（6 个状态） ----------
int checkRecord(int n) {
    Mat start = {{1, 1, 0, 1, 0, 0}};
    Mat base = {
        {1, 1, 0, 1, 0, 0},
        {1, 0, 1, 1, 0, 0},
        {1, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 1, 0},
        {0, 0, 0, 1, 0, 1},
        {0, 0, 0, 1, 0, 0}
    };
    Mat ret = multiply(start, quickPowerMat(base, n - 1));
    long long ans = 0;
    for (long long x : ret[0]) ans = (ans + x) % MOD;
    return (int)ans;
}

int main() {
    printf("quickPower(3,10,1e9+7) = %lld\n", quickPower(3, 10, MOD));   // 59049
    printf("fib(10) = %lld\n", fib(10));                                 // 55
    printf("tribonacci(10) = %lld\n", tribonacci(10));                   // 149
    printf("numTilings(3) = %lld\n", numTilings(3));                     // 5
    printf("countVowelPermutation(2) = %d\n", countVowelPermutation(2)); // 10
    printf("checkRecord(2) = %d\n", checkRecord(2));                     // 8
    return 0;
}
