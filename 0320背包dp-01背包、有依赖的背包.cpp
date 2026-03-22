#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <queue>
#include <utility>
#include <algorithm>
using namespace std;




//01背包：每个物品 要和不要 两种可能性展开
//有依赖的背包：多个物品变成一个复合物品（互斥），每件复合物品 不要和怎么要 多种可能性展开
//不能用01背包来解，但是非常重要的问题：非负数组前k个最小的子序列和的问题




//题目一：
// 01背包(模版)
// 给定一个正数t，表示背包的容量
// 有m个货物，每个货物可以选择一次
// 每个货物有自己的体积costs[i]和价值values[i]
// 返回在不超过总容量的情况下，怎么挑选货物能达到价值最大
// 返回最大的价值
// 测试链接 : https://www.luogu.com.cn/problem/P1048

//dp[i][j]: 1...i个物品自由选择，容量不超过j的情况下的最大价值
//转移方程：
//1.不要当前物品: dp[i-1][j]
//2.要当前物品: dp[i-1][j-cost[i]]+val[i]

//动态规划：
namespace test1_1
{
	#include <iostream>
	#include <vector>
	using namespace std;

	int t, m;
	const int MAXN = 105;
	vector<int> times(MAXN);
	vector<int> values(MAXN);

	int main()
	{
		cin >> t >> m;
		for (int i = 1; i <= m; i++)
			cin >> times[i] >> values[i];

		vector<vector<int>> dp(m + 1, vector<int>(t + 1, 0));

		for (int i = 1; i <= m; i++)
		{
			for (int j = 1; j <= t; j++)
			{
				dp[i][j] = dp[i - 1][j];
				if (j - times[i] >= 0)
					dp[i][j] = max(dp[i][j], dp[i - 1][j - times[i]] + values[i]);
			}
		}

		cout << dp[m][t];

		return 0;
	}
}

//动态规划+空间压缩
namespace test1_2
{
	#include <iostream>
	#include <vector>
	using namespace std;

	int t, m;
	const int MAXN = 105;
	vector<int> times(MAXN);
	vector<int> values(MAXN);

	int main()
	{
		cin >> t >> m;
		for (int i = 1; i <= m; i++)
			cin >> times[i] >> values[i];

		vector<int> dp(t + 1, 0);
		for (int i = 1; i <= m; i++)
		{
			for (int j = t; j >= times[i]; j--)//注意要从右往左遍历，才会收集上一层的数据
			{
				dp[j] = max(dp[j], dp[j - times[i]] + values[i]);
			}
		}

		cout << dp[t];

		return 0;
	}
}



//题目二：
// 夏季特惠
// 某公司游戏平台的夏季特惠开始了，你决定入手一些游戏
// 现在你一共有X元的预算，平台上所有的 n 个游戏均有折扣
// 标号为 i 的游戏的原价a_i元，现价只要b_i元
// 也就是说该游戏可以优惠 a_i - b_i，并且你购买该游戏能获得快乐值为w_i
// 由于优惠的存在，你可能做出一些冲动消费导致最终买游戏的总费用超过预算
// 只要满足 : 获得的总优惠金额不低于超过预算的总金额
// 那在心理上就不会觉得吃亏。
// 现在你希望在心理上不觉得吃亏的前提下，获得尽可能多的快乐值。
// 测试链接 : https://leetcode.cn/problems/tJau2o/

//核心：优惠的价钱，就是给你提升的预算
// 
//动态规划+空间压缩
namespace test2
{
	#include <iostream>
	#include <vector>
	using namespace std;

	const int MAXN = 505;
	const int MAXX = 100001;
	int n, x;
	// 对于"一定要买的商品"，直接买！
	// 只把"需要考虑的商品"放入cost、val数组
	vector<long> costs(MAXN);
	vector<long> values(MAXN);
	vector<long> dp(MAXX);

	long ans = 0;
	long len = 1;

