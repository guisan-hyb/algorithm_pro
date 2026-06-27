#define _CRT_SECURE_NO_WARNINGS


#include <iostream>
#include <vector>

using namespace std;


//前置：乘法快速幂
//矩阵乘法、矩阵快速幂，题目2、3、4、5、6、7
//固定关系的1维k阶递推表达式，用矩阵快速幂求解时间复杂度O(logn * k的3次方)，题目2、3、4、5
//固定关系的k维1阶递推表达式，用矩阵快速幂求解时间复杂度O(logn * k的3次方)，题目6、7

//矩阵快速幂只能解决严格关系的递推，有条件转移的不行


//题目一：
// 乘法快速幂模版
// 求a的b次方，对p取模的结果
// 测试链接 : https://www.luogu.com.cn/problem/P1226

//时间复杂度 O(logn)

namespace test1 {
	#include <iostream>
	using namespace std;

	long long quickPower(long long a, long long b, long long p) {
		long long ans = 1;
		while (b > 0) {
			if ((b & 1) == 1) {
				ans = (ans * a) % p;
			}
			a = (a * a) % p;
			b >>= 1;
		}
		return ans;
	}

	int main() {
		long long a, b, p;
		cin >> a >> b >> p;
		long long s = quickPower(a, b, p);
		cout << a << "^" << b << " mod " << p << "=" << s << endl;

		return 0;
	}
}



//题目二：
// 矩阵乘法
// 矩阵快速幂
// 矩阵快速幂解决斐波那契第n项的问题

//关系矩阵的第0列直接由递推表达式确定，剩下的项可以用前面的初始项带入求出

namespace test2 {
	class Solution {
	public:
		// 矩阵相乘
		// a的列数一定要等于b的行数
		vector<vector<int>> multiply(vector<vector<int>>& a, vector<vector<int>>& b) {
			int ma = a.size(), na = a[0].size();
			int mb = b.size(), nb = b[0].size();
			vector<vector<int>> ret(ma, vector<int>(nb, 0));
			for (int i = 0; i < ma; i++) {
				for (int j = 0; j < nb; j++) {
					for (int k = 0; k < na; k++) {
						ret[i][j] += a[i][k] * b[k][j];
					}
				}
			}
			return ret;
		}

		// 矩阵快速幂
		// 要求矩阵m是正方形矩阵
		vector<vector<int>> quickPower(vector<vector<int>>& m, int p) {
			int n = m.size();
			// 对角线全是1、剩下数字都是0的正方形矩阵，称为单位矩阵
			// 相当于正方形矩阵中的1，矩阵a * 单位矩阵 = 矩阵a
			vector<vector<int>> ans(n, vector<int>(n, 0));
			for (int i = 0; i < n; i++) {
				ans[i][i] = 1;
			}

			for (; p > 0; p >>= 1) {
				if ((p & 1) == 1) {
					ans = multiply(ans, m);
				}
				m = multiply(m, m);
			}
			return ans;
		}
	};

	void print(vector<vector<int>>& m) {
		for (int i = 0; i < m.size(); i++) {
			for (int j = 0; j < m[0].size(); j++) {
				cout << m[i][j] << " ";
			}
			cout << endl;
		}
	}

    int main() {
		vector<vector<int>> test = { {1,2},{3,4} };

		Solution solution;
		vector<vector<int>> result1 = solution.quickPower(test, 3);
		print(result1);
		cout << "==========" << endl;
		auto t1 = solution.multiply(test, test);
		auto t2 = solution.multiply(t1, test);
		print(t2);

		return 0;
    }
}



// 求斐波那契数列第n项
// 测试链接 : https://leetcode.cn/problems/fibonacci-number/
// 这个测试的数据量太小，并且不牵扯取模的事情
// 所以矩阵快速幂看不出优势
class Solution {
public:
	vector<vector<int>> multiply(vector<vector<int>>& a, vector<vector<int>>& b) {
		int ma = a.size(), na = a[0].size();
		int mb = b.size(), nb = b[0].size();
		vector<vector<int>> ret(ma, vector<int>(nb, 0));
		for (int i = 0; i < ma; i++) {
			for (int j = 0; j < nb; j++) {
				for (int k = 0; k < na; k++) {
					ret[i][j] += a[i][k] * b[k][j];
				}
			}
		}
		return ret;
	}

