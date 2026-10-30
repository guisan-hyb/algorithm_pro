// 位运算骚操作模板：2/3 的幂 / 最近 2 的幂 / 区间按位与 / 翻转 / 数 1
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

// ---------- 1. 2 的幂 ----------
bool isPowerOfTwo(int n) { return n > 0 && n == (n & -n); }

// ---------- 2. 3 的幂 ----------
bool isPowerOfThree(int n) { return n > 0 && 1162261467 % n == 0; }   // 3^19

// ---------- 3. ≥ n 的最小 2 的幂 ----------
int near2Power(int n) {
    if (n <= 0) return 1;
    n--;                                        // ★防 n 本身是 2 的幂
    n |= (unsigned)n >> 1;
    n |= (unsigned)n >> 2;
    n |= (unsigned)n >> 4;
    n |= (unsigned)n >> 8;
    n |= (unsigned)n >> 16;
    return n + 1;
}

// ---------- 4. 区间 [left, right] 按位与 ----------
int rangeBitwiseAnd(int left, int right) {
    while (left < right) right -= right & -right;
    return right;
}

// ---------- 5. 翻转二进制（分治） ----------
unsigned reverseBits(unsigned n) {
    n = ((n & 0xaaaaaaaau) >> 1) | ((n & 0x55555555u) << 1);
    n = ((n & 0xccccccccu) >> 2) | ((n & 0x33333333u) << 2);
    n = ((n & 0xf0f0f0f0u) >> 4) | ((n & 0x0f0f0f0fu) << 4);
    n = ((n & 0xff00ff00u) >> 8) | ((n & 0x00ff00ffu) << 8);
    n = (n >> 16) | (n << 16);
    return n;
}

// ---------- 6. 统计 1 的个数（分治） + 汉明距离 ----------
int countOne(unsigned n) {
    n = (n & 0x55555555u) + ((n >> 1) & 0x55555555u);
    n = (n & 0x33333333u) + ((n >> 2) & 0x33333333u);
    n = (n & 0x0f0f0f0fu) + ((n >> 4) & 0x0f0f0f0fu);
    n = (n & 0x00ff00ffu) + ((n >> 8) & 0x00ff00ffu);
    n = (n & 0x0000ffffu) + ((n >> 16) & 0x0000ffffu);
    return (int)n;
}
int hammingDistance(int x, int y) { return countOne((unsigned)(x ^ y)); }

int main() {
    std::cout << "isPowerOfTwo(16) = " << (int)isPowerOfTwo(16) << "\n";      // 1
    std::cout << "isPowerOfThree(27) = " << (int)isPowerOfThree(27) << "\n";  // 1
    std::cout << "near2Power(17) = " << near2Power(17) << "\n";               // 32
    std::cout << "rangeBitwiseAnd(5,7) = " << rangeBitwiseAnd(5, 7) << "\n";  // 4
    std::cout << "reverseBits(1) = " << reverseBits(1) << "\n";               // 2147483648
    std::cout << "countOne(0b1011) = " << countOne(0b1011) << "\n";           // 3
    std::cout << "hammingDistance(1,4) = " << hammingDistance(1, 4) << "\n";  // 2
    return 0;
}