	long compute()
	{
		for (int i = 1; i <= len; i++)
		{
			for (long j = x; j >= costs[i]; j--)
			{
				dp[j] = max(dp[j], dp[j - costs[i]] + values[i]);
			}
		}
		return dp[x];
	}

	int main()
	{
		cin >> n >> x;
		for (int i = 0; i < n; i++)
		{
			long preCost, curCost, happy;
			cin >> preCost >> curCost >> happy;

			long diff = (preCost - curCost) - curCost;
			if (diff >= 0)
			{
				ans += happy;
				x += diff;
			}
			else
			{
				costs[len] = -diff;
				values[len++] = happy;
			}
		}

		ans += compute();
		cout << ans;

		return 0;
	}

}



//题目三：
// 目标和
// 给你一个非负整数数组 nums 和一个整数 target 。
// 向数组中的每个整数前添加 '+' 或 '-' ，然后串联起所有整数
// 可以构造一个表达式
// 例如nums=[2, 1]，可以在2之前添加'+' ，在1之前添加'-'
// 然后串联起来得到表达式 "+2-1" 。
// 返回可以通过上述方法构造的，运算结果等于 target 的不同表达式的数目
// 测试链接 : https://leetcode.cn/problems/target-sum/
//
//法一：DFS
class Solution
{
public:
	int f(vector<int>& nums, int target, int i, int sum)
	{
		if (i < 0) return sum == target ? 1 : 0;
		return  f(nums, target, i - 1, sum + nums[i]) + f(nums, target, i - 1, sum - nums[i]);
	}

	int findTargetSumWays(vector<int>& nums, int target)
	{
		int n = nums.size();
		return f(nums, target, n - 1, 0);
	}
};

//法二：记忆化搜索
//挂缓存表时，由于sum可能为负数，所以可以使用哈希表
class Solution
{
public:
	int f(vector<int>& nums, int target, int i, int sum, map<pair<int, int>, int>& dp)
	{
		if (i < 0) return sum == target ? 1 : 0;

		if (dp.find({ i,sum }) != dp.end()) return dp[{i, sum}];

		dp[{i, sum}] = f(nums, target, i - 1, sum + nums[i], dp) + f(nums, target, i - 1, sum - nums[i], dp);
		return dp[{i, sum}];
	}

	int findTargetSumWays(vector<int>& nums, int target)
	{
		int n = nums.size();
		map<pair<int, int>,int> dp;
		return f(nums, target, n - 1, 0, dp);
	}
};

//第二种缓存表
class Solution
{
public:
	int f(vector<int>& nums, int target, int i, int sum, unordered_map<int, unordered_map<int, int>>& dp)
	{
		if (i < 0) return sum == target ? 1 : 0;

		if (dp.count(i) && dp[i].count(sum)) return dp[i][sum];

		dp[i][sum] = f(nums, target, i - 1, sum + nums[i], dp) + f(nums, target, i - 1, sum - nums[i], dp);
		return dp[i][sum];
	}

	int findTargetSumWays(vector<int>& nums, int target)
	{
		int n = nums.size();
		unordered_map<int, unordered_map<int, int>> dp;
		return f(nums, target, n - 1, 0, dp);
	}
};

//法三：普通动态规划
// 核心：平移技巧
//dp[i][j] 的定义是：只考虑前 i 个数字时，凑出和为 j 的方法数。
class Solution 
{
public:
	int findTargetSumWays(vector<int>& nums, int target) 
	{
		int n = nums.size();
		int s = 0;//最大可能值，即为平移数组的偏移量
		for (auto& num : nums)
			s += num;

		//特判：
		if (s<target || -s>target) return 0;
		int m = 2 * s + 1;// -s ~ +s -> 2 * s + 1
		vector<vector<int>> dp(n, vector<int>(m, 0));
		//初始化：
		if (nums[0] == 0) dp[0][nums[0] + s] = 2;
		else
		{
			dp[0][-nums[0] + s] = 1;
			dp[0][nums[0] + s] = 1;
		}

		for (int i = 1; i < n; i++)
		{
			for (int j = -s; j <= s; j++)
			{
				if (j + nums[i] + s < m)
				{
					// 原本是 : dp[i][j] = dp[i + 1][j + nums[i]]
					// 平移！
					dp[i][j + s] = dp[i - 1][j + nums[i] + s];
				}
				if (j - nums[i] + s >= 0)
				{
					// 原本是 : dp[i][j] += dp[i + 1][j - nums[i]]
					// 平移！
					dp[i][j + s] += dp[i - 1][j - nums[i] + s];
				}
			}
		}
		return dp[n - 1][target + s];
	}
};

