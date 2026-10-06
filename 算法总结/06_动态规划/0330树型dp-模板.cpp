// 树型 DP 模板：最大BST子树 / 直径 / 打家劫舍 / 监控 / 分配硬币 / dfn 序
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int v = 0, TreeNode* l = nullptr, TreeNode* r = nullptr) : val(v), left(l), right(r) {}
};

// ================= 1. 最大 BST 子树 =================
struct BstInfo { int maxSize; bool isBst; long long mx, mn; };
BstInfo largestBst(TreeNode* node) {
    if (!node) return {0, true, LLONG_MIN, LLONG_MAX};
    BstInfo L = largestBst(node->left), R = largestBst(node->right);
    long long mx = max((long long)node->val, max(L.mx, R.mx));
    long long mn = min((long long)node->val, min(L.mn, R.mn));
    bool isBst = L.isBst && R.isBst && L.mx < node->val && node->val < R.mn;
    int sz = isBst ? 1 + L.maxSize + R.maxSize : max(L.maxSize, R.maxSize);
    return {sz, isBst, mx, mn};
}
int largestBSTSubtree(TreeNode* root) { return largestBst(root).maxSize; }

// ================= 2. 二叉搜索子树的最大键值和（节点可为负） =================
struct SumBstInfo { bool isBst; int mx, mn, sum, maxBstSum; };
SumBstInfo maxSumBst(TreeNode* node) {
    if (!node) return {true, INT_MIN, INT_MAX, 0, 0};
    SumBstInfo L = maxSumBst(node->left), R = maxSumBst(node->right);
    bool isBst = L.isBst && R.isBst && L.mx < node->val && node->val < R.mn;
    int mx = max(node->val, max(L.mx, R.mx));
    int mn = min(node->val, min(L.mn, R.mn));
    int sum = L.sum + R.sum + node->val;
    int best = max(L.maxBstSum, R.maxBstSum);
    if (isBst) best = max(best, sum);
    return {isBst, mx, mn, sum, best};
}

// ================= 3. 二叉树直径 =================
pair<int,int> diameterDfs(TreeNode* node) {        // {diameter, height}
    if (!node) return {0, 0};
    pair<int,int> L = diameterDfs(node->left), R = diameterDfs(node->right);
    int h = max(L.second, R.second) + 1;
    int d = max(max(L.first, R.first), L.second + R.second);
    return {d, h};
}
int diameterOfBinaryTree(TreeNode* root) { return diameterDfs(root).first; }

// ================= 4. 打家劫舍 III / 没有上司的舞会 =================
pair<int,int> robDfs(TreeNode* node) {             // {选, 不选}
    if (!node) return {0, 0};
    pair<int,int> L = robDfs(node->left), R = robDfs(node->right);
    int yes = node->val + L.second + R.second;
    int no  = max(L.first, L.second) + max(R.first, R.second);
    return {yes, no};
}
int rob(TreeNode* root) { pair<int,int> r = robDfs(root); return max(r.first, r.second); }

// ================= 5. 在二叉树中分配硬币（贡献法） =================
struct CoinInfo { int cnt, sum, step; };
CoinInfo coinDfs(TreeNode* node) {
    if (!node) return {0, 0, 0};
    CoinInfo L = coinDfs(node->left), R = coinDfs(node->right);
    int cnt = L.cnt + R.cnt + 1;
    int sum = node->val + L.sum + R.sum;
    int step = L.step + R.step + abs(L.cnt - L.sum) + abs(R.cnt - R.sum);
    return {cnt, sum, step};
}
int distributeCoins(TreeNode* root) { return coinDfs(root).step; }

// ================= 6. 监控二叉树（三状态） =================
int cameraAns = 0;
int cameraDfs(TreeNode* node) {
    if (!node) return 1;                            // 0 无覆盖 / 1 已覆盖无摄像头 / 2 有摄像头
    int l = cameraDfs(node->left), r = cameraDfs(node->right);
    if (l == 0 || r == 0) { cameraAns++; return 2; }
    if (l == 1 && r == 1) return 0;
    return 1;
}
int minCameraCover(TreeNode* root) {
    cameraAns = 0;
    if (cameraDfs(root) == 0) cameraAns++;
    return cameraAns;
}

// ================= 7. dfn 序：子树变成连续区间 =================
vector<int> dfn, sz, deep;
int dfnCnt = 0;
void buildDfn(TreeNode* node, int h) {              // 假设 val 就是 1..n 的编号
    dfn[node->val] = ++dfnCnt;
    deep[dfn[node->val]] = h;
    sz[dfn[node->val]] = 1;
    if (node->left)  { buildDfn(node->left, h + 1);  sz[dfn[node->val]] += sz[dfn[node->left->val]]; }
    if (node->right) { buildDfn(node->right, h + 1); sz[dfn[node->val]] += sz[dfn[node->right->val]]; }
}
// 删除某节点子树后的树高：max(lmax[dfn-1], rmax[dfn+size])
vector<int> treeQueriesByDfn(TreeNode* root, const vector<int>& queries) {
    int n = 0;                                       // 节点编号最大值
    function<void(TreeNode*)> getMax = [&](TreeNode* p) { if (!p) return; n = max(n, p->val); getMax(p->left); getMax(p->right); };
    getMax(root);
    dfn.assign(n + 1, 0); sz.assign(n + 1, 0); deep.assign(n + 1, 0);
    dfnCnt = 0;
    buildDfn(root, 0);
    vector<int> lmax(dfnCnt + 2, 0), rmax(dfnCnt + 2, 0);
    for (int i = 1; i <= dfnCnt; i++) lmax[i] = max(lmax[i - 1], deep[i]);
    for (int i = dfnCnt; i >= 1; i--) rmax[i] = max(rmax[i + 1], deep[i]);
    vector<int> ans;
    for (int v : queries)
        ans.push_back(max(lmax[dfn[v] - 1], rmax[dfn[v] + sz[dfn[v]]]));
    return ans;
}

int main() {
    //        1
    //      /   \
    //     2     3
    //    / \
    //   4   5
    TreeNode* root = new TreeNode(1,
        new TreeNode(2, new TreeNode(4), new TreeNode(5)),
        new TreeNode(3));
    printf("最大BST子树 = %d\n", largestBSTSubtree(root));       // 1
    printf("直径 = %d\n", diameterOfBinaryTree(root));           // 3
    printf("打家劫舍 = %d\n", rob(root));                        // 7
    printf("分配硬币步数 = %d\n", distributeCoins(root));        // 0
    printf("最少摄像头 = %d\n", minCameraCover(root));           // 1
    return 0;
}
