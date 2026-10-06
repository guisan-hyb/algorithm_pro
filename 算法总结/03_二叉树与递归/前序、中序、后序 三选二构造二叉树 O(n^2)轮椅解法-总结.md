# 三选二构造二叉树（O(n²) 轮椅解法）

> 来源笔记：`前序、中序、后序 三选二构造二叉树 O(n^2)轮椅解法.md`｜性质：能不推下标就不推下标｜分类：03 二叉树与递归

## 一句话

"轮椅解法"= 每次递归**直接复制出左右子树的子数组**，靠 `find` 定位根；虽然额外 O(n²) 时间和空间，但**几乎不可能写错**，面试紧张时是保命方案。

## 三组组合的可行性

| 组合 | 是否唯一 | 说明 |
| --- | --- | --- |
| 前序 + 中序 | 唯一 | 前序第一个是根，中序用它把左右子树分开 |
| 中序 + 后序 | 唯一 | 后序最后一个是根，中序同上 |
| 前序 + 后序 | **不唯一** | 无法区分"只有左孩子"和"只有右孩子" |

前序 + 后序必须**人为规定**：无论什么情况，都认为 `preorder[1]` 是**左子树**的根（笔记原文的规定），这样答案才唯一。

## 核心思想

### 前序 + 中序

1. `preorder[0]` 是根；
2. `lsize = find(inorder, preorder[0]) - inorder.begin()` 是左子树大小；
3. 切出四个子数组：

```
pre_l = pre[1, 1+lsize)      pre_r = pre[1+lsize, end)
in_l  = in[0, lsize)         in_r  = in[lsize+1, end)
```

4. 递归构建左右孩子，返回 `new TreeNode(preorder[0], lchild, rchild)`。

### 中序 + 后序

对称：

```
lsize = find(inorder, postorder.back()) - inorder.begin();
post_l = post[0, lsize)              post_r = post[lsize, end-1)
in_l   = in[0, lsize)                in_r   = in[lsize+1, end)
根 = postorder.back()
```

注意 `post_r` 的右端是 `end - 1`（排除最后的根）。

### 前序 + 后序

```
lsize = find(postorder, preorder[1]) - postorder.begin() + 1;   // 左子树的长度
pre_l = pre[1, 1+lsize+1)     pre_r = pre[1+lsize+1, end)
post_l = post[0, lsize)       post_r = post[lsize, end-1)
根 = preorder[0]
```

外加递归终止：`if (preorder.size() == 1) return new TreeNode(preorder[0]);`（只有一个节点时无法再切分）。

## 复杂度

- 时间 O(n²)：每层都要 `find`（O(n)）且复制数组（O(n)）；
- 空间 O(n²)：递归过程中同时存多份子数组副本。
- 对比：`1210二叉树高频题目` 里的哈希表 + 下标版是 **O(n)**。

## 适用特征（看到这些信号就选它）

- 面试/笔试要求"由两种遍历序列构造二叉树"，且你不想推下标；
- 题目数据量小（n ≤ 100 级别），O(n²) 完全够；
- 作为"先写出正确版本"的第一步，之后再优化成 O(n)。

## 易错点

1. 左子树大小的 `find` 必须在**中序数组**里做（前序+中序、中序+后序）；前序+后序则是**在 postorder 里找 `preorder[1]`**；
2. 前序+后序的 `lsize` 要 `+1`（因为找到的是下标，长度要 +1）；
3. `in_r` / `post_r` 的左端都是 `lsize + 1` / `lsize`，别写成 `lsize` / `lsize+1` 搞混；
4. 前序+后序**没有中序**，所以结果本来不唯一，必须按题面规定（`preorder[1]` 归左子树）；
5. 前序+后序必须单独处理 `size() == 1` 的情况，否则会越界或无限递归；
6. 空数组要返回 `nullptr`，这是唯一的 base case 之一。

## 模板（默写骨架）

```cpp
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int v = 0, TreeNode* l = nullptr, TreeNode* r = nullptr) : val(v), left(l), right(r) {}
};

// ---------- 1. 前序 + 中序 ----------
TreeNode* buildFromPreIn(vector<int> pre, vector<int> in) {
    if (pre.empty()) return nullptr;
    int lsize = find(in.begin(), in.end(), pre[0]) - in.begin();

    vector<int> pre_l(pre.begin() + 1, pre.begin() + 1 + lsize);
    vector<int> pre_r(pre.begin() + 1 + lsize, pre.end());
    vector<int> in_l(in.begin(), in.begin() + lsize);
    vector<int> in_r(in.begin() + lsize + 1, in.end());

    TreeNode* l = buildFromPreIn(pre_l, in_l);
    TreeNode* r = buildFromPreIn(pre_r, in_r);
    return new TreeNode(pre[0], l, r);
}

// ---------- 2. 中序 + 后序 ----------
TreeNode* buildFromInPost(vector<int> in, vector<int> post) {
    if (post.empty()) return nullptr;
    int lsize = find(in.begin(), in.end(), post.back()) - in.begin();

    vector<int> post_l(post.begin(), post.begin() + lsize);
    vector<int> post_r(post.begin() + lsize, post.end() - 1);
    vector<int> in_l(in.begin(), in.begin() + lsize);
    vector<int> in_r(in.begin() + lsize + 1, in.end());

    TreeNode* l = buildFromInPost(in_l, post_l);
    TreeNode* r = buildFromInPost(in_r, post_r);
    return new TreeNode(post.back(), l, r);
}

// ---------- 3. 前序 + 后序（规定 pre[1] 属于左子树） ----------
TreeNode* buildFromPrePost(vector<int> pre, vector<int> post) {
    if (pre.empty()) return nullptr;
    if (pre.size() == 1) return new TreeNode(pre[0]);

    int lsize = find(post.begin(), post.end(), pre[1]) - post.begin() + 1;

    vector<int> pre_l(pre.begin() + 1, pre.begin() + lsize + 1);
    vector<int> pre_r(pre.begin() + lsize + 1, pre.end());
    vector<int> post_l(post.begin(), post.begin() + lsize);
    vector<int> post_r(post.begin() + lsize, post.end() - 1);

    TreeNode* l = buildFromPrePost(pre_l, post_l);
    TreeNode* r = buildFromPrePost(pre_r, post_r);
    return new TreeNode(pre[0], l, r);
}
```

## 笔记里的题目

LeetCode 105 前序与中序构造二叉树、106 中序与后序构造二叉树、889 前序与后序构造二叉树。

## 延伸

- O(n) 的哈希表 + 下标版本见 `1210二叉树高频题目-不含树型dp`（题目七）；
- 遍历序列的生成见 `1124二叉树遍历的非递归实现和复杂度分析`；
- 为什么中序序列化不唯一、为什么三种组合只有两种唯一，本质是"根的位置能否把区间切开"。