	vector<vector<int>> quickPower(vector<vector<int>>& m, int p) {
		int n = m.size();
		vector<vector<int>> ret(n, vector<int>(n, 0));
		for (int i = 0; i < n; i++) {
			ret[i][i] = 1;
		}

		for (; p > 0; p >>= 1) {
			if ((p & 1) == 1) {
				ret = multiply(ret, m);
			}
			m = multiply(m, m);
		}
		return ret;
	}


	int fib(int n) {
		if (n == 0) return 0;
		if (n == 1) return 1;

		vector<vector<int>> start = { {1, 0} }; //f(n-1) f(n-2) -> f(1) f(0)
		vector<vector<int>> base = {  //关系矩阵
			{1, 1},
			{1, 0}
		};
		//					1     1
		// f(n-1) f(n-2)    1     0
		//                 f(n) f(n-1)

		vector<vector<int>> ret = quickPower(base, n - 1);
		vector<vector<int>> ans = multiply(start, ret);
		return ans[0][0];
	}
};



//题目三：
// 爬楼梯
// 假设你正在爬楼梯，每次你可以爬1或2个台阶
// 你有多少种不同的方法可以爬到n层
// 测试链接 : https://leetcode.cn/problems/climbing-stairs/
class Solution {
public:
	vector<vector<long long>> multiply(vector<vector<long long>>& a, vector<vector<long long>>& b) {
		int ma = a.size(), na = a[0].size();
		int mb = b.size(), nb = b[0].size();
		vector<vector<long long>> ret(ma, vector<long long>(nb, 0));
		for (int i = 0; i < ma; i++) {
			for (int j = 0; j < nb; j++) {
				for (int k = 0; k < na; k++) {
					ret[i][j] += a[i][k] * b[k][j];
				}
			}
		}
		return ret;
	}

	vector<vector<long long>> quickPower(vector<vector<long long>>& m, int p) {
		int n = m.size();
		vector<vector<long long>> ret(n, vector<long long>(n, 0));
		for (int i = 0; i < n; i++) {
			ret[i][i] = 1;
		}

		for (; p > 0; p >>= 1) {
			if ((p & 1) == 1) {
				ret = multiply(ret, m);
			}
			m = multiply(m, m);
		}
		return ret;
	}

	int climbStairs(int n) {
		if (n == 0 || n == 1) return 1;

		vector<vector<long long>> start = { {1, 1} };// f(n-1) f(n-2)
		vector<vector<long long>> base = {
			{1, 1},
			{1, 0}
		};
		vector<vector<long long>> ret = quickPower(base, n - 1);
		vector<vector<long long>> ans = multiply(start, ret);
		return ans[0][0];
	}
};



//题目四：
// 第n个泰波那契数
// t(0) = 0, t(1) = 1, t(2) = 1
// t(i) = t(i-1) + t(i-2) + t(i-3)
// 求t(n)
// 测试链接 : https://leetcode.cn/problems/n-th-tribonacci-number/
// 这个测试的数据量太小，并且不牵扯取模的事情
// 所以矩阵快速幂看不出优势
class Solution {
public:
	vector<vector<long long>> multiply(vector<vector<long long>>& a, vector<vector<long long>>& b) {
		int ma = a.size(), na = a[0].size();
		int mb = b.size(), nb = b[0].size();
		vector<vector<long long>> ret(ma, vector<long long>(nb, 0));
		for (int i = 0; i < ma; i++) {
			for (int j = 0; j < nb; j++) {
				for (int k = 0; k < na; k++) {
					ret[i][j] += a[i][k] * b[k][j];
				}
			}
		}
		return ret;
	}

	vector<vector<long long>> quickPower(vector<vector<long long>>& m, int p) {
		int n = m.size();
		vector<vector<long long>> ret(n, vector<long long>(n, 0));
		for (int i = 0; i < n; i++) {
			ret[i][i] = 1;
		}

		for (; p > 0; p >>= 1) {
			if ((p & 1) == 1) {
				ret = multiply(ret, m);
			}
			m = multiply(m, m);
		}
		return ret;
	}

	int tribonacci(int n) {
		if (n == 0) return 0;
		if (n == 1 || n == 2) return 1;

		vector<vector<long long>> start = {
			{1, 1, 0}
		};
		vector<vector<long long>> base = {
			{1, 1, 0},
			{1, 0, 1},
			{1, 0, 0}
		};

		vector<vector<long long>> ret = quickPower(base, n - 2);
		vector<vector<long long>> ans = multiply(start, ret);
		return ans[0][0];
	}
};



