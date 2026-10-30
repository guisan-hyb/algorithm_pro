// 异或技巧模板：交换 / 不用判断求最大值 / 缺失数 / 奇数次 / 按位统计
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

// ---------- 1. 交换（i==j 会清零，不推荐） ----------
void mySwap(int& a, int& b) { a ^= b; b ^= a; a ^= b; }

// ---------- 2. 不用判断求最大值（无溢出版） ----------
int flip(int n) { return n ^ 1; }
int sign(int n) { return flip((int)((unsigned)n >> 31)); }   // 非负 1，负 0
int getMax(int a, int b) {
    int c = a - b;
    int sa = sign(a), sb = sign(b), sc = sign(c);
    int diffAB = sa ^ sb, sameAB = flip(diffAB);
    int returnA = sameAB * sc + diffAB * sa;
    int returnB = flip(returnA);
    return a * returnA + b * returnB;
}

// ---------- 3. 缺失的数字（0~n 缺一个） ----------
int missingNumber(const vector<int>& a) {
    int eorAll = 0, eorHas = 0;
    for (int i = 0; i < (int)a.size(); i++) { eorAll ^= i; eorHas ^= a[i]; }
    eorAll ^= (int)a.size();
    return eorAll ^ eorHas;
}

// ---------- 4. 唯一出现奇数次的数 ----------
int singleNumber(const vector<int>& a) {
    int eor = 0;
    for (int x : a) eor ^= x;
    return eor;
}

// ---------- 5. 两种数出现奇数次 ----------
pair<int,int> singleNumber2(const vector<int>& a) {
    int eor1 = 0;
    for (int x : a) eor1 ^= x;
    int rightOne = (int)(eor1 & (-(long long)eor1));   // Brian Kernighan
    int eor2 = 0;
    for (int x : a) if ((x & rightOne) == 0) eor2 ^= x;
    return {eor2, eor1 ^ eor2};
}

// ---------- 6. 只有一种数出现次数 < m ----------
int findLessThanM(const vector<int>& a, int m) {
    vector<int> cnt(32, 0);
    for (int x : a)
        for (int j = 0; j < 32; j++)
            cnt[j] += (x >> j) & 1;                     // ★下标是 j
    int ans = 0;
    for (int j = 0; j < 32; j++)
        if (cnt[j] % m != 0) ans |= (1 << j);
    return ans;
}

int main() {
    std::cout << "getMax(3,-5) = " << getMax(3, -5) << "\n";                // 3
    std::cout << "缺失的数字 = " << missingNumber({0,1,2,3,5}) << "\n";      // 4
    pair<int,int> p = singleNumber2({1,2,1,3});
    std::cout << "两种奇数次 = " << p.first << " " << p.second << "\n";            // 2 3
    std::cout << "出现次数<m的数 = " << findLessThanM({5,5,5,3}, 3) << "\n";  // 3
    int x = 6, y = 7; mySwap(x, y);
    std::cout << "swap 后 = " << x << " " << y << "\n";
    return 0;
}
