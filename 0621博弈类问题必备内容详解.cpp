#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>

using namespace std;



//博弈专题分为上、下两期

//上期为经典博弈问题的讲解：
//巴什博弈(Bash)、尼姆博弈(Nim)、斐波那契博弈(Fibonacci)、威佐夫博弈(Wythoff)
//通过这些讲解会发现，这些博弈问题在考场上要临时想清楚是不太可能的，所以需要下期内容

//下期为SG函数、SG定理的内容，大多数博弈类问题都可以根据SG定理来解决
//这才是最重要的！因为你不可能学完所有的博弈，但是你能具备解决博弈类问题的通用技巧


//博弈类问题大致分为：公平组合游戏、非公平组合游戏（绝大多数的棋类游戏）、反常游戏

//只需要关注公平组合游戏(ICG)，反常游戏是公平组合游戏的变形，经济类博弈也不是课程所讨论的范围
//1.两个玩家轮流行动且游戏方式一致
//2.两个玩家对状况完全了解
//3.游戏一定会在有限步数内分出胜负
//4.游戏以玩家无法行动结束

//博弈的双方都被认为是神之个体，因为所有玩家对状况完全了解，且充分为自己打算，绝对理性
//当局面确定，结果必然注定，并且没有任何随机的成分
//游戏中的每一个状态，最终导致的结果也必然注定，只有必胜态、必败态，两种状态
// 
//这一类博弈问题的结果没有任何意外，一方可以通过努力去改变结果是不可能的，这一点是反直觉的

//常用对数器打表来找规律





//博弈类问题必备内容详解--上


//题目一：
// 巴什博弈(Bash Game)
// 一共有n颗石子，两个人轮流拿，每次可以拿1~m颗石子
// 拿到最后一颗石子的人获胜，根据n、m返回谁赢

//结论：
//如果 n != (m+1)的整数倍，那么先手赢
//如果 n == (m+1)的整数倍，那么后手赢

namespace test1 {
	class Solution1 {
	public:
		string bashGame(int n, int m) {
			return n % (m + 1) != 0 ? "先手" : "后手";
		}
	};

	class Solution2 {
	public:
		//dp[rest]: 当前玩家为先手，最终的情况是什么
		//返回值 "先手"：在这里代表的真实含义是 “当前玩家能赢（必胜）”
		//返回值 "后手"：在这里代表的真实含义是 “当前玩家会输（必败）”
		string f(int m, int rest, vector<string>& dp) {
			if (rest == 0) return "后手";
			if (dp[rest] != "") return dp[rest];

			string ans = "后手";
			//当前玩家可以拿 1 到 m 个物品。拿完之后，剩余 rest - pick 个物品，轮到对手面对 rest - pick 个物品
			for (int pick = 1; pick <= m; pick++) {
				if (rest - pick < 0) break;

				if (f(m, rest - pick, dp) == "后手") {
					//如果对手面对 rest - pick 个物品时，结果是 "后手"（即对手输了），那就意味着我赢了
					ans = "先手";
					break;
				}
			}

			dp[rest] = ans;
			return ans;
		}

		string bashGame(int n, int m) {
			vector<string> dp(n + 1, "");
			return f(m, n, dp);
		}
	};

	int main() {
		srand((unsigned int)time(nullptr));

		int V = 500;
		int testTimes = 2000;
		cout << "测试开始" << endl;
		for (int i = 1; i <= testTimes; i++) {
			int n = rand() % V;
			int m = rand() % V + 1;
			string ans1 = Solution1().bashGame(n, m);
			string ans2 = Solution2().bashGame(n, m);
			if (ans1 != ans2) {
				cout << "出错了" << endl;
			}

			if (i % 100 == 0) {
				cout << "当前测试到第 " << i << " 组" << endl;
			}
		}
		cout << "测试结束" << endl;

		return 0;
	}
}



