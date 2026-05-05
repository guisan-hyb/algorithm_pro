#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <climits>
#include <unordered_map>
#include <string>
#include <algorithm>
using namespace std;




//树形dp--上
//本节讲解最常见的树形dp问题，详解树形dp的解题套路

//树
//头节点没有父亲，其他节点只有一个父亲的有向无环图，直观理解为发散状
//在树上，从头节点出发到任何节点的路径是唯一的，不管二叉树还是多叉树都是如此

//树形dp在树上做动态规划，依赖关系比一般动态规划简单
//因为绝大部分多数都是父依赖子
//只是依赖关系简单，不代表题目简单

//树形dp套路：
//1. 分析父树得到答案需要子树的哪些信息
//2. 把子树信息的全集定义成递归返回值
//3. 通过递归让子树返回全集信息
//4. 整合子树的全集信息得到父树的全集信息并返回





// Definition for a binary tree node.
struct TreeNode {
     int val;
     TreeNode *left;
     TreeNode *right;
     TreeNode() : val(0), left(nullptr), right(nullptr) {}
     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 };
 


//题目一：
// 最大BST子树
// 给定一个二叉树，找到其中最大的二叉搜索树（BST）子树，并返回该子树的大小
// 其中，最大指的是子树节点数最多的
// 二叉搜索树（BST）中的所有节点都具备以下属性：
// 左子树的值小于其父（根）节点的值
// 右子树的值大于其父（根）节点的值
// 注意：子树必须包含其所有后代
// 测试链接 : https://leetcode.cn/problems/largest-bst-subtree/

//分析：
//     x
//   /   \
//  左    右
//
//定义：以x为根节点的 BST 最大的大小
//1.不包含x : max(左maxBSTsize, 右maxBSTsize)
//2.包含x : 左子树必须为BST, 右子树必须为BST，左子树最大值<x<右子树最小值
//         若满足上述条件，则大小为 左maxBSTsize + 右maxBSTsize + 1
//
//需要的子树信息：
//此题，左右树都要：
//1.maxBSTsize(int) 左/右子树上最大BST大小
//2.isBST(bool) 是否是BST
//3.max(int) 左/右子树上最大值是什么
//4.min(int) 左/右子树上最小值是什么

namespace test1
{
	class TreeNode
	{
	public:
		int val;
		TreeNode* lchild;
		TreeNode* rchild;
	};


	class Info
	{
	public:
		long max_;
		long min_;
		bool isBst_;
		int maxBstSize_;

		Info(long maxs, long mins, bool isBst, int maxBstSize)
			: max_(maxs), min_(mins), isBst_(isBst), maxBstSize_(maxBstSize) {}
	};

	Info f(TreeNode* node)
	{
		if (node == nullptr)
			return Info(LLONG_MIN, LLONG_MAX, true, 0);

		Info lchild = f(node->lchild);
		Info rchild = f(node->rchild);
		long maxs = max((long)node->val, max(lchild.max_, rchild.max_));
		long mins = min((long)node->val, min(lchild.min_, rchild.min_));

		bool isBst = lchild.isBst_ && rchild.isBst_ && lchild.max_ < node->val && node->val < rchild.min_;
		int maxBstSize = 0;
		if (isBst)
			maxBstSize = 1 + lchild.maxBstSize_ + rchild.maxBstSize_;
		else
			maxBstSize = max(lchild.maxBstSize_, rchild.maxBstSize_);
		return Info(maxs, mins, isBst, maxBstSize);
	}

	int largestBSTSubtree(TreeNode* root)
	{
		return f(root).maxBstSize_;
	}
}



//题目二；
// 二叉搜索子树的最大键值和
// 给你一棵以 root 为根的二叉树
// 请你返回 任意 二叉搜索子树的最大键值和
// 二叉搜索树的定义如下：
// 任意节点的左子树中的键值都 小于 此节点的键值
// 任意节点的右子树中的键值都 大于 此节点的键值
// 任意节点的左子树和右子树都是二叉搜索树
// 测试链接 : https://leetcode.cn/problems/maximum-sum-bst-in-binary-tree/

