// 堆结构 + 堆排序模板（大根堆）
// heapInsert / heapify 均 O(log n)；从底到顶建堆 O(n)；堆排序 O(n log n)，额外空间 O(1)
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// -------- 向上调整：i 位置新插入的值 --------
void heapInsert(vector<int>& heap, int currentIndex) {
    while (currentIndex > 0 &&
           heap[currentIndex] > heap[(currentIndex - 1) / 2]) {
        swap(heap[currentIndex], heap[(currentIndex - 1) / 2]);
        currentIndex = (currentIndex - 1) / 2;
    }
}

// -------- 向下调整：i 位置的值变小了，堆大小为 size --------
void heapify(vector<int>& heap, int currentIndex, int heapSize) {
    int leftChild = 2 * currentIndex + 1;
    while (leftChild < heapSize) {
        int rightChild = leftChild + 1;
        int largerChild = (rightChild < heapSize &&
                           heap[rightChild] > heap[leftChild])
                              ? rightChild
                              : leftChild;
        if (heap[largerChild] <= heap[currentIndex]) {
            break;
        }
        swap(heap[currentIndex], heap[largerChild]);
        currentIndex = largerChild;
        leftChild = 2 * currentIndex + 1;
    }
}

// -------- 堆排序 --------
void heapSort(vector<int>& heap) {
    int heapSize = static_cast<int>(heap.size());
    for (int index = heapSize / 2 - 1; index >= 0; index--) {
        heapify(heap, index, heapSize);
    }
    while (heapSize > 1) {
        swap(heap[0], heap[--heapSize]);
        heapify(heap, 0, heapSize);
    }
}

int main() {
    int length;
    if (!(cin >> length) || length <= 0) {
        return 0;
    }
    vector<int> heap(length);
    for (int& value : heap) {
        cin >> value;
    }
    heapSort(heap);
    for (int index = 0; index < length; index++) {
        cout << heap[index] << (index + 1 == length ? "\n" : " ");
    }
    return 0;
}
