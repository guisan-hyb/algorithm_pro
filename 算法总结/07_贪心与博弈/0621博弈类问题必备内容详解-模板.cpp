// 博弈模板：巴什 / 尼姆 / 反尼姆 / SG 打表 / lowZero / 分裂游戏
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

// ================= 经典结论 =================
bool bashWin(long long n, long long m) { return n % (m + 1) != 0; }

bool nimWin(const vector<int>& a) {
    int eor = 0;
    for (int x : a) eor ^= x;
    return eor != 0;
}

bool antiNimWin(const vector<int>& a) {                 // 拿最后一个输
    int eor = 0, sum = 0;
    for (int x : a) { eor ^= x; sum += x; }
    if (sum == (int)a.size()) return (sum & 1) == 0;    // 全是 1：偶数个 → 先手赢
    return eor != 0;
}

bool primePowerBashWin(long long n) { return n % 6 != 0; }   // 可拿 p^k 颗

// ================= SG 函数打表 =================
// moves：每次可取的步数集合
vector<int> buildSG(int n, const vector<int>& moves) {
    vector<int> sg(n + 1, 0);
    vector<char> appear(n + 2, 0);
    for (int i = 1; i <= n; i++) {
        for (int m : moves) if (i - m >= 0) appear[sg[i - m]] = 1;
        int s = 0;
        while (appear[s]) s++;
        sg[i] = s;
        for (int m : moves) if (i - m >= 0) appear[sg[i - m]] = 0;
    }
    return sg;
}

// ================= lowZero：最低的 0 在第几位 =================
int lowZero(unsigned status) {
    int cnt = 0;
    while (status & 1) { status >>= 1; cnt++; }
    return cnt;
}

// ================= E&D 游戏（洛谷 P2148） =================
bool eAndDWin(const vector<pair<int,int>>& groups) {
    int sg = 0;
    for (auto& g : groups)
        sg ^= lowZero((unsigned)((g.first - 1) | (g.second - 1)));
    return sg != 0;
}

// ================= 分裂游戏（洛谷 P3185） =================
// 每颗糖豆是一个独立子游戏；一次操作把一颗分裂成两颗 → 两个子游戏 SG 异或
vector<int> buildSplitSG(int n) {
    vector<int> sg(n, 0);
    vector<char> appear(300, 0);
    for (int i = 1; i < n; i++) {
        fill(appear.begin(), appear.end(), 0);
        for (int j = i - 1; j >= 0; j--)
            for (int k = j; k >= 0; k--)
                appear[sg[j] ^ sg[k]] = 1;
        int s = 0;
        while (appear[s]) s++;
        sg[i] = s;
    }
    return sg;
}

// 三堆石头、每次取斐波那契数（洛谷 P6487 型）
bool fibNimWin(int a, int b, int c) {
    static const vector<int> fib = {1,2,3,5,8,13,21,34,55,89,144};
    int n = max(a, max(b, c));
    vector<int> sg = buildSG(n, fib);
    return (sg[a] ^ sg[b] ^ sg[c]) != 0;
}

int main() {
    std::cout << "巴什(10,3) = " << (int)bashWin(10, 3) << "\n";            // 1
    std::cout << "尼姆(1,2,3) = " << (int)nimWin({1,2,3}) << "\n";           // 0
    std::cout << "反尼姆(1,1) = " << (int)antiNimWin({1,1}) << "\n";         // 1
    std::cout << "质数次方版(12) = " << (int)primePowerBashWin(12) << "\n";  // 0
    auto sg = buildSG(10, {1, 2, 3});
    std::cout << "sg[10] = " << sg[10] << "\n";
    std::cout << "lowZero(0b1011) = " << lowZero(0b1011) << "\n";            // 2
    std::cout << "E&D = " << (int)eAndDWin({{1,1},{2,3}}) << "\n";
    std::cout << "斐波那契尼姆(1,2,3) = " << (int)fibNimWin(1,2,3) << "\n";
    return 0;
}