//定义：以x为根节点的BST最大累加和
//1.不包含x: max(左maxBSTsum, 右maxBSTsum)
//2.包含x : 左子树必须为BST, 右子树必须为BST，左子树最大值<x<右子树最小值
//         若满足上述条件，则大小为 左maxBSTsum + 右maxBSTsum + x_val

//注意：因为节点可能有负数，因此要设置两个值记录和的情况：
//1.sum_:记录当前子树所有节点的和
//2.maxBstSum:记录当前子树内部的最大BST值之和

class Solution 
{
public:
	class Info
	{
	public:
		bool isBst_;
		int max_;
		int min_;
		int sum_;
		int maxBstSum_;

		Info(bool isBst, int maxs, int mins, int sum, int maxBstSum)
			:isBst_(isBst), max_(maxs), min_(mins), sum_(sum), maxBstSum_(maxBstSum) {}
	};

	Info f(TreeNode* node)
	{
		if (node == nullptr)
			return Info(true, INT_MIN, INT_MAX, 0, 0);

		int x_val = node->val;
		Info lchild = f(node->left);
		Info rchild = f(node->right);
		bool judge = lchild.isBst_ && rchild.isBst_ && lchild.max_ < x_val && x_val < rchild.min_;
		int maxs = max(x_val, max(lchild.max_, rchild.max_));
		int mins = min(x_val, min(lchild.min_, rchild.min_));
		int maxBstSum = max(lchild.maxBstSum_, rchild.maxBstSum_);
		int sum = lchild.sum_ + rchild.sum_ + x_val;
		if (judge)
		{
			maxBstSum = max(sum, maxBstSum);
		}
		return Info(judge, maxs, mins, sum, maxBstSum);
	}

	int maxSumBST(TreeNode* root)
	{
		return f(root).maxBstSum_;
	}
};



//题目三：
// 二叉树的直径
// 给你一棵二叉树的根节点，返回该树的直径
// 二叉树的 直径 是指树中任意两个节点之间最长路径的长度
// 这条路径可能经过也可能不经过根节点 root
// 两节点之间路径的 长度 由它们之间边数表示
// 测试链接 : https://leetcode.cn/problems/diameter-of-binary-tree/
class Solution {
public:
	struct Info
	{
		int diameter, height;
		Info(int d, int h) : diameter(d), height(h) {}
	};


	Info f(TreeNode* node)
	{
		if (node == nullptr) return Info(0, 0);

		Info lchild = f(node->left);
		Info rchild = f(node->right);
		int height = max(lchild.height, rchild.height) + 1;
		int diameter = max(lchild.diameter, rchild.diameter);
		diameter = max(diameter, lchild.height + rchild.height);
		return Info(diameter, height);
	}

	int diameterOfBinaryTree(TreeNode* root) {
		return f(root).diameter;
	}
};



//题目四：
// 在二叉树中分配硬币
// 给你一个有 n 个结点的二叉树的根结点 root
// 其中树中每个结点 node 都对应有 node.val 枚硬币
// 整棵树上一共有 n 枚硬币
// 在一次移动中，我们可以选择两个相邻的结点，然后将一枚硬币从其中一个结点移动到另一个结点
// 移动可以是从父结点到子结点，或者从子结点移动到父结点
// 返回使每个结点上 只有 一枚硬币所需的 最少 移动次数
// 测试链接 : https://leetcode.cn/problems/distribute-coins-in-binary-tree/

//核心：贡献法：移动步数 = 节点数 - 币数的绝对值

class Solution {
public:
	struct Info {
		int cnt, sum, step;
		Info(int c, int s, int st) : cnt(c), sum(s), step(st) {}
	};

	Info f(TreeNode* node)
	{
		if (node == nullptr) return Info(0, 0, 0);

		Info lchild = f(node->left);
		Info rchild = f(node->right);
		int cnt = lchild.cnt + rchild.cnt + 1;
		int sum = node->val + lchild.sum + rchild.sum;
		int step = lchild.step + rchild.step + abs(lchild.cnt - lchild.sum) + abs(rchild.cnt - rchild.sum);
		return Info(cnt, sum, step);
	}

