#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
// ACM 模式 IO 模板：多组数据、快读、按行读取、格式化输出
// 适用：CSP / 大厂笔试（自己写 main、自己解析输入、自己控制输出格式）
using namespace std;

// ---------- 1. 基本加速 ----------
void fastIO() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
}

// ---------- 2. 读入 n 个数 ----------
vector<long long> readNumbers(int count) {
    vector<long long> values(count);
    for (long long& value : values) {
        cin >> value;
    }
    return values;
}

// ---------- 3. 多组数据：先给组数 ----------
void solveWithTestCount() {
    int testCount = 0;
    cin >> testCount;
    while (testCount--) {
        int n = 0;
        cin >> n;
        vector<long long> values = readNumbers(n);
        long long sum = 0;
        for (long long value : values) {
            sum += value;
        }
        cout << sum << '\n';
    }
}

// ---------- 4. 多组数据：读到 EOF ----------
void solveUntilEOF() {
    int n = 0;
    while (cin >> n) {
        vector<long long> values = readNumbers(n);
        long long sum = 0;
        for (long long value : values) {
            sum += value;
        }
        cout << sum << '\n';
    }
}

// ---------- 5. 按行读取：先读掉行尾换行 ----------
void readLineExample() {
    string line;
    getline(cin, line);
    if (line.empty()) {
        getline(cin, line);
    }

    stringstream stream(line);
    string token;
    vector<string> tokens;
    while (stream >> token) {
        tokens.push_back(token);
    }
    cout << "token 数 = " << tokens.size() << '\n';
}

// ---------- 6. 格式化输出 ----------
void formatOutput() {
    cout << fixed << setprecision(2);
    cout << "保留两位小数 = " << 3.14159 << '\n';
    cout << "十六进制 = " << hex << showbase << uppercase << 255 << '\n';
    cout << dec << noshowbase << nouppercase;
}

// ---------- 7. 常见坑自检 ----------
// [ ] 多组数据：全局变量/容器是否在每组开始前清空
// [ ] cin >> 与 getline 混用时，是否处理行尾的 '\n'
// [ ] 输出是否多/少空格、换行（用 " \n"[i == n - 1] 技巧）
// [ ] 是否用 long long（1e9 * 1e9 会爆 int）
// [ ] 读入失败（cin 为 false）时是否退出
int main() {
    fastIO();
    solveWithTestCount();
    return 0;
}
