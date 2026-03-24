#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;



//分组背包：多个物品分组，每组只能取1件
//          每一组的物品都可能性展开就可以了
//注意时间复杂度不会升阶，O(物品数量 * 背包容量)

//完全背包：与01背包的区别仅在于 每种商品可以选取无限次
//          时间复杂度O(物品数量 * 背包容量)



//题目一：
// 分组背包(模版)
// 给定一个正数m表示背包的容量，有n个货物可供挑选
// 每个货物有自己的体积(容量消耗)、价值(获得收益)、组号(分组)
// 同一个组的物品只能挑选1件，所有挑选物品的体积总和不能超过背包容量
// 怎么挑选货物能达到价值最大，返回最大的价值
// 测试链接 : https://www.luogu.com.cn/problem/P1757
//
//思路：
//dp[i][j]: [1~i]组上，每组只能选1件商品，容量不超过j的情况下，最大价值
//          注意：i代表组，不是商品！
//1) 不要i组商品，dp[i-1][j]
//2) 要i组商品，要哪一件？全试！
//   a.dp[i-1][j-a的体积]+a的价值
//   b.dp[i-1][j-b的体积]+b的价值
//   c.dp[i-1][j-c的体积]+c的价值
//   ......
//求最大值
namespace test1
{
	#include <iostream>
	#include <vector>
	using namespace std;
	const int MAXK = 101;
	int m, n;
	vector<vector<vector<int>>> nums(MAXK);
	int main()
	{
		cin >> m >> n;
		int k = 0;
		for (int i = 0; i < n; i++)
		{
			int a, b, c;
			cin >> a >> b >> c;
			k = max(k, c);
			nums[c].push_back({ a,b });
		}

		vector<vector<int>> dp(k + 1, vector<int>(m + 1, 0));
		for (int i = 1; i <= k; i++)
		{
			for (int j = 0; j <= m; j++)
			{
				dp[i][j] = dp[i - 1][j];
				for (auto& ele : nums[i])
				{
					int weigh = ele[0];
					int val = ele[1];
					if (j - weigh >= 0)
						dp[i][j] = max(dp[i][j], dp[i - 1][j - weigh] + val);
				}
			}
		}

		cout << dp[k][m];

		return 0;
	}
}



//题目二：
// 从栈中取出K个硬币的最大面值和
// 一张桌子上总共有 n 个硬币 栈 。每个栈有 正整数 个带面值的硬币
// 每一次操作中，你可以从任意一个栈的 顶部 取出 1 个硬币，从栈中移除它，并放入你的钱包里
// 给你一个列表 piles ，其中 piles[i] 是一个整数数组
// 分别表示第 i 个栈里 从顶到底 的硬币面值。同时给你一个正整数 k
// 请你返回在 恰好 进行 k 次操作的前提下，你钱包里硬币面值之和 最大为多少
// 测试链接 : https://leetcode.cn/problems/maximum-value-of-k-coins-from-piles/
//
//dp[i][j]: 1~i组上，拿走硬币不超过j个的情况下，获得的最大价值
class Solution
{
public:
	int maxValueOfCoins(vector<vector<int>>& piles, int m) 
	{
		int n = piles.size();
		vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

		for (int i = 1; i <= n; i++)
		{
			//题目给的piles数组下标从0开始，这里转成1开始
			vector<int> nums = piles[i - 1];
			int t = min((int)nums.size(), m);
			vector<int> preSum(t + 1, 0);
			//preSum[0]=0;
			for (int j = 1; j <= t; j++)
				preSum[j] = preSum[j - 1] + nums[j - 1];

			for (int j = 0; j <= m; j++)
			{
				dp[i][j] = dp[i - 1][j];
				for (int k = 0; k <= t; k++)
				{
					if (j - k >= 0)
						dp[i][j] = max(dp[i][j], dp[i - 1][j - k] + preSum[k]);
				}
			}
		}
		return dp[n][m];
	}
};