	int distributeCoins(TreeNode* root) {
		return f(root).step;
	}
};



//题目五：
// 没有上司的舞会
// 某大学有n个职员，编号为1...n
// 他们之间有从属关系，也就是说他们的关系就像一棵以校长为根的树
// 父结点就是子结点的直接上司
// 现在有个周年庆宴会，宴会每邀请来一个职员都会增加一定的快乐指数 
// 但是如果某个职员的直接上司来参加舞会了
// 那么这个职员就无论如何也不肯来参加舞会了
// 所以请你编程计算邀请哪些职员可以使快乐指数最大
// 返回最大的快乐指数。
// 测试链接 : https://www.luogu.com.cn/problem/P1352
// 本题和讲解037的题目7类似
// 链式链接 : https://leetcode.cn/problems/house-robber-iii/

//定义：以x为根节点获得的最大快乐值
//1. x来：x+子节点不来的情况下子树的快乐值
//2. x不来：max(子节点来的情况下子树的快乐值，子节点不来的情况下子树的快乐值)

class Solution
{
public:
	struct Info
	{
		int yes, no;
		Info(int y,int n) : yes(y),no(n) {}
	};

	Info f(TreeNode* node)
	{
		if (node == nullptr) return Info(0, 0);

		Info lchild = f(node->left);
		Info rchild = f(node->right);
		int yes = node->val + lchild.no + rchild.no;
		int no = max(lchild.no, lchild.yes) + max(rchild.no, rchild.yes);
		return Info(yes, no);
	}

	int rob(TreeNode* root) 
	{
		return max(f(root).yes, f(root).no);
	}
};

namespace t5
{
	#include <iostream>
	#include <vector>
	using namespace std;

	const int MAXN = 6001;
	vector<int> happy(MAXN, 0);//记录每个人快乐值
	vector<int> boss(MAXN, 1);//记录谁是根节点
	//开始时所有人都有可能作为根节点，在建图的过程中可以确认最终谁是根节点
	vector<int> yes(MAXN, 0);
	vector<int> no(MAXN, 0);

	//链式前向星
	int cnt = 1;
	vector<int> head(MAXN, 0);
	vector<int> nxt(MAXN, 0);
	vector<int> to(MAXN, 0);

	void build(int n)
	{
		for (int i = 1; i <= n; i++)
		{
			boss[i] = 1;
			head[i] = 0;
		}
		cnt = 1;
	}

	void addEdge(int l, int k)//k指向l
	{
		nxt[cnt] = head[k];
		head[k] = cnt;
		to[cnt] = l;
		cnt++;
	}

	void f(int node)
	{
		yes[node] = happy[node];
		no[node] = 0;
		for (int ei = head[node]; ei > 0; ei = nxt[ei])
		{
			int child = to[ei];
			f(child);
			yes[node] += no[child];
			no[node] += max(yes[child], no[child]);
		}
	}

	int main()
	{
		int n;
		cin >> n;
		build(n);
		for (int i = 1; i <= n; i++)
			cin >> happy[i];
		for (int i = 1; i < n; i++)
		{
			int l, k;
			cin >> l >> k;
			boss[l] = 0;
			addEdge(l, k);
		}

		int root = 0;
		for (int i = 1; i <= n; i++)
		{
			if (boss[i])
			{
				root = i;
				break;
			}
		}

		f(root);
		cout << max(yes[root], no[root]);

		return 0;
	}

}



