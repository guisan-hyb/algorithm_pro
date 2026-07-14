#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;


//哈希函数的用处
//把复杂样本变成数字，以后复杂样本之间的对比，就变成了数字之间的对比

//哈希函数的基本性质：
//1. 输入参数的可能性是无限的，输出的值范围相对有限
//2. 输入同样的样本一定得到同样的输出值，也就是哈希函数没有任何随机机制
//3. 输入不同的样本也可能得到同样的输出值，此时叫哈希碰撞
//4. 输入大量不同的样本，得到的大量输出值，会几乎均匀的分布在整个输出域上

//哈希函数的种类很多，但都符合上述性质
//性质4是最重要的，哈希碰撞理论上无法避免，但是好的哈希函数会让碰撞几率变得很低

//可以把性质4直观理解为：
//不管有没有规律，也不管多么相似，总之一堆不同的输入，得到的输出结果从分布上看，熵最大！



//哈希函数的算法有很多，字符串哈希是其中最常用的，也是唯一需要掌握代码实现的哈希函数

//字符串哈希：如何得到整个字符串的哈希值
//1. 理解long long类型自然溢出，计算加、减、乘时，自然溢出后的状态等同于对2的64次方取模的值状态
//2. 字符串转化成base进制的数字并让其自然溢出
//3. base可以选择一些质数比如：433、499、599、1000000007
//   也可以选择已经被证实了很好用的值：31、131、1313、13131、131313等
//   建议选择质数，不要选经典值，因为会被出题人刻意构造碰撞
//4. 转化时让每一位的值从1开始，不从0开始，这样就得到了一个long long类型的数字代表字符串
//5. 利用数字的比较去代替字符串的比较，可以大大减少复杂度


//字符串哈希从理论上说会有碰撞导致出错，但现实中的算法考察样本量太少了，出错概率非常低
//即便是出错了，也可以更换进制数base，再去赌，一定能赌赢
//没错！是玄学！但是好用！堪称赌狗的胜利！



//题目一：
// 统计有多少个不同的字符串
// 测试链接 : https://www.luogu.com.cn/problem/P3370
namespace test1 {
	#include <iostream>
	#include <vector>
	#include <string>
	#include <cctype>
	#include <algorithm>

	using namespace std;

	const int BASE = 599;

	int getVal(char ch) {
		if (isdigit(ch)) {
			return ch - '0' + 1;
		}
		else if (islower(ch)) {
			return ch - 'a' + 11;
		}
		else if (isupper(ch)) {
			return ch - 'A' + 37;
		}
	}

	unsigned long long f(const string& s) {
		unsigned long long ret = 0;
		for (int i = 0; i < s.size(); i++) {
			ret = ret * BASE + getVal(s[i]);
		}
		return ret;
	}

	int main() {
		int n;
		cin >> n;
		vector<unsigned long long> arr(n);
		for (int i = 0; i < n; i++) {
			string s;
			cin >> s;
			unsigned long long ret = f(s);
			arr[i] = ret;
		}

		sort(arr.begin(), arr.end());
		int ans = 1;
		for (int i = 1; i < n; i++) {
			if (arr[i] != arr[i - 1]) {
				ans++;
			}
		}

		cout << ans << endl;

		return 0;
	}
}



//题目二：
// 独特子串的数量
// 给你一个由数字组成的字符串s
// 返回s中独特子字符串数量
// 其中的每一个数字出现的频率都相同
// 测试链接 : https://leetcode.cn/problems/unique-substrings-with-equal-digit-frequency/

//思路：
//维护三个变量：
// maxCnt: 记录最大词频
// maxCntKind：记录拥有最大词频的字符种类
// allKind：记录所有字符种类
// 若 allKind == maxCntKind，说明该子串是一个独特子串