//法四：转化为01背包问题
// 新思路，转化为01背包问题
	// 思考1:
	// 虽然题目说nums是非负数组，但即使nums中有负数比如[3,-4,2]
	// 因为能在每个数前面用+或者-号
	// 所以[3,-4,2]其实和[3,4,2]会达成一样的结果
	// 所以即使nums中有负数，也可以把负数直接变成正数，也不会影响结果
	// 思考2:
	// 如果nums都是非负数，并且所有数的累加和是sum
	// 那么如果target>sum，很明显没有任何方法可以达到target，可以直接返回0
	// 思考3:
	// nums内部的数组，不管怎么+和-，最终的结果都一定不会改变奇偶性
	// 所以，如果所有数的累加和是sum，并且与target的奇偶性不一样
	// 那么没有任何方法可以达到target，可以直接返回0
	// 思考4(最重要):
	// 比如说给定一个数组, nums = [1, 2, 3, 4, 5] 并且 target = 3
	// 其中一个方案是 : +1 -2 +3 -4 +5 = 3
	// 该方案中取了正的集合为A = {1，3，5}
	// 该方案中取了负的集合为B = {2，4}
	// 所以任何一种方案，都一定有 sum(A) - sum(B) = target
	// 现在我们来处理一下这个等式，把左右两边都加上sum(A) + sum(B)，那么就会变成如下：
	// sum(A) - sum(B) + sum(A) + sum(B) = target + sum(A) + sum(B)
	// 2 * sum(A) = target + 数组所有数的累加和
	// sum(A) = (target + 数组所有数的累加和) / 2
	// 也就是说，任何一个集合，只要累加和是(target + 数组所有数的累加和) / 2
	// 那么就一定对应一种target的方式
	// 比如非负数组nums，target = 1, nums所有数累加和是11
	// 求有多少方法组成1，其实就是求，有多少种子集累加和达到6的方法，(1+11)/2=6
	// 因为，子集累加和6 - 另一半的子集累加和5 = 1(target)
	// 所以有多少个累加和为6的不同集合，就代表有多少个target==1的表达式数量
	// 至此已经转化为01背包问题了
class Solution
{
public:
	int findTargetSumWays(vector<int>& nums, int target) 
	{
		int sum = 0;
		for (auto& ele : nums) sum += ele;
		if (-sum > target || sum < target || ((target & 1) ^ (sum & 1)) == 1) return 0;

		int aim = (sum + target) / 2;
		int n = nums.size();
		//dp[i][j]: [0,...,i]范围上累加和不超过j的方法数
		vector<vector<int>> dp(n, vector<int>(aim + 1, 0));
		//初始化：
		dp[0][0] = 1;
		if (nums[0] <= aim) dp[0][nums[0]] += 1;

		for (int i = 1; i < n; i++)
		{
			for (int j = 0; j <= aim; j++)
			{
				dp[i][j] = dp[i - 1][j];
				if (j - nums[i] >= 0)
					dp[i][j] += dp[i - 1][j - nums[i]];
			}
		}
		return dp[n - 1][aim];
	}
};



