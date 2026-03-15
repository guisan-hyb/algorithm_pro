#define _CRT_SECURE_NO_WARNINGS

// 设计循环双端队列
// 测试链接 : https://leetcode.cn/problems/design-circular-deque/

class MyCircularDeque {
private:
    int size, capacity;
    int l, r;
    vector<int> deq;

public:
    MyCircularDeque(int k) {
        size = 0;
        l = r = 0;
        capacity = k;
        deq.resize(k, 0);
    }

    bool insertFront(int value) {
        if (isFull())
            return false;
        if (isEmpty()) {
            l = r = 0;
        } else {
            l = (l - 1 + capacity) % capacity;
        }
        deq[l] = value;
        size++;
        return true;
    }

    bool insertLast(int value) {
        if (isFull())
            return false;
        if (isEmpty()) {
            l = r = 0;
        } else {
            r = (r + 1) % capacity;
        }
        deq[r] = value;
        size++;
        return true;
    }

    bool deleteFront() {
        if (isEmpty())
            return false;
        l = (l + 1) % capacity;
        size--;
        return true;
    }

    bool deleteLast() {
        if (isEmpty())
            return false;
        r = (r - 1 + capacity) % capacity;
        size--;
        return true;
    }

    int getFront() {
        if (isEmpty())
            return -1;
        return deq[l];
    }

    int getRear() {
        if (isEmpty())
            return -1;
        return deq[r];
    }

    bool isEmpty() { return size == 0; }

    bool isFull() { return size == capacity; }
};

/**
 * Your MyCircularDeque object will be instantiated and called as such:
 * MyCircularDeque* obj = new MyCircularDeque(k);
 * bool param_1 = obj->insertFront(value);
 * bool param_2 = obj->insertLast(value);
 * bool param_3 = obj->deleteFront();
 * bool param_4 = obj->deleteLast();
 * int param_5 = obj->getFront();
 * int param_6 = obj->getRear();
 * bool param_7 = obj->isEmpty();
 * bool param_8 = obj->isFull();
 */


#include <iostream>
using namespace std;
//数组实现双端队列
#define MAXSIZE 100
int* arr = new int[MAXSIZE];
int left, right;
int limit;//最大存储数量
int size = 0;//记录当前数组中元素个数

bool isEmpty()
{
	if (::size == 0)
		return true;
	return false;
}

bool isFull()
{
	if (::size == ::limit)
		return true;
	return false;
}

bool insertFront(int val)
{
	if (isFull())
		return false;
	else
	{
		if (isEmpty())
		{
			::left = ::right = 0;
			arr[::left] = val;
		}
		else
		{
			::left = ::left == 0 ? (limit - 1) : (::left - 1);
			arr[::left] = val;
		}
	}
	::size++;
	return true;
}

bool insertRear(int val)
{
	if (isFull())
		return false;
	else
	{
		if (isEmpty())
		{
			::left = ::right = 0;
			arr[::right] = val;
		}
		else
		{
			::right = ::right == (limit - 1) ? 0 : (::right + 1);
			arr[::right] = val;
		}
	}
	::size++;
	return true;
}

bool deleteFront()
{
	if (isEmpty())
		return false;
	else
	{
		::left = ::left == (::limit - 1) ? 0 : (::left + 1);
		::size--;
	}
	return true;
}

bool deleteRear()
{
	if (isEmpty())
		return false;
	else
	{
		::right = ::right == 0 ? (::limit - 1) : (::right - 1);
		::size--;
	}
	return true;
}

int getFront()
{
	if (isEmpty())
		return -1;
	return arr[::left];
}

int getRear()
{
	if (isEmpty())
		return -1;
	return arr[::right];
}
