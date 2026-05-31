#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <utility>
using namespace std;



//对于一个具体的题目，方法运行的指令条数不能超过 10^7 ~ 10^8 规模，否则就会超时
//那么就可以利用这个条件：
//1.想出能通过的方法再去实现
//2.确定优化做到什么程度才能通过



//题目一：
// 贿赂怪兽
// 开始时你的能力是0，你的目标是从0号怪兽开始，通过所有的n只怪兽
// 如果你当前的能力小于i号怪兽的能力，则必须付出b[i]的钱贿赂这个怪兽
// 然后怪兽就会加入你，他的能力a[i]直接累加到你的能力上
// 如果你当前的能力大于等于i号怪兽的能力，你可以选择直接通过，且能力不会下降
// 但你依然可以选择贿赂这个怪兽，然后怪兽的能力直接累加到你的能力上
// 返回通过所有的怪兽，需要花的最小钱数
// 测试链接 : https://www.nowcoder.com/practice/736e12861f9746ab8ae064d4aae2d5a9

//进行如下思考：
//一. 假设a[i]数值的范围很大，但是b[i]数值的范围不大，该怎么做？
//二. 假设a[i]数值的范围不大，但是b[i]数值的范围很大，又该怎么做？

//解答问题一：
//dp[i][j]: 1~i号怪兽，花的钱<=j的情况下，通过前i个怪兽，最大能力是多少: 如果不能通过 ->-∞; 能通过则记录最大能力
//1.不贿赂i号怪兽: 如果 dp[i-1][j]>=a[i], 则 dp[i][j] = dp[i-1][j]
//2.贿赂i号怪兽: 如果 dp[i-1][j-b[i]] != -∞, 则 dp[i][j] = dp[i-1][j-b[i]]
//  1,2求max即为dp[i][j]
// （转换为背包问题）

namespace test1_1 {
	#include <iostream>
	#include <vector>
	#include <climits>
	using namespace std;

	const int MAXN = 505;
	vector<int> a(MAXN, 0);
	vector<int> b(MAXN, 0);

	int main() {
		int n;
		cin >> n;
		for (int i = 1; i <= n; i++) {
			cin >> a[i] >> b[i];
		}

		int sum = 0;
		for (auto& ele : b) sum += ele;

		vector<vector<int>> dp(n + 1, vector<int>(sum + 1, 0));
		for (int i = 1; i <= n; i++) {
			for (int j = 0; j <= sum; j++) {
				dp[i][j] = INT_MIN;
				//1.不贿赂i位置的怪兽
				if (dp[i - 1][j] >= a[i])
					dp[i][j] = dp[i - 1][j];
				//2.贿赂i位置的怪兽
				if (j - b[i] >= 0 && dp[i - 1][j - b[i]] != INT_MIN)
					dp[i][j] = max(dp[i][j], dp[i - 1][j - b[i]] + a[i]);
			}
		}

		int ans = -1;
		for (int j = 0; j <= sum; j++) {
			if (dp[n][j] != INT_MIN) {
				ans = j;
				break;
			}
		}

		cout << ans << endl;

		return 0;
	}
}


//解答问题二：
//dp[i][j]: 通过 1~i 号怪兽的过程中，能力正好是j : 如果能通过，记录最小的钱数；如果不能通过，+∞表示无效
//1.不贿赂i号怪兽: 如果 j >= a[i] && dp[i-1][j]!=+∞ 则 dp[i][j] = dp[i-1][j]
//2.贿赂i号怪兽: 如果 dp[i-1][j-a[i]]!=+∞ 则 dp[i][j] = dp[i-1][j-a[i]]+b[i]
//  1,2求min即为dp[i][j]
// （也是一个经典背包）

namespace test1_2 {
	#include <iostream>
	#include <vector>
	#include <climits>
	using namespace std;

