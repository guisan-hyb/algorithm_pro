// 树状数组模板：单点加/区间查、区间加/单点查、区间加/区间查、二维
// 全部 O(log n) 单次操作；下标从 1 开始
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
using namespace std;

// ================= 1. 单点增加 + 范围查询 =================
template <class T>
struct BIT {
    int n;
    vector<T> tree;
    explicit BIT(int n_ = 0) { init(n_); }
    void init(int n_) { n = n_; tree.assign(n + 2, T{}); }
    static int lowbit(int x) { return x & -x; }
    void add(int idx, T v) {
        for (int i = idx; i <= n; i += lowbit(i)) tree[i] += v;
    }
    T query(int idx) const {
        T ans{};
        for (int i = idx; i > 0; i -= lowbit(i)) ans += tree[i];
        return ans;
    }
    T range(int l, int r) const { return query(r) - query(l - 1); }
};

// ================= 3. 范围增加 + 范围查询 =================
struct BITRange {
    BIT<long long> b1, b2;                    // b1: D[i]    b2: (i-1)*D[i]
    explicit BITRange(int n = 0) : b1(n), b2(n) {}
    void add(int l, int r, long long v) {
        b1.add(l, v);           b1.add(r + 1, -v);
        b2.add(l, v * (l - 1)); b2.add(r + 1, -v * r);
    }
    long long prefix(int x) { return b1.query(x) * x - b2.query(x); }
    long long range(int l, int r) { return prefix(r) - prefix(l - 1); }
};

// ================= 4. 二维单点增加 + 范围查询 =================
struct BIT2D {
    int n, m;
    vector<vector<long long>> tree;
    BIT2D(int n_, int m_)
        : n(n_), m(m_), tree(n_ + 2, vector<long long>(m_ + 2, 0)) {}
    static int lowbit(int x) { return x & -x; }
    void add(int x, int y, long long v) {
        for (int i = x; i <= n; i += lowbit(i))
            for (int j = y; j <= m; j += lowbit(j))
                tree[i][j] += v;
    }
    long long query(int x, int y) const {
        long long ans = 0;
        for (int i = x; i > 0; i -= lowbit(i))
            for (int j = y; j > 0; j -= lowbit(j))
                ans += tree[i][j];
        return ans;
    }
    long long sum(int a, int b, int c, int d) const {   // 矩形 [a..c] x [b..d]
        return query(c, d) - query(c, b - 1) - query(a - 1, d) + query(a - 1, b - 1);
    }
};

int main() {
    int n, m;
    if (!(std::cin >> n >> m)) return 0;

    BIT<long long> bit(n);
    for (int i = 1, x; i <= n; i++) { (std::cin >> x); bit.add(i, x); }

    while (m--) {
        int opt; (std::cin >> opt);
        if (opt == 1) {                 // 单点加
            int x, v; std::cin >> x >> v;
            bit.add(x, v);
        } else {                        // 区间查
            int l, r; std::cin >> l >> r;
            std::cout << bit.range(l, r) << "\n";
        }
    }
    return 0;
}