//题目三：
// 完全背包(模版)
// 给定一个正数t，表示背包的容量
// 有m种货物，每种货物可以选择任意个
// 每种货物都有体积costs[i]和价值values[i]
// 返回在不超过总容量的情况下，怎么挑选货物能达到价值最大
// 返回最大的价值
// 测试链接 : https://www.luogu.com.cn/problem/P1616
//
//思路：
//dp[i][j]: 1~i号商品，每种商品可以无限拿，容量不超过j，最大价值
//1) dp[i-1][j]
//2) dp[i][j - i号商品的体积] + i号商品的价值
//
//不空间压缩，无法通过全部样例
namespace test3
{
	#include <iostream>
	#include <vector>
	using namespace std;

	int t, m;
	vector<vector<int>> nums;

	int main()
	{
		cin >> t >> m;
		nums.push_back({});//dummy
		for (int i = 1; i <= m; i++)
		{
			int time, val;
			cin >> time >> val;
			nums.push_back({ time,val });
		}

		vector<vector<int>> dp(m + 1, vector<int>(t + 1, 0));
		for (int i = 1; i <= m; i++)
		{
			for (int j = 1; j <= t; j++)
			{
				dp[i][j] = dp[i - 1][j];
				if (j - nums[i][0] >= 0)
					dp[i][j] = max(dp[i][j], dp[i][j - nums[i][0]] + nums[i][1]);
			}
		}
		cout << dp[m][t];
		return 0;
	}
}

//空间压缩版本
namespace test3_pro
{
	#include <iostream>
	#include <vector>
	using namespace std;

	int t, m;
	vector<vector<int>> nums;

	int main()
	{
		cin >> t >> m;
		nums.push_back({});//dummy
		for (int i = 1; i <= m; i++)
		{
			int time, val;
			cin >> time >> val;
			nums.push_back({ time,val });
		}

		vector<long long> dp(t + 1, 0);
		for (int i = 1; i <= m; i++)
		{
			for (int j = 1; j <= t; j++)
			{
				//dp[j] = dp[j];
				if (j - nums[i][0] >= 0)
					dp[j] = max(dp[j], dp[j - nums[i][0]] + nums[i][1]);
			}
		}
		cout << dp[t];
		return 0;
	}
}



//题目四；
// 正则表达式匹配
// 给你字符串s、字符串p
// s中一定不含有'.'、'*'字符，p中可能含有'.'、'*'字符
// '.' 表示可以变成任意字符，数量1个
// '*' 表示可以让 '*' 前面那个字符数量任意(甚至可以是0个)
// p中即便有'*'，一定不会出现以'*'开头的情况，也一定不会出现多个'*'相邻的情况(无意义)
// 请实现一个支持 '.' 和 '*' 的正则表达式匹配
// 返回p的整个字符串能不能匹配出s的整个字符串
// 测试链接 : https://leetcode.cn/problems/regular-expression-matching/

//关键理解 * 的含义：将*与前面元素视作一个整体
//               eg: a* 可以表示：空字符串，a，aa，aaa，……

