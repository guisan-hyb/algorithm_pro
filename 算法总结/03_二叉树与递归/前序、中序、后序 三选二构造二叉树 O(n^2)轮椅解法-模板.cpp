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
