#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;



//很多动态规划题目要求返回的答案不是一个简单数值，而是一个具体的方案
//1.利用动态规划表生成决策路径，本节课 题目1、题目2、题目3
//2.有时候需要增加额外的路径收集结构，本节课 题目4

//找方案从哪开始？
//  就从递归的入口（也就是 DP 表的终点）开始

//对这一类的题目来说，动态规划是最重要的，得到具体方案只是一个比较简单的处理技巧



//题目一：
// 最长公共子序列其中一个结果
// 给定两个字符串str1和str2
// 输出两个字符串的最长公共子序列
// 如果最长公共子序列为空，则输出-1
// 测试链接 : https://www.nowcoder.com/practice/4727c06b9ee9446cab2e859b4bb86bb8
namespace test1 {
	#include <iostream>
	#include <vector>
	#include <string>
	using namespace std;

	void f(string& a, string& b, vector<vector<int>>& dp) {
		int m = a.size(), n = b.size();
		for (int i = 1; i <= m; i++) {
			for (int j = 1; j <= n; j++) {
				if (a[i - 1] == b[j - 1]) {
					dp[i][j] = 1 + dp[i - 1][j - 1];
				}
				else {
					dp[i][j] = max(dp[i][j - 1], dp[i - 1][j]);
				}
			}
		}
	}

	int main() {
		string a, b;
		cin >> a >> b;
		int m = a.size();
		int n = b.size();
		vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
		f(a, b, dp);

		int len = dp[m][n];

		if (len == 0) {
			cout << -1;
			return 0;
		}

		vector<char> ans(len, 0);
		for (int i = m, j = n; len >= 1;) {
			if (a[i - 1] == b[j - 1]) {
				ans[--len] = a[i - 1];
				i--;
				j--;
			}
			else {
				if (dp[i - 1][j] >= dp[i][j - 1]) {
					i--;
				}
				else {
					j--;
				}
			}
		}

		for (char ch : ans) {
			cout << ch;
		}

		return 0;
	}

}



//题目二：
// 最小的必要团队
// 作为项目经理，你规划了一份需求的技能清单req_skills
// 并打算从备选人员名单people中选出些人组成必要团队
// 编号为i的备选人员people[i]含有一份该备选人员掌握的技能列表
// 所谓必要团队，就是在这个团队中
// 对于所需求的技能列表req_skills中列出的每项技能，团队中至少有一名成员已经掌握
// 请你返回规模最小的必要团队，团队成员用人员编号表示
// 你可以按 任意顺序 返回答案，题目数据保证答案存在
// 测试链接 : https://leetcode.cn/problems/smallest-sufficient-team/
class Solution {
public:
	int f(vector<int>& arr, int num, int i, int status, vector<vector<int>>& dp) {
		if (status == (1 << num) - 1) return 0;
		if (i == arr.size()) return INT_MAX / 2;

		if (dp[i][status] != -1) return dp[i][status];

		int p1 = 0, p2 = 0;
		p1 = f(arr, num, i + 1, status, dp);
		p2 = 1 + f(arr, num, i + 1, status | arr[i], dp);
		dp[i][status] = min(p1, p2);

		return dp[i][status];
	}

	vector<int> smallestSufficientTeam(vector<string>& req_skills, vector<vector<string>>& people) {
		unordered_map<string, int> cnt;
		int num = 0;
		for (auto& ele : req_skills) {
			cnt[ele] = num++;
		}

		//将每个人会的技能改为位信息
		int n = people.size();
		vector<int> arr(n, 0);
		for (int i = 0; i < n; i++) {
			int s = 0;
			for (auto str : people[i]) {
				if (cnt.find(str) != cnt.end())
					s |= 1 << (cnt[str]);
			}
			arr[i] = s;
		}

		vector<vector<int>> dp(n, vector<int>((1 << num) - 1, -1));
		int size = f(arr, num, 0, 0, dp);
		vector<int> ans(size, 0);
		for (int k = 0, i = 0, s = 0; s != (1 << num) - 1; i++) {
			if (i == n - 1 || dp[i][s] != dp[i + 1][s]) { //来到最后一个人 或 选择了第i号人
				ans[k++] = i;
				s |= arr[i];
			}
		}

		return ans;
	}
};



//题目三：
// 最长递增子序列字典序最小的结果
// 给定数组arr，设长度为n
// 输出arr的最长递增子序列
// 如果有多个答案，请输出其中字典序最小的
// 注意这道题的字典序设定（根据提交的结果推论的）：
// 每个数字看作是单独的字符，比如120认为比36的字典序大
// 保证从左到右每个数字尽量小
// 测试链接 : https://www.nowcoder.com/practice/30fb9b3cab9742ecae9acda1c75bf927
// 测试链接 : https://www.luogu.com.cn/problem/T386911

//关键：
// 由于定义的dp[i]含义为：以i位置为开头，往右延伸所能得到的最长递增子序列
// 所以，使用LIS算法时要转化一下：从最后一位开始倒着求 -> 不单调递增(<=) !

//附：如果正常求LIS, dp[i]的定义是：以 nums[i] 作为终点的最长上升子序列的长度

namespace test3 {
	#include <iostream>
	#include <vector>
	#include <climits>
	using namespace std;

	vector<int> nums;
	vector<int> dp;

	// 二分的方式找<=num的最左位置
	int f(vector<int>& help, int x, int len) {
		int ans = -1;
		for (int l = 0, r = len - 1; l <= r;) {
			int m = l + (r - l) / 2;
			if (help[m] <= x) {
				ans = m;
				r = m - 1;
			}
			else {
				l = m + 1;
			}
		}
		return ans;
	}