//法一：递归
class Solution 
{
public:
	//由于*必须搭配前一个字符才能发挥作用
	//  p[j]这个字符，一定不是'*'
	//所以递归定义为: s[si~m-1]是否与p[pi~n-1]匹配
	bool f(string s, string p, int sIdx, int pIdx)
	{
		//base case:
		if (sIdx == s.size())//s串遍历完
		{
			if (pIdx == p.size())//p串也遍历完
				return true;
			else
				// p还剩下一些后缀
				// 如果p[j+1]是*，那么p[j..j+1]可以消掉，然后看看p[j+2....]是不是都能消掉
				return pIdx + 1 < p.size() && p[pIdx + 1] == '*' && f(s, p, sIdx, pIdx + 2);
		}
		else if (pIdx == p.size())
		{
			// s有后缀
			// p没后缀了
			return false;
		}

		// s有后缀
		// p有后缀
		if (pIdx + 1 == p.size() || p[pIdx + 1] != '*')
		{
			// s[i....]
			// p[j....]
			// 如果p[j+1]不是*，那么当前的字符必须能匹配：(s[i] == p[j] || p[j] == '?')
			// 同时，后续也必须匹配上：process1(s, p, i + 1, j + 1);
			return (p[pIdx] == s[sIdx] || p[pIdx] == '.') && f(s, p, sIdx + 1, pIdx + 1);
		}
		else if (p[pIdx + 1] == '*')
		{
			// 如果p[j+1]是*
			// 完全背包！
			// s[i....]
			// p[j....]
			
			// 选择1: 当前p[j..j+1]是x*，就是不让它搞定s[i]，那么继续 : process1(s, p, i, j + 2)
			bool p1 = f(s, p, sIdx, pIdx + 2);

			// 选择2: 当前p[j..j+1]是x*，如果可以搞定s[i]，那么继续 : process1(s, p, i + 1, j)
			// 如果可以搞定s[i] : (s[i] == p[j] || p[j] == '.')
			// 继续匹配 : process1(s, p, i + 1, j)
			bool p2 = (p[pIdx] == s[sIdx] || p[pIdx] == '.') && f(s, p, sIdx + 1, pIdx);

			return p1 || p2;// 两个选择，有一个可以搞定就返回true，都无法搞定返回false
		}
		return false;
	}

	bool isMatch(string s, string p) 
	{
		return f(s, p, 0, 0);
	}
};

//法二：记忆化搜索
class Solution
{
public:
	int f(string s, string p, int i, int j, vector<vector<int>>& dp)
	{
		if (i == s.size())
		{
			if (j == p.size())
			{
				return true;
			}
			else
			{
				return j + 1 < p.size() && p[j + 1] == '*' && f(s, p, i, j + 2, dp);
			}
		}
		else if (j == p.size())
			return false;

		if (dp[i][j] != -1) return dp[i][j];

		if (j + 1 == p.size() || p[j + 1] != '*')
		{
			dp[i][j] = ((p[j] == s[i] || p[j] == '.') && f(s, p, i + 1, j + 1, dp)) ? 1 : 0;
		}
		else if (p[j + 1] == '*')
		{
			int p1 = f(s, p, i, j + 2, dp);
			int p2 = (p[j] == s[i] || p[j] == '.') && f(s, p, i + 1, j, dp);
			dp[i][j] = (p1 || p2) ? 1 : 0;
		}
		return dp[i][j];
	}

	bool isMatch(string s, string p) 
	{
		int m = s.size();
		int n = p.size();
		vector<vector<int>> dp(m, vector<int>(n, -1));
		return f(s, p, 0, 0, dp);
	}
};

//法三：动态规划
class Solution 
{
public:
	bool isMatch(string s, string p) 
	{
		int m = s.size();
		int n = p.size();

		vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
		//初始化：
		dp[m][n] = 1;
		for (int j = n - 1; j >= 0; j--)
			dp[m][j] = j + 1 < n && p[j + 1] == '*' && dp[m][j + 2];
		
		for (int i = m - 1; i >= 0; i--)
		{
			for (int j = n - 1; j >= 0; j--)
			{
				if (j + 1 == n || p[j + 1] != '*')
				{
					dp[i][j] = (p[j] == s[i] || p[j] == '.') && dp[i + 1][j + 1] ? 1 : 0;
				}
				else if (p[j + 1] == '*')
				{
					int p1 = dp[i][j + 2];
					int p2 = (p[j] == s[i] || p[j] == '.') && dp[i + 1][j] ? 1 : 0;
					dp[i][j] = p1 || p2;
				}
			}
		}
		return dp[0][0];
	}
};