//题目二：
// 质数次方版取石子(巴什博弈扩展)
// 一共有n颗石子，两个人轮流拿
// 每一轮当前选手可以拿 p的k次方 颗石子
// 当前选手可以随意决定p和k，但要保证p是质数、k是自然数
// 拿到最后一颗石子的人获胜
// 根据石子数返回谁赢
// 如果先手赢，返回"October wins!"
// 如果后手赢，输出"Roy wins!"
// 测试链接 : https://www.luogu.com.cn/problem/P4018
// (注: 0算自然数)

//结论：任意6的整数倍，一定不由质数的自然次方得到
// n % 6 != 0 ,先手赢
// n % 6 != 0 ,后手赢

namespace test2 {
	#include <iostream>
	using namespace std;
	int main() {
		int t;
		cin >> t;
		for (int i = 0; i < t; i++) {
			int n;
			cin >> n;
			cout << (n % 6 != 0 ? "October wins!" : "Roy wins!") << endl;
		}

		return 0;
	}
}



//题目三：
// 尼姆博弈(Nim Game)
// 一共有n堆石头，两人轮流进行游戏
// 在每个玩家的回合中，玩家需要选择任何一个非空的石头堆，并从这堆石头中移除任意正数的石头数量
// 谁先拿走最后的石头就获胜，返回最终谁会获胜
// 测试链接 : https://www.luogu.com.cn/problem/P2197

//结论：
//所有的数字异或起来 不等于 0 ，先手赢
//所有的数字异或起来   等于 0 ，后手赢

//思路；
//输的状态是什么：所有石头堆中石头的数量都是0 -> 所有数异或起来为0
//先手面对一个所有数字异或起来不等于0的状态，一定可以通过自己的行动让后手面对所有数字异或起来为0的状态
//不论后手进行什么操作，面对所有数字异或起来为0的状态，交还给先手的时候，一定会变成所有数字异或起来不为0的状态
//
//也就是说，所有数字异或起来不等于0 为 必胜态
//          所有数字异或起来等于0 为必败态

namespace test3 {
	#include <iostream>
	using namespace std;

	int main() {
		int t;
		cin >> t;
		for (int i = 0; i < t; i++) {
			int n;
			cin >> n;
			int eor = 0;
			for (int j = 0; j < n; j++) {
				int a;
				cin >> a;
				eor ^= a;
			}
			cout << (eor != 0 ? "Yes" : "No") << endl;
		}

		return 0;
	}
}



//题目四：
// 反尼姆博弈(反常游戏)
// 一共有n堆石头，两人轮流进行游戏
// 在每个玩家的回合中，玩家需要选择任何一个非空的石头堆，并从这堆石头中移除任意正数的石头数量
// 谁先拿走最后的石头就失败，返回最终谁会获胜
// 先手获胜，打印John
// 后手获胜，打印Brother
// 测试链接 : https://www.luogu.com.cn/problem/P4279

//实际上你有能力让对手下一次没法取，就有能力留唯一一个让他取，但是全是1的情况除外
//1.全是1：看1的数量的奇偶
//2.不全是1：
//如果异或起来 != 0 , 先手赢 ； 如果异或起来 == 0 ，后手赢
//（经典尼姆博弈是让后手最先撞上输的状态；而此时的反常游戏，是让先手最先撞上能赢的状态）

namespace test {
	#include <iostream>
	#include <vector>
	using namespace std;

	int main() {
		int t;
		cin >> t;
		for (int i = 0; i < t; i++) {
			int n;
			cin >> n;
			int eor = 0;
			int sum = 0;
			for (int j = 0; j < n; j++) {
				int x;
				cin >> x;
				eor ^= x;
				sum += x;
			}
			if (sum == n) {
				cout << (sum & 1 ? "Brother" : "John") << endl;
			}
			else {
				cout << (eor != 0 ? "John" : "Brothrt") << endl;
			}
		}

		return 0;
	}
}



//题目五：
// 斐波那契博弈(Fibonacci Game + Zeckendorf定理)
// 一共有n枚石子，两位玩家定了如下规则进行游戏：
// 先手后手轮流取石子，先手在第一轮可以取走任意的石子
// 接下来的每一轮当前的玩家最少要取走一个石子，最多取走上一次取的数量的2倍
// 当然，玩家取走的数量必须不大于目前场上剩余的石子数量，双方都以最优策略取石子
// 你也看出来了，根据规律先手一定会获胜，但是先手想知道
// 第一轮自己取走至少几颗石子就可以保证获胜了
// 测试链接 : https://www.luogu.com.cn/problem/P6487



