## 前序、中序、后序 三选二构造二叉树 O(n^2)轮椅解法

### 前序和中序

```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        if(preorder.empty()) return nullptr;
        int lsize = find(inorder.begin(),inorder.end(),preorder[0])-inorder.begin();

        vector<int> pre_l(preorder.begin()+1,preorder.begin()+1+lsize);
        vector<int> pre_r(preorder.begin()+1+lsize,preorder.end());

        vector<int> in_l(inorder.begin(),inorder.begin()+lsize);
        vector<int> in_r(inorder.begin()+lsize+1,inorder.end());

        TreeNode* lchild = buildTree(pre_l,in_l);
        TreeNode* rchild = buildTree(pre_r,in_r);
        return new TreeNode(preorder[0],lchild,rchild);
    }
};
```



## 中序和后序

```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        if(postorder.empty()) return nullptr;
        int lsize = find(inorder.begin(),inorder.end(),postorder.back()) - inorder.begin();

        vector<int> post_l(postorder.begin(),postorder.begin()+lsize);
        vector<int> post_r(postorder.begin()+lsize,postorder.end()-1);

        vector<int> in_l(inorder.begin(),inorder.begin()+lsize);
        vector<int> in_r(inorder.begin()+lsize+1,inorder.end());

        TreeNode* lchild = buildTree(in_l,post_l);
        TreeNode* rchild = buildTree(in_r,post_r);
        return new TreeNode(postorder.back(),lchild,rchild);
    }
};
```



## 前序和后序

### 这种情况下二叉树不唯一

### 因此**规定**：无论什么情况，在前序遍历中，*preorder*[1] 都是**左子树**的根节点值

```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    TreeNode* constructFromPrePost(vector<int>& preorder, vector<int>& postorder) {
        if(preorder.empty()) return nullptr;
        if(preorder.size()==1) return new TreeNode(preorder[0]);

        int lsize = find(postorder.begin(),postorder.end(),preorder[1]) - postorder.begin()+1;

        vector<int> pre_l(preorder.begin()+1,preorder.begin()+lsize+1);
        vector<int> pre_r(preorder.begin()+lsize+1,preorder.end());

        vector<int> post_l(postorder.begin(),postorder.begin()+lsize);
        vector<int> post_r(postorder.begin()+lsize,postorder.end()-1);

        TreeNode* lchild = constructFromPrePost(pre_l,post_l);
        TreeNode* rchild = constructFromPrePost(pre_r,post_r);
        return new TreeNode(preorder[0],lchild,rchild);
    }
};
```

