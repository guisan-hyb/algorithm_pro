// 二进制与位运算基础模板
#define _CRT_SECURE_NO_WARNINGS
#include <bitset>
#include <climits>
#include <iostream>
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
    std::cout << "第 4 位 = " << bitAt(x, 4) << "\n";                    // 1
    std::cout << "lowBit(12) = " << lowBit(12) << "\n";                // 4
    std::cout << "removeLowBit(12) = " << removeLowBit(12) << "\n";    // 8
    std::cout << "无符号右移 (-8) >> 2 = " << logicalShiftRight(-8, 2) << "\n";
    std::cout << "INT_MIN = " << INT_MIN << ", -INT_MIN = " << -INT_MIN << "\n";   // 取绝对值还是自己
    return 0;
}