//题目六：
// 威佐夫博弈(Wythoff Game)
// 有两堆石子，数量任意，可以不同，游戏开始由两个人轮流取石子
// 游戏规定，每次有两种不同的取法
// 1) 在任意的一堆中取走任意多的石子
// 2) 可以在两堆中同时取走相同数量的石子
// 最后把石子全部取完者为胜者
// 现在给出初始的两堆石子的数目，返回先手能不能获胜
// 测试链接 : https://www.luogu.com.cn/problem/P2252









//博弈类问题必备内容详解--下


//图游戏的概念
//任何局面都认为是图中的点，每一个局面都可以通过一种行动，走向图中的下一个点
//如果当前行动有若干个，那么后继节点就有若干个。最终，必败局面的点认为不再有后继节点
//那么公平组合游戏(ICG)，就可以对应成一张图

//eg: 1 <- 2 <- 3 <- 4  , 则1，2，3为4的后继节点

//SG函数(Sprague-Grundy函数)，如下是SG返回值的求解方式，俗称mex过程
//最终必败点是A，规定 SG(A) = 0
//假设状态点是B，那么 SG(B) = 查看B所有后继节点的sg值，其中没有出现过的最小自然数
// SG(B) != 0，那么状态B为必胜态；SG(B) == 0，那么状态B为必胜态

//SG定理(Bouton定理)
//如果一个ICG游戏(总)，由若干个独立的ICG子游戏构成(分1、分2、分3...)，那么：
// SG(总) = SG(分1)^SG(分2)^SG(分3)...       任何ICG游戏都是如此，正确性证明类似尼姆博弈
//当数据规模较大时，要善于通过对数器的手段，打印SG表并观察，看看能不能发现简洁规律



//mex函数：给定一个集合，求出不属于该集合的最小非负整数。
//例如：mex({ 0, 1, 2 }) = 3
//例如：mex({ 0, 2 }) = 1
//例如：mex({ 1, 2 }) = 0
//
//SG函数：对于一个游戏状态x，它的 SG 值定义为它所有 下一步状态（后续状态） 的 SG 值的 mex 值。
//公式：sg[x] = mex({ sg[y] | y 是 x 的后续状态 })
//
//SG定理：如果一个游戏由多个独立的子游戏组成（比如 Nim 游戏中有好几堆石子），整个游戏的 SG 值等于所有子游戏 SG 值的异或和。
//如果最终 SG != 0，先手必胜。
//如果最终 SG == 0，后手必胜。

//填 SG 表的过程本质上就是一个非常标准的动态规划过程
// 
//定义 sg[i] 表示当前游戏状态为 i 时的 SG 函数值
// 
//sg[i] = mex({ sg[j] | j 是 i 的后续状态 })。这正是 SG 问题的状态转移方程！
//它通过后续状态的 SG 值，经过 mex 运算推导出当前状态的 SG 值
//
//边界：没有后续状态的“必败态”（比如石子数为 0），它的 SG 值直接定义为 sg[0] = 0



//题目一：
// 巴什博弈(SG函数求解过程展示)
// 一共有n颗石子，两个人轮流拿，每次可以拿1~m颗石子
// 拿到最后一颗石子的人获胜，根据n、m返回谁赢
// 对数器验证

namespace test1_ {
	class Solution1 {
	public:
		string bashGame(int n, int m) {
			return n % (m + 1) != 0 ? "先手" : "后手";
		}
	};

