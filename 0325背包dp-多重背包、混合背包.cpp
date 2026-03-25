#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;



//多重背包：每一种物品给定数量的限制，进行可能性展开
//多重背包的枚举优化：二进制分组优化（最常用）、单调队列优化（复杂度最好，理解稍难）

//注意：
//二进制优化可以将“多重背包”转化为“01背包”，
//但这仅适用于“可行性问题”（能否凑出）或“最优值问题”（最大价值/最小代价），
//而不适用于“计数问题”（方案数）。

//混合背包：多种背包模型的组合与转化



//题目一：
// 多重背包不进行枚举优化
// 宝物筛选
// 一共有n种货物, 背包容量为t
// 每种货物的价值(v[i])、重量(w[i])、数量(c[i])都给出
// 请返回选择货物不超过背包容量的情况下，能得到的最大的价值
// 测试链接 : https://www.luogu.com.cn/problem/P1776
//
//思路：
//dp[i][j]: 1~i号货物自由选择，每种货物的个数都不超过限制，容量也不超过j的情况下，获得的最大价值
//1) dp[i-1][j] (当前货物要0个)
//2) dp[i-1][j-w[i]]+v[i] (要1个)
//3) dp[i-1][j-2*w[i]]+2*v[i] (要2个)
//4)   ......
//  ->枚举所有可能的个数，不能超过c[i]

// 严格位置依赖的动态规划
// 时间复杂度O(n * t * 每种商品的平均个数)
namespace test1
{
	#include <iostream>
	#include <vector>
	using namespace std;

	int n, W;
	vector<vector<int>> nums;

	int main()
	{
		cin >> n >> W;
		nums.push_back({});//dummy
		for (int i = 1; i <= n; i++)
		{
			int v, w, m;
			cin >> v >> w >> m;
			nums.push_back({ v,w,m });
		}

		vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));
		//初始化：dp[0][...] : 没有货物的情况下，不管背包容量是多少，获得的最大价值都是0
		
		for (int i = 1; i <= n; i++)
		{
			for (int j = 0; j <= W; j++)
			{
				dp[i][j] = dp[i - 1][j];

				int v = nums[i][0];
				int w = nums[i][1];
				int m = nums[i][2];
				for (int k = 1; k <= m; k++)
				{
					if (j - k * w >= 0)
						dp[i][j] = max(dp[i][j], dp[i - 1][j - k * w] + k * v);//注意这里是i-1
				}
			}
		}
		cout << dp[n][W];

		return 0;
	}
}



//题目二：
// 多重背包通过二进制分组转化成01背包(模版)
// 宝物筛选
// 一共有n种货物, 背包容量为t
// 每种货物的价值(v[i])、重量(w[i])、数量(c[i])都给出
// 请返回选择货物不超过背包容量的情况下，能得到的最大的价值
// 测试链接 : https://www.luogu.com.cn/problem/P1776
//
//核心在于：
//可能有一些张数情况有重复计算，但是不会漏掉任何一种张数情况，也不会多算任何一种张数情况
//因为是二进制分组，让原本 0~cnt 规模的枚举，变成了 0~log(cnt) 规模的枚举
// 
//原理：任何一个数都可以用二进制拆分完全/拆分一部分
//eg: 13 = 1+2+4 +6     50 = 1+2+4+8+16 +19

// 时间复杂度O(t * (log(第1种商品的个数) + log(第2种商品的个数) + ... + log(第n种商品的个数)))
// 对每一种商品的个数取log，所以时间复杂度虽然大于O(n * t)，但也不会大多少
// 多重背包最常用的方式，转化成01背包
namespace test2
{
	#include <iostream>
	#include <vector>
	using namespace std;

	const int MAXM = 40001;
	const int MAXN = 1001;

	// 把每一种货物根据个数做二进制分组，去生成衍生商品
	// 衍生出来的每一种商品，价值放入v、重量放入w
	vector<int> v(MAXN, 0);
	vector<int> w(MAXN, 0);

