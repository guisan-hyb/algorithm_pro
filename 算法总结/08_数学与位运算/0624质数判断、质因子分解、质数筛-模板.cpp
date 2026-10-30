// 质数模板：试除判质 / 埃氏筛 / 欧拉筛 / 质因子分解 / Miller-Rabin / 计数
#define _CRT_SECURE_NO_WARNINGS
#include <array>
#include <iostream>
#include <vector>
using namespace std;

// ---------- 1. 小范围判质数 O(√n) ----------
bool isPrime(long long n) {
    if (n <= 1) return false;
    for (long long i = 2; i * i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

// ---------- 2. 埃氏筛（从 i*i 开始标记） ----------
vector<int> primesUpTo(int n) {
    vector<char> composite(n + 1, 0);
    vector<int> primes;
    for (int i = 2; i <= n; i++)
        if (!composite[i]) {
            primes.push_back(i);
            if ((long long)i * i <= n)
                for (long long j = (long long)i * i; j <= n; j += i)
                    composite[(int)j] = 1;
        }
    return primes;
}

// ---------- 3. 欧拉筛（线性，每个合数只被最小质因子筛掉） ----------
vector<int> primesLinear(int n) {
    vector<char> composite(n + 1, 0);
    vector<int> primes;
    for (int i = 2; i <= n; i++) {
        if (!composite[i]) primes.push_back(i);
        for (int p : primes) {
            if ((long long)p * i > n) break;
            composite[p * i] = 1;
            if (i % p == 0) break;                  // ★
        }
    }
    return primes;
}

// ---------- 4. 质因子分解（不同的质因子） ----------
vector<long long> primeFactors(long long x) {
    vector<long long> fac;
    for (long long j = 2; j * j <= x; j++)
        if (x % j == 0) {
            fac.push_back(j);
            while (x % j == 0) x /= j;              // 除干净
        }
    if (x > 1) fac.push_back(x);
    return fac;
}

// ---------- 5. Miller-Rabin（10^9 量级） ----------
long long qpow(long long a, long long b, long long m) {
    long long ans = 1; a %= m;
    while (b) { if (b & 1) ans = ans * a % m; a = a * a % m; b >>= 1; }
    return ans;
}
bool witness(long long a, long long n) {
    long long u = n - 1; int t = 0;
    while (!(u & 1)) { t++; u >>= 1; }
    long long x1 = qpow(a, u, n), x2;
    for (int i = 1; i <= t; i++) {
        x2 = qpow(x1, 2, n);
        if (x2 == 1 && x1 != 1 && x1 != n - 1) return true;   // 判定是合数
        x1 = x2;
    }
    return x1 != 1;
}
bool millerRabin(long long n) {
    if (n <= 2) return n == 2;
    if (!(n & 1)) return false;
    static const std::array<long long, 12> bases = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
    };
    for (long long a : bases) {
        if (a >= n) break;
        if (witness(a, n)) return false;
    }
    return true;
}

// ---------- 6. 统计 < n 的质数个数（改进埃氏筛） ----------
int countPrimes(int n) {
    if (n <= 2) return 0;
    vector<char> composite(n + 1, 0);
    int cnt = n / 2;                                 // 先去掉偶数
    for (int i = 3; (long long)i * i <= n; i += 2)
        if (!composite[i])
            for (long long j = (long long)i * i; j <= n; j += 2 * i)
                if (!composite[(int)j]) { composite[(int)j] = 1; cnt--; }
    return cnt;
}

int main() {
    std::cout << "isPrime(97) = " << (int)isPrime(97) << "\n";                       // 1
    std::cout << "埃氏筛 30 以内个数 = " << (int)primesUpTo(30).size() << "\n";       // 10
    std::cout << "欧拉筛 30 以内个数 = " << (int)primesLinear(30).size() << "\n";     // 10
    std::cout << "360 的不同质因子: ";
    for (long long x : primeFactors(360)) std::cout << x << " ";              // 2 3 5
    std::cout << "\n";
    std::cout << "millerRabin(1000000007) = " << (int)millerRabin(1000000007) << "\n";// 1
    std::cout << "countPrimes(10) = " << countPrimes(10) << "\n";                     // 4
    return 0;
}
