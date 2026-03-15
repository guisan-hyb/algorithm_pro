#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
//栈和队列相互实现
// 栈实现队列
// 测试链接 : https://leetcode.cn/problems/implement-queue-using-stacks/

class MyQueue {
private:
    stack<int> in;
    stack<int> out;

    void inToOut() {
        if (out.empty()) {
            while (!in.empty()) {
                int tp = in.top();
                out.push(tp);
                in.pop();
            }
        }
    }

public:
    MyQueue() {}

    void push(int x) {
        in.push(x);
        inToOut();
    }

    int pop() {
        inToOut();
        int tp = out.top();
        out.pop();
        return tp;
    }

    int peek() {
        inToOut();
        return out.top();
    }

    bool empty() { return in.empty() && out.empty(); }
};

/**
 * Your MyQueue object will be instantiated and called as such:
 * MyQueue* obj = new MyQueue();
 * obj->push(x);
 * int param_2 = obj->pop();
 * int param_3 = obj->peek();
 * bool param_4 = obj->empty();
 */

#include <stack>
stack<int> in;
stack<int> out;
//倒数字：
//从in栈中，把数据倒入out栈
//1.out栈空了才能倒数据
//2.如果倒数据，in栈必须倒完
void inToOut()
{
	if (out.empty())
	{
		while (!in.empty())
		{
			out.push(in.top());
			in.pop();
		}
	}
}

// 队列实现栈
// 测试链接 : https://leetcode.cn/problems/implement-stack-using-queues/

class MyStack {
    queue<int> que;

public:
    MyStack() {}

    void push(int x) {
        que.push(x);
        int n = que.size();
        for (int i = 0; i < n - 1; i++) {
            que.push(que.front());
            que.pop();
        }
    }

    int pop() {
        int tp = que.front();
        que.pop();
        return tp;
    }

    int top() { return que.front(); }

    bool empty() { return que.empty(); }
};

/**
 * Your MyStack object will be instantiated and called as such:
 * MyStack* obj = new MyStack();
 * obj->push(x);
 * int param_2 = obj->pop();
 * int param_3 = obj->top();
 * bool param_4 = obj->empty();
 */

#include <queue>
queue<int> q;
void myStack_push(int value)
{
	// 先将新元素入队
	q.push(value);

	// 将之前的元素全部移到新元素后面
	for (int i = 0; i < q.size() - 1; i++)
	{
		q.push(q.front());
		q.pop();
	}
}



//最小栈----求栈中最小元素
//创建一个栈存储每种情况下原先栈中的最小值
//1.当原先栈新存入的数据<=min,最小栈压入这个新数据
//2.当原先栈新存入的数据>min,最小栈依旧压入min
//3.当原先栈出栈时，最小栈也同步出栈
//4.当最小栈为空时，说明原先栈也为空
// 测试链接 : https://leetcode.cn/problems/min-stack/

class MinStack {
private:
    stack<int> data;
    stack<int> mins;

public:
    MinStack() {}

    void push(int val) {
        data.push(val);
        if(mins.empty()||val<mins.top())
        mins.push(val);
        else
        mins.push(mins.top());
    }

    void pop() {
        data.pop();
        mins.pop();
    }

    int top() { return data.top(); }

    int getMin() { return mins.top(); }
};

/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack* obj = new MinStack();
 * obj->push(val);
 * obj->pop();
 * int param_3 = obj->top();
 * int param_4 = obj->getMin();
 */


stack<int> init;
stack<int> min_;
int mins;//记录原先栈中最小值
void myPush(int x)
{
	init.push(x);
	if (min_.empty() || x <= min_.top())
		min_.push(x);
	else//!min_.empty()&&x>min_.top()
		min_.push(min_.top());
}

void myPop()
{
	init.pop();
	min_.pop();
}

int getMin()
{
	return min_.top();
}

