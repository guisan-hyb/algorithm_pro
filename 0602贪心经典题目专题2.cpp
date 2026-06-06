#define _CRT_SECURE_NO_WARNINGS


#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <queue>

using namespace std;



//题目一：
// 砍竹子II
// 现需要将一根长为正整数bamboo_len的竹子砍为若干段
// 每段长度均为正整数
// 请返回每段竹子长度的最大乘积是多少
// 答案需要对 1000000007 取模
// 测试链接 : https://leetcode.cn/problems/jian-sheng-zi-ii-lcof/

//补充：乘法快速幂
//思考：为什么本题不能用动态规划？
//  ->  因为数据太大，如果不取模，即使用 long long 也会溢出；
//      如果取模，因为求的是最大值，取模会使源数据的大小关系丧失

class Solution {
public:
    const int MOD = 1e9 + 7;

	long long quickPow(long long x, int power) {
		long long ans = 1;
		while (power > 0) {
			if ((power & 1) == 1) {
				ans = (ans * x) % MOD;
			}
			x = (x * x) % MOD;
			power >>= 1;
		}
		return ans;
	}

    int cuttingBamboo(int n) {
        if (n <= 2) return 1;
        if (n == 3) return 2;

        //观察：
		// n = 4  -> 2 * 2
		// n = 5  -> 3 * 2
		// n = 6  -> 3 * 3
		// n = 7  -> 3 * 2 * 2
		// n = 8  -> 3 * 3 * 2
		// n = 9  -> 3 * 3 * 3
		// n = 10 -> 3 * 3 * 2 * 2
		// n = 11 -> 3 * 3 * 3 * 2
		// n = 12 -> 3 * 3 * 3 * 3

		int tail = n % 3 == 0 ? 1 : (n % 3 == 1 ? 4 : 2);
		int power = (tail == 1 ? n : n - tail) / 3;
		return (quickPow(3, power) * tail) % MOD;
    }
};



//题目二：
// 分成k份的最大乘积
// 一个数字n一定要分成k份，得到的乘积尽量大是多少
// 数字n和k，可能非常大，到达10^12规模
// 结果可能更大，所以返回结果对 1000000007 取模
// 来自真实大厂笔试，没有在线测试，对数器验证

//思路：联想到基本不等式，肯定数字均分得到的乘积最大

namespace test2 {
	class Solution1 {
	public:
		const int MOD = 1e9 + 7;

		long long quickPower(long long x, int power) {
			long long ans = 1;
			while (power > 0) {
				if (power & 1) {
					ans = (ans * x) % MOD;
				}
				x = (x * x) % MOD;
				power >>= 1;
			}
			return ans;
		}

		int maxValue(long long n, long long k) {
			long long a = n / k;
			long long b = n % k;

			long long part1 = quickPower(a + 1, b) % MOD;
			long long part2 = quickPower(a, k - b) % MOD;
			return (part1 * part2) % MOD;
		}
	};

	class Solution2 {
	public:
		long long f(long long n, long long k) {
			if (k == 1) {
				return n;
			}

			// 如果 n < k，说明无法分成 k 份正整数，返回 0 处理
			if (n < k) return 0;

			long long ans = LLONG_MIN;
			for (int i = 1; i <= n; i++) {
				ans = max(ans, (long long)i * f(n - i, k - 1));
			}
			return ans;
		}

		int maxValue(long long n, long long k) {
			return f(n, k);
		}
	};

	int main() {
		srand(unsigned(time(nullptr)));
		int N = 30;
		int testTimes = 2000;
		cout << "测试开始" << endl;
		for (int i = 1; i <= testTimes; i++) {
			int n = rand() % N + 1;
			int k = rand() % N + 1;
			Solution1 s1;
			Solution2 s2;
			long long ans1 = s1.maxValue(n, k);
			long long ans2 = s2.maxValue(n, k);

			if (ans1 != ans2) {
				cout << "出错了" << endl;
			}
			if (i % 100 == 0) {
				cout << "测试到第" << i << "组" << endl;
			}
		}
		cout << "测试结束" << endl;

		return 0;
	}
}



