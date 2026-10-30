// 数据结构设计高频题模板：LRU / RandomizedSet / 中位数 / 最大频率栈 / setAll
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

// ================= 1. LRU 缓存 =================
class LRUCache {
    int cap;
    list<pair<int,int>> lst;                    // 头 = 最久未使用，尾 = 最近使用
    unordered_map<int, list<pair<int,int>>::iterator> mp;
public:
    explicit LRUCache(int capacity) : cap(capacity) {}
    int get(int key) {
        auto it = mp.find(key);
        if (it == mp.end()) return -1;
        int v = it->second->second;
        lst.erase(it->second);
        lst.push_back({key, v});
        mp[key] = prev(lst.end());
        return v;
    }
    void put(int key, int value) {
        auto it = mp.find(key);
        if (it != mp.end()) {
            lst.erase(it->second);
        } else if ((int)lst.size() == cap) {
            mp.erase(lst.front().first);         // 淘汰最旧
            lst.pop_front();
        }
        lst.push_back({key, value});
        mp[key] = prev(lst.end());
    }
};

// ================= 2. O(1) 插入 / 删除 / 随机取 =================
class RandomizedSet {
    vector<int> arr;
    unordered_map<int,int> pos;
public:
    bool insert(int val) {
        if (pos.count(val)) return false;
        pos[val] = (int)arr.size();
        arr.push_back(val);
        return true;
    }
    bool remove(int val) {
        auto it = pos.find(val);
        if (it == pos.end()) return false;
        int p = it->second;
        int last = arr.back();
        arr[p] = last; pos[last] = p;            // 末尾元素补到被删位置
        arr.pop_back();
        pos.erase(val);
        return true;
    }
    int getRandom() { return arr[rand() % arr.size()]; }
};

// ================= 3. 数据流中位数（对顶堆） =================
class MedianFinder {
    priority_queue<int> big;
    priority_queue<int, vector<int>, greater<int>> small;
    void balance() {
        if ((int)big.size() - (int)small.size() == 2) { small.push(big.top()); big.pop(); }
        else if ((int)small.size() - (int)big.size() == 2) { big.push(small.top()); small.pop(); }
    }
public:
    void addNum(int num) {
        if (big.empty() || num <= big.top()) big.push(num);
        else small.push(num);
        balance();
    }
    double findMedian() {
        if (big.size() == small.size()) return (big.top() + small.top()) / 2.0;
        return big.size() > small.size() ? (double)big.top() : (double)small.top();
    }
};

// ================= 4. 最大频率栈 =================
class FreqStack {
    int maxFreq = 0;
    unordered_map<int,int> valToFre;
    unordered_map<int, vector<int>> freqToVals;
public:
    void push(int val) {
        int f = ++valToFre[val];
        freqToVals[f].push_back(val);
        maxFreq = max(maxFreq, f);
    }
    int pop() {
        int v = freqToVals[maxFreq].back();
        freqToVals[maxFreq].pop_back();
        if (--valToFre[v] == 0) valToFre.erase(v);
        if (freqToVals[maxFreq].empty()) { freqToVals.erase(maxFreq); --maxFreq; }
        return v;
    }
};

// ================= 5. 带 setAll 的哈希表 =================
class MyMap {
    unordered_map<int, pair<int,int>> mp;       // key -> (value, 写入时间)
    int setAllValue = 0, setAllTime = 0, cnt = 1;
public:
    void put(int key, int value) { mp[key] = {value, cnt++}; }
    bool containKey(int key) { return mp.count(key) > 0; }
    int get(int key) {
        if (!containKey(key)) return -1;
        if (mp[key].second < setAllTime) return setAllValue;   // 严格小于
        return mp[key].first;
    }
    void setAll(int value) { setAllValue = value; setAllTime = cnt++; }
};

int main() {
    LRUCache lru(2);
    lru.put(1, 1); lru.put(2, 2);
    std::cout << lru.get(1) << "\n";      // 1
    lru.put(3, 3);                   // 淘汰 key=2
    std::cout << lru.get(2) << "\n";      // -1

    MedianFinder mf;
    mf.addNum(1); mf.addNum(2); mf.addNum(3);
    std::cout << std::fixed << std::setprecision(1) << mf.findMedian() << std::defaultfloat << std::setprecision(6) << "\n";   // 2.0
    return 0;
}