class Solution {
public:
	int equalDigitFrequency(string& str) {
		const int BASE = 499;
		int n = str.size();
		int ans = 0;
		unordered_set<long long> st;
		for (int i = 0; i < n; i++) {
			long long hashCode = 0;//当前子串的哈希值
			int maxCnt = 0, maxCntKind = 0, allKind = 0;
			vector<int> cnt_table(10, 0);
			for (int j = i; j < n; j++) {
				int curCh = str[j] - '0';
				if (++cnt_table[curCh] == 1) {
					allKind++;
				}
				hashCode = hashCode * BASE + (curCh + 1);

				if (cnt_table[curCh] > maxCnt) {
					maxCnt = cnt_table[curCh];
					maxCntKind = 1;
				}
				else if (cnt_table[curCh] == maxCnt) {
					maxCntKind++;
				}

				if (maxCntKind == allKind) {
					st.insert(hashCode);
				}
			}
		}

		return st.size();
	}
};



//字符串哈希：如何快速得到字符串中任意子串的哈希值 （核心）
//1. 选择一个质数做进制数，base
//2. 得到base的各种次方，在自然溢出下的结果，用pow数组记录
//3. 得到每个位置的hash[i], hash[i] = hash[i-1] * base + s[i] - 'a' + 1
//4. 子串s[l...r]的哈希值 = hash[r] - hash[l-1] * base的(r-l+1)次方

//字符串中 子串对比 变成 哈希值对比 是非常好用的！大量节省时间
//很多较难的算法都可以被字符串哈希替代，都是因为子串对比的代价变为O(1)
//字符串哈希易于理解且使用灵活，因为非常方便的子串对比，很多难题变得非常好想



//题目三：
// 字符串哈希得到子串哈希
// 利用字符串哈希的便利性替代KMP算法
// 测试链接 : https://leetcode.cn/problems/find-the-index-of-the-first-occurrence-in-a-string/

// 如下代码是字符串哈希的原理和模版
	// 比如，base = 499, 也就是课上说的选择的质数进制
	// 再比如字符串s如下
	// " c a b e f "
	//   0 1 2 3 4
	// hash[0] = 3 * base的0次方
	// hash[1] = 3 * base的1次方 + 1 * base的0次方
	// hash[2] = 3 * base的2次方 + 1 * base的1次方 + 2 * base的0次方
	// hash[3] = 3 * base的3次方 + 1 * base的2次方 + 2 * base的1次方 + 5 * base的0次方
	// hash[4] = 3 * base的4次方 + 1 * base的3次方 + 2 * base的2次方 + 5 * base的1次方 + 6 *
	// base的0次方
	// hash[i] = hash[i-1] * base + s[i] - 'a' + 1，就是上面说的意思
	// 想计算子串"be"的哈希值 -> 2 * base的1次方 + 5 * base的0次方
	// 子串"be"的哈希值 = hash[3] - hash[1] * base的2次方(子串"be"的长度次方)
	// hash[1] = 3 * base的1次方 + 1 * base的0次方
	// hash[1] * base的2次方 = 3 * base的3次方 + 1 * base的2次方
	// hash[3] = 3 * base的3次方 + 1 * base的2次方 + 2 * base的1次方 + 5 * base的0次方
	// hash[3] - hash[1] * base的2次方 = 2 * base的1次方 + 5 * base的0次方
	// 这样就得到子串"be"的哈希值了
	// 子串s[l...r]的哈希值 = hash[r] - hash[l-1] * base的(r-l+1)次方，就是上面说的意思

class Solution {
public:
	const int BASE = 499;
	vector<unsigned long long> pow;
	vector<unsigned long long> hash;

	void build(string& s, int n) {
		pow.assign(n, 0);
		hash.assign(n, 0);

		pow[0] = 1;
		for (int i = 1; i < n; i++) {
			pow[i] = pow[i - 1] * BASE;
		}

		hash[0] = s[0] - 'a' + 1;
		for (int i = 1; i < n; i++) {
			hash[i] = hash[i - 1] * BASE + s[i] - 'a' + 1;
		}
	}

	unsigned long long getHash(int l, int r) {
		unsigned long long ans = hash[r];
		if (l > 0) {
			ans -= hash[l - 1] * pow[r - l + 1];
		}
		return ans;
	}

	int strStr(string haystack, string needle) {
		build(haystack, haystack.size());
		unsigned long long hash_code = 0;
		for (int i = 0; i < needle.size(); i++) {
			hash_code = hash_code * BASE + needle[i] - 'a' + 1;
		}

		for (int l = 0, r = needle.size() - 1; r < haystack.size(); r++, l++) {
			if (hash_code == getHash(l, r)) {
				return l;
			}
		}

		return -1;
	}
};



