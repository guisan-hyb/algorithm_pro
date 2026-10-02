#include <iostream>
#include <vector>

using namespace std;



// 树状数组 -- 上
// 本节课讲述：
// 树状数组维护信息的类型 & 树状数组与线段树的比较 & 线段树预告
// 一维数组上实现：单点增加、范围查询的树状数组
// 一维数组上实现：范围增加、单点查询的树状数组
// 一维数组上实现：范围增加、范围查询的树状数组
// 二维数组上实现：单点增加、范围查询的树状数组
// 二维数组上实现：范围增加、范围查询的树状数组



// 树状数组维护信息的类型
// 树状数组一般用来维护可差分的信息
// 比如：累加和、累乘积、或者出题人发现了某个可差分信息来出题考你

// 不可差分的信息，比如：最大值、最小值、除此之外的很多信息
// 不可差分的信息一般不用树状数组维护，会选择线段树维护，因为线段树维护的方式思考难度更低
// 树状数组维护不可差分信息的内容不再讲述，后续会详细讲解线段树

// 大多数情况下，线段树可以替代树状数组，两者的时间复杂度差不多，单次调用都是 O(log n)
//
// 线段树的优势：用法全面、思考难度低、维护信息类型多（包括可差分信息、不可差分信息）
// 线段树的劣势：代码较多、使用空间较大、常数时间较差
// 
// 树状数组优势：代码量少、使用空间少、常数时间优异
// 树状数组劣势：维护信息的类型少、维护某些不可差分的信息时思考难度大且不易实现




// 题目一：
// 树状数组单点增加、范围查询模版
// 测试链接 : https://www.luogu.com.cn/problem/P3374
//
// 这是使用树状数组最常见的方式，笔试、比赛中都大量出现
// 
//  (下标一定从 1 开始！！！)
// 
// 当 i 位置增加 v 时，如下位置都获得该值 ：i += i & -i
// 当计算[1...i]范围的累加和时，把如下位置的值都加上：i -= i & -i
// 神奇的 lowbit，可以将上述的组织，优雅、方便的实现
//
// 单次调用时间复杂度 O(logn)

// 将原数组进行二进制分块归并划分
// 树状数组中每一个下标(i)管理 以该下标为右端点的最长子数组 即：i-lowbit(i)+1 ~ i

namespace test1 {
	#include <iostream>
	#include <vector>

	using namespace std;

	vector<int> tree;
	int n, m;

	int lowbit(int x) {
		return x & (-x);
	}

	void add(int idx, int v) {
		for (int i = idx; i <= n; i += lowbit(i)) {
			tree[i] += v;
		}
	}

	int query(int idx) {
		int ans = 0;
		for (int i = idx; i > 0; i -= lowbit(i)) {
			ans += tree[i];
		}
		return ans;
	}

	int range(int l, int r) {
		return query(r) - query(l - 1);
	}

	int main() {
		cin >> n >> m;
		tree.resize(n + 1);

		// 下标从1开始
		for (int i = 1; i <= n; i++) {
			int v;
			cin >> v;
			add(i, v);
		}

		for (; m > 0; m--) {
			int opt, x, t;
			cin >> opt >> x >> t;
			if (opt == 1) {
				add(x, t);
			}
			else if (opt == 2) {
				cout << range(x, t) << endl;
			}
		}

		return 0;
	}
}




// 题目二
// 树状数组范围增加、单点查询模版
// 测试链接 : https://www.luogu.com.cn/problem/P3368
namespace test2 {
	#include <iostream>
	#include <vector>
	using namespace std;

	// 树状数组不维护原数组的信息，维护原数组的差分信息
	// 注意下标一定从1开始，不从0开始
	vector<int> tree;
	int n, m;

	int lowbit(int i) { return i & (-i); }

	void add(int idx, int v) {
		for (int i = idx; i <= n; i += lowbit(i)) {
			tree[i] += v;
		}
	}

	int query(int idx) {
		int ans = 0;
		for (int i = idx; i > 0; i -= lowbit(i)) {
			ans += tree[i];
		}
		return ans;
	}

	int main() {
		ios::sync_with_stdio(false);
		cin.tie(nullptr);

		cin >> n >> m;
		tree.resize(n + 2, 0);

		for (int i = 1; i <= n; i++) {
			int v;
			cin >> v;
			add(i, v);
			add(i + 1, -v);
		}

		while (m--) {
			int opt;
			cin >> opt;
			if (opt == 1) {
				int x, y, k;
				cin >> x >> y >> k;
				add(x, k);
				add(y + 1, -k);
			}
			else {  
				int x;
				cin >> x;
				cout << query(x) << '\n';
			}
		}
		return 0;
	}
}




// 题目三：
// 树状数组范围增加、范围查询模版
// 测试链接 : https://www.luogu.com.cn/problem/P3372
namespace test3 {
	#include <iostream>
	#include <vector>
	using namespace std;
	using ll = long long;

	// 维护原始数组的差分信息：Di
	vector<ll> info1;
	// 维护原始数组的差分加工信息：(i-1) * Di
	vector<ll> info2;

	int n, m;

	int lowbit(int i) { return i & (-i); }

	void add(vector<ll>& arr, int idx, ll v) {
		for (int i = idx; i <= n; i += lowbit(i)) arr[i] += v;
	}