	int main() {
		int n;
		cin >> n;
		vector<int> a(n + 1, 0);
		vector<int> b(n + 1, 0);
		for (int i = 1; i <= n; i++) {
			cin >> a[i] >> b[i];
		}

		int ablity = 0;
		for (auto& ele : a) ablity += ele;
		
		vector<vector<int>> dp(n + 1, vector<int>(ablity + 1, 0));
		//初始化
		for (int j = 1; j <= ablity; j++) dp[0][j] = INT_MAX;

		for (int i = 1; i <= n; i++) {
			for (int j = 0; j <= ablity; j++) {
				dp[i][j] = INT_MAX;
				//不贿赂i号怪兽
				if (j >= a[i] && dp[i - 1][j] != INT_MAX) {
					dp[i][j] = dp[i - 1][j];
				}
				//贿赂i号怪兽
				if (j - a[i] >= 0 && dp[i - 1][j - a[i]] != INT_MAX) {
					dp[i][j] = min(dp[i][j], dp[i - 1][j - a[i]] + b[i]);
				}
			}
		}

		int ans = INT_MAX;
		for (int j = 0; j <= ablity; j++) {
			ans = min(ans, dp[n][j]);
		}

		cout << (ans == INT_MAX ? -1 : ans) << endl;

		return 0;
	}
}

//解答问题二的空间压缩版本
namespace test1_2_2 {
	#include <iostream>
	#include <vector>
	#include <climits>
	using namespace std;

	int main() {
		int n;
		cin >> n;
		vector<int> a(n + 1, 0);
		vector<int> b(n + 1, 0);
		for (int i = 1; i <= n; i++) {
			cin >> a[i] >> b[i];
		}

		int ablity = 0;
		for (auto& ele : a) ablity += ele;

		vector<int> dp(ablity + 1, 0);
		//初始化
		for (int j = 1; j <= ablity; j++)
			dp[j] = INT_MAX;

		for (int i = 1; i <= n; i++) {
			for (int j = ablity; j >= 0; j--) {
				int cur = INT_MAX;
				//不贿赂
				if (j >= a[i] && dp[j] != INT_MAX) {
					cur = dp[j];
				}

				//贿赂
				if (j - a[i] >= 0 && dp[j - a[i]] != INT_MAX) {
					cur = min(cur, dp[j - a[i]] + b[i]);
				}
				dp[j] = cur;
			}
		}

		int ans = INT_MAX;
		for (int j = 0; j <= ablity; j++) {
			ans = min(ans, dp[j]);
		}

		cout << (ans == INT_MAX ? -1 : ans) << endl;

		return 0;
	}
}



//题目二：
// 选择k个数字使得两集合累加和相差不超过1
// 给定一个正数n，表示1~n这些数字都可以选择
// 给定一个正数k，表示要从1~n中选择k个数字组成集合A，剩下数字组成集合B
// 希望做到集合A和集合B的累加和相差不超过1
// 如果能做到，返回集合A选择了哪些数字，任何一种方案都可以
// 如果不能做到，返回长度为0的数组
// 2 <= n <= 10^6
// 1 <= k <= n
// 来自真实大厂笔试，没有测试链接，用对数器验证

//评估一下数据规模，01背包的解法可行吗?
//不可行，数据量太大

//分析题目，给出的数组数据特殊，则最优解一定利用这个性质！

namespace test2 {
	#include <iostream>
	#include <vector>
	using namespace std;

	class Solution {
	public:
		vector<int> f(long long sum, int n, int k) {
			long long kSum = (1LL + k) * k / 2;

			//边界情况：如果 n == k，只能全部选走，此时 oneEleDiff 为 0，需要单独判断避免除零错误
			if (n == k) {
				if (sum == kSum) {
					vector<int> ans;
					for (int i = 1; i <= k; i++) ans.push_back(i);
					return ans;
				}
				else {
					return {};
				}
			}

			int oneEleDiff = n - k;
			if (sum < kSum || kSum + (long)k * oneEleDiff < sum) return {};

			long long need = sum - kSum;
			int rightSize = need / oneEleDiff;
			int midIndex = k - rightSize + (need % oneEleDiff);
			int leftIndex = k - rightSize - (need % oneEleDiff == 0 ? 0 : 1);
			vector<int> ans;
			for (int i = 1; i <= leftIndex; i++) {
				ans.push_back(i);
			}

			if (need % oneEleDiff != 0)
				ans.push_back(midIndex);

			for (int i = n; i > n - rightSize; i--) {
				ans.push_back(i);
			}

			return ans;
		}