	int lis() {
		int len = 0;
		int n = nums.size();
		vector<int> help(n, 0);
		for (int i = n - 1; i >= 0; i--) {
			int find = f(help, nums[i], len);
			if (find == -1) {
				help[len++] = nums[i];
				dp[i] = len;
			}
			else {
				help[find] = nums[i];
				dp[i] = find + 1;
			}
			
		}
		return len;
	}

	int main() {
		int n;
		cin >> n;
		nums.assign(n, 0);
		dp.assign(n, 0);
		for (int i = 0; i < n; i++)
			cin >> nums[i];

		int len = lis();
		vector<int> ans(len, INT_MAX);
		for (int i = 0; i < n; i++) {
			if (dp[i] == len) //对于dp表中值相等的两个下标，右边下标对应的数一定比左边的小
				ans[0] = nums[i];
			else {
				if (ans[len - 1 - dp[i]] < nums[i]) { //比前一个填好的数字大
					ans[len - dp[i]] = nums[i];
				}
			}
		}

		for (auto& ele : ans) {
			cout << ele << " ";
		}

		return 0;
	}

}



//题目四：
// 潜水的最大时间与方案
// 一共有n个工具，每个工具都有自己的重量a、阻力b、提升的停留时间c
// 因为背包有限，所以只能背重量不超过m的工具
// 因为力气有限，所以只能背阻力不超过v的工具
// 希望能在水下停留的时间最久
// 返回最久的停留时间和下标字典序最小的选择工具的方案
// 注意这道题的字典序设定（根据提交的结果推论的）：
// 下标方案整体构成的字符串保证字典序最小
// 比如下标方案"1 120"比下标方案"1 2"字典序小
// 测试链接 : https://www.luogu.com.cn/problem/P1759

//dp[i][j][k] : 1...i 货物自由选，重量不超过j，阻力不超过k，最大价值是多少
// 重量: a[i]  阻力: b[i]  价值: c[i]
//1.dp[i-1][j][k]
//2.dp[i-1][j-a[i]][k-b[i]] + c[i]

namespace test4 {
	#include <iostream>
	#include <vector>
	#include <string>

	using namespace std;

	const int MAXN = 101;
	const int MAXM = 201;

	int m, v, n;
	vector<int> a, b, c;
	vector<vector<vector<int>>> dp;
	vector<vector<vector<string>>> path;

	void build() {
		a.assign(MAXN, 0);
		b.assign(MAXN, 0);
		c.assign(MAXN, 0);
		dp.assign(MAXN, vector<vector<int>>(MAXM, vector<int>(MAXM, 0)));
		path.assign(MAXN, vector<vector<string>>(MAXM, vector<string>(MAXM, "")));
	}

	void compute() {
		string p2;
		for (int i = 1; i <= n; i++) {
			for (int j = 0; j <= m; j++) {
				for (int k = 0; k <= v; k++) {
					// 可能性1 : 不要i位置的货
					dp[i][j][k] = dp[i - 1][j][k];
					path[i][j][k] = path[i - 1][j][k];

					if (j >= a[i] && k >= b[i]) {
						// 可能性2 : 要i位置的货
						if (path[i - 1][j - a[i]][k - b[i]].empty()) {
							p2 = to_string(i);
						}
						else {
							p2 = path[i - 1][j - a[i]][k - b[i]] + " " + to_string(i);
						}

						if (dp[i][j][k] < dp[i - 1][j - a[i]][k - b[i]] + c[i]) {
							dp[i][j][k] = dp[i - 1][j - a[i]][k - b[i]] + c[i];
							path[i][j][k] = p2;
						}
						else if (dp[i][j][k] == dp[i - 1][j - a[i]][k - b[i]] + c[i]) {
							if (p2 < path[i][j][k]) {
								path[i][j][k] = p2;
							}
						}
					}
				}
			}
		}
	}

	int main() {
		// 优化标准输入输出速度
		ios_base::sync_with_stdio(false);
		cin.tie(NULL);

		while (cin >> m >> v >> n) {
			build();
			for (int i = 1; i <= n; i++) {
				cin >> a[i] >> b[i] >> c[i];
			}
			compute();
			cout << dp[n][m][v] << "\n" << path[n][m][v] << "\n";
		}

		return 0;
	}
}

//空间压缩版本
namespace test4_ {
#include <iostream>
#include <vector>
#include <string>
	using namespace std;

	const int MAXN = 101;
	const int MAXM = 201;

	int m, v, n;
	vector<int> a(MAXN, 0);
	vector<int> b(MAXN, 0);
	vector<int> c(MAXN, 0);
	vector<vector<int>> dp(MAXM, vector<int>(MAXM, 0));
	vector<vector<string>> path(MAXM, vector<string>(MAXM, ""));

	void f() {
		string p;
		for (int i = 1; i <= n; i++) {
			for (int j = m; j >= a[i]; j--) {
				for (int k = v; k >= b[i]; k--) {
					if (path[j - a[i]][k - b[i]].empty())
						p = to_string(i);
					else
						p = path[j - a[i]][k - b[i]] + " " + to_string(i);

					if (dp[j - a[i]][k - b[i]] + c[i] > dp[j][k]) {
						dp[j][k] = dp[j - a[i]][k - b[i]] + c[i];
						path[j][k] = p;
					}
					else if (dp[j][k] == dp[j - a[i]][k - b[i]] + c[i]) {
						if (p < path[j][k]) {
							path[j][k] = p;
						}
					}
				}
			}
		}
	}

	int main() {
		cin >> m >> v >> n;
		for (int i = 1; i <= n; i++) {
			cin >> a[i] >> b[i] >> c[i];
		}
		f();
		cout << dp[m][v] << "\n" << path[m][v];

		return 0;
	}
}