//题目四：
// 最后一块石头的重量 II
// 有一堆石头，用整数数组 stones 表示
// 其中 stones[i] 表示第 i 块石头的重量。
// 每一回合，从中选出任意两块石头，然后将它们一起粉碎
// 假设石头的重量分别为 x 和 y，且 x <= y
// 那么粉碎的可能结果如下：
// 如果 x == y，那么两块石头都会被完全粉碎；
// 如果 x != y，那么重量为 x 的石头将会完全粉碎，而重量为 y 的石头新重量为 y-x
// 最后，最多只会剩下一块 石头，返回此石头 最小的可能重量
// 如果没有石头剩下，就返回 0
// 测试链接 : https://leetcode.cn/problems/last-stone-weight-ii/
//
//思路：将所有石头分成两个集合，是两个集合相减的绝对值最小：|sum1 - sum2| 最小
//    ->选出一个集合使其累加和最接近 sum/2 ，(sum/2 - 该集合的和)即为最终答案  
class Solution 
{
public:
	int lastStoneWeightII(vector<int>& stones) 
	{
		int n = stones.size();
		int sum = 0;
		for (auto& ele : stones) sum += ele;
		int aim = sum / 2;
		//dp[i][j]: [0,...i]上子序列累加和不超过j，且最接近j的累加和
		vector<vector<int>> dp(n, vector<int>(aim + 1, 0));
		//初始化：
		for (int j = stones[0]; j <= aim; j++)
			dp[0][j] = stones[0];
		
		for (int i = 1; i < n; i++)
		{
			for (int j = 0; j <= aim; j++)
			{
				dp[i][j] = dp[i - 1][j];
				if (j - stones[i] >= 0)
					dp[i][j] = max(dp[i][j], dp[i - 1][j - stones[i]] + stones[i]);
			}
		}
		return (sum - dp[n - 1][aim]) - dp[n - 1][aim];
	}
};



//题目五：
// 有依赖的背包(模版)
// 物品分为两大类：主件和附件
// 主件的购买没有限制，钱够就可以；附件的购买有限制，该附件所归属的主件先购买，才能购买这个附件
// 例如，若想买打印机或扫描仪这样的附件，必须先购买电脑这个主件
// 以下是一些主件及其附件的展示：
// 电脑：打印机，扫描仪 | 书柜：图书 | 书桌：台灯，文具 | 工作椅：无附件
// 每个主件最多有2个附件，并且附件不会再有附件，主件购买后，怎么去选择归属附件完全随意，钱够就可以
// 所有的物品编号都在1~m之间，每个物品有三个信息：价格v、重要度p、归属q
// 价格就是花费，价格 * 重要度 就是收益，归属就是该商品是依附于哪个编号的主件
// 比如一件商品信息为[300,2,6]，花费300，收益600，该商品是6号主件商品的附件
// 再比如一件商品信息[100,4,0]，花费100，收益400，该商品自身是主件(q==0)
// 给定m件商品的信息，给定总钱数n，返回在不违反购买规则的情况下最大的收益
// 测试链接 : https://www.luogu.com.cn/problem/P1064
// 测试链接 : https://www.nowcoder.com/practice/f9c6f980eeec43ef85be20755ddbeaf4
//
//思路：
//由于要购买附件一定要先买主件，因此根据主件展开
//dp[i][j]: [1,...i]件商品中根据主件展开，费用不超过j，获得的最大价值
//1.不要当前主件：dp[i][j]=dp[i-1][j]
//2.只要当前主件，不要任何附件：dp[i][j]=dp[i-1][j-cost[i]]+val[i]
//3.要当前主件，且要附件：根据附件数目分类讨论即可
namespace test5
{
	#include <iostream>
	#include <vector>
	using namespace std;

	const int MAXM = 32001;
	const int MAXN = 61;
	int n, m;
	vector<int> cost(MAXN);
	vector<int> val(MAXN);
	vector<int> king(MAXN);//记录谁是主件
	vector<int> fans(MAXN);//记录主件对应的附件数目
	vector<vector<int>> follow(MAXN, vector<int>(2));//记录每个主件对应附件的下标

	vector<vector<int>> dp;

	void clean()
	{
		for (int i = 1; i <= m; i++)
			fans[i] = 0;
	}