//题目六：
// 监控二叉树
// 给定一个二叉树，我们在树的节点上安装摄像头
// 节点上的每个摄影头都可以监视其父对象、自身及其直接子对象
// 计算监控树的所有节点所需的最小摄像头数量
// 测试链接 : https://leetcode.cn/problems/binary-tree-cameras/
class Solution 
{
public:
	// 递归含义
	// 假设x上方一定有父亲的情况下，这个假设很重要
	// x为头的整棵树，最终想都覆盖，
	// 并且想使用最少的摄像头，x应该是什么样的状态
	// 返回值含义
	// 0: x是无覆盖的状态，x下方的节点都已经被覆盖
	// 1: x是覆盖状态，x上没摄像头，x下方的节点都已经被覆盖
	// 2: x是覆盖状态，x上有摄像头，x下方的节点都已经被覆盖
	int ans;

	int f(TreeNode* node)
	{
		if (node == nullptr) return 1;

		int lchild = f(node->left);
		int rchild = f(node->right);
		if (lchild == 0 || rchild == 0)
		{
			ans++;
			return 2;
		}
		if (lchild == 1 && rchild == 1) return 0;

		return 1;
	}

	int minCameraCover(TreeNode* root)
	{
		ans = 0;
		if (f(root) == 0) ans++;//如果根节点是无覆盖状态，则给根节点装个摄像头，ans加一
		return ans;
	}
};



//题目七；
// 路径总和 III
// 给定一个二叉树的根节点 root ，和一个整数 targetSum
// 求该二叉树里节点值之和等于 targetSum 的 路径 的数目
// 路径 不需要从根节点开始，也不需要在叶子节点结束
// 但是路径方向必须是向下的（只能从父节点到子节点）
// 测试链接 : https://leetcode.cn/problems/path-sum-iii/

//思路：以当前节点为终点，有几条路径
//类似于 “和为K的子数组”

class Solution {
public:
	int f(TreeNode* node, int targetSum, long long sum, unordered_map<long long, int>& cnt) {
		if (node == nullptr) return 0;

		sum += node->val;
		int path = 0;
		if (cnt.find(sum - targetSum) != cnt.end()) path += cnt[sum - targetSum];
		cnt[sum]++;
		path += f(node->left, targetSum, sum, cnt);
		path += f(node->right, targetSum, sum, cnt);
		cnt[sum]--;//回溯
		return path;
	}

	int pathSum(TreeNode* root, int targetSum) {
		unordered_map<long long, int> cnt;
		cnt[0] = 1L;
		return f(root, targetSum, 0L, cnt);
	}
};








//树型dp--下
//本节课
//见识更多树型dp问题（题目1、2）
//树上dfn序和相关题目（题目3、4、5）
//本节课的题目5，树上01背包，推荐掌握最优解


//题目一：
// 到达首都的最少油耗
// 给你一棵 n 个节点的树（一个无向、连通、无环图）
// 每个节点表示一个城市，编号从 0 到 n - 1 ，且恰好有 n - 1 条路
// 0 是首都。给你一个二维整数数组 roads
// 其中 roads[i] = [ai, bi] ，表示城市 ai 和 bi 之间有一条 双向路
// 每个城市里有一个代表，他们都要去首都参加一个会议
// 每座城市里有一辆车。给你一个整数 seats 表示每辆车里面座位的数目
// 城市里的代表可以选择乘坐所在城市的车，或者乘坐其他城市的车
// 相邻城市之间一辆车的油耗是一升汽油
// 请你返回到达首都最少需要多少升汽油
// 测试链接 : https://leetcode.cn/problems/minimum-fuel-cost-to-report-to-the-capital/
class Solution {
public:
	struct Info
	{
		int _cnt;//当前子树所有节点数目
		long long _cost;//当前子树的花费
		Info(int a,long long b):_cnt(a),_cost(b) {}
	};

	Info f(vector<vector<int>>& AdjList, int seats, int node, int father) {
		int cnt = 1;
		long long cost = 0;
		for (auto& nxt : AdjList[node]) {
			if (nxt != father) {
				Info nxtInfo = f(AdjList, seats, nxt, node);
				cnt += nxtInfo._cnt;
				cost += nxtInfo._cost;
				cost += (nxtInfo._cnt + seats - 1) / seats;
			}
		}
		return Info(cnt, cost);
	}

