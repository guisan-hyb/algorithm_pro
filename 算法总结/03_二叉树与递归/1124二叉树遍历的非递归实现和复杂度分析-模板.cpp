// 二叉树非递归遍历模板：前序 / 中序 / 后序
// 时间 O(n)，额外空间 O(h)（h 为树高）
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int v = 0, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(v), left(l), right(r) {}
};

// ---------- 前序：根 左 右（先压右、再压左） ----------
vector<int> preorder(TreeNode* root) {
    vector<int> res;
    if (!root) return res;
    stack<TreeNode*> st;
    st.push(root);
    while (!st.empty()) {
        TreeNode* cur = st.top(); st.pop();
        res.push_back(cur->val);
        if (cur->right) st.push(cur->right);
        if (cur->left)  st.push(cur->left);
    }
    return res;
}

// ---------- 中序：左 根 右 ----------
vector<int> inorder(TreeNode* root) {
    vector<int> res;
    stack<TreeNode*> st;
    TreeNode* cur = root;
    while (!st.empty() || cur) {
        if (cur) { st.push(cur); cur = cur->left; }
        else {
            cur = st.top(); st.pop();
            res.push_back(cur->val);
            cur = cur->right;
        }
    }
    return res;
}

// ---------- 后序：左 右 根（变体先序 + 反转） ----------
vector<int> postorder(TreeNode* root) {
    vector<int> res;
    if (!root) return res;
    stack<TreeNode*> st;
    st.push(root);
    while (!st.empty()) {
        TreeNode* cur = st.top(); st.pop();
        res.push_back(cur->val);          // 得到 根 右 左
        if (cur->left)  st.push(cur->left);
        if (cur->right) st.push(cur->right);
    }
    reverse(res.begin(), res.end());      // 反转为 左 右 根
    return res;
}

// ---------- 后序：单栈 + prev 指针写法（不修改结果数组） ----------
vector<int> postorderOneStack(TreeNode* root) {
    vector<int> res;
    if (!root) return res;
    stack<TreeNode*> st;
    st.push(root);
    TreeNode* prev = root;                 // 上一个被打印的节点
    while (!st.empty()) {
        TreeNode* cur = st.top();
        if (cur->left && cur->left != prev && cur->right != prev) {
            st.push(cur->left);
        } else if (cur->right && cur->right != prev) {
            st.push(cur->right);
        } else {
            res.push_back(cur->val);
            prev = cur;
            st.pop();
        }
    }
    return res;
}

// ---------- 简单自测：建一棵小树 ----------
int main() {
    //        1
    //      /   \
    //     2     3
    //    /
    //   4
    TreeNode* root = new TreeNode(1,
        new TreeNode(2, new TreeNode(4), nullptr),
        new TreeNode(3));
    auto print = [](const vector<int>& v) {
        for (int x : v) printf("%d ", x);
        puts("");
    };
    print(preorder(root));            // 1 2 4 3
    print(inorder(root));             // 4 2 1 3
    print(postorder(root));           // 4 2 3 1
    print(postorderOneStack(root));   // 4 2 3 1
    return 0;
}