//题目四：
// 重复叠加字符串匹配
// 给定两个字符串a和b，寻找重复叠加字符串a的最小次数，使得字符串b成为叠加后的字符串a的子串
// 如果不存在则返回-1
// 字符串"abc"重复叠加0次是""
// 重复叠加1次是"abc"
// 重复叠加2次是"abcabc"
// 测试链接 : https://leetcode.cn/problems/repeated-string-match/
class Solution {
public:
	const int BASE = 499;
	vector<unsigned long long> pow;
	vector<unsigned long long> hash;

	void build(string& s) {
		int len = s.size();
		pow.assign(len, 0);
		hash.assign(len, 0);

		pow[0] = 1;
		for (int i = 1; i < len; i++) {
			pow[i] = pow[i - 1] * BASE;
		}

		hash[0] = s[0] - 'a' + 1;
		for (int i = 1; i < len; i++) {
			hash[i] = hash[i - 1] * BASE + s[i] - 'a' + 1;
		}
	}

	unsigned long long getHash(int l, int r) {
		unsigned long long ans = hash[r];
		if (l > 0) {
			ans -= hash[l - 1] * pow[r - l + 1];
		}
		return ans;
	}

	int repeatedStringMatch(string a, string b) {
		int n = a.size(), m = b.size();
		int k = (m + n - 1) / n;

		for (int i = 0; i < k + 1; i++) {
			for (int j = 0; j < n; j++) {
				a += a[j];
			}
		}

		build(a);

		unsigned long long hash_code = 0;
		for (int i = 0; i < m; i++) {
			hash_code = hash_code * BASE + b[i] - 'a' + 1;
		}

		for (int l = 0, r = m - 1; r < a.size(); r++, l++) {
			if (hash_code == getHash(l, r)) {
				return r < k * n ? k : k + 1;
			}
		}

		return -1;
	}
};



//题目五：
// 串联所有单词的子串
// 给定一个字符串s和一个字符串数组words
// words中所有字符串长度相同
// s中的串联子串是指一个包含 words中所有字符串以任意顺序排列连接起来的子串
// 例如words = { "ab","cd","ef" }
// 那么"abcdef"、"abefcd"、"cdabef"、"cdefab"、"efabcd"、"efcdab"都是串联子串。 
// "acdbef"不是串联子串，因为他不是任何words排列的连接
// 返回所有串联子串在s中的开始索引
// 你可以以任意顺序返回答案
// 测试链接 : https://leetcode.cn/problems/substring-with-concatenation-of-all-words/

//核心：把s根据0~words[0].size()-2位置开头，长度为words[0].size()划分为若干数据块，在划分好的基础上使用滑窗+字符串哈希

class Solution {
public:
	const int BASE = 499;
	vector<unsigned long long> pow;
	vector<unsigned long long> hash;

	void build(string& s) {
		int m = s.size();
		pow.assign(m, 0);
		hash.assign(m, 0);

		pow[0] = 1;
		for (int i = 1; i < m; i++) {
			pow[i] = pow[i - 1] * BASE;
		}

		hash[0] = s[0] - 'a' + 1;
		for (int i = 1; i < m; i++) {
			hash[i] = hash[i - 1] * BASE + s[i] - 'a' + 1;
		}
	}

	unsigned long long getHash(int l, int r) {
		unsigned long long ans = hash[r];
		if (l > 0) {
			ans -= hash[l - 1] * pow[r - l + 1];
		}
		return ans;
	}

