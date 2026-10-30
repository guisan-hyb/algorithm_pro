// KMP 模板 + 例题（子树判断）
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ================= next 数组 =================
// next[i]：不含 s2[i] 时，s2[0..i-1] 的前后缀最大匹配长度（不能是整体）
vector<int> getNext(const string& s2) {
    int m = (int)s2.size();
    if (m == 1) return {-1};
    vector<int> nxt(m, 0);
    nxt[0] = -1;
    nxt[1] = 0;
    int i = 2, cn = 0;                          // cn：要和 s2[i-1] 比较的下标
    while (i < m) {
        if (s2[i - 1] == s2[cn])      nxt[i++] = ++cn;
        else if (cn > 0)              cn = nxt[cn];   // 跳跃
        else                          nxt[i++] = 0;
    }
    return nxt;
}

// ================= KMP 主流程 =================
int kmp(const string& s1, const string& s2) {   // s2 在 s1 中最左出现的下标，没有返回 -1
    int n = (int)s1.size(), m = (int)s2.size();
    if (m == 0) return 0;
    vector<int> nxt = getNext(s2);
    int x = 0, y = 0;
    while (x < n && y < m) {
        if (s1[x] == s2[y])  { x++; y++; }
        else if (y == 0)     { x++; }           // 模式串首字符就失配
        else                 { y = nxt[y]; }    // 模式串跳，主串不动
    }
    return y == m ? x - y : -1;
}

// ================= 例题：另一棵树的子树 =================
struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int v = 0, TreeNode* l = nullptr, TreeNode* r = nullptr) : val(v), left(l), right(r) {}
};
void serialize(TreeNode* node, string& s) {     // 先序序列化，空节点占位
    if (!node) { s += "_#_"; return; }
    s += '_';
    s += to_string(node->val);
    s += '_';
    serialize(node->left, s);
    serialize(node->right, s);
}
bool isSubtree(TreeNode* root, TreeNode* subRoot) {
    string s1, s2;
    serialize(root, s1);
    serialize(subRoot, s2);
    return kmp(s1, s2) != -1;
}

int main() {
    std::cout << "kmp(abcabcabc, abc) = " << kmp("abcabcabc", "abc") << "\n";   // 0
    std::cout << "kmp(hello, ll) = " << kmp("hello", "ll") << "\n";             // 2
    std::cout << "kmp(hello, xyz) = " << kmp("hello", "xyz") << "\n";           // -1
    auto nxt = getNext("aabaab");
    std::cout << "next(aabaab) = ";
    for (int v : nxt) std::cout << v << " ";                             // -1 0 1 0 1 2
    std::cout << "\n";
    TreeNode* root = new TreeNode(3, new TreeNode(4, new TreeNode(1), new TreeNode(2)), new TreeNode(5));
    TreeNode* sub = new TreeNode(4, new TreeNode(1), new TreeNode(2));
    std::cout << "isSubtree = " << (int)isSubtree(root, sub) << "\n";           // 1
    return 0;
}
