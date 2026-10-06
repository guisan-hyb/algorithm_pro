// 一维差分模板：区间加常数 / 区间加等差数列 / 带 OFFSET 的负下标处理
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1000005;
long long diffArr[MAXN + 5];
long long arithArr[MAXN + 5];
int n;

// ---------- 1. 区间加常数：O(1) 修改 + O(n) 还原 ----------
void rangeAdd(int l, int r, long long v) {
    diffArr[l] += v;                       // 左端点开状态
    diffArr[r + 1] -= v;                   // 右端点后一位关状态
}
void buildConst() {
    for (int i = 1; i <= n; i++) diffArr[i] += diffArr[i - 1];
}

// ---------- 2. 区间加等差数列 [s, s+d, ..., e] ----------
void rangeAddArith(int l, int r, long long s, long long e, long long d) {
    arithArr[l]     += s;
    arithArr[l + 1] += d - s;
    arithArr[r + 1] -= d + e;
    arithArr[r + 2] += e;
}
void buildArith() {                        // 两遍前缀和
    for (int i = 1; i <= n; i++) arithArr[i] += arithArr[i - 1];
    for (int i = 1; i <= n; i++) arithArr[i] += arithArr[i - 1];
}

int main() {
    // ---- 示例 1：区间加常数 ----
    n = 5;
    rangeAdd(1, 3, 2);
    rangeAdd(2, 4, 1);
    buildConst();
    printf("区间加：");
    for (int i = 1; i <= n; i++) printf("%lld ", diffArr[i]);   // 2 3 3 1 0
    puts("");

    // ---- 示例 2：区间加等差数列 ----
    // 在 [1,5] 上依次加 1,3,5,7,9（首项 1、末项 9、公差 2）
    rangeAddArith(1, 5, 1, 9, 2);
    buildArith();
    printf("等差加：");
    for (int i = 1; i <= n; i++) printf("%lld ", arithArr[i]);  // 1 3 5 7 9
    puts("");

    // ---- 示例 3：洛谷 P4231 题型（每行输入 l r s e，公差自动算） ----
    // int m; scanf("%d %d", &n, &m);
    // memset(arithArr, 0, sizeof(arithArr));
    // while (m--) {
    //     long long l, r, s, e;
    //     scanf("%lld %lld %lld %lld", &l, &r, &s, &e);
    //     rangeAddArith((int)l, (int)r, s, e, (e - s) / (r - l));
    // }
    // buildArith();
    // 统计最大值与异或和
    return 0;
}