	class Solution2 {
	public:
		string bashGame(int n, int m) {
			vector<int> sg(n + 1, 0);
			vector<int> appear(m + 1, false);
			for (int i = 1; i <= n; i++) {
				fill(appear.begin(), appear.end(), false);
				for (int j = 1; j <= m && i - j >= 0; j++) {
					appear[sg[i - j]] = true;
				}
				for (int s = 0; s <= m; s++) {
					if (!appear[s]) {
						sg[i] = s;
						break;
					}
				}
			}

			/*cout << "打印 n = " << n << ", m = " << m << " 的sg表" << endl;
			for (int i = 0; i <= n; i++) {
				cout << "sg[" << i << "] : " << sg[i] << endl;
			}*/

			return sg[n] != 0 ? "先手" : "后手";
		}
	};

	int main() {
		srand((unsigned int)time(nullptr));

		int V = 500;
		int testTimes = 2000;
		cout << "测试开始" << endl;
		for (int i = 1; i <= testTimes; i++) {
			int n = rand() % V;
			int m = rand() % V + 1;
			string ans1 = Solution1().bashGame(n, m);
			string ans2 = Solution2().bashGame(n, m);
			if (ans1 != ans2) {
				cout << "出错了" << endl;
			}

			if (i % 100 == 0) {
				cout << "当前测试到第 " << i << " 组" << endl;
			}
		}
		cout << "测试结束" << endl;

		return 0;
	}
}



//题目二：
// 尼姆博弈(SG定理简单用法展示)
// 一共有 n 堆石头，两人轮流进行游戏
// 在每个玩家的回合中，玩家需要 选择任一 非空 石头堆，从中移除任意 非零 数量的石头
// 如果不能移除任意的石头，就输掉游戏
// 返回先手是否一定获胜
// 对数器验证

namespace test2_ {
	class Solution1 {
	public:
		string nim(vector<int>& arr) {
			int eor = 0;
			for (auto& ele : arr) {
				eor ^= ele;
			}

			return eor != 0 ? "先手" : "后手";
		}
	};

	class Solution2 {
	public:
		string nim(vector<int>& arr) {
			int maxs = 0;
			for (auto& ele : arr) {
				maxs = max(maxs, ele);
			}

			vector<int> sg(maxs + 1, 0);
			vector<int> appear(maxs + 1, false);
			for (int i = 1; i <= maxs; i++) {
				fill(appear.begin(), appear.end(), false);
				for (int j = 0; j < i; j++) {
					appear[sg[j]] = true;
				}
				for (int s = 0; s <= maxs; s++) {
					if (!appear[s]) {
						sg[i] = s;
						break;
					}
				}
			}

			/*for (int i = 0; i <= maxs; i++) {
				cout << sg[i] << endl;
			}*/

			// 打印sg表之后，可以发现，sg[x] = x
			// 那么eor ^= sg[num] 等同于 eor ^= num
			// 从sg定理发现了最优解
			int eor = 0;
			for (auto& ele : arr) {
				eor ^= sg[ele];
			}

			return eor != 0 ? "先手" : "后手";
		}
	};

	vector<int> randomArray(int n, int v) {
		vector<int> ret(n);
		for (int i = 0; i < n; i++) {
			ret[i] = rand() % v;
		}
		return ret;
	}

	int main() {
		srand((unsigned int)time(nullptr));
		int N = 200;
		int V = 1000;
		int testTimes = 10000;
		cout << "测试开始" << endl;
		for (int i = 1; i <= testTimes; i++) {
			int n = rand() % N + 1;
			vector<int> arr = randomArray(n, V);
			string ans1 = Solution1().nim(arr);
			string ans2 = Solution2().nim(arr);
			if (ans1 != ans2) {
				cout << "出错了" << endl;
			}

			if (i % 100 == 0) {
				cout << "当前测试到第 " << i << " 组" << endl;
			}
		}
		cout << "测试结束" << endl;

		return 0;
	}
}