//题目五：
// 多米诺和托米诺平铺
// 有两种形状的瓷砖，一种是2*1的多米诺形，另一种是形如"L"的托米诺形
// 两种形状都可以旋转，给定整数n，返回可以平铺2*n的面板的方法数量
// 返回对1000000007取模的值
// 测试链接 : https://leetcode.cn/problems/domino-and-tromino-tiling/

//法一：暴力方法
//用于打表找规律
namespace test5 {
	class Solution {
	public:
		// 如果h==0，返回2*n的区域铺满的方法数
		// 如果h==1，返回1 + 2*n的区域铺满的方法数
		int f(int n, int h) {
			if (n == 0) return h == 0 ? 1 : 0;
			if (n == 1) return 1;

			if (h == 1) {
				return f(n - 1, 1) + f(n - 1, 0);
			}
			else {
				return f(n - 1, 0) + f(n - 2, 0) + 2 * f(n - 2, 1);
			}
		}

		int numTilings(int n) {
			return f(n, 0);
		}
	};

	// f(1) = 1
	// f(2) = 2
	// f(3) = 5
	// 前三项是初始项
	// f(4) = 11
	// f(n) = 2 * f(n-1) + f(n-3)
	// 打表或者公式化简都可以发现规律，这里推荐打表找规律
	int main() {
		for (int i = 1; i <= 9; i++) {
			cout << "铺满 2 * " << i << " 的区域方法数 ：" << Solution().numTilings(i) << endl;
		}

		return 0;
	}
}

//法二：正式方法
class Solution {
public:
	const int MOD = 1e9 + 7;

	vector<vector<int>> multiply(vector<vector<int>>& a, vector<vector<int>>& b) {
		int ma = a.size(), na = a[0].size();
		int mb = b.size(), nb = b[0].size();
		vector<vector<int>> ret(ma, vector<int>(nb, 0));
		for (int i = 0; i < ma; i++) {
			for (int j = 0; j < nb; j++) {
				for (int k = 0; k < na; k++) {
					ret[i][j] = (ret[i][j] + ((long long)a[i][k] * b[k][j]) % MOD) % MOD;
				}
			}
		}
		return ret;
	}

	vector<vector<int>> quickPower(vector<vector<int>>& m, int p) {
		int n = m.size();
		vector<vector<int>> ret(n, vector<int>(n, 0));
		for (int i = 0; i < n; i++) {
			ret[i][i] = 1;
		}

		for (; p > 0; p >>= 1) {
			if ((p & 1) == 1) {
				ret = multiply(ret, m);
			}
			m = multiply(m, m);
		}
		return ret;
	}

	int f(int n) {
		if (n == 0) return 1;
		if (n == 1) return 2;
		if (n == 2) return 5;

		vector<vector<int>> start = { {5,2,1} };
		vector<vector<int>> base = {
			{2, 1, 0},
			{0, 0, 1},
			{1, 0, 0}
		};
		vector<vector<int>> tmp = quickPower(base, n - 2);
		vector<vector<int>> ans = multiply(start, tmp);
		return ans[0][0];
	}

	int numTilings(int n) {
		// 1 2 5
		// 1 2 3

		// 1 2 5
		// 0 1 2

		return f(n - 1); //把每一项都往前移，方便计算，所以这里取n-1
	}
};



//题目六：
// 统计元音字母序列的数目
// 给你一个整数n，请你帮忙统计一下我们可以按下述规则形成多少个长度为n的字符串：
// 字符串中的每个字符都应当是小写元音字母（'a', 'e', 'i', 'o', 'u'）
// 每个元音 'a' 后面都只能跟着 'e'
// 每个元音 'e' 后面只能跟着 'a' 或者是 'i'
// 每个元音 'i' 后面 不能 再跟着另一个 'i'
// 每个元音 'o' 后面只能跟着 'i' 或者是 'u'
// 每个元音 'u' 后面只能跟着 'a'
// 由于答案可能会很大，结果对1000000007取模
// 测试链接 : https://leetcode.cn/problems/count-vowels-permutation/

//分析：
// 结尾   前一个
//   a      e,u,i
//   e      a,i
//   i      e,o
//   o      i
//   u      i,o