	long long minimumFuelCost(vector<vector<int>>& roads, int seats) {
		int n = roads.size() + 1;
		vector<vector<int>> AdjList(n);
		//本题建图是个难点，因为不知道给出的顺序->可以建立无向图，并在递归时带上父节点编号来避免这个问题
		for (auto& edge : roads)
		{
			AdjList[edge[0]].push_back(edge[1]);
			AdjList[edge[1]].push_back(edge[0]);
		}

		return f(AdjList, seats, 0, -1)._cost;
	}
};



//题目二:
// 相邻字符不同的最长路径
// 给你一棵 树（即一个连通、无向、无环图），根节点是节点 0
// 这棵树由编号从 0 到 n - 1 的 n 个节点组成
// 用下标从 0 开始、长度为 n 的数组 parent 来表示这棵树
// 其中 parent[i] 是节点 i 的父节点
// 由于节点 0 是根节点，所以 parent[0] == -1
// 另给你一个字符串 s ，长度也是 n ，其中 s[i] 表示分配给节点 i 的字符
// 请你找出路径上任意一对相邻节点都没有分配到相同字符的 最长路径
// 并返回该路径的长度
// 测试链接 : https://leetcode.cn/problems/longest-path-with-different-adjacent-characters/

//经典分析：
//1.包含根节点   2.不包含根节点

class Solution {
public:
	struct Info
	{
		int maxPath;//整棵树的最大路径长度
		int maxPathFromHead;//必须包括根节点的最大路径长度

		Info(int a,int b) : maxPath(a),maxPathFromHead(b) {}
	};

	Info f(vector<vector<int>>& AdjList, string& s,int node) {
		if (AdjList[node].empty()) return Info(1, 1);

		int max1 = 0;//从根节点出发的最大路径长度
		int max2 = 0;//从根节点出发的第二大路径长度
		int maxPath = 1;
		for (auto& ele : AdjList[node]) {
			Info nxt = f(AdjList, s, ele);
			maxPath = max(maxPath, nxt.maxPath);

			if (s[ele] != s[node]) {
				if (nxt.maxPathFromHead > max1) {
					max2 = max1;
					max1 = nxt.maxPathFromHead;
				}
				else if (nxt.maxPathFromHead > max2) {
					max2 = nxt.maxPathFromHead;
				}
			}
		}

		int maxPathFromHead = max1 + 1;
		maxPath = max(maxPath, max1 + max2 + 1);
		return Info(maxPath, maxPathFromHead);
	}

	int longestPath(vector<int>& parent, string s) {
		int n = parent.size();
		vector<vector<int>> AdjList(n);
		for (int i = 0; i < n; i++) {
			if (parent[i] >= 0)
				AdjList[parent[i]].push_back(i);
		}

		return f(AdjList, s, 0).maxPath;
	}
};




// [dfn序]:
//用深度优先遍历的方式遍历整棵树
//给每个节点依次标记序号
//编号从小到大的顺序就是dfn序
// 
//由于dfs的关系，祖先的dfn序是小于子孙的dfn序
// 如果节点u是节点v的祖先，则 dfn[u] < dfn[v]
//
//dfn序 + 每棵子树的大小，可以起到定位子树节点的作用
//如果某个节点的dfn序号是x，以这个节点为头的子树大小为y
//那么可知，dfn序号从 x~x+y-1 所代表的节点，都属于这个节点的子树
//利用这个性质，节点间的关系判断（题目三、四），跨子树的讨论（题目五） 就会变得方便
//
//dfn序除了和树型dp相关，后续还和很多算法数据结构有关（树链剖分等）