//题目三：
// 两堆石头的巴什博弈
// 有两堆石头，数量分别为a、b
// 两个人轮流拿，每次可以选择其中一堆石头，拿1~m颗
// 拿到最后一颗石子的人获胜，根据a、b、m返回谁赢
// 来自真实大厂笔试，没有在线测试，对数器验证
namespace test3_ {
	class Solution1 {
	public:
		string win(int a, int b, int m) {
			int n = max(a, b);
			vector<int> sg(n + 1, 0);
			vector<int> appear(n + 1, false);
			for (int i = 1; i <= n; i++) {
				fill(appear.begin(), appear.end(), false);
				for (int j = 1; j <= m && i - j >= 0; j++) {
					appear[sg[i - j]] = true;
				}
				for (int s = 0; s <= n; s++) {
					if (!appear[s]) {
						sg[i] = s;
						break;
					}
				}
			}

			return (sg[a] ^ sg[b]) != 0 ? "先手" : "后手";
		}
	};

	class Solution2 {
	public:
		string f(int m, int rest_a, int rest_b, vector<vector<string>>& dp) {
			if (rest_a <= 0 && rest_b <= 0) return "后手";

			if (dp[rest_a][rest_b] != "") return dp[rest_a][rest_b];

			string ans = "后手";
			for (int pick = 1; pick <= m; pick++) {
				if (rest_a - pick >= 0 && f(m, rest_a - pick, rest_b, dp) == "后手") {
					ans = "先手";
					break;
				}
				if (rest_b - pick >= 0 && f(m, rest_a, rest_b - pick, dp) == "后手") {
					ans = "先手";
					break;
				}
			}

			dp[rest_a][rest_b] = ans;
			return ans;
		}

		string win(int a, int b, int m) {
			vector<vector<string>> dp(a + 1, vector<string>(b + 1, ""));
			return f(m, a, b, dp);
		}
	};

	class Solution3 {
	public:
		string win(int a, int b, int m) {
			return a % (m + 1) != b % (m + 1) ? "先手" : "后手";
		}
	};

	int main() {
		cout << "测试开始" << endl;
		for (int a = 0; a <= 100; a++) {
			for (int b = 0; b <= 100; b++) {
				for (int m = 1; m <= 100; m++) {
					string ans1 = Solution1().win(a, b, m);
					string ans2 = Solution2().win(a, b, m);
					string ans3 = Solution3().win(a, b, m);
					if (ans1 != ans2 || ans1 != ans3) {
						cout << "出错了" << endl;
					}
					cout << "当前测试完成" << endl;
				}
			}
		}
		cout << "测试结束" << endl;

		return 0;
	}
}



//题目四：
// 三堆石头拿取斐波那契数博弈
// 有三堆石头，数量分别为a、b、c
// 两个人轮流拿，每次可以选择其中一堆石头，拿取斐波那契数的石头
// 拿到最后一颗石子的人获胜，根据a、b、c返回谁赢
// 来自真实大厂笔试，每堆石子的数量在10^5以内
// 没有在线测试，对数器验证
namespace test4 {
	const int MAXN = 201;
	vector<int> fib = { 1,2,3,5,8,13,21,34,55,89,144 };

	class Solution1 {
	public:
		string f(int a, int b, int c, vector<vector<vector<string>>>& dp) {
			if (a + b + c == 0) return "后手";
			if (dp[a][b][c] != "") return dp[a][b][c];

			string ans = "后手";
			for (int i = 0; i < fib.size(); i++) {
				if (a - fib[i] >= 0 && f(a - fib[i], b, c, dp) == "后手") {
					ans = "先手";
					break;
				}
				if (b - fib[i] >= 0 && f(a, b - fib[i], c, dp) == "后手") {
					ans = "先手";
					break;
				}
				if (c - fib[i] >= 0 && f(a, b, c - fib[i], dp) == "后手") {
					ans = "先手";
					break;
				}
			}

			dp[a][b][c] = ans;
			return ans;
		}

		string win(int a, int b, int c) {
			vector<vector<vector<string>>> dp(a + 1, vector<vector<string>>(b + 1, vector<string>(c + 1, "")));
			return f(a, b, c, dp);
		}
	};

