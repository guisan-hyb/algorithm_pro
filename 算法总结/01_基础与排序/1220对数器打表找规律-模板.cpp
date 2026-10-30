// 对数器打表找规律模板：暴力 -> 打表 -> 规律
// 适用于：入参/返回值都是简单类型、n 很大但有周期或分段的题
#define _CRT_SECURE_NO_WARNINGS
#include <iomanip>
#include <iostream>
using namespace std;

// ---------- 示例：吃草博弈（每次吃 4 的幂，吃完者赢，A 先手） ----------
// 第一步：暴力递归，只要求小范围正确
char brute(int rest, char cur) {
    char enemy = (cur == 'A') ? 'B' : 'A';
    if (rest < 5) {                        // 0 或 2 留给对手，当前选手必胜
        if (rest == 0 || rest == 2) return enemy;
        return cur;
    }
    for (long long pick = 1; pick <= rest; pick *= 4)
        if (brute(rest - (int)pick, enemy) == cur)
            return cur;                    // 存在一步让对手必败 -> 我必胜
    return enemy;
}

// 第二步：打表
void printTable(int up) {
    for (int n = 0; n <= up; n++)
        std::cout << std::setw(2) << n << " -> " << brute(n, 'A') << "\n";
}

// 第三步：把观察到的规律写成代码（周期 5：余 0 或 2 则 B 赢）
char byPattern(long long n) {
    return (n % 5 == 0 || n % 5 == 2) ? 'B' : 'A';
}

int main() {
    printTable(50);                        // 1) 看表找规律

    long long n;                           // 2) 锁定规律后用闭式提交
    if ((std::cin >> n))
        std::cout << byPattern(n) << "\n";
    return 0;
}