//题目三：
// 移除子树后的二叉树高度
// 给你一棵 二叉树 的根节点 root ，树中有 n 个节点
// 每个节点都可以被分配一个从 1 到 n 且互不相同的值
// 另给你一个长度为 m 的数组 queries
// 你必须在树上执行 m 个 独立 的查询，其中第 i 个查询你需要执行以下操作：
// 从树中 移除 以 queries[i] 的值作为根节点的子树
// 题目所用测试用例保证 queries[i] 不等于根节点的值
// 返回一个长度为 m 的数组 answer
// 其中 answer[i] 是执行第 i 个查询后树的高度
// 注意：
// 查询之间是独立的，所以在每个查询执行后，树会回到其初始状态
// 树的高度是从根到树中某个节点的 最长简单路径中的边数
// 测试链接 : https://leetcode.cn/problems/height-of-binary-tree-after-subtree-removal-queries/
class Solution {
public:
	static const int MAXN = 100010;
	vector<int> dfn;//每个节点的数值-该节点对应的dfn序
	vector<int> size;//dfn序-以该节点为根节点的子树的大小
	vector<int> deep;//dfn序-根节点到该节点的深度/距离
	int dfnCnt;

	//提供答案的预处理数组
	vector<int> lmax;//dfn序-从左边起，根节点到该节点范围上最大深度
	vector<int> rmax;//dfn序-从右边起，根节点到该节点范围上最大深度

	void build() {
		dfn.assign(MAXN, 0);
		size.assign(MAXN, 0);
		deep.assign(MAXN, 0);
		dfnCnt = 0;
		lmax.assign(MAXN, 0);
		rmax.assign(MAXN, 0);
	}

	void f(TreeNode* node, int h) {
		dfn[node->val] = ++dfnCnt;
		deep[dfn[node->val]] = h;
		size[dfn[node->val]] = 1;
		if (node->left != nullptr) {
			f(node->left, h + 1);
			size[dfn[node->val]] += size[dfn[node->left->val]];
		}
		if (node->right != nullptr) {
			f(node->right, h + 1);
			size[dfn[node->val]] += size[dfn[node->right->val]];
		}
	}

	vector<int> treeQueries(TreeNode* root, vector<int>& queries) {
		build();
		f(root, 0);//第二个参数是根节点到当前节点的距离/深度

		for (int i = 1; i <= dfnCnt; i++) {
			lmax[i] = max(lmax[i - 1], deep[i]);
		}
		for (int i = dfnCnt; i >= 1; i--) {
			rmax[i] = max(rmax[i + 1], deep[i]);
		}

		int m = queries.size();
		vector<int> ans(m, 0);
		for (int i = 0; i < m; i++) {
			int remove_val = queries[i];
			int leftMax = lmax[dfn[remove_val] - 1];
			int rightMax = rmax[dfn[remove_val] + size[dfn[remove_val]]];

			ans[i] = max(leftMax, rightMax);
		}
		return ans;
	}
};



//题目四：
// 从树中删除边的最小分数
// 存在一棵无向连通树，树中有编号从0到n-1的n个节点，以及n-1条边
// 给你一个下标从0开始的整数数组nums长度为n，其中nums[i]表示第i个节点的值
// 另给你一个二维整数数组edges长度为n-1
// 其中 edges[i] = [ai, bi] 表示树中存在一条位于节点 ai 和 bi 之间的边
// 删除树中两条不同的边以形成三个连通组件，对于一种删除边方案，定义如下步骤以计算其分数：
// 分别获取三个组件每个组件中所有节点值的异或值
// 最大 异或值和 最小 异或值的 差值 就是这种删除边方案的分数
// 返回可能的最小分数
// 测试链接 : https://leetcode.cn/problems/minimum-score-after-removals-on-a-tree/
class Solution {
public:
	vector<int> dfn;//下标为原始节点编号
	vector<int> size;//下标为dfn编号
	vector<int> xOr;//下标为dfn编号
	int dfnCnt;

	void build(int n) {
		dfnCnt = 0;
		dfn.assign(n + 1, 0);
		size.assign(n + 1, 0);
		xOr.assign(n + 1, 0);
	}

	void f(vector<vector<int>>& AdjList, vector<int>& nums, int node) {
		dfn[node] = ++dfnCnt;
		size[dfn[node]] = 1;
		xOr[dfn[node]] = nums[node];
		for (auto& nxt : AdjList[node]) {
			if (dfn[nxt] == 0) {//不是父节点
				f(AdjList, nums, nxt);
				size[dfn[node]] += size[dfn[nxt]];
				xOr[dfn[node]] ^= xOr[dfn[nxt]];
			}
		}
	}