	class Solution2 {
	public:
		string win(int a, int b, int c) {
			int n = max(a, max(b, c));
			vector<int> sg(n + 1, 0);
			vector<int> appear(n + 1, false);
			for (int i = 1; i <= n; i++) {
				fill(appear.begin(), appear.end(), false);
				for (int j = 0; j < fib.size() && i - fib[j] >= 0; j++) {
					appear[sg[i - fib[j]]] = true;
				}
				for (int s = 0; s <= n; s++) {
					if (!appear[s]) {
						sg[i] = s;
						break;
					}
				}
			}

			int eor = 0;
			return (sg[a] ^ sg[b] ^ sg[c]) != 0 ? "先手" : "后手";
		}
	};

	int main() {
		cout << "测试开始" << endl;
		for (int a = 0; a < MAXN; a++) {
			for (int b = 0; b < MAXN; b++) {
				for (int c = 0; c < MAXN; c++) {
					string ans1 = Solution1().win(a, b, c);
					string ans2 = Solution2().win(a, b, c);
					if (ans1 != ans2) {
						cout << "出错了" << endl;
					}
					cout << "当前测试结束" << endl;
				}
			}
		}
		cout << "测试结束" << endl;

		// 试图找到简洁规律，想通过O(1)的过程就得到sg(x)
		// 于是打印200以内的sg值，开始观察
		// 刚开始有规律，但是在sg(138)之后开始发生异常波动
		// 这道题在考的时候，数据量并没有大到需要O(1)的过程才能通过
		// 那就用build方法计算sg值，不再找寻简洁规律
		// 考试时一切根据题目数据量来决定是否继续优化

		return 0;
	}
}



//题目五：
// E&D游戏
// 桌子上有2n堆石子，编号为1、2、3...2n
// 其中1、2为一组；3、4为一组；5、6为一组...2n-1、2n为一组
// 每组可以进行分割操作：
// 任取一堆石子，将其移走，然后分割同一组的另一堆石子
// 从中取出若干个石子放在被移走的位置，组成新的一堆
// 操作完成后，组内每堆的石子数必须保证大于0
// 显然，被分割的一堆的石子数至少要为2
// 两个人轮流进行分割操作，如果轮到某人进行操作时，所有堆的石子数均为1，判此人输掉比赛
// 返回先手能不能获胜
// 测试链接 : https://www.luogu.com.cn/problem/P2148

// N/2堆二元组都是独立游戏
// 计算sg[a][b]的值并找到简洁规律
// 打表找规律
namespace test5_1 {
	const int MAXN = 1001;
	vector<vector<int>> dp(MAXN, vector<int>(MAXN, -1));

	int sg(int a, int b) {
		if (a == 1 && b == 1) return 0;
		if (dp[a][b] != -1) return dp[a][b];

		vector<int> appear(max(a, b) + 1, false);
		if (a > 1) {
			for (int pick = 1; pick < a; pick++) {
				appear[sg(a - pick, pick)] = true;
			}
		}
		if (b > 1) {
			for (int pick = 1; pick < b; pick++) {
				appear[sg(pick, b - pick)] = true;
			}
		}

		int ans = 0;
		for (int s = 0; s <= max(a, b); s++) {
			if (!appear[s]) {
				ans = s;
				break;
			}
		}

		return dp[a][b] = ans;
	}

	// 返回status最低位的0在第几位
	int lowZero(int status) {
		int cnt = 0;
		while (status > 0) {
			if ((status & 1) == 0) {
				break;
			}
			status >>= 1;
			cnt++;
		}
		return cnt;
	}

	void f1() {
		cout << "石子数9以内所有组合的sg值" << endl;
		cout << endl;
		cout << "    ";
		for (int i = 1; i <= 9; i++) {
			cout << i << " ";
		}
		cout << endl;
		cout << endl;
		for (int a = 1; a <= 9; a++) {
			cout << a << "   ";
			for (int b = 1; b < a; b++) {
				cout << "X ";
			}
			for (int b = a; b <= 9; b++) {
				int sgVal = sg(a, b);
				cout << sgVal << " ";
			}
			cout << endl;
		}
	}