//题目三：
// 会议必须独占时间段的最大会议数量
// 给定若干会议的开始、结束时间
// 你参加某个会议的期间，不能参加其他会议
// 返回你能参加的最大会议数量
// 同学找到了Leetcode的在线测试，题意类似
// 测试链接 :https://leetcode.cn/problems/non-overlapping-intervals/
class Solution {
public:
	int eraseOverlapIntervals(vector<vector<int>>& intervals) {
		int n = intervals.size();
		//按照结束时间升序排序
		sort(intervals.begin(), intervals.end(), [](vector<int>& a, vector<int>& b) {
			return a[1] < b[1];
			});
		int ans = 0;
		for (int i = 0, cur = -50001; i < n; i++) {
			if (intervals[i][0] >= cur) {
				ans++;
				cur = intervals[i][1];
			}
		}
		return n - ans;
	}
};



//题目四：
// 会议只占一天的最大会议数量
// 给定若干会议的开始、结束时间
// 任何会议的召开期间，你只需要抽出1天来参加
// 但是你安排的那一天，只能参加这个会议，不能同时参加其他会议
// 返回你能参加的最大会议数量
// 测试链接 : https://leetcode.cn/problems/maximum-number-of-events-that-can-be-attended/

//核心：优先开最紧急的会议

class Solution {
public:
	int maxEvents(vector<vector<int>>& events) {
		sort(events.begin(), events.end(), [](vector<int>& a, vector<int>& b) {
			return a[0] < b[0];
			});
		int mins = events[0][0];
		int maxs = 0;
		for (auto& ele : events) {
			maxs = max(maxs, ele[1]);
		}

		int ans = 0, idx = 0;
		priority_queue<int, vector<int>, greater<>> heap;
		for (int day = mins; day <= maxs; day++) {
			while (idx < events.size() && events[idx][0] == day) {
				heap.push(events[idx++][1]);
			}

			while (!heap.empty() && heap.top() < day) {
				heap.pop();
			}

			if (!heap.empty()) {
				ans++;
				heap.pop();
			}
		}

		return ans;
	}
};



//题目五：
// IPO
// 给你n个项目，对于每个项目i
// 它都有一个纯利润profits[i]
// 和启动该项目需要的最小资本capital[i]
// 最初你的资本为w，当你完成一个项目时，你将获得纯利润，添加到你的总资本中
// 总而言之，从给定项目中选择最多k个不同项目的列表
// 以最大化最终资本，并输出最终可获得的最多资本
// 测试链接 : https://leetcode.cn/problems/ipo/

//核心：选力所能及最大的

class Solution {
public:
	class Info {
	public:
		int profit;
		int cap;

		Info(int p,int c):profit(p),cap(c) {}
	};

	class cmp1 {
	public:
		bool operator()(Info& a, Info& b) {
			return a.cap > b.cap;
		}
	};

	class cmp2 {
	public:
		bool operator()(Info& a, Info& b) {
			return a.profit < b.profit;
		}
	};

	int findMaximizedCapital(int k, int w, vector<int>& profits, vector<int>& capital) {
		priority_queue<Info, vector<Info>, cmp1> heap1;//按照启动资金从小到大的小根堆
		priority_queue<Info, vector<Info>, cmp2> heap2;//按照利润从大到小的大根堆

		int n = profits.size();
		for (int i = 0; i < n; i++) {
			heap1.push({ profits[i],capital[i] });
		}

		while (k > 0) {
			while (!heap1.empty() && w >= heap1.top().cap) {
				heap2.push(heap1.top());
				heap1.pop();
			}

			if (heap2.empty()) {
				break;
			}

			w += heap2.top().profit;
			heap2.pop();
			k--;
		}

		return w;
	}
};

//代码风格优化：
class Solution {
public:
	struct Info {
		int profit;
		int cap;
	};

	int findMaximizedCapital(int k, int w, vector<int>& profits, vector<int>& capital) {
		auto cmpCap = [](const Info& a, const Info& b) { return a.cap > b.cap; };
		auto cmpPro = [](const Info& a, const Info& b) { return a.profit < b.profit; };

		priority_queue<Info, vector<Info>, decltype(cmpCap)> minCap(cmpCap);
		priority_queue<Info, vector<Info>, decltype(cmpPro)> maxPro(cmpPro);

		int n = profits.size();
		for (int i = 0; i < n; i++) {
			minCap.push({ profits[i], capital[i] }); 
		}

		while (k--) {
			while (!minCap.empty() && w >= minCap.top().cap) {
				maxPro.push(minCap.top());
				minCap.pop();
			}

			if (maxPro.empty()) break;

			w += maxPro.top().profit;
			maxPro.pop();
		}

		return w;
	}
};



