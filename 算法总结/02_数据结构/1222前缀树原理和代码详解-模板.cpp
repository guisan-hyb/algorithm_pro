// 前缀树模板：静态数组实现（竞赛/笔试推荐）
// 支持 insert / search(出现次数) / prefixNumber(前缀数) / erase / clear
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Trie {
    static constexpr int MAX_NODE = 200005;
    vector<array<int, 26>> nextNode;     // nextNode[cur][path] = 子节点编号，0 表示没有路
    vector<int> pass;                    // 经过该节点的词数
    vector<int> endCount;                // 以该节点结尾的词数
    int nodeCount = 1;                   // 节点数量，根节点是 1（下标从 1 开始）

    Trie() : nextNode(MAX_NODE), pass(MAX_NODE), endCount(MAX_NODE) {
        clear();
    }

    void clear() {
        for (int i = 0; i <= nodeCount; i++) {
            nextNode[i].fill(0);
            pass[i] = 0;
            endCount[i] = 0;
        }
        nodeCount = 1;
    }

    void insert(const string& w) {
        int cur = 1;
        pass[cur]++;
        for (char c : w) {
            int p = c - 'a';
            if (!nextNode[cur][p]) nextNode[cur][p] = ++nodeCount;
            cur = nextNode[cur][p];
            pass[cur]++;
        }
        endCount[cur]++;
    }

    int search(const string& w) {            // 单词出现次数
        int cur = 1;
        for (char c : w) {
            int p = c - 'a';
            if (!nextNode[cur][p]) return 0;
            cur = nextNode[cur][p];
        }
        return endCount[cur];
    }

    int prefixNumber(const string& pre) {    // 以 pre 为前缀的单词数
        int cur = 1;
        for (char c : pre) {
            int p = c - 'a';
            if (!nextNode[cur][p]) return 0;
            cur = nextNode[cur][p];
        }
        return pass[cur];
    }

    void erase(const string& w) {            // 删除一次
        if (search(w) == 0) return;          // 不存在则什么也不做
        int cur = 1;
        pass[cur]--;
        for (char c : w) {
            int p = c - 'a';
            if (--pass[nextNode[cur][p]] == 0) {
                nextNode[cur][p] = 0;
                return;
            }
            cur = nextNode[cur][p];
        }
        endCount[cur]--;
    }
};

int main() {
    int m;
    if (!(std::cin >> m)) return 0;
    Trie trie;
    for (int i = 0, op; i < m; i++) {
        string word;
        std::cin >> op >> word;
        if (op == 1) {
            trie.insert(word);
        } else if (op == 2) {
            trie.erase(word);
        } else if (op == 3) {
            std::cout << (trie.search(word) ? "YES" : "NO") << "\n";
        } else {
            std::cout << trie.prefixNumber(word) << "\n";
        }
    }
    return 0;
}
