// 经典递归模板合集：子集 / 组合 / 全排列 / 栈递归 / 汉诺塔
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> ans;
vector<int> path;

// ================= 1. 全部子集（选 / 不选） =================
void subsets(vector<int>& a, int idx) {
    if (idx == (int)a.size()) { ans.push_back(path); return; }
    path.push_back(a[idx]);                 // 选
    subsets(a, idx + 1);
    path.pop_back();                        // 不选（恢复现场）
    subsets(a, idx + 1);
}

// ================= 2. 组合去重（排序 + 分组递归） =================
void combinations(vector<int>& a, int idx) {
    if (idx == (int)a.size()) { ans.push_back(path); return; }
    int next = idx;
    while (next + 1 < (int)a.size() && a[next] == a[next + 1]) next++;
    next++;
    combinations(a, next);                  // 本组取 0 个
    int back = idx;
    for (; idx < next; idx++) {
        path.push_back(a[idx]);             // 本组取若干个
        combinations(a, next);
    }
    for (; back < next; back++) path.pop_back();
}

// ================= 3. 全排列（无重复，交换法） =================
void permute(vector<int>& a, int idx) {
    if (idx == (int)a.size()) { ans.push_back(a); return; }
    for (int i = idx; i < (int)a.size(); i++) {
        swap(a[idx], a[i]);
        permute(a, idx + 1);
        swap(a[idx], a[i]);
    }
}

// ================= 4. 全排列（有重复，本层去重） =================
void permuteUnique(vector<int>& a, int idx) {
    if (idx == (int)a.size()) { ans.push_back(a); return; }
    unordered_set<int> used;                // 关键：每层新建
    for (int i = idx; i < (int)a.size(); i++) {
        if (used.count(a[i])) continue;
        used.insert(a[i]);
        swap(a[idx], a[i]);
        permuteUnique(a, idx + 1);
        swap(a[idx], a[i]);
    }
}

// ================= 5. 递归逆序一个栈（O(n^2) 时间，O(h) 栈空间） =================
int bottomOut(stack<int>& s) {              // 弹出栈底元素，其余保持原序
    int top = s.top(); s.pop();
    if (s.empty()) return top;
    int last = bottomOut(s);
    s.push(top);
    return last;
}
void reverseStack(stack<int>& s) {
    if (s.empty()) return;
    int bt = bottomOut(s);
    reverseStack(s);
    s.push(bt);
}

// ================= 6. 汉诺塔（2^n - 1 步） =================
void hanoi(int n, char from, char to, char other) {
    if (n == 1) { printf("盘 1: %c -> %c\n", from, to); return; }
    hanoi(n - 1, from, other, to);
    printf("盘 %d: %c -> %c\n", n, from, to);
    hanoi(n - 1, other, to, from);
}

int main() {
    vector<int> a = {1, 2, 2, 3};
    sort(a.begin(), a.end());
    ans.clear(); path.clear();
    combinations(a, 0);
    printf("组合(去重)共 %d 个\n", (int)ans.size());

    vector<int> b = {1, 1, 2};
    ans.clear();
    permuteUnique(b, 0);
    printf("排列(去重)共 %d 个\n", (int)ans.size());

    stack<int> s;
    for (int x : {1, 2, 3, 4}) s.push(x);
    reverseStack(s);
    printf("逆序后的栈顶 = %d\n", s.top());

    hanoi(3, 'A', 'C', 'B');
    return 0;
}