	ll query(vector<ll>& arr, int idx) {
		ll ans = 0;
		for (int i = idx; i > 0; i -= lowbit(i)) ans += arr[i];
		return ans;
	}

	void add(int l, int r, ll v) {
		add(info1, l, v);
		add(info1, r + 1, -v);
		add(info2, l, v * (l - 1));
		add(info2, r + 1, -(v * r));
	}

	ll range(int l, int r) {
		return query(info1, r) * r - query(info2, r)
			- (query(info1, l - 1) * (l - 1) - query(info2, l - 1));
	}

	int main() {
		ios::sync_with_stdio(false);
		cin.tie(nullptr);

		cin >> n >> m;
		info1.resize(n + 2, 0);
		info2.resize(n + 2, 0);

		for (int i = 1; i <= n; i++) {
			ll v;
			cin >> v;
			add(i, i, v);
		}

		while (m--) {
			int opt;
			cin >> opt;
			if (opt == 1) {
				ll x, y, k;
				cin >> x >> y >> k;
				add(x, y, k);
			}
			else {
				int x, y;
				cin >> x >> y;
				cout << range(x, y) << '\n';
			}
		}
		return 0;
	}
}




// 题目四：
// 二维数组上单点增加、范围查询，使用树状数组的模版
// 测试链接 : https://leetcode.cn/problems/range-sum-query-2d-mutable/
//            https://www.luogu.com.cn/problem/U321725

namespace test4 {
	#include <iostream>
	#include <vector>
	using namespace std;

	using ll = long long;

	int n, m;
	vector<vector<ll>> tree;         

	int lowbit(int i) {
		return i & (-i);
	}

	void add(int x, int y, ll v) {    
		for (int i = x; i <= n; i += lowbit(i)) {
			for (int j = y; j <= m; j += lowbit(j)) {
				tree[i][j] += v;
			}
		}
	}

	ll query(int x, int y) {         
		ll ans = 0;
		for (int i = x; i > 0; i -= lowbit(i)) {
			for (int j = y; j > 0; j -= lowbit(j)) {
				ans += tree[i][j];
			}
		}
		return ans;
	}

	ll sum(int a, int b, int c, int d) {   
		return query(c, d) - query(c, b - 1) - query(a - 1, d) + query(a - 1, b - 1);
	}

	int main() {
		ios::sync_with_stdio(false);
		cin.tie(nullptr);

		cin >> n >> m;
		tree.resize(n + 1, vector<ll>(m + 1, 0));

		int opt;
		while (cin >> opt) {
			if (opt == 1) {
				int x, y;
				ll k;
				cin >> x >> y >> k;
				add(x, y, k);
			}
			else {
				int a, b, c, d;
				cin >> a >> b >> c >> d;
				cout << sum(a, b, c, d) << '\n';   
			}
		}
		return 0;
	}
}




// 题目五：
// 二维数组上范围增加、范围查询，使用树状数组的模版(C++)
// 测试链接 : https://www.luogu.com.cn/problem/P4514
//            https://www.luogu.com.cn/problem/U321726
namespace test5 {
	#include <iostream>
	#include <vector>
	using namespace std;

	// 维护信息 : d[i][j]
	vector<vector<int>> info1;
	// 维护信息 : d[i][j] * i
	vector<vector<int>> info2;
	// 维护信息 : d[i][j] * j
	vector<vector<int>> info3;
	// 维护信息 : d[i][j] * i * j
	vector<vector<int>> info4;

	int n, m;

	int lowbit(int i) {
		return i & (-i);
	}

	void add(int x, int y, int v) {
		int v1 = v;
		int v2 = x * v;
		int v3 = y * v;
		int v4 = x * y * v;
		for (int i = x; i <= n; i += lowbit(i)) {
			for (int j = y; j <= m; j += lowbit(j)) {
				info1[i][j] += v1;
				info2[i][j] += v2;
				info3[i][j] += v3;
				info4[i][j] += v4;
			}
		}
	}

	// 以(1,1)左上角，以(x,y)右下角
	int query(int x, int y) {
		int ans = 0;
		for (int i = x; i > 0; i -= lowbit(i)) {
			for (int j = y; j > 0; j -= lowbit(j)) {
				ans += (x + 1) * (y + 1) * info1[i][j] - (y + 1) * info2[i][j] - (x + 1) * info3[i][j] + info4[i][j];
			}
		}
		return ans;
	}

	void add(int a, int b, int c, int d, int v) {
		add(a, b, v);
		add(c + 1, b, -v);
		add(a, d + 1, -v);
		add(c + 1, d + 1, v);
	}

	int range(int a, int b, int c, int d) {
		return query(c, d) - query(c, b - 1) - query(a - 1, d) + query(a - 1, b - 1);
	}

	int main() {
		char t; cin >> t;
		cin >> n >> m;
		info1.resize(n + 2, vector<int>(m + 2, 0));
		info2.resize(n + 2, vector<int>(m + 2, 0));
		info3.resize(n + 2, vector<int>(m + 2, 0));
		info4.resize(n + 2, vector<int>(m + 2, 0));

		char opt;
		while (cin >> opt) {
			if (opt == 'L') {
				int a, b, c, d, v;
				cin >> a >> b >> c >> d >> v;
				add(a, b, c, d, v);
			}
			else {
				int a, b, c, d;
				cin >> a >> b >> c >> d;
				cout << range(a, b, c, d) << endl;
			}
		}

		return 0;
	}
}

