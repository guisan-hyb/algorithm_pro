// 三傻排序模板：选择 / 冒泡 / 插入
// 用途：小数据量排序、对数器暴力解、手写热身
// 复杂度：时间 O(n^2)，额外空间 O(1)
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// ---------- 选择排序：不稳定 ----------
void selectionSort(vector<int>& values) {
    int length = static_cast<int>(values.size());
    for (int start = 0; start < length - 1; start++) {
        int minimumIndex = start;
        for (int index = start + 1; index < length; index++) {
            if (values[index] < values[minimumIndex]) {
                minimumIndex = index;
            }
        }
        swap(values[start], values[minimumIndex]);
    }
}

// ---------- 冒泡排序：稳定 ----------
void bubbleSort(vector<int>& values) {
    int length = static_cast<int>(values.size());
    for (int end = length - 1; end > 0; end--) {
        bool isSorted = true;
        for (int index = 0; index < end; index++) {
            if (values[index] > values[index + 1]) {
                swap(values[index], values[index + 1]);
                isSorted = false;
            }
        }
        if (isSorted) {
            break;
        }
    }
}

// ---------- 插入排序：稳定，近乎有序时接近 O(n) ----------
void insertionSort(vector<int>& values) {
    int length = static_cast<int>(values.size());
    for (int index = 1; index < length; index++) {
        for (int position = index;
             position > 0 && values[position - 1] > values[position];
             position--) {
            swap(values[position - 1], values[position]);
        }
    }
}

// ---------- 自测 ----------
int main() {
    int length;
    if (!(cin >> length)) {
        return 0;
    }
    vector<int> selectionValues(length);
    for (int& value : selectionValues) {
        cin >> value;
    }
    vector<int> bubbleValues = selectionValues;
    vector<int> insertionValues = selectionValues;
    selectionSort(selectionValues);
    bubbleSort(bubbleValues);
    insertionSort(insertionValues);
    for (int index = 0; index < length; index++) {
        cout << selectionValues[index]
             << (index + 1 == length ? "\n" : " ");
    }
    return 0;
}
