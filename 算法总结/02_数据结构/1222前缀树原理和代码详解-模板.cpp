// 前缀树模板：静态数组实现（竞赛/笔试推荐）
// 支持 insert / search(出现次数) / prefixNumber(前缀数) / erase / clear
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

struct Trie {
    static const int MAXN = 200005;      // 按"总字符数 + 1"开
    int tree[MAXN][26];                  // tree[cur][path] = 子节点编号，0 表示没有路
    int pass[MAXN];                      // 经过该节点的词数
    int en[MAXN];                        // 以该节点结尾的词数
    int cnt;                             // 节点数量，根节点是 1（下标从 1 开始）

    Trie() { clear(); }

    void clear() {
        for (int i = 0; i <= cnt; i++) {
            memset(tree[i], 0, sizeof(tree[i]));
            pass[i] = en[i] = 0;
        }
        cnt = 1;
    }

    void insert(const string& w) {
        int cur = 1;
        pass[cur]++;
        for (char c : w) {
            int p = c - 'a';
            if (!tree[cur][p]) tree[cur][p] = ++cnt;   // 没有路就新建
            cur = tree[cur][p];
            pass[cur]++;
        }
        en[cur]++;
    }

    int search(const string& w) {            // 单词出现次数
        int cur = 1;
        for (char c : w) {
            int p = c - 'a';
            if (!tree[cur][p]) return 0;
            cur = tree[cur][p];
        }
        return en[cur];
    }

    int prefixNumber(const string& pre) {    // 以 pre 为前缀的单词数
        int cur = 1;
        for (char c : pre) {
            int p = c - 'a';
            if (!tree[cur][p]) return 0;
            cur = tree[cur][p];
        }
        return pass[cur];
    }

    void erase(const string& w) {            // 删除一次
        if (search(w) == 0) return;          // 不存在则什么也不做
        int cur = 1;
        pass[cur]--;
        for (char c : w) {
            int p = c - 'a';
            if (--pass[tree[cur][p]] == 0) { // 这条子树已经没词了
                tree[cur][p] = 0;            // 直接剪枝
                return;
            }
            cur = tree[cur][p];
        }
        en[cur]--;
    }
};

int main() {
    int m;
    if (scanf("%d", &m) != 1) return 0;
    Trie trie;
    char str[100005];
    for (int i = 0, op; i < m; i++) {
        scanf("%d %s", &op, str);
        string s(str);
        if (op == 1)      trie.insert(s);
        else if (op == 2) trie.erase(s);
        else if (op == 3) printf("%s\n", trie.search(s) ? "YES" : "NO");
        else              printf("%d\n", trie.prefixNumber(s));
    }
    return 0;
}
