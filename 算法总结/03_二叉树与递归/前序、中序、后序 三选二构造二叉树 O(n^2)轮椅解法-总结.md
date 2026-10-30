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

## 模板（可直接编译运行）

```cpp
// 三选二构造二叉树：每次复制左右子树区间，优先保证写法正确
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;

    explicit TreeNode(int value = 0, TreeNode* leftChild = nullptr,
                      TreeNode* rightChild = nullptr)
        : val(value), left(leftChild), right(rightChild) {}
};

TreeNode* buildFromPreorderInorder(const vector<int>& preorder,
                                   const vector<int>& inorder) {
    if (preorder.empty()) {
        return nullptr;
    }

    int rootValue = preorder.front();
    auto rootInorder = find(inorder.begin(), inorder.end(), rootValue);
    int leftSize = static_cast<int>(rootInorder - inorder.begin());

    vector<int> leftPreorder(preorder.begin() + 1,
                             preorder.begin() + 1 + leftSize);
    vector<int> rightPreorder(preorder.begin() + 1 + leftSize,
                              preorder.end());
    vector<int> leftInorder(inorder.begin(), inorder.begin() + leftSize);
    vector<int> rightInorder(inorder.begin() + leftSize + 1, inorder.end());

    return new TreeNode(
        rootValue,
        buildFromPreorderInorder(leftPreorder, leftInorder),
        buildFromPreorderInorder(rightPreorder, rightInorder));
}

TreeNode* buildFromInorderPostorder(const vector<int>& inorder,
                                    const vector<int>& postorder) {
    if (postorder.empty()) {
        return nullptr;
    }

    int rootValue = postorder.back();
    auto rootInorder = find(inorder.begin(), inorder.end(), rootValue);
    int leftSize = static_cast<int>(rootInorder - inorder.begin());

    vector<int> leftInorder(inorder.begin(), inorder.begin() + leftSize);
    vector<int> rightInorder(inorder.begin() + leftSize + 1, inorder.end());
    vector<int> leftPostorder(postorder.begin(), postorder.begin() + leftSize);
    vector<int> rightPostorder(postorder.begin() + leftSize,
                               postorder.end() - 1);

    return new TreeNode(
        rootValue,
        buildFromInorderPostorder(leftInorder, leftPostorder),
        buildFromInorderPostorder(rightInorder, rightPostorder));
}

TreeNode* buildFromPreorderPostorder(const vector<int>& preorder,
                                     const vector<int>& postorder) {
    if (preorder.empty()) {
        return nullptr;
    }
    if (preorder.size() == 1) {
        return new TreeNode(preorder.front());
    }

    auto leftRootInPostorder =
        find(postorder.begin(), postorder.end(), preorder[1]);
    int leftSize = static_cast<int>(leftRootInPostorder - postorder.begin()) + 1;

    vector<int> leftPreorder(preorder.begin() + 1,
                             preorder.begin() + 1 + leftSize);
    vector<int> rightPreorder(preorder.begin() + 1 + leftSize,
                              preorder.end());
    vector<int> leftPostorder(postorder.begin(), postorder.begin() + leftSize);
    vector<int> rightPostorder(postorder.begin() + leftSize,
                               postorder.end() - 1);

    return new TreeNode(
        preorder.front(),
        buildFromPreorderPostorder(leftPreorder, leftPostorder),
        buildFromPreorderPostorder(rightPreorder, rightPostorder));
}

void printPostorder(TreeNode* root) {
    if (root == nullptr) {
        return;
    }
    printPostorder(root->left);
    printPostorder(root->right);
    cout << root->val << " ";
}

int main() {
    TreeNode* root1 =
        buildFromPreorderInorder({3, 9, 20, 15, 7}, {9, 3, 15, 20, 7});
    printPostorder(root1);
    cout << "\n";

    TreeNode* root2 =
        buildFromInorderPostorder({9, 3, 15, 20, 7}, {9, 15, 7, 20, 3});
    printPostorder(root2);
    cout << "\n";

    TreeNode* root3 =
        buildFromPreorderPostorder({1, 2, 4, 5, 3, 6, 7},
                                   {4, 5, 2, 6, 7, 3, 1});
    printPostorder(root3);
    cout << "\n";
    return 0;
}
```

## 笔记里的题目

LeetCode 105 前序与中序构造二叉树、106 中序与后序构造二叉树、889 前序与后序构造二叉树。

## 延伸

- O(n) 的哈希表 + 下标版本见 `1210二叉树高频题目-不含树型dp`（题目七）；
- 遍历序列的生成见 `1124二叉树遍历的非递归实现和复杂度分析`；
- 为什么中序序列化不唯一、为什么三种组合只有两种唯一，本质是"根的位置能否把区间切开"。

## 专题深化：为什么成立

前序的第一个元素、后序的最后一个元素确定根；中序中根的位置把左右子树分开。用区间递归即可证明唯一划分。前序+后序通常不能唯一确定二叉树，因为无法区分只有一个孩子时它在左侧还是右侧。

## 专题深化：手算与边界

前序 `[3,9,20,15,7]`、中序 `[9,3,15,20,7]`：根 3，左中序 `[9]`，右 `[15,20,7]`，递归得到 `9` 与 `20(15,7)`。

### 对照与取舍

O(n²) 每次扫描中序找根；哈希表存中序下标降 O(n)。前序+后序可构造但不唯一，需要额外约束。

### 易错点与反例

1. 空区间返回空节点
2. 左右子树长度必须一致
3. 重复值时需明确匹配规则
4. 前序+后序不能唯一确定左右单孩子

## 专题深化：进阶拓展

用迭代+栈也可从遍历序列构造；表达式树可由前序/后序加运算符信息构造。

## 专题深化：面试追问

1. 为什么前序+中序唯一？
2. 前序+后序为什么会不唯一？
3. 哈希下标如何把复杂度降为 O(n)？

## 专题深化：自测（闭卷）

1. 手构三节点树
2. 找前序+后序不唯一例子
3. 校验区间长度
