// 二进制与位运算基础模板
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

void printBinary(int num) { cout << bitset<sizeof(int) * 8>(num) << '\n'; }

int bitAt(long long num, int i) { return (int)((num >> i) & 1LL); }
long long setBit(long long x, int i)   { return x | (1LL << i); }
long long clearBit(long long x, int i) { return x & ~(1LL << i); }
long long flipBit(long long x, int i)  { return x ^ (1LL << i); }
long long lowBit(long long x)          { return x & (-x); }        // 提取最低位的 1
long long removeLowBit(long long x)    { return x & (x - 1); }     // 去掉最低位的 1
unsigned logicalShiftRight(int x, int k) { return (unsigned)x >> k; }

// 手写打印（注意用 != 0 而不是 == 1）
void printBinaryManual(int num) {
    for (int i = 31; i >= 0; i--) cout << ((num & (1 << i)) != 0 ? '1' : '0');
    cout << '\n';
}

int main() {
    int x = 0b0011010;
    printBinary(x);
    printBinaryManual(x << 1);
    printf("第 4 位 = %d\n", bitAt(x, 4));                    // 1
    printf("lowBit(12) = %lld\n", lowBit(12));                // 4
    printf("removeLowBit(12) = %lld\n", removeLowBit(12));    // 8
    printf("无符号右移 (-8) >> 2 = %u\n", logicalShiftRight(-8, 2));
    printf("INT_MIN = %d, -INT_MIN = %d\n", INT_MIN, -INT_MIN);   // 取绝对值还是自己
    return 0;
}