// dp[i][ch] : 0~i范围上,i位置字符为ch的答案
//dp[i][a] = dp[i-1][e,i,u]
//dp[i][e] = dp[i-1][a,i]
//dp[i][I] = dp[i-1][e,o]
//dp[i][o] = dp[i-1][I]
//dp[i][u] = dp[i][I,o]

//                        a  0 1 0 0 0
//                        e  1 0 1 0 0
//                        i  1 1 0 1 1
//                        o  0 0 1 0 1
//                        u  1 0 0 0 0
// [a] [e] [i] [o] [u]       a e i o u

class Solution {
public:
	const int MOD = 1e9 + 7;

	vector<vector<int>> multiply(vector<vector<int>>& a, vector<vector<int>>& b) {
		int ma = a.size(), na = a[0].size();
		int nb = b[0].size();
		vector<vector<int>> ret(ma, vector<int>(nb, 0));
		for (int i = 0; i < ma; i++) {
			for (int j = 0; j < nb; j++) {
				for (int k = 0; k < na; k++) {
					ret[i][j] = (ret[i][j] + ((long long)a[i][k] * b[k][j]) % MOD) % MOD;
				}
			}
		}
		return ret;
	}

	vector<vector<int>> quickPower(vector<vector<int>>& m, int p) {
		int n = m.size();
		vector<vector<int>> ret(n, vector<int>(n, 0));
		for (int i = 0; i < n; i++) {
			ret[i][i] = 1;
		}

		for (; p > 0; p >>= 1) {
			if ((p & 1) == 1) {
				ret = multiply(ret, m);
			}
			m = multiply(m, m);
		}
		return ret;
	}

	int countVowelPermutation(int n) {
		vector<vector<int>> start = { {1,1,1,1,1} };
		vector<vector<int>> base = {
			{0,1,0,0,0},
			{1,0,1,0,0},
			{1,1,0,1,1},
			{0,0,1,0,1},
			{1,0,0,0,0}
		};
		vector<vector<int>> tmp = quickPower(base, n - 1);
		vector<vector<int>> ret = multiply(start, tmp);
		int ans = 0;
		for (int i = 0; i < ret[0].size(); i++) {
			ans = (ans + ret[0][i]) % MOD;
		}
		return ans;
	}
};

//补个记忆化搜索：
//dp[i][ch]: i~n-1位置上填字符，当前字符为ch
class Solution {
public:
	const int MOD = 1e9 + 7;

	int f(int n, int i, int ch, vector<vector<int>>& dp) {
		if (i == n - 1) return 1;//注意到 n-1 这里就停止了，肯定有1种方法
		if (dp[i][ch] != -1) return dp[i][ch];

		int ans = 0;
		if (ch == 0) {          // a -> e
			ans = f(n, i + 1, 1, dp);
		}
		else if (ch == 1) {   // e -> a, i
			ans = (f(n, i + 1, 0, dp) + f(n, i + 1, 2, dp)) % MOD;
		}
		else if (ch == 2) {   // i -> a, e, o, u (不能是 i)
			for (int j = 0; j < 5; j++) {
				if (j != 2) {
					ans = (ans + f(n, i + 1, j, dp)) % MOD;
				}
			}
		}
		else if (ch == 3) {   // o -> i, u
			ans = (f(n, i + 1, 2, dp) + f(n, i + 1, 4, dp)) % MOD;
		}
		else {                // u -> a
			ans = f(n, i + 1, 0, dp);
		}
		return dp[i][ch] = ans;
	}

	int countVowelPermutation(int n) {
		vector<vector<int>> dp(n, vector<int>(5, -1));
		int ans = 0;
		for (int i = 0; i < 5; i++) {
			ans = (ans + f(n, 0, i, dp)) % MOD;
		}
		return ans;
	}
};



//题目七：
// 学生出勤记录II
// 可以用字符串表示一个学生的出勤记录，其中的每个字符用来标记当天的出勤情况（缺勤、迟到、到场）
// 记录中只含下面三种字符：
// 'A'：Absent，缺勤
// 'L'：Late，迟到
// 'P'：Present，到场
// 如果学生能够 同时 满足下面两个条件，则可以获得出勤奖励：
// 按 总出勤 计，学生缺勤（'A'）严格 少于两天
// 学生 不会 存在 连续 3 天或 连续 3 天以上的迟到（'L'）记录。
// 给你一个整数n，表示出勤记录的长度（次数）
// 请你返回记录长度为n时，可能获得出勤奖励的记录情况数量
// 答案可能很大，结果对1000000007取模
// 测试链接 : https://leetcode.cn/problems/student-attendance-record-ii/

