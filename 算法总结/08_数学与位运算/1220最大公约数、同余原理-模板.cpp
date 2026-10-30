// 数论基础模板：gcd/lcm / 神奇数字 / 同余工具 / 逆元
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;

// ---------- gcd / lcm ----------
long long gcd_(long long a, long long b) { return b == 0 ? a : gcd_(b, a % b); }
long long lcm_(long long a, long long b) { return a / gcd_(a, b) * b; }     // 先除后乘

// ---------- 第 n 个能被 a 或 b 整除的数（二分 + 容斥） ----------
int nthMagicalNumber(long long n, long long a, long long b) {
    const long long MOD = 1000000007;
    long long L = lcm_(a, b);
    long long lo = 0, hi = n * min(a, b), ans = 0;
    while (lo <= hi) {
        long long m = lo + (hi - lo) / 2;
        if (m / a + m / b - m / L >= n) { ans = m; hi = m - 1; }
        else lo = m + 1;
    }
    return (int)(ans % MOD);
}

// ---------- 同余工具 ----------
long long addMod(long long a, long long b, long long m) { return ((a % m) + (b % m)) % m; }
long long subMod(long long a, long long b, long long m) { return ((a - b) % m + m) % m; }
long long mulMod(long long a, long long b, long long m) { return ((a % m) * (b % m)) % m; }

long long quickPowMod(long long a, long long b, long long m) {
    long long ans = 1;
    a %= m;
    while (b > 0) {
        if (b & 1) ans = ans * a % m;
        a = a * a % m;
        b >>= 1;
    }
    return ans;
}
// 费马小定理求逆元（p 必须是质数且 a % p != 0）
long long invModFermat(long long a, long long p) { return quickPowMod(a, p - 2, p); }

// 扩展欧几里得求逆元（模数不必是质数，但需 gcd(a, m) == 1）
long long invModExgcd(long long a, long long m) {
    long long old_r = a, r = m, old_s = 1, s = 0;
    while (r != 0) {
        long long q = old_r / r;
        long long tmp = old_r - q * r; old_r = r; r = tmp;
        tmp = old_s - q * s; old_s = s; s = tmp;
    }
    if (old_r != 1) return -1;                      // 逆元不存在
    return (old_s % m + m) % m;
}

int main() {
    std::cout << "gcd(12,18) = " << gcd_(12, 18) << "\n";                        // 6
    std::cout << "lcm(4,6) = " << lcm_(4, 6) << "\n";                            // 12
    std::cout << "第 3 个神奇数字(2,3) = " << nthMagicalNumber(3, 2, 3) << "\n";   // 6
    std::cout << "subMod(3,5,7) = " << subMod(3, 5, 7) << "\n";                  // 5
    std::cout << "invModFermat(2,1e9+7) = " << invModFermat(2, 1000000007) << "\n";  // 500000004
    std::cout << "invModExgcd(3,11) = " << invModExgcd(3, 11) << "\n";           // 4
    return 0;
}
