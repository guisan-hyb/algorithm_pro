// 栈与队列相互实现 + 最小栈：可直接使用的类模板
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
using namespace std;

// ============ 1. 两个栈实现队列 ============
class MyQueue {
    stack<int> in, out;
    void inToOut() {
        if (!out.empty()) return;      // 纪律一：out 空才能倒
        while (!in.empty()) {          // 纪律二：倒就倒完
            out.push(in.top());
            in.pop();
        }
    }
public:
    void push(int x) { in.push(x); }
    int pop() { inToOut(); int v = out.top(); out.pop(); return v; }
    int peek() { inToOut(); return out.top(); }
    bool empty() { return in.empty() && out.empty(); }
};

// ============ 2. 一个队列实现栈 ============
class MyStack {
    queue<int> q;
public:
    void push(int x) {
        q.push(x);
        for (int i = 0; i < (int)q.size() - 1; i++) {   // 新元素转到队首
            q.push(q.front());
            q.pop();
        }
    }
    int pop() { int v = q.front(); q.pop(); return v; }
    int top() { return q.front(); }
    bool empty() { return q.empty(); }
};

// ============ 3. 最小栈 ============
class MinStack {
    stack<int> data, mn;
public:
    void push(int x) {
        data.push(x);
        mn.push(mn.empty() ? x : min(x, mn.top()));    // x <= 当前最小值都要记录
    }
    void pop() { data.pop(); mn.pop(); }               // 必须同步
    int top() { return data.top(); }
    int getMin() { return mn.top(); }
};

int main() {
    MinStack ms;
    int n;
    if (!(std::cin >> n)) return 0;
    for (int i = 0, x; i < n; i++) {
        (std::cin >> x);
        ms.push(x);
        std::cout << "push " << x << " -> min = " << ms.getMin() << "\n";
    }
    return 0;
}