//dp[i][a][b]: 前i天中，有a天缺勤，且最后正好有b天连续迟到的情况下，所有可能的合法记录数量
//             a<=1, b<=2

//三维转二维：  为方便表示成矩阵乘法，首先把数组改成二维的
//dp[i][a][b] : dp[i][a*3+b]
//
//[i][0][0] -> [i][0] -> 0缺，0天
//[i][0][1] -> [i][1] -> 0缺，1天
//[i][0][2] -> [i][2] -> 0缺，2天
//[i][1][0] -> [i][3] -> 1缺，0天
//[i][1][1] -> [i][4] -> 1缺，1天
//[i][1][2] -> [i][5] -> 1缺，2天

//dp[i][0] = dp[i-1][0,1,2]
//dp[i][1] = dp[i-1][0]
//dp[i][2] = dp[i-1][1]
//dp[i][3] = dp[i-1][0,1,2,3,4,5]
//dp[i][4] = dp[i-1][3]
//dp[i][5] = dp[i-1][4]

//    0   1 1 0 1 0 0
//    1   1 0 1 1 0 0
//    2   1 0 0 1 0 0
//    3   0 0 0 1 1 0
//    4   0 0 0 1 0 1
//    5   0 0 0 1 0 0
// 
//        0 1 2 3 4 5

class Solution {
public:
	const int MOD = 1e9 + 7;

	vector<vector<int>> multiply(vector<vector<int>>& a, vector<vector<int>>& b) {
		int ma = a.size(), na = a[0].size();
		int nb = b[0].size();
		vector<vector<int>> ret(ma, vector<int>(nb, 0));
		for (int i = 0; i < ma; i++) {
			for (int j = 0; j < nb; j++) {
				for (int k = 0; k < na; k++) {
					ret[i][j] = (ret[i][j] + ((long long)a[i][k] * b[k][j]) % MOD) % MOD;
				}
			}
		}
		return ret;
	}

	vector<vector<int>> quickPower(vector<vector<int>>& m, int p) {
		int n = m.size();
		vector<vector<int>> ret(n, vector<int>(n, 0));
		for (int i = 0; i < n; i++) {
			ret[i][i] = 1;
		}

		for (; p > 0; p >>= 1) {
			if ((p & 1) == 1) {
				ret = multiply(ret, m);
			}
			m = multiply(m, m);
		}
		return ret;
	}

	int checkRecord(int n) {
		vector<vector<int>> start = { {1,1,0,1,0,0} };
		//                             0 1 2 3 4 5
		vector<vector<int>> base = {
			{1,1,0,1,0,0},
			{1,0,1,1,0,0},
			{1,0,0,1,0,0},
			{0,0,0,1,1,0},
			{0,0,0,1,0,1},
			{0,0,0,1,0,0}
		};

		vector<vector<int>> tmp = quickPower(base, n - 1);
		vector<vector<int>> ret = multiply(start, tmp);
		int ans = 0;
		for (int i = 0; i < ret[0].size(); i++) {
			ans = (ans + ret[0][i]) % MOD;
		}
		return ans;
	}
};


//补个记忆化搜索
class Solution {
public:
	const int MOD = 1e9 + 7;

	int f(int i, int a, int b, vector<vector<vector<int>>>& dp) {
		if (a >= 2 || b >= 3) return 0;
		if (i < 0) return 1;// 所有位置填完，且状态合法 -> 1 种方式

		if (dp[i][a][b] != -1) return dp[i][a][b];
		int ans = 0;
		//1.放 P
		ans = f(i - 1, a, 0, dp);
		//2.放 A
		ans = (ans + f(i - 1, a + 1, 0, dp)) % MOD;
		//3.放 L
		ans = (ans + f(i - 1, a, b + 1, dp)) % MOD;

		dp[i][a][b] = ans;
		return ans;
	}

	int checkRecord(int n) {
		vector<vector<vector<int>>> dp(n, vector<vector<int>>(2, vector<int>(3, -1)));
		return f(n - 1, 0, 0, dp);
	}
};