	int compute()
	{
		// dp[i][j] : 0...i范围上，只关心主商品，并且进行展开
		//            花费不超过j的情况下，获得的最大收益
		// dp[0][....] = 0 : 无商品的时候
		int preMain = 0;//上一件主件的编号
		for (int i = 1; i <= m; i++)
		{
			if (king[i])
			{
				for (int j = 0; j <= n; j++)
				{
					//不要当前主件
					dp[i][j] = dp[preMain][j];
					//只要当前主件
					if (j - cost[i] >= 0) dp[i][j] = max(dp[i][j], dp[preMain][j - cost[i]] + val[i]);

					//讨论当前主件对应附件
					int p1 = fans[i] >= 1 ? follow[i][0] : -1;
					int p2 = fans[i] >= 2 ? follow[i][1] : -1;
					//1) 要当前主件和附件1
					if (p1 != -1 && j - cost[i] - cost[p1] >= 0)
						dp[i][j] = max(dp[i][j], dp[preMain][j - cost[i] - cost[p1]] + val[i] + val[p1]);
					//2) 要当前主件和附件2
					if (p2 != -1 && j - cost[i] - cost[p2] >= 0)
						dp[i][j] = max(dp[i][j], dp[preMain][j - cost[i] - cost[p2]] + val[i] + val[p2]);
					//3) 我全都要
					if (p1 != -1 && p2 != -1 && j - cost[i] - cost[p1] - cost[p2] >= 0)
						dp[i][j] = max(dp[i][j], dp[preMain][j - cost[i] - cost[p1] - cost[p2]] + val[i] + val[p1] + val[p2]);
				}
				preMain = i;//更新上一件主件编号
			}
		}
		return dp[preMain][n];
	}

	int main()
	{
		clean();
		cin >> n >> m;
		dp.assign(m + 1, vector<int>(n + 1, 0));
		for (int i = 1; i <= m; i++)
		{
			int v, p, q;
			cin >> v >> p >> q;
			cost[i] = v;
			val[i] = v * p;
			if (q == 0)
				king[i] = 1;
			else
			{
				follow[q][fans[q]++] = i;
			}
		}

		cout << compute();

		return 0;
	}
}



//题目六：
// 非负数组前k个最小的子序列累加和
// 给定一个数组nums，含有n个数字，都是非负数
// 给定一个正数k，返回所有子序列中累加和最小的前k个累加和
// 子序列是包含空集的
// 1 <= n <= 10^5
// 1 <= nums[i] <= 10^6
// 1 <= k <= 10^5
// 注意这个数据量，用01背包的解法是不行的，时间复杂度太高了
// 对数器验证
//
//法一：01背包
// 这种方法此时不是最优解
// 因为n很大，数值也很大，那么可能的累加和就更大
// 时间复杂度太差
//
//dp[i][j]: [0,...i]位置上累加和要求为j的方法数
class Solution
{
	vector<int> topKSum(vector<int>& nums, int k)
	{
		int n = nums.size();
		int sum = 0;
		for (auto& ele : nums)
			sum += ele;

		vector<vector<int>> dp(n, vector<int>(sum + 1, 0));
		dp[0][0] = 1;
		dp[0][nums[0]] += 1;
		for (int i = 1; i < n; i++)
		{
			for (int j = 0; j <= sum; j++)
			{
				dp[i][j] = dp[i - 1][j];
				if (j - nums[i] >= 0)
					dp[i][j] += dp[i - 1][j - nums[i]];
			}
		}
		vector<int> ans(k, 0);
		int idx = 0;
		for (int j = 0; j <= sum && idx < k; j++)
		{
			for (int i = 0; i < dp[n - 1][j] && idx < k; i++)
				ans[idx++] = j;
		}
		return ans;
	}
};

//法二：正式方法
// 用堆来做是最优解，时间复杂度O(n * log n) + O(k * log k)
// 特别注意：这个方法利用贪心，只在数组中所有数字非负才能成立
class Solution
{
	struct cmp
	{
		bool operator()(vector<int>& a, vector<int>& b)
		{
			return a[1] > b[1];//小根堆
		}
	};