	int minimumScore(vector<int>& nums, vector<vector<int>>& edges) {
		//本题不知道谁是根节点
		//但答案的确定与谁是根节点无关 -> 默认 0 为根节点
		//也不知道谁是父节点 -> 建立无向图
		int n = nums.size();
		build(n);
		vector<vector<int>> AdjList(n);
		for (auto& edge : edges) {
			AdjList[edge[0]].push_back(edge[1]);
			AdjList[edge[1]].push_back(edge[0]);
		}

		f(AdjList, nums, 0);
		int ans = INT_MAX;
		int m = edges.size();
		for (int i = 0; i < m; i++) {
			for (int j = i + 1; j < m; j++) {
				int sum1 = 0, sum2 = 0, sum3 = 0;
				int p1 = 0, p2 = 0;//选取一条边中靠后的那个节点
				p1 = max(dfn[edges[i][0]], dfn[edges[i][1]]);
				p2 = max(dfn[edges[j][0]], dfn[edges[j][1]]);

				int a = min(p1, p2);
				int b = max(p1, p2);

				sum1 = xOr[1];//整棵树的异或值
				if (b <= a + size[a] - 1) {
					sum1 ^= xOr[a];
					sum2 = xOr[a] ^ xOr[b];
					sum3 = xOr[b];
				}
				else {
					sum1 = sum1 ^ xOr[a] ^ xOr[b];
					sum2 = xOr[a];
					sum3 = xOr[b];
				}

				ans = min(ans, max(sum1, max(sum2, sum3)) - min(sum1, min(sum2, sum3)));
			}
		}

		return ans;
	}
};



//题目五
// 选课，树上01背包的普通解法
// 在大学里每个学生，为了达到一定的学分，必须从很多课程里选择一些课程来学习
// 在课程里有些课程必须在某些课程之前学习，如高等数学总是在其它课程之前学习
// 现在有 N 门功课，每门课有个学分，每门课有一门或没有直接先修课
// 若课程 a 是课程 b 的先修课即只有学完了课程 a，才能学习课程 b
// 一个学生要从这些课程里选择 M 门课程学习
// 问他能获得的最大学分是多少
// 测试链接 : https://www.luogu.com.cn/problem/P2014

// 普通解法，邻接表建图 + 相对好懂的动态规划
// 几乎所有题解都是普通解法的思路，只不过优化了常数时间、做了空间压缩
// 但时间复杂度依然是O(n * 每个节点的孩子平均数量 * m的平方)

//dp[i][j][k]: 以i为根节点，在前j棵子树里选k门课，要求连续  所获得的最大学分
//1.第j棵不选(最后一棵子树不选): dp[i][j-1][k]
//2.选第j棵，已知第j棵子树的根节点为x: dp[x][x全部子树][s] + dp[i][j-1][k-s] (1<=s<k) -> : s<k是因为要给原根节点i留一个位置

namespace test5 {
	#include <iostream>
	#include <vector>
	using namespace std;

	// 当前来到i号节点为头的子树
	// 只在i号节点、及其i号节点下方的前j棵子树上挑选节点
	// 一共挑选k个节点，并且保证挑选的节点连成一片
	// 返回最大的累加和
	int f(vector<vector<int>>& AdjList, vector<int>& score, int i, int j, int k, vector<vector<vector<int>>>& dp) {
		if (k == 0) return 0;
		if (j == 0 || k == 1) return score[i];
		if (dp[i][j][k] != -1) return dp[i][j][k];

		int ans = f(AdjList, score, i, j - 1, k, dp);
		int endNode = AdjList[i][j - 1];
		for (int s = 1; s < k; s++) {
			ans = max(ans, f(AdjList, score, i, j - 1, k - s, dp) + f(AdjList, score, endNode, AdjList[endNode].size(), s, dp));
		}
		
		dp[i][j][k] = ans;
		return ans;
	}