		vector<int> pick(int n, int k) {
			long long sum = (1LL + n) * n / 2;
			vector<int> ans = f(sum / 2, n, k);
			if (ans.empty() && (sum & 1) == 1) {
				ans = f(sum / 2 + 1, n, k);
			}
			return ans;
		}
	};
}



//题目三：
// 两个排列的最长公共子序列长度
// 给出由1~n这些数字组成的两个排列
// 求它们的最长公共子序列长度
// n <= 10^5
// 测试链接 : https://www.luogu.com.cn/problem/P1439

//评估一下数据规模，经典的求最长公共子序列的做法可行吗？
//不可行！

//思路：记 两个序列分别为 s1,s2
//维护一个表记录s1的 数据-该数据在s1中对应的下标
//根据这个表，再用一个数组维护 s2的每一个数据在这个表中对应的下标 ，对这个数组进行LIS算法即可！
//非常巧妙！

namespace test3 {
#include <iostream>
	using namespace std;

	const int MAXN = 100005;
	int a[MAXN];
	int b[MAXN];
	int where[MAXN];
	int arr[MAXN];
	int ends[MAXN];

	int main() {
		int n;
		cin >> n;
		for (int i = 0; i < n; i++) {
			cin >> a[i];
			where[a[i]] = i;
		}
		for (int i = 0; i < n; i++) {
			cin >> b[i];
			arr[i] = where[b[i]];
		}

		//LIS
		auto f = [](int* arr, int len, int x) {
			int ans = -1;
			for (int l = 0, r = len - 1; l <= r;) {
				int m = l + (r - l) / 2;
				if (arr[m] >= x) {
					ans = m;
					r = m - 1;
				}
				else {
					l = m + 1;
				}
			}
			return ans;

			};

		int len = 0;
		for (int i = 0; i < n; i++) {
			int find = f(ends, len, arr[i]);
			if (find != -1) {
				ends[find] = arr[i];
			}
			else {
				ends[len++] = arr[i];
			}
		}

		cout << len << endl;

		return 0;
	}

}



//题目四：
// 使数组严格递增的最小操作数
// 给你两个整数数组 arr1 和 arr2
// 返回使 arr1 严格递增所需要的最小操作数（可能为0）
// 每一步操作中，你可以分别从 arr1 和 arr2 中各选出一个索引
// 分别为 i 和 j，0 <= i < arr1.length 和 0 <= j < arr2.length
// 然后进行赋值运算 arr1[i] = arr2[j]
// 如果无法让 arr1 严格递增，请返回-1
// 1 <= arr1.length, arr2.length <= 2000
// 0 <= arr1[i], arr2[i] <= 10^9
// 测试链接 : https://leetcode.cn/problems/make-array-strictly-increasing/

//评估一下数据规模，用数组中的值做可变参数可行吗?
//如果定义 f(i,pre) 表示 i~n-1范围上，前一个数为pre的情况下的最小操作数 ，显然不行
//因为arr[i]的规模达到10^9

//法一：（枚举选哪个 -- 有区间dp内味了）
class Solution {
public:
	int bs(vector<int>& arr2, int x) {
		int ans = -1;
		for (int l = 0, r = arr2.size() - 1; l <= r;) {
			int m = l + (r - l) / 2;
			if (arr2[m] > x) {
				ans = m;
				r = m - 1;
			}
			else {
				l = m + 1;
			}
		}
		return ans;
	}

