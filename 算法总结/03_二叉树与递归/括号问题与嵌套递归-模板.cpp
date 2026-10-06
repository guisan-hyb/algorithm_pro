// 嵌套/括号问题通用模板：递归下降 V3（&i + if...break + 末尾 flush）
// 三题示例：表达式求值 / 字符串解码 / 分子式原子计数
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

// ============================================================
// 通用骨架（V3）
//
// Result f(string& s, int& i) {
//     Result acc;                    // 累积结果
//     LocalBuf buf;                  // 临时缓冲
//     int times = 0;                 // 倍数
//     while (i < (int)s.size()) {
//         char c = s[i];
//         if (c == TERM) { i++; break; }        // ★终止符自检自吃
//         if (c == BEGIN) {
//             flush(buf, acc, times);            // 先结算当前缓冲
//             buf.clear(); times = 0;
//             i++;                                // 跳过 BEGIN
//             Result sub = f(s, i);               // 递归
//             merge(acc, sub, times);             // 按语义合并
//             times = 0;
//             continue;
//         }
//         if (isdigit(c)) { times = times * 10 + (c - '0'); i++; continue; }
//         buf.append(c); i++;
//     }
//     flush(buf, acc, times);                    // ★末尾补结算
//     return acc;
// }
// ============================================================

// ---------------- 1. 表达式求值（+ - * / 与括号） ----------------
class Calculator {
    void push(vector<int>& nums, vector<char>& ops, int cur, char op) {
        if (!ops.empty()) {
            char top = ops.back();
            if (top == '*' || top == '/') {
                ops.pop_back();
                int a = nums.back(); nums.pop_back();
                nums.push_back(top == '*' ? a * cur : a / cur);
                ops.push_back(op);
                return;
            }
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
    int f(const string& s, int& i) {
        vector<int> nums; vector<char> ops;
        int cur = 0;
        while (i < (int)s.size()) {
            char c = s[i];
            if (c == ')') { i++; break; }                 // ★终止符
            if (isdigit(c)) { cur = cur * 10 + (c - '0'); i++; continue; }
            if (c == '(') { i++; cur = f(s, i); continue; } // ★嵌套
            push(nums, ops, cur, c); i++; cur = 0;
        }
        push(nums, ops, cur, '+');                        // ★末尾补结算
        return compute(nums, ops);
    }
public:
    int calculate(const string& s) { int i = 0; return f(s, i); }
};

// ---------------- 2. 字符串解码 3[a2[c]] ----------------
class Decode {
    string f(const string& s, int& i) {
        string path;
        int times = 0;
        while (i < (int)s.size()) {
            char c = s[i];
            if (c == ']') { i++; break; }                  // ★终止符
            if (isdigit(c)) { times = times * 10 + (c - '0'); i++; continue; }
            if (c == '[') {
                i++;                                       // 跳过 '['
                string sub = f(s, i);                      // 递归（i 已过 ']'）
                for (int k = 0; k < times; k++) path += sub;
                times = 0;
                continue;
            }
            path += c; i++;
        }
        return path;
    }
public:
    string decode(const string& s) { int i = 0; return f(s, i); }
};

// ---------------- 3. 分子式原子计数 K4(ON(SO3)2)2 ----------------
class Atoms {
    void flush(map<string,int>& acc, string& path, map<string,int>& sub, int times) {
        times = (times == 0 ? 1 : times);
        if (!path.empty()) acc[path] += times;
        for (auto& e : sub) acc[e.first] += e.second * times;
    }
    map<string,int> f(const string& s, int& i) {
        map<string,int> acc, sub;
        string path;
        int times = 0;
        while (i < (int)s.size()) {
            char c = s[i];
            if (c == ')') { i++; break; }                  // ★终止符
            if (isupper(c) || c == '(') {                  // ★遇大写字母或 '(' 就结算
                flush(acc, path, sub, times);
                path.clear(); sub.clear(); times = 0;
                if (isupper(c)) { path += c; i++; }
                else { i++; sub = f(s, i); }               // 括号里是一整块
                continue;
            }
            if (islower(c)) { path += c; i++; continue; }
            times = times * 10 + (c - '0'); i++;
        }
        flush(acc, path, sub, times);                      // ★末尾补结算
        return acc;
    }
public:
    string count(const string& formula) {
        int i = 0;
        map<string,int> mp = f(formula, i);
        string ans;
        for (auto& e : mp) {
            ans += e.first;
            if (e.second > 1) ans += to_string(e.second);
        }
        return ans;
    }
};

// ---------------- 4. 括号验证三件套（对照用） ----------------
bool isValidNoWildcard(const string& s) {          // 单种括号 O(1) 空间
    int cnt = 0;
    for (char c : s) {
        if (c == '(') cnt++;
        else { if (--cnt < 0) return false; }
    }
    return cnt == 0;
}

bool isValidWithStar(const string& s) {            // 含 '*' 的贪心区间
    int lo = 0, hi = 0;
    for (char c : s) {
        if (c == '(')      { lo++; hi++; }
        else if (c == ')') { lo--; hi--; }
        else               { lo--; hi++; }
        if (hi < 0) return false;
        if (lo < 0) lo = 0;
    }
    return lo == 0;
}

int main() {
    Calculator calc;
    printf("2*(3+4)-5 = %d\n", calc.calculate("2*(3+4)-5"));
    Decode dec;
    printf("3[a2[c]] = %s\n", dec.decode("3[a2[c]]").c_str());
    Atoms at;
    printf("K4(ON(SO3)2)2 = %s\n", at.count("K4(ON(SO3)2)2").c_str());
    printf("valid(*) = %d, valid(no wildcard) = %d\n",
           (int)isValidWithStar("(*)"), (int)isValidNoWildcard("(())"));
    return 0;
}
