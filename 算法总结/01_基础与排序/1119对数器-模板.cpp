// 对数器模板：随机样本 + 双解法比对
// 用法：把 solveA / solveB 换成你要验证的两个实现，先跑小数据量
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

static mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());

// ================= 待替换的三个部分 =================
// 方法 a：要验证的解法（贪心 / 规律 / 优化 DP ...）
long long solveA(vector<int> v) {
    sort(v.begin(), v.end());
    long long s = 0;
    for (int x : v) s += x;
    return s;
}

// 方法 b：暴力解，复杂度差但一定正确
long long solveB(vector<int> v) {
    long long s = 0;
    for (size_t i = 0; i < v.size(); i++)
        for (size_t j = 0; j <= i; j++)
            if (j == 0) s += v[i];          // 仅示范"思路不同"，实际请换成真暴力
    return s;
}

// 随机样本产生器
vector<int> randomSample(int maxN, int maxV) {
    uniform_int_distribution<int> dn(1, maxN), dv(0, maxV);
    int n = dn(gen);
    vector<int> v(n);
    for (int& x : v) x = dv(gen);
    return v;
}
// ===================================================

int main() {
    const int TEST = 100000;
    for (int t = 0; t < TEST; t++) {
        vector<int> a = randomSample(8, 20);      // 先小数据量：更容易暴露错误
        vector<int> b = a;                        // 两份相同样本
        long long ra = solveA(a), rb = solveB(b);
        if (ra != rb) {
            std::cout << "发现不一致！样本 n=" << (int)b.size() << "\n";
            for (int x : b) std::cout << x << " ";
            std::cout << "\na = " << ra << ", b = " << rb << "\n";
            return 0;                             // 打印错误样本，人工干预
        }
    }
    std::cout << "全部 " << TEST << " 组样本一致\n";
    return 0;
}
