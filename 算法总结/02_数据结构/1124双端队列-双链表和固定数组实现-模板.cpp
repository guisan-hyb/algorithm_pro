// 双端队列模板：固定数组（环形缓冲）实现
// 四种操作 O(1)；空间 O(limit)，不可扩容
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
using namespace std;

class MyDeque {
    vector<int> arr;
    int limit;
    int left, right, sz;
public:
    explicit MyDeque(int capacity)
        : arr(capacity), limit(capacity), left(0), right(0), sz(0) {}

    bool isEmpty() const { return sz == 0; }
    bool isFull()  const { return sz == limit; }

    bool insertFront(int val) {
        if (isFull()) return false;
        if (isEmpty()) left = right = 0;                       // 唯一的特判点
        else left = (left == 0) ? limit - 1 : left - 1;
        arr[left] = val;
        ++sz;
        return true;
    }
    bool insertRear(int val) {
        if (isFull()) return false;
        if (isEmpty()) left = right = 0;
        else right = (right == limit - 1) ? 0 : right + 1;
        arr[right] = val;
        ++sz;
        return true;
    }
    bool deleteFront() {
        if (isEmpty()) return false;
        left = (left == limit - 1) ? 0 : left + 1;
        --sz;
        return true;
    }
    bool deleteRear() {
        if (isEmpty()) return false;
        right = (right == 0) ? limit - 1 : right - 1;
        --sz;
        return true;
    }
    int getFront() const { return isEmpty() ? -1 : arr[left]; }
    int getRear()  const { return isEmpty() ? -1 : arr[right]; }
};

int main() {
    MyDeque dq(5);
    dq.insertRear(1); dq.insertRear(2); dq.insertFront(0);
    std::cout << "front=" << dq.getFront() << " rear=" << dq.getRear() << "\n";  // 0 2
    dq.deleteFront(); dq.deleteRear();
    std::cout << "front=" << dq.getFront() << " rear=" << dq.getRear() << "\n";  // 1 1
    return 0;
}
