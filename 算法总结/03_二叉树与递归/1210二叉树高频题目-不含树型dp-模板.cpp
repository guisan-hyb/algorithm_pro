// 二叉树高频题模板：层序 / 锯齿 / 宽度 / 深度 / 完全二叉树 / 序列化 / 构造 / LCA / BST / 打家劫舍
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <climits>
#include <iostream>
#include <list>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int v = 0, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(v), left(l), right(r) {}
};

// ---------- 1. 层序遍历（按层切分，推荐写法） ----------
vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> ans;
    if (!root) return ans;
    queue<TreeNode*> q; q.push(root);
    while (!q.empty()) {
        int sz = q.size();
        vector<int> cur;
        while (sz--) {
            TreeNode* t = q.front(); q.pop();
            cur.push_back(t->val);
            if (t->left)  q.push(t->left);
            if (t->right) q.push(t->right);
        }
        ans.push_back(cur);
    }
    return ans;
}

// ---------- 2. 锯齿形层序（改写下标，不改入队顺序） ----------
vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
    vector<vector<int>> ans;
    if (!root) return ans;
    queue<TreeNode*> q; q.push(root);
    bool leftToRight = true;
    while (!q.empty()) {
        int sz = q.size();
        vector<int> cur(sz);
        for (int i = 0; i < sz; i++) {
            TreeNode* t = q.front(); q.pop();
            cur[leftToRight ? i : sz - 1 - i] = t->val;
            if (t->left)  q.push(t->left);
            if (t->right) q.push(t->right);
        }
        leftToRight = !leftToRight;
        ans.push_back(cur);
    }
    return ans;
}

// ---------- 3. 最大特殊宽度（完全二叉树编号） ----------
int widthOfBinaryTree(TreeNode* root) {
    if (!root) return 0;
    int ans = 0;
    queue<pair<TreeNode*, unsigned long long>> q;
    q.push({root, 1});
    while (!q.empty()) {
        unsigned long long l = q.front().second, r = q.back().second;
        ans = max(ans, (int)(r - l + 1));
        int sz = q.size();
        while (sz--) {
            auto pr = q.front(); q.pop();
            TreeNode* t = pr.first;
            unsigned long long code = pr.second;
            if (t->left)  q.push({t->left,  code * 2});
            if (t->right) q.push({t->right, code * 2 + 1});
        }
    }
    return ans;
}

// ---------- 4. 最大 / 最小深度 ----------
int maxDepth(TreeNode* root) {
    if (!root) return 0;
    return max(maxDepth(root->left), maxDepth(root->right)) + 1;
}
int minDepth(TreeNode* root) {
    if (!root) return 0;
    if (!root->left && !root->right) return 1;          // 叶子
    int ld = INT_MAX, rd = INT_MAX;
    if (root->left)  ld = minDepth(root->left);         // 防空孩子干扰
    if (root->right) rd = minDepth(root->right);
    return min(ld, rd) + 1;
}

// ---------- 5. 验证完全二叉树 ----------
bool isCompleteTree(TreeNode* root) {
    if (!root) return true;
    queue<TreeNode*> q; q.push(root);
    bool leafFlag = false;
    while (!q.empty()) {
        TreeNode* cur = q.front(); q.pop();
        if ((cur->right && !cur->left) ||
            (leafFlag && (cur->left || cur->right))) return false;
        if (cur->left)  q.push(cur->left);
        if (cur->right) q.push(cur->right);
        if (!cur->left || !cur->right) leafFlag = true;
    }
    return true;
}

// ---------- 6. 完全二叉树节点个数（O(log^2 n)） ----------
int leftDepthOf(TreeNode* node) {
    int d = 0;
    while (node) { d++; node = node->left; }
    return d;
}
int countNodes(TreeNode* root) {
    if (!root) return 0;
    int ld = leftDepthOf(root->left), rd = leftDepthOf(root->right);
    if (ld == rd) return (1 << ld) + countNodes(root->right);   // 左子树满
    return (1 << rd) + countNodes(root->left);                  // 右子树满（矮一层）
}

// ---------- 7. 前序序列化 / 反序列化 ----------
class Codec {
    void ser(TreeNode* root, string& s) {
        if (!root) { s += "null,"; return; }
        s += to_string(root->val) + ",";
        ser(root->left, s);
        ser(root->right, s);
    }
    TreeNode* des(list<string>& lst) {
        if (lst.front() == "null") { lst.pop_front(); return nullptr; }
        TreeNode* node = new TreeNode(stoi(lst.front()));
        lst.pop_front();
        node->left  = des(lst);
        node->right = des(lst);
        return node;
    }
public:
    string serialize(TreeNode* root) { string s; ser(root, s); return s; }
    TreeNode* deserialize(const string& data) {
        list<string> lst;
        string tmp;
        for (char c : data) {
            if (c == ',') { lst.push_back(tmp); tmp.clear(); }
            else tmp += c;
        }
        if (!tmp.empty()) lst.push_back(tmp);
        return lst.empty() ? nullptr : des(lst);
    }
};