	void f2() {
		cout << "石子数9以内所有组合的sg值，但是行列都-1" << endl;
		cout << endl;
		cout << "    ";
		for (int i = 0; i <= 8; i++) {
			cout << i << " ";
		}
		cout << endl;
		cout << endl;
		for (int a = 1; a <= 9; a++) {
			cout << (a - 1) << "   ";
			for (int b = 1; b < a; b++) {
				cout << "X ";
			}
			for (int b = a; b <= 9; b++) {
				int sgVal = sg(a, b);
				cout << sgVal << " ";
			}
			cout << endl;
		}
	}

	void f3() {
		cout << "测试开始" << endl;
		for (int a = 1; a < MAXN; a++) {
			for (int b = 1; b < MAXN; b++) {
				int sg1 = sg(a, b);
				int sg2 = lowZero((a - 1) | (b - 1));
				if (sg1 != sg2) {
					cout << "出错了!" << endl;
				}
			}
		}
		cout << "测试结束" << endl;
	}

	int main() {
		f1();
		cout << endl << endl;
		f2();
		cout << endl << endl;
		f3();

		return 0;
	}
}

namespace test5_2 {
	#include <iostream>
	using namespace std;

	int lowZero(int status) {
		int cnt = 0;
		while (status) {
			if ((status & 1) == 0) {
				break;
			}
			cnt++;
			status >>= 1;
		}
		return cnt;
	}

	int main() {
		int t;
		cin >> t;
		for (int i = 0; i < t; i++) {
			int N;
			cin >> N;
			int sg = 0;
			for (int j = 0; j < N / 2; j++) {
				int a, b;
				cin >> a >> b;
				sg ^= lowZero((a - 1) | (b - 1));
			}
			cout << (sg != 0 ? "YES" : "NO") << endl;
		}

		return 0;
	}
}



//题目六：
// 分裂游戏
// 一共有n个瓶子，编号为0 ~ n-1，第i瓶里装有nums[i]个糖豆，每个糖豆认为无差别
// 有两个玩家轮流取糖豆，每一轮的玩家必须选i、j、k三个编号，并且满足i < j <= k
// 当前玩家从i号瓶中拿出一颗糖豆，分裂成两颗糖豆，并且往j、k瓶子中各放入一颗，分裂的糖豆继续无差别
// 要求i号瓶一定要有糖豆，如果j == k，那么相当于从i号瓶中拿出一颗，向另一个瓶子放入了两颗
// 如果轮到某个玩家发现所有糖豆都在n-1号瓶里，导致无法行动，那么该玩家输掉比赛
// 先手希望知道，第一步如何行动可以保证自己获胜，要求返回字典序最小的行动
// 第一步从0号瓶拿出一颗糖豆，并且往2、3号瓶中各放入一颗，可以确保最终自己获胜
// 第一步从0号瓶拿出一颗糖豆，并且往11、13号瓶中各放入一颗，也可以确保自己获胜
// 本题要求每个瓶子的编号看做是一个字符的情况下，最小的字典序，所以返回"0 2 3"
// 如果先手怎么行动都无法获胜，返回"-1 -1 -1"
// 先手还希望知道自己有多少种第一步取糖的行动，可以确保自己获胜，返回方法数
// 测试链接 : https://www.luogu.com.cn/problem/P3185

//本题不能把单个瓶子看作独立游戏，因为它还和别的瓶子有关系
//而是把单独的糖豆看作一个独立游戏，每一次对糖豆的操作，都会变成2个子游戏(依照题意，一个糖豆分裂成两个)
//要把这2个子游戏的sg值异或起来才是我后继的sg值
// sg[i]: 第i号瓶的糖豆的sg值

const int MAXN = 21;

int nums[MAXN];

int sg[MAXN];

const int MAXV = 101;

bool appear[MAXV];

void build() {
	for (int i = 1; i < MAXN; i++) {
		fill(appear, appear + MAXV, false);
		for (int j = i - 1; j >= 0; j--) {
			for (int k = j; k >= 0; k--) {
				appear[sg[j] ^ sg[k]] = true;
			}
		}
		for (int s = 0; s < MAXV; s++) {
			if (!appear[s]) {
				sg[i] = s;
				break;
			}
		}
	}
}

