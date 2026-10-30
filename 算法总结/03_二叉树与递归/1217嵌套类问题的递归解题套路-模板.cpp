// 嵌套类问题模板：递归下降（where 游标版）
// 三道经典题：字符串解码 / 表达式求值 / 分子式原子计数
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

// ================= 1. 字符串解码（LeetCode 394） =================
class DecodeString {
    int where = 0;
    string f(const string& s, int i) {
        int curTimes = 0;
        string path;
        while (i < (int)s.size() && s[i] != ']') {
            if (isdigit(s[i])) {
                curTimes = curTimes * 10 + (s[i++] - '0');
            } else if (s[i] == '[') {
                string tmp = f(s, i + 1);              // 递归处理嵌套
                for (int k = 0; k < curTimes; k++) path += tmp;
                i = where + 1;                         // ★跳过 ']'
                curTimes = 0;
            } else {
                path += s[i++];
            }
        }
        where = i;                                     // ★返回前更新
        return path;
    }
public:
    string decode(const string& s) { where = 0; return s.empty() ? s : f(s, 0); }
};

// ================= 2. 表达式求值（含括号、+ - * /） =================
class Calc {
    int where = 0;

    // 遇到 * 或 / 立即结算栈顶，否则直接入栈
    void push(vector<int>& nums, vector<char>& ops, int cur, char op) {
        if (!ops.empty()) {
            char top = ops.back();
            if (top == '*') { ops.pop_back(); int a = nums.back(); nums.pop_back();
                              nums.push_back(a * cur); ops.push_back(op); return; }
            if (top == '/') { ops.pop_back(); int a = nums.back(); nums.pop_back();
                              nums.push_back(a / cur); ops.push_back(op); return; }
        }
        nums.push_back(cur);
        ops.push_back(op);
    }
    int compute(vector<int>& nums, vector<char>& ops) {
        if (!ops.empty() && ops.back() == '+') ops.pop_back();   // 去掉末尾补的 '+'
        int ans = nums[0];
        for (size_t i = 1; i < nums.size(); i++)
            ans = (ops[i - 1] == '+') ? ans + nums[i] : ans - nums[i];
        return ans;
    }
    int f(const string& s, int i) {
        vector<int> nums; vector<char> ops;
        int cur = 0;
        while (i < (int)s.size() && s[i] != ')') {
            char c = s[i];
            if (isdigit(c)) {
                cur = cur * 10 + (c - '0'); i++;
            } else if (c == '(') {
                cur = f(s, i + 1);                     // 括号里的值 = 一个数
                i = where + 1;
            } else {
                push(nums, ops, cur, c); i++;
                cur = 0;
            }
        }
        push(nums, ops, cur, '+');                     // ★末尾补结算
        where = i;
        return compute(nums, ops);
    }
public:
    int calculate(const string& s) { where = 0; return f(s, 0); }
};

// ================= 3. 分子式原子计数（LeetCode 726） =================
class CountAtoms {
    int where = 0;
    void flush(map<string,int>& acc, string& path, map<string,int>& prev, int cnt) {
        cnt = (cnt == 0 ? 1 : cnt);
        if (!path.empty()) acc[path] += cnt;
        for (auto& e : prev) acc[e.first] += e.second * cnt;
    }
    map<string,int> f(const string& s, int i) {
        map<string,int> acc, prev;
        string path;
        int cnt = 0;
        while (i < (int)s.size() && s[i] != ')') {
            char c = s[i];
            if (isupper(c) || c == '(') {
                flush(acc, path, prev, cnt);           // 结算上一个单元
                path.clear(); prev.clear(); cnt = 0;
                if (isupper(c)) { path += c; i++; }
                else { prev = f(s, i + 1); i = where + 1; }
            } else if (islower(c)) {
                path += c; i++;
            } else {                                   // 数字
                cnt = cnt * 10 + (c - '0'); i++;
            }
        }
        flush(acc, path, prev, cnt);                   // ★末尾补结算
        where = i;
        return acc;
    }
public:
    string count(const string& formula) {
        where = 0;
        map<string,int> mp = f(formula, 0);
        string ans;
        for (auto& e : mp) {
            ans += e.first;
            if (e.second > 1) ans += to_string(e.second);
        }
        return ans;
    }
};

int main() {
    DecodeString ds;
    std::cout << ds.decode("3[a2[c]]") << "\n";     // accaccacc
    Calc c;
    std::cout << c.calculate("2*(3+4)-5") << "\n";          // 9
    CountAtoms ca;
    std::cout << ca.count("K4(ON(SO3)2)2") << "\n"; // K4N2O14S4
    return 0;
}