// ---------- 8. 前序 + 中序构造二叉树 ----------
TreeNode* buildFrom(vector<int>& pre, int l1, int r1, vector<int>& in, int l2, int r2,
                    unordered_map<int,int>& pos) {
    if (l1 > r1) return nullptr;
    TreeNode* root = new TreeNode(pre[l1]);
    int k = pos[pre[l1]];
    int leftSize = k - l2;
    root->left  = buildFrom(pre, l1 + 1, l1 + leftSize, in, l2, k - 1, pos);
    root->right = buildFrom(pre, l1 + leftSize + 1, r1, in, k + 1, r2, pos);
    return root;
}
TreeNode* buildTree(vector<int>& pre, vector<int>& in) {
    unordered_map<int,int> pos;
    for (int i = 0; i < (int)in.size(); i++) pos[in[i]] = i;
    return buildFrom(pre, 0, (int)pre.size() - 1, in, 0, (int)in.size() - 1, pos);
}

// ---------- 9. LCA：普通二叉树 / 搜索二叉树 ----------
TreeNode* lcaBinaryTree(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (!root || root == p || root == q) return root;
    TreeNode* l = lcaBinaryTree(root->left, p, q);
    TreeNode* r = lcaBinaryTree(root->right, p, q);
    if (l && r) return root;                 // 左右各找到一个 -> 当前即 LCA
    return l ? l : r;
}
TreeNode* lcaBST(TreeNode* root, TreeNode* p, TreeNode* q) {
    int lo = min(p->val, q->val), hi = max(p->val, q->val);
    while (root) {
        if (root->val > hi)      root = root->left;
        else if (root->val < lo) root = root->right;
        else return root;
    }
    return nullptr;
}

// ---------- 10. 验证 BST（中序迭代） ----------
bool isValidBST(TreeNode* root) {
    stack<TreeNode*> st;
    TreeNode *cur = root, *prev = nullptr;
    while (cur || !st.empty()) {
        while (cur) { st.push(cur); cur = cur->left; }
        cur = st.top(); st.pop();
        if (prev && prev->val >= cur->val) return false;
        prev = cur;
        cur = cur->right;
    }
    return true;
}

// ---------- 11. 路径和等于 aim 的所有路径 ----------
vector<vector<int>> pathAns;
vector<int> pathBuf;
void pathDfs(TreeNode* cur, int preSum, int target) {
    if (!cur->left && !cur->right) {                       // 只在叶子判断
        if (preSum + cur->val == target) {
            pathBuf.push_back(cur->val);
            pathAns.push_back(pathBuf);
            pathBuf.pop_back();
        }
        return;
    }
    pathBuf.push_back(cur->val);
    if (cur->left)  pathDfs(cur->left,  preSum + cur->val, target);
    if (cur->right) pathDfs(cur->right, preSum + cur->val, target);
    pathBuf.pop_back();                                    // 恢复现场
}

// ---------- 12. 打家劫舍 III（树形 DP 入门） ----------
pair<int,int> robDp(TreeNode* root) {                      // {选, 不选}
    if (!root) return {0, 0};
    pair<int,int> L = robDp(root->left), R = robDp(root->right);
    int pick = root->val + L.second + R.second;
    int skip = max(L.first, L.second) + max(R.first, R.second);
    return {pick, skip};
}
int rob(TreeNode* root) { pair<int,int> r = robDp(root); return max(r.first, r.second); }

int main() {
    //        1
    //      /   \
    //     2     3
    //    / \
    //   4   5
    TreeNode* root = new TreeNode(1,
        new TreeNode(2, new TreeNode(4), new TreeNode(5)),
        new TreeNode(3));
    std::cout << "maxDepth=" << maxDepth(root) << " minDepth=" << minDepth(root) << " width=" << widthOfBinaryTree(root) << " complete=" << (int)isCompleteTree(root) << "\n";
    std::cout << "countNodes=" << countNodes(root) << " rob=" << rob(root) << "\n";
    Codec codec;
    string data = codec.serialize(root);
    std::cout << "serialize: " << data << "\n";
    return 0;
}
