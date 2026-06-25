#define _CRT_SECURE_NO_WARNINGS


#include <iostream>
#include <vector>
using namespace std;



//判断值较小的数字是否为质数
//判断值较大的数字是否为质数，了解Miller-Rabin测试的大概过程 + 会用模板即可
//某个数字所有质数因子的分解，掌握最常用的方法足够了
//找出1~n范围内所有的质数，埃氏筛、欧拉筛，其实掌握埃氏筛足够

//因为证明麻烦，并且证明过程没啥扩展性，记住用法和模板即可，当作原子技能使用



//题目一：
// 判断较小的数字是否是质数
// 时间复杂度O(根号n)

//思路：如果a不是，那么所有大于等于n/a的数都没必要再去试了; 最终到根号n

class Solution {
public:
	bool isPrime(long long n) {
		if (n <= 1) return false;
		for (long long i = 2; i * i <= n; i++) {
			if (n % i == 0) {
				return false;
			}
		}
		return true;
	}
};



//题目二：
// 判断较大的数字是否是质数(Miller-Rabin测试)
// 测试链接 : https://www.luogu.com.cn/problem/U148828

//判断n是否是质数，Miller-Rabin测试大概过程：
//1.每次选择 1 ~ n-1 范围上的随机数字，或者指定一个比n小的质数，进行测试
//2.测试过程中的数学原理不用纠结，不重要，因为该原理除了判断质数以外，不再用于别的方面
//3.原理：费马小定理、Carmichael(卡米切尔数)、二次探测定理(算法导论31章)、乘法同余、快速幂
//4.经过s次Miller-Rabin测试，s越大出错几率越低，但是速度也会越慢，一般测试20次以内即可

//重点是用法
//因为有乘法同余，所以想验证任意的long类型的数字，需要注意位数的事情
//时间复杂度 O( s * (logn)的三次方 )，速度很快


//下面代码可以解决10^9范围内数字的质数检查
namespace test2_1 {
	#include <iostream>
	#include <vector>
	using namespace std;

	using ll = long long;

	vector<ll> p = { 2,3,5,7,11,13,17,19,23,29,31,37 };

	ll quickPower(ll n, ll p, ll mod) {
		ll ans = 1;
		while (p > 0) {
			if ((p & 1) == 1) {
				ans = (ans * n) % mod;
			}
			n = (n * n) % mod;
			p >>= 1;
		}
		return ans;
	}

	// 返回n是不是合数
	bool witness(ll a, ll n) {
		ll u = n - 1;
		int t = 0;
		while ((u & 1) == 0) {
			t++;
			u >>= 1;
		}

		ll x1 = quickPower(a, u, n), x2;
		for (int i = 1; i <= t; i++) {
			x2 = quickPower(x1, 2, n);
			if (x2 == 1 && x1 != 1 && x1 != n - 1) {
				return true;
			}
			x1 = x2;
		}
		if (x1 != 1) {
			return true;
		}
		return false;
	}

	bool millerRabin(ll n) {
		if (n <= 2) {
			return n == 2;
		}
		if ((n & 1) == 0) {
			return false;
		}
		for (int i = 0; i < p.size() && p[i] < n; i++) {
			if (witness(p[i], n)) {
				return false;
			}
		}
		return true;
	}


	int main() {
		int n;
		cin >> n;
		for (int i = 0; i < n; i++) {
			ll x;
			cin >> x;
			cout << (millerRabin(x) ? "Yes" : "No") << endl;
		}

		return 0;
	}
}

//下面代码可以通过
namespace test2_2 {
	//#include <bits/stdc++.h>
	using namespace std;

	//__int128 是GCC/Clang扩展，在VS的MSVC编译器下可能不支持，但算法比赛用的都是GCC/Linux环境)
	typedef __int128 ll;
	typedef pair<int, int> pii;

	template<typename T> inline T read() {
		T x = 0, f = 1; char ch = 0;
		for (; !isdigit(ch); ch = getchar()) if (ch == '-') f = -1;
		for (; isdigit(ch); ch = getchar()) x = (x << 3) + (x << 1) + (ch - '0');
		return x * f;
	}

	template<typename T> inline void write(T x) {
		if (x < 0) putchar('-'), x = -x;
		if (x > 9) write(x / 10);
		putchar(x % 10 + '0');
	}

	template<typename T> inline void print(T x, char ed = '\n') {
		write(x), putchar(ed);
	}

	ll t, n;

	ll qpow(ll a, ll b, ll mod) {
		ll ret = 1;
		while (b) {
			if (b & 1) ret = (ret * a) % mod;
			a = (a * a) % mod;
			b >>= 1;
		}
		return ret % mod;
	}