//题目六：
// 加入差值绝对值直到长度固定
// 给定一个非负数组arr，计算任何两个数差值的绝对值
// 如果arr中没有，都要加入到arr里，但是只加一份
// 然后新的arr继续计算任何两个数差值的绝对值，
// 如果arr中没有，都要加入到arr里，但是只加一份
// 一直到arr大小固定，返回arr最终的长度
// 来自真实大厂笔试，没有在线测试，对数器验证

//结论：从最大公约数开始，到数组中最大值为止，中间每一个数都在前一个数的基础上加最大公约数
//      这个规则下生成的数组长度即为最终答案

//根据数论中的 裴蜀定理（Bézout's identity） 推论，
// 一个非负整数集合不断求任意两个数的差值绝对值，最终能生成的所有不同元素，必定构成一个从 0 到 maxs，步长为 gcd 的等差数列。
//这个等差数列包含的不同元素个数为：maxs / gcd + 1（包含 0）。
//最终数组的长度 = 原数组长度 + 新增的不同元素个数。
//新增的不同元素个数 = 等差数列元素总个数 - 原数组中已有的不同元素个数（即 cnt.size()）。
//因此，正确公式应为：arr.size() + (maxs / gcd + 1 - cnt.size())。


namespace test6 {
	#include <iostream>
	#include <vector>
	#include <cstdlib>
	#include <unordered_set>
	#include <unordered_map>
	using namespace std;

	class Solution1 {
	public:
		bool f(vector<int>& arr, unordered_set<int>& cnt) {
			int sz = cnt.size();
			vector<int> newDiff; //用数组暂存本轮发现的新差值，等本轮遍历完再统一加入
			for (int i = 0; i < arr.size(); i++) {
				for (int j = i + 1; j < arr.size(); j++) {
					int diff = abs(arr[i] - arr[j]);
					if (!cnt.count(diff)) {
						cnt.insert(diff);
						newDiff.push_back(diff); 
					}
				}
			}

			for (int d : newDiff) {
				arr.push_back(d);
			}
			return cnt.size() == sz;
		}

		int len(vector<int>& arr) {
			unordered_set<int> cnt(arr.begin(), arr.end());
			while (!f(arr, cnt));
			return arr.size();
		}
	};

	class Solution2 {
	public:
		int gcd_(int a, int b) {
			return b == 0 ? a : gcd_(b, a % b);
		}

		int len(vector<int>& arr) {
			if (arr.size() < 2) return arr.size();

			unordered_map<int, int> cnt;
			int g = 0;
			int maxs = 0;
			for (auto& ele : arr) {
				maxs = max(maxs, ele);
				g = gcd_(g, ele);
				cnt[ele]++;
			}

			if (g == 0) return arr.size(); // 全是 0

			int U = cnt.size();
			// 判断最终集合是否包含 0
			bool hasZero = (cnt.count(0) > 0) || (U < arr.size()); // 有重复一定会产生0

			int sequenceLength = maxs / g + (hasZero ? 1 : 0);
			return arr.size() + (sequenceLength - U);
		}
	};

	vector<int> randomArray(int n, int V) {
		vector<int> test(n, 0);
		for (int i = 0; i < n; i++) {
			test[i] = rand() % V;
		}

		return test;
	}

	int main() {
		srand((unsigned int)time(nullptr));

		int N = 50;
		int V = 100;
		int testTimes = 20000;
		cout << "测试开始" << endl;
		for (int i = 1; i <= testTimes; i++) {
			int n = rand() % N + 1;
			vector<int> test = randomArray(n, V);
			Solution1 s1;
			Solution2 s2;
			int ans1 = s1.len(test);
			int ans2 = s2.len(test);
			if (ans1 != ans2) {
				cout << "出错了" << endl;
			}
		}
		cout << "测试结束" << endl;

		return 0;
	}
}