	vector<int> findSubstring(string s, vector<string>& words) {
		vector<int> ans;
		if (s.size() == 0 || words.size() == 0) return ans;

		build(s);
		unordered_map<unsigned long long, int> hash_record;
		for (auto& ele : words) {
			unsigned long long hash_code = 0;
			for (int i = 0; i < ele.size(); i++) {
				hash_code = hash_code * BASE + ele[i] - 'a' + 1;
			}
			hash_record[hash_code]++;
		}

		int wlen = words[0].size(), n = words.size(), total_len = wlen * n;
		for (int startPos = 0; startPos < wlen && startPos + total_len <= s.size(); startPos++) {
			unordered_map<unsigned long long, int> cnt;
			for (int i = startPos; i < startPos + total_len; i += wlen) {
				cnt[getHash(i, i + wlen - 1)]++;
			}

			if (cnt == hash_record) ans.push_back(startPos);
			
			for (int l = startPos + wlen, r = l + total_len; r <= s.size(); l += wlen, r += wlen) {
				cnt[getHash(l - wlen, l - 1)]--;
				cnt[getHash(r - wlen, r - 1)]++;

				if (cnt[getHash(l - wlen, l - 1)] == 0) cnt.erase(getHash(l - wlen, l - 1));
				if (cnt == hash_record) ans.push_back(l);
			}
		}

		return ans;
	}
};



//题目六：
// 根据匹配定义求匹配子串的数量
// 给定长为n的字符串s，以及长度为m的字符串p，还有一个正数k
// s'与s匹配的定义为，s'与s长度相同，且最多有k个位置字符不同
// 要求查找字符串s中有多少子串与字符串p匹配
// 测试链接 : https://www.luogu.com.cn/problem/P3763

//思路：
//对s和s'分别构建hash数组（可以共用一个pow数组），利用二分求出字符不同的位置
//
//核心思想是利用二分查找+字符串哈希来快速求出两个字符串的“最长公共前缀”（LCP）

namespace test6 {
	#include <iostream>
	#include <vector>
	#include <string>
	using namespace std;

	const int BASE = 499;

	vector<unsigned long long> pow;
	vector<unsigned long long> hash_p;
	vector<unsigned long long> hash_s;

	string p, s;

	void build(string& p, string& s) {
		int m = p.size(), n = s.size();
		pow.assign(m, 0);
		hash_p.assign(m, 0);
		hash_s.assign(n, 0);

		pow[0] = 1;
		for (int i = 1; i < m; i++) {
			pow[i] = pow[i - 1] * BASE;
		}

		hash_p[0] = p[0] - 'A' + 1;
		for (int i = 1; i < m; i++) {
			hash_p[i] = hash_p[i - 1] * BASE + p[i] - 'A' + 1;
		}

		hash_s[0] = s[0] - 'A' + 1;
		for (int i = 1; i < n; i++) {
			hash_s[i] = hash_s[i - 1] * BASE + s[i] - 'A' + 1;
		}
	}

	unsigned long long getHash(vector<unsigned long long>& arr, int l, int r) {
		unsigned long long ans = arr[r];
		if (l > 0) {
			ans -= arr[l - 1] * pow[r - l + 1];
		}
		return ans;
	}

	int f(int i, int n, int k) { //二分找最长公共前缀
		int lp = i, rp = i + n - 1;
		int ls = 0;
		int diff = 0;
		while (lp <= rp && diff <= k) {
			int remain_len = rp - lp + 1;//剩余需要比较的长度
			int l = 1;//每次至少比较1个字符
			int r = remain_len;//右边界最多到remain_len
			int len = 0;//p的子串与s相同部分的长度 -> 最长公共前缀长度
			while (l <= r) {
				int m = l + (r - l) / 2;
				if (getHash(hash_p, lp, lp + m - 1) == getHash(hash_s, ls, ls + m - 1)) {
					len = m;
					l = m + 1;
				}
				else {
					r = m - 1;
				}
			}

			//根据二分的逻辑，我们找到了最长的匹配长度 len，这意味着 长度为 len + 1 时一定不匹配（否则二分还会继续往右找）
			//所以，位置在 lp + len 的字符，一定与S中位置在 ls + len 的字符不同
			if (lp + len <= rp) {
				diff++;
			}

			lp += len + 1;
			ls += len + 1;
		}

		return diff <= k;
	}

	int main() {
		int t;
		cin >> t;
		for (int x = 0; x < t; x++) {
			cin >> p >> s;

			int m = p.size(), n = s.size();
			
			if (m < n) {
				cout << 0 << endl;
				continue;
			}

			build(p, s);

			int ans = 0;
			for (int i = 0; i <= m - n; i++) {
				if (f(i, n, 3)) {
					ans++;
				}
			}

			cout << ans << endl;
		}

		return 0;
	}
}