	int main() {
		int n, m;
		cin >> n >> m;
		vector<vector<int>> AdjList(n + 1);
		vector<int> score(n + 1, 0);
		for (int i = 1; i <= n; i++) {
			int k, s;
			cin >> k >> s;
			AdjList[k].push_back(i);
			score[i] = s;
		}

		//由于创建了虚拟节点 0
		//因此传入参数m时要多加一个
		vector<vector<vector<int>>> dp(n + 1, vector<vector<int>>(n + 1, vector<int>(m + 2, -1)));
		cout << f(AdjList, score, 0, AdjList[0].size(), m + 1, dp) << endl;

		return 0;
	}
}



//题目六
// 选课，树上01背包的最优解
// 在大学里每个学生，为了达到一定的学分，必须从很多课程里选择一些课程来学习
// 在课程里有些课程必须在某些课程之前学习，如高等数学总是在其它课程之前学习
// 现在有 N 门功课，每门课有个学分，每门课有一门或没有直接先修课
// 若课程 a 是课程 b 的先修课即只有学完了课程 a，才能学习课程 b
// 一个学生要从这些课程里选择 M 门课程学习
// 问他能获得的最大学分是多少
// 测试链接 : https://www.luogu.com.cn/problem/P2014

// 最优解，链式前向星建图 + dfn序的利用 + 巧妙定义下的尝试
// 时间复杂度O(n * m)，推荐掌握，尤其是理解有效结构

//定义一个 有效结构 ：当来到i节点，后续所有dfn序号的节点我都考虑，
//                    所形成的结构中，如果有一个总头部出现，分别连上我挑选的结构，能形成一整个没有断连的情况
// ->当头节点出现的时候，选的几个节点需要与头节点连接上 且 不出现断连
// dp[i][j]: i`n+1(是dfn序号)，选j个，得是有效结构    得到的最大学分
//1.不考虑i号点（其子树上的所有点都不再考虑了）：dp[i+size[i]][j]
//2.考虑i号点：dp[i+1][j-1] + i的收益

namespace test6 {
	#include <iostream>
	#include <vector>
	using namespace std;

	const int MAXN = 301;
	vector<int> head(MAXN, 0);
	vector<int> nxt(MAXN, 0);
	vector<int> to(MAXN, 0);
	int edgeCnt = 1;
	vector<int> score(MAXN, 0);

	vector<int> dfn(MAXN, 0);
	vector<int> size(MAXN, 0);
	int dfnCnt = 0;
	vector<int> dfnscore(MAXN, 0);

	int n, m;

	void f(int node) {
		dfn[node] = ++dfnCnt;
		dfnscore[dfn[node]] = score[node];
		size[dfn[node]] = 1;
		for (int ei = head[node]; ei > 0; ei = nxt[ei]) {
			f(to[ei]);
			size[dfn[node]] += size[dfn[to[ei]]];
		}
	}

	void addEdge(int u, int v, int w) {
		to[edgeCnt] = v;
		nxt[edgeCnt] = head[u];
		head[u] = edgeCnt++;
		score[v] = w;
	}

	int compute() {
		f(0);                              
		vector<vector<int>> dp(dfnCnt + 2, vector<int>(m + 2, 0));
		for (int i = dfnCnt; i >= 1; i--) {        
			for (int j = 0; j <= m + 1; j++) {     
				if (j == 0)                      
					dp[i][j] = dp[i + size[i]][j];
				else
					dp[i][j] = max(dp[i + size[i]][j],
						dp[i + 1][j - 1] + dfnscore[i]);
			}
		}
		return dp[1][m + 1];                      
	}

	int main() {
		
		cin >> n >> m;
		vector<vector<int>> AdjList(n + 1);
		for (int i = 1; i <= n; i++) {
			int k, s;
			cin >> k >> s;
			addEdge(k, i, s);
		}
		
		cout << compute() << endl;

		return 0;
	}
}