	vector<int> topKSum(vector<int>& nums, int k)
	{
		sort(nums.begin(), nums.end());
		//[当前序列最右侧数字的下标，当前序列累加和]
		priority_queue<vector<int>, vector<vector<int>>, cmp> heap;
		heap.push({ 0,nums[0] });
		vector<int> ans(k, 0);
		//ans[0]=0; 空集为0
		for (int i = 1; i < k; i++)//注意这里是< ,执行k-1次
		{
			int rightIdx = heap.top()[0];
			int sum = heap.top()[1];
			heap.pop();
			ans[i] = sum;

			if (rightIdx + 1 < nums.size())
			{
				//不要当前的数字(即最右侧下标对应的数字)
				heap.push({ rightIdx + 1,sum - nums[rightIdx] + nums[rightIdx + 1] });
				//要当前的数字(即最右侧下标对应的数字)
				heap.push({ rightIdx + 1, sum + nums[rightIdx + 1] });
			}
		}
		return ans;
	}
};



//题目七；
// 思路来自课上的题目6
// 找出数组的第K大和
// 给定一个数组nums和正数k
// 可以选择数组的任何子序列并对其元素求和
// 希望找到第k大的子序列和，子序列和允许出现重复
// 空子序列的和视作0，数组中的值可正、可负、可0
// 测试链接 : https://leetcode.cn/problems/find-the-k-sum-of-an-array/description/

//把握核心：
//可以轻松得到所有正数的和，那就是最大的序列和。如何得到第 k 大的呢？删正数 或 添负数
//如何做？可以统一成 删绝对值。将负数取反，然后排序，每次取最小的数，得到的就是最小的正数或最大的负数。sum 中减去它，就可以得到下一个更小的子序列和

// 转化逻辑如下 : 
// 1，先把所有正数加起来，得到sum，这是nums第1大子序列和
// 2，nums第2大子序列和，要么sum中去掉一个最小的正数，要么sum + (选剩下的非正数字，拼出最大的累加和)
// 3，nums第2大子序列和 = (sum - 最小的正数) 或者 (sum - 选剩下的非正数字，拼出最小的绝对值)
// 4，原始数组记为nums，把每个数转成绝对值的数组记为a
// 5，nums第1大子序列和 = sum = sum - 0，其中0表示数组a取空集的累加和，认为是a的第1小子序列和
// 6，nums第1大子序列和 = sum - a的第1小子序列和
// 7，nums第2大子序列和 = sum - a的第2小子序列和
// 8，nums第k大子序列和 = sum - a的第k小子序列和
// 9，求a的第k小子序列和即可，注意，a的第1小子序列和，是空集的情况
class Solution 
{
public:
	struct cmp
	{
		bool operator()(vector<long long>& a, vector<long long>& b)
		{
			return a[1] > b[1];//小根堆
		}
	};

	long long kSum(vector<int>& nums, int k) 
	{
		long long maxSum = 0;//最大子序列和
		for (auto& ele : nums)
		{
			if (ele > 0) maxSum += ele;
			else ele = abs(ele);//取绝对值，全部转化为非负数
		}

		sort(nums.begin(), nums.end());
		priority_queue<vector<long long>, vector<vector<long long>>, cmp> heap;
		heap.push({ 0,nums[0] });//初始化
		
		long long aim = 0;
		for (int i = 1; i < k; i++)
		{
			long long rightIdx = heap.top()[0];
			long long sum = heap.top()[1];
			heap.pop();
			aim = sum;

			if (rightIdx + 1 < nums.size())
			{
				heap.push({ rightIdx + 1,sum - nums[rightIdx] + nums[rightIdx + 1] });
				heap.push({ rightIdx + 1,sum + nums[rightIdx + 1] });
			}
		}
		return maxSum - aim;
	}
};