	int n, W;
	int m = 0;//"新商品"的下标
	int main()
	{
		cin >> n >> W;
		for (int i = 1; i <= n; i++)
		{
			int val, weight, cnt;
			cin >> val >> weight >> cnt;

			// 整个文件最重要的逻辑 : 二进制分组
			// 一般都使用这种技巧，这段代码非常重要
			// 虽然时间复杂度不如单调队列优化的版本
			// 但是好写，而且即便是比赛，时间复杂度也达标
			// 二进制分组的时间复杂度为O(log cnt)
			for (int k = 1; k <= cnt; k <<= 1)
			{
				v[++m] = val * k;
				w[m] = weight * k;
				cnt -= k;
			}
			if (cnt > 0)
			{
				v[++m] = cnt * val;
				w[m] = cnt * weight;
			}
		}

		vector<int> dp(W + 1, 0);//空间压缩
		for (int i = 1; i <= m; i++)
		{
			for (int j = W; j >= w[i]; j--)
			{
				dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
			}
		}
		cout << dp[W];

		return 0;
	}
}



//题目三：
// 观赏樱花
// 给定一个背包的容量t，一共有n种货物，并且给定每种货物的信息
// 花费(cost)、价值(val)、数量(cnt)
// 如果cnt == 0，代表这种货物可以无限选择
// 如果cnt > 0，那么cnt代表这种货物的数量
// 挑选货物的总容量在不超过t的情况下，返回能得到的最大价值
// 背包容量不超过1000，每一种货物的花费都>0
// 测试链接 : https://www.luogu.com.cn/problem/P1833
//
//核心：完全背包转化为多重背包，再把多重背包通过二进制分组转化为01背包

namespace test3
{
	#include <iostream>
	#include <vector>
	using namespace std;

	const int MAXM = 100000;

	int h1, h2, m1, m2, n;
	vector<int> weight(MAXM);
	vector<int> val(MAXM);
	int len = 0;

	int main()
	{
		char ch1, ch2;
		cin >> h1 >> ch1 >> m1 >> h2 >> ch2 >> m2 >> n;
		int time = (h2 - h1) * 60 + (m2 - m1);
		for (int i = 1; i <= n; i++)
		{
			int t, c, p;
			cin >> t >> c >> p;
			if (p == 0)//完全背包转化为多重背包
				p = time / t;

			for (int k = 1; k <= p; k <<= 1)
			{
				weight[++len] = k * t;
				val[len] = k * c;
				p -= k;
			}
			if (p > 0)
			{
				weight[++len] = p * t;
				val[len] = p * c;
			}
		}

		vector<int> dp(time + 1, 0);//空间压缩
		for (int i = 1; i <= len; i++)
		{
			for (int j = time; j >= weight[i]; j--)
			{
				dp[j] = max(dp[j], dp[j - weight[i]] + val[i]);
			}
		}
		cout << dp[time];

		return 0;
	}
}



//题目四：
// 多重背包单调队列优化
// 宝物筛选
// 一共有n种货物, 背包容量为t
// 每种货物的价值(v[i])、重量(w[i])、数量(c[i])都给出
// 请返回选择货物不超过背包容量的情况下，能得到的最大的价值
// 测试链接 : https://www.luogu.com.cn/problem/P1776
namespace test4
{

	
}




//题目五；
// 混合背包 + 多重背包普通窗口优化
// 能成功找零的钱数种类
// 每一种货币都给定面值val[i]，和拥有的数量cnt[i]
// 想知道目前拥有的货币，在钱数为1、2、3...m时
// 能找零成功的钱数有多少
// 也就是说当钱数的范围是1~m
// 返回这个范围上有多少可以找零成功的钱数
// 比如只有3元的货币，数量是5张
// m = 10
// 那么在1~10范围上，只有钱数是3、6、9时，可以成功找零
// 所以返回3表示有3种钱数可以找零成功
// 测试链接 : http://poj.org/problem?id=1742

//https://leetcode.cn/problems/count-of-sub-multisets-with-bounded-sum/description/