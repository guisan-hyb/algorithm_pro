// 位图模板：基础位图 + O(1) 整体翻转的 Bitset
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ================= 基础位图 =================
class Bitmap {
    vector<unsigned> set;
public:
    explicit Bitmap(int n) : set((n + 31) / 32, 0) {}   // 向上取整
    void add(int x)     { set[x >> 5] |= 1u << (x & 31); }
    void remove(int x)  { set[x >> 5] &= ~(1u << (x & 31)); }
    void flip(int x)    { set[x >> 5] ^= 1u << (x & 31); }
    bool contain(int x) { return (set[x >> 5] >> (x & 31)) & 1u; }
};

// ================= 进阶位集（懒翻转） =================
class Bitset {
    vector<unsigned> set;
    int size, ones, zeros;
    bool rev;
public:
    explicit Bitset(int n)
        : set((n + 31) / 32, 0), size(n), ones(0), zeros(n), rev(false) {}

    void fix(int i) {
        int idx = i >> 5, bit = i & 31;
        bool cur = (set[idx] >> bit) & 1u;
        if (rev) cur = !cur;
        if (!cur) { ones++; zeros--; set[idx] ^= 1u << bit; }
    }
    void unfix(int i) {
        int idx = i >> 5, bit = i & 31;
        bool cur = (set[idx] >> bit) & 1u;
        if (rev) cur = !cur;
        if (cur) { ones--; zeros++; set[idx] ^= 1u << bit; }
    }
    void flip() { rev = !rev; swap(ones, zeros); }
    bool all() const { return ones == size; }
    bool one() const { return ones > 0; }
    int count() const { return ones; }
    string toString() const {
        string s; s.reserve(size);
        for (int i = 0; i < size; i++) {
            int st = (set[i >> 5] >> (i & 31)) & 1u;
            s.push_back(char('0' + (rev ? !st : st)));
        }
        return s;
    }
};

int main() {
    Bitset bs(5);
    bs.fix(0); bs.fix(2);
    std::cout << bs.toString() << " count=" << bs.count() << "\n";   // 10100 count=2
    bs.flip();                                                    // 整体翻转，O(1)
    std::cout << bs.toString() << " count=" << bs.count() << " all=" << (int)bs.all() << "\n";
    return 0;
}
