// 前序、中序、后序的统一迭代模板：节点后压入访问标记
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stack>
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

enum class TraversalOrder {
    PREORDER,
    INORDER,
    POSTORDER,
};

vector<int> traverse(TreeNode* root, TraversalOrder order) {
    vector<int> result;
    if (root == nullptr) {
        return result;
    }

    stack<TreeNode*> nodes;
    stack<bool> visited;
    nodes.push(root);
    visited.push(false);

    while (!nodes.empty()) {
        TreeNode* current = nodes.top();
        bool hasVisited = visited.top();
        nodes.pop();
        visited.pop();

        if (hasVisited) {
            result.push_back(current->val);
            continue;
        }

        // 入栈顺序与访问顺序相反。
        if (order == TraversalOrder::PREORDER) {
            if (current->right != nullptr) {
                nodes.push(current->right);
                visited.push(false);
            }
            if (current->left != nullptr) {
                nodes.push(current->left);
                visited.push(false);
            }
            nodes.push(current);
            visited.push(true);
        } else if (order == TraversalOrder::INORDER) {
            if (current->right != nullptr) {
                nodes.push(current->right);
                visited.push(false);
            }
            nodes.push(current);
            visited.push(true);
            if (current->left != nullptr) {
                nodes.push(current->left);
                visited.push(false);
            }
        } else {
            nodes.push(current);
            visited.push(true);
            if (current->right != nullptr) {
                nodes.push(current->right);
                visited.push(false);
            }
            if (current->left != nullptr) {
                nodes.push(current->left);
                visited.push(false);
            }
        }
    }
    return result;
}

void printValues(const vector<int>& values) {
    for (int value : values) {
        cout << value << " ";
    }
    cout << "\n";
}

int main() {
    TreeNode* root = new TreeNode(
        1,
        new TreeNode(2, new TreeNode(4), new TreeNode(5)),
        new TreeNode(3));

    printValues(traverse(root, TraversalOrder::PREORDER));
    printValues(traverse(root, TraversalOrder::INORDER));
    printValues(traverse(root, TraversalOrder::POSTORDER));
    return 0;
}
