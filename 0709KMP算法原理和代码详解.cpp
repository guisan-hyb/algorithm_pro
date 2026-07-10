#define _CRT_SECURE_NO_WARNINGS


#include <iostream>
#include <vector>
#include <string>

using namespace std;

//s1字符串是否包含s2字符串，如果包含，返回s1中包含s2的最左开头位置，不包含返回-1
//暴力方法就是s1中的每个位置都做开头，然后去匹配s2整体，时间复杂度 O(n*m)
//KMP算法可以做到时间复杂度 O(n+m)


//KMP算法详解：
// 
// 1) 理解next数组的定义，定义是一切的关键，前缀和后缀的最大匹配长度
//    next数组： 由s2生成
//    不含当前字符，前后缀最大匹配长度（不能整体）
//	  还代表：这个前缀的再下一个字符的所在
//
// 2) 假设已经有了next数组，详解匹配过程是如何得到加速的，加速过程有2个理解核心
//    
// 3) 理解了匹配主流程之后，详解next数组如何快速生成，不停跳跃的过程有1个理解核心
//
// 4) KMP算法代码详解，主流程 + next数组生成
//
// 5) 时间复杂度O(n)的证明，直接从代码层次就可以分析出来，分析方式好理解，但是比较特别


//很多题目中都有next数组的使用，具体请看下期，KMP算法相关题目


// KMP算法模版
// 测试链接 : https://leetcode.cn/problems/find-the-index-of-the-first-occurrence-in-a-string/
class Solution {
public:
    vector<int> getNext(string& s2) {
        int m = s2.size();
        if (m == 1) return { -1 };

        vector<int> next(m, 0);
        next[0] = -1;
        next[1] = 0;

        int i = 2;// i表示当前要求next值的位置
        int cn = 0;// cn表示当前要和前一个字符比对的下标
        while (i < m) {
            if (s2[i - 1] == s2[cn]) {
                next[i++] = ++cn;
            }
            else if (cn > 0) {
                cn = next[cn];
            }
            else {
                next[i++] = 0;
            }
        }
        return next;
    }

    int KMP(string& s1, string& s2) {
        int n = s1.size();
        int m = s2.size();
        int x = 0;//s1中当前比对的位置是x
        int y = 0;//s2中当前比对的位置是y
        vector<int> next = getNext(s2);

        //如果 x==n , 说明s1中不包含s2
        //如果 y==m ，说明s1中包含完整的s2
        while (x < n && y < m) {
            if (s1[x] == s2[y]) {
                x++;
                y++;
            }
            else if (y == 0) { //s2当前指向第一个字符
                x++;
            }
            else {
                y = next[y];
            }
        }
        return y == m ? x - y : -1;
    }

    int strStr(string haystack, string needle) {
        return KMP(haystack, needle);
    }
};



//练习：
// 另一棵树的子树
// 给你两棵二叉树root和subRoot
// 检验root中是否包含和subRoot具有相同结构和节点值的子树
// 如果存在，返回true
// 否则，返回false
// 测试链接 : https://leetcode.cn/problems/subtree-of-another-tree/


struct TreeNode {
     int val;
     TreeNode *left;
     TreeNode *right;
     TreeNode() : val(0), left(nullptr), right(nullptr) {}
     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 };

//法一：暴力递归
class Solution {
public:
    bool isSame(TreeNode* a, TreeNode* b) {
        if (a == nullptr && b == nullptr) return true;

        if (a != nullptr && b != nullptr) {
            return a->val == b->val && isSame(a->left, b->left) && isSame(a->right, b->right);
        }

        return false;
    }

    bool isSubtree(TreeNode* root, TreeNode* subRoot) {
        if (root != nullptr && subRoot != nullptr) {
            return isSame(root, subRoot) || isSubtree(root->left, subRoot) || isSubtree(root->right, subRoot);
        }
        return subRoot == nullptr;
    }
};

//法二：先序遍历转化为字符串+KMP
class Solution {
public:
    void f(TreeNode* node, string& s) { //先序遍历序列化 serial
        if (node == nullptr) {
            s += "_#_";
            return;
        }
        s += '_';
        s += to_string(node->val);
        s += '_';

        f(node->left, s);
        f(node->right, s);
    }

    bool isSubtree(TreeNode* root, TreeNode* subRoot) {
        string s1, s2;
        f(root, s1);
        f(subRoot, s2);

        return KMP(s1, s2) == -1 ? false : true;
    }

    int KMP(string& s1, string& s2) {
        int n = s1.size();
        int m = s2.size();
        vector<int> next = getNext(s2);
        int x = 0, y = 0;
        while (x < n && y < m) {
            if (s1[x] == s2[y]) {
                x++;
                y++;
            }
            else if (y == 0) {
                x++;
            }
            else {
                y = next[y];
            }
        }

        return y == m ? x - y : -1;
    }

    vector<int> getNext(string& s) {
        int m = s.size();
        if (m == 1) return { -1 };

        vector<int> next(m, 0);
        int i = 2;
        int cn = 0;
        next[0] = -1, next[1] = 0;

        while (i < m) {
            if (s[i - 1] == s[cn]) {
                next[i++] = ++cn;
            }
            else if (cn > 0) {
                cn = next[cn];
            }
            else {
                next[i++] = 0;
            }
        }
        return next;
    }
};