//题目五：
// 通配符匹配（和题目4高度相似，只是边界条件不同而已，而且更简单）
// 给你字符串s、字符串p
// s中一定不含有'?'、'*'字符，p中可能含有'?'、'*'字符
// '?' 表示可以变成任意字符，数量1个
// '*' 表示可以匹配任何字符串
// 请实现一个支持 '?' 和 '*' 的通配符匹配
// 返回p的整个字符串能不能匹配出s的整个字符串
// 测试链接 : https://leetcode.cn/problems/wildcard-matching/
//
//法一：记忆化搜索
//这里的递归定义是：  s[0~i] 是否与 p[0~j] 匹配
class Solution 
{
public:
	int f(string& s, string& p, int i, int j, vector<vector<int>>& dp)
	{
		if (i < 0)
		{
			if (j < 0)
				return 1;
			else
				return p[j] == '*' && f(s, p, i, j - 1, dp);
		}
		else if (j < 0)
			return 0;

		if (dp[i][j] != -1) return dp[i][j];

		if (p[j] != '*')
			dp[i][j] = (p[j] == s[i] || p[j] == '?') && f(s, p, i - 1, j - 1, dp);
		else if (p[j] == '*')
		{
			int p1 = f(s, p, i, j - 1, dp);
			int p2 = f(s, p, i - 1, j, dp);
			dp[i][j] = p1 || p2;
		}
		return dp[i][j];
	}

	bool isMatch(string s, string p) 
	{
		int m = s.size();
		int n = p.size();
		vector<vector<int>> dp(m, vector<int>(n, -1));
		return f(s, p, m - 1, n - 1, dp);
	}
};

//法二：动态规划
//这里就不按记忆化搜索的定义推了，因为base case 不好在dp表中初始化
//dp[i][j]: s[i~m-1] 是否与 p[j~n-1] 匹配
class Solution 
{
public:
	bool isMatch(string s, string p)
	{
		int m = s.size();
		int n = p.size();
		vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
		//初始化
		dp[m][n] = 1;
		for (int j = n - 1; j >= 0; j--)
			dp[m][j] = p[j] == '*' && dp[m][j + 1];

		for (int i = m - 1; i >= 0; i--)
		{
			for (int j = n - 1; j >= 0; j--)
			{
				if (p[j] != '*')
					dp[i][j] = (p[j] == s[i] || p[j] == '?') && dp[i + 1][j + 1];
				else if (p[j] == '*')
					dp[i][j] = dp[i][j + 1] || dp[i + 1][j];
			}
		}
		return dp[0][0];
	}
};



//题目六：
// 购买足量干草的最小花费
// 有n个提供干草的公司，每个公司都有两个信息
// cost[i]代表购买1次产品需要花的钱
// val[i]代表购买1次产品所获得的干草数量
// 每个公司的产品都可以购买任意次
// 你一定要至少购买h数量的干草，返回最少要花多少钱
// 测试链接 : https://www.luogu.com.cn/problem/P2918
//
//dp[i][j]: 1~i 公司买甘草,数量严格为j 所需要的最小花费
//特别注意的是：这个定义有缺陷，例如：
//    只有公司1，每份价格5，每份数量3    题目给出的 H = 7
//    0  1  2  3  4  5  6  7  8  9 ...
//   √       √       √  ？   √
//因此需要对dp表扩充，扩充的最小数量为 所有公司的甘草每份数量 的最大值
namespace test6
{
	#include <iostream>
	#include <vector>
	#include <climits>
	using namespace std;

	int n, h;
	vector<vector<int>> nums;
	
	int main()
	{
		cin >> n >> h;
		nums.push_back({});//dummy
		int m = 0;//扩充信息
		for (int i = 0; i < n; i++)
		{
			int p, c;
			cin >> p >> c;
			m = max(m, p);
			nums.push_back({ p,c });
		}
		
		vector<vector<int>> dp(n + 1, vector<int>(h + 1 + m, INT_MAX));
		//初始化：
		for (int i = 0; i <= n; i++)
			dp[i][0] = 0;

		for (int i = 1; i <= n; i++)
		{
			for (int j = 1; j <= h + m; j++)
			{
				dp[i][j] = dp[i - 1][j];
				if (j - nums[i][0] >= 0 && dp[i][j - nums[i][0]] != INT_MAX)
					dp[i][j] = min(dp[i][j], dp[i][j - nums[i][0]] + nums[i][1]);
			}
		}

		int ans = INT_MAX;
		for (int j = h; j <= h + m; j++)
			ans = min(ans, dp[n][j]);
		cout << ans;
		
		return 0;
	}
}