	vector<ll> p = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37 };

	bool miller_rabin(ll n) {
		if (n < 3 || n % 2 == 0) return n == 2;
		ll u = n - 1, t = 0;
		while (u % 2 == 0) u /= 2, ++t;
		for (auto a : p) {
			if (n == a) return 1;
			if (n % a == 0) return 0;
			ll v = qpow(a, u, n);
			if (v == 1) continue;
			ll s = 1;
			for (; s <= t; ++s) {
				if (v == n - 1) break;
				v = v * v % n;
			}
			if (s > t) return 0;
		}
		return 1;
	}

	int main() {
		t = read<ll>();
		while (t--) {
			n = read<ll>();
			if (miller_rabin(n)) puts("Yes");
			else puts("No");
		}
		return 0;
	}
}



//题目三：
// 数字n拆分质数因子
// 时间复杂度O(根号n)
class Solution {
public:
	void f(int n) {
		for (int i = 2; i * i <= n; i++) {
			if (n % i == 0) {
				cout << i << endl;
				while (n % i == 0) {
					n /= i;
				}
			}
		}

		if (n > 1) {
			cout << n << endl;
		}
	}
};


// 按公因数计算最大组件大小
// 给定一个由不同正整数的组成的非空数组 nums
// 如果 nums[i] 和 nums[j] 有一个大于1的公因子，那么这两个数之间有一条无向边
// 返回 nums中最大连通组件的大小。
// 测试链接 : https://leetcode.cn/problems/largest-component-size-by-common-factor/
class Solution {
public:
	const int MAXV = 100001;
	const int MAXN = 20001;

	vector<int> father;
	vector<int> size;
	vector<int> factors;//质因子 - 首个包含该因子的数的下标

	void build() {
		size.assign(MAXN, 1);
		factors.assign(MAXV, -1);
		father.assign(MAXN,0);
		for (int i = 0; i < MAXN; i++) {
			father[i] = i;
		}
	}

	int find(int i) {
		if (i == father[i]) return i;
		return father[i] = find(father[i]);
	}

	void Union(int a, int b) {
		int fa = find(a);
		int fb = find(b);
		if (fa != fb) {
			father[fa] = fb;
			size[fb] += size[fa];
		}
	}

	int largestComponentSize(vector<int>& nums) {
		int n = nums.size();
		build();
		for (int i = 0; i < n; i++) {
			for (int j = 2; j * j <= nums[i]; j++) {
				if (nums[i] % j == 0) {
					if (factors[j] == -1) {
						factors[j] = i;
					}
					else {
						Union(factors[j], i);
					}

					while (nums[i] % j == 0) {
						nums[i] /= j;
					}
				}
			}

			if (nums[i] > 1) {
				if (factors[nums[i]] == -1) {
					factors[nums[i]] = i;
				}
				else {
					Union(factors[nums[i]], i);
				}
			}
		}

		int ans = 0;
		for (int i = 0; i < n; i++) {
			ans = max(ans, size[find(i)]);
		}

		return ans;
	}
};



//题目三：
// 计数质数
// 给定整数n，返回小于非负整数n的质数的数量
// 测试链接 : https://leetcode.cn/problems/count-primes/

//法一：埃氏筛
// 时间复杂度O(n * log(logn))
class Solution {
public:
	int countPrimes(int n) {
		// visit[i] = true，代表i是合数
		// visit[i] = false，代表i是质数
		// 初始时认为0~n所有数都是质数
		vector<int> visited(n + 1, false);
		for (int i = 2; i * i <= n; i++) {
			if (!visited[i]) {
				for (int j = i * i; j <= n; j += i) {
					visited[j] = true;
				}
			}
		}

		int ans = 0;
		for (int i = 2; i < n; i++) {
			if (!visited[i]) {
				ans++;
			}
		}

		return ans;
	}
};

//法二：欧拉筛
// 时间复杂度O(n)

//核心：
//每个合数只被自己的最小质因子筛掉

class Solution {
public:
	int countPrimes(int n) {
		vector<int> visited(n + 1, false);
		//收集所有质数
		vector<int> prime(n / 2 + 1, 0);
		int cnt = 0;

		for (int i = 2; i < n; i++) {
			if (!visited[i]) {
				prime[cnt++] = i;
			}
			for (int j = 0; j < cnt; j++) {
				if (prime[j] * i >= n) {
					break;
				}

				visited[prime[j] * i] = true;

				if (i % prime[j] == 0) { //当前数字拥有更小的质因子，应该被更小的质因子筛掉
					break;
				}
			}
		}

		return cnt;
	}
};



// 只是计数的话
// 埃氏筛还能改进
class Solution {
public:
	int countPrimes(int n) {
		if (n <= 2) return 0;

		vector<int> visited(n + 1, false);
		// 先把所有的偶数去掉，但是算上2
		// 估计的质数数量，如果发现更多合数，那么cnt--
		int cnt = n / 2;

		for (int i = 3; i * i <= n; i+=2) {
			if (!visited[i]) {
				for (int j = i * i; j <= n; j += 2 * i) {
					if (!visited[j]) {
						visited[j] = true;
						cnt--;
					}
				}
			}
		}

		return cnt;
	}
};