	// arr1[0..i-1]已经严格递增且arr1[i-1]一定没有替换
	// 返回让arr1整体都严格递增，arr1[i...]范围上还需要几次替换
	// 如果做不到，返回无穷大
	int f(vector<int>& arr1, vector<int>& arr2, int i, vector<int>& dp) {
		if (i == arr1.size()) return 0;
		if (dp[i] != -1) return dp[i];

		int ans = INT_MAX;
		int n = arr1.size();
		int pre = i == 0 ? INT_MIN : arr1[i - 1];//前一位数字
		int idx = bs(arr2, pre);//在arr2上找到比pre大的最小的数

		// 枚举arr1[i...]范围上，第一个不需要替换的位置j
		for (int j = i, k = 0; j <= n; j++, k++) {
			if (j == n) {
				ans = min(ans, k);
			}
			else {
				if (pre < arr1[j]) {
					int p = f(arr1, arr2, j + 1, dp);
					if (p != INT_MAX) {
						ans = min(ans, k + p);
					}
				}

				if (idx != -1 && idx < arr2.size()) {
					pre = arr2[idx++];
				}
				else {
					break;
				}
			}
		}

		dp[i] = ans;
		return ans;
	}

	int makeArrayIncreasing(vector<int>& arr1, vector<int>& arr2) {
		sort(arr2.begin(), arr2.end());
		arr2.erase(unique(arr2.begin(), arr2.end()), arr2.end());
		
		vector<int> dp(arr1.size(), -1);
		int ret = f(arr1, arr2, 0, dp);
		return ret == INT_MAX ? -1 : ret;
	}
};


//法二： （选或不选）
class Solution {
public:
	int bs(vector<int>& arr2, int x) {
		int ans = -1;
		for (int l = 0, r = arr2.size() - 1; l <= r;) {
			int m = l + (r - l) / 2;
			if (arr2[m] > x) {
				ans = m;
				r = m - 1;
			}
			else {
				l = m + 1;
			}
		}

		return ans;
	}

	int f(vector<int>& arr1, vector<int>& arr2, int pre, int i, map<pair<int, int>, int>& dp) {
		if (i == arr1.size()) return 0;
		pair<int, int> key = make_pair(pre, i);
		if (dp.find(key) != dp.end()) return dp[key];

		int ans = INT_MAX / 2;
		//1.不操作
		if (arr1[i] > pre) ans = min(ans, f(arr1, arr2, arr1[i], i + 1, dp));
		//2.操作
		int idx = bs(arr2, pre);
		if (idx != -1) {
			ans = min(ans, 1 + f(arr1, arr2, arr2[idx], i + 1, dp));
		}

		pair<int, int> t = make_pair(pre, i);
		dp[t] = ans;
		return ans;
	}

	int makeArrayIncreasing(vector<int>& arr1, vector<int>& arr2) {
		sort(arr2.begin(), arr2.end());
		arr2.erase(unique(arr2.begin(), arr2.end()), arr2.end());

		map<pair<int, int>, int> dp;
		int ret = f(arr1, arr2, INT_MIN / 2, 0, dp);
		return ret == INT_MAX / 2 ? -1 : ret;
	}
};

//法二的优化：将两个int压缩成一个long long 来作为哈希表的键
class Solution {
public:
	int bs(vector<int>& arr2, int x) {
		int ans = -1;
		for (int l = 0, r = arr2.size() - 1; l <= r;) {
			int m = l + (r - l) / 2;
			if (arr2[m] > x) {
				ans = m;
				r = m - 1;
			}
			else {
				l = m + 1;
			}
		}

		return ans;
	}

	int f(vector<int>& arr1, vector<int>& arr2, int pre, int i, unordered_map<long long, int>& dp) {
		if (i == arr1.size()) return 0;
		long long key = ((long long)pre << 32) | i;
		if (dp.find(key) != dp.end()) return dp[key];

		int ans = INT_MAX / 2;
		//1.不操作
		if (arr1[i] > pre) ans = min(ans, f(arr1, arr2, arr1[i], i + 1, dp));
		//2.操作
		int idx = bs(arr2, pre);
		if (idx != -1) {
			ans = min(ans, 1 + f(arr1, arr2, arr2[idx], i + 1, dp));
		}

		return dp[key] = ans;
	}

	int makeArrayIncreasing(vector<int>& arr1, vector<int>& arr2) {
		sort(arr2.begin(), arr2.end());
		arr2.erase(unique(arr2.begin(), arr2.end()), arr2.end());

		unordered_map<long long, int> dp;
		int ret = f(arr1, arr2, INT_MIN / 2, 0, dp);
		return ret == INT_MAX / 2 ? -1 : ret;
	}
};

