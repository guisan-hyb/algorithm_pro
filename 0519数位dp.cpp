#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <algorithm>
using namespace std;


//数位dp好好看灵神的模板！


//数位dp--上


//数位dp的尝试方式并不特殊，绝大多数都是线性展开，类似从左往右的尝试
//之前的课已经讲过 大量在数组上进行线性展开的题目，数位dp是在数字的每一位上进行线性展开而已
//不同的题目有不同的限制，解题核心在于：可能性的整理、排列组合的相关知识

//解决数位dp的问题 推荐使用记忆化搜索的方式，可能性的展开会很好写，不必刻意追求进一步改写
//递归写出来问题就解决了，位数多就挂缓存，位数不多甚至不挂缓存也能通过



//题目一：
// 统计各位数字都不同的数字个数
// 给你一个整数n，代表十进制数字最多有n位
// 如果某个数字，每一位都不同，那么这个数字叫做有效数字
// 返回有效数字的个数，不统计负数范围
// 测试链接 : https://leetcode.cn/problems/count-numbers-with-unique-digits/
class Solution {
public:
    int countNumbersWithUniqueDigits(int n) {
        if (n == 0) return 1;

        int ans = 10;
        // 1 : 10
        // 2 : 9 * 9
        // 3 : 9 * 9 * 8
        // 4 : 9 * 9 * 8 * 7
        // ...都累加起来...
        for (int cursum = 9, i = 9, k = 2; k <= n; k++, i--) {
            cursum *= i;
            ans += cursum;
        }
        return ans;
    }
};

//补个记忆化搜索
class Solution {
public:
    /*这个题就是高中时学的排列组合
        f(0) = 1
        f(1) = 10
        f(2) = 9 * 9 + f(1)
        f(3) = 9 * 9 * 8 + f(2)
        f(4) = 9 * 9 * 8 * 7 + f(3)
        左边开始数
        首位数不取 0 其他位数可以取 0，下一位比前一位取法少一种，因为不能重复
        首位数取 0 时就是 f(n - 1)的数量*/

    int f(int i, vector<int>& dp) {
        if (i == 0) return 1;
        if (i == 1) return 10;
        if (dp[i] != -1) return dp[i];
        int ans = 9;
        for (int t = 9, k = 2; k <= i; k++, t--) {
            ans *= t;
        }
        ans += f(i - 1, dp);
        dp[i] = ans;
        return ans;
    }

    int countNumbersWithUniqueDigits(int n) {
        vector<int> dp(n + 1, -1);
        return f(n, dp);
    }
};



//题目二：
// 最大为N的数字组合
// 给定一个按 非递减顺序 排列的数字数组 digits
// 已知digits一定不包含'0'，可能包含'1' ~ '9'，且无重复字符
// 你可以用任意次数 digits[i] 来写的数字
// 例如，如果 digits = ['1','3','5']
// 我们可以写数字，如 '13', '551', 和 '1351315'
// 返回 可以生成的小于或等于给定整数 n 的正整数的个数
// 测试链接 : https://leetcode.cn/problems/numbers-at-most-n-given-digit-set/

// f(i, isLimit, isNum) 表示构造从左往右第 i 位及其之后数位的合法方案数
// isLimit 表示当前是否受到了 n 的约束。若为真，则第 i 位填入的数字至多为 s[i]，否则至多为 9
// isNum 表示之前位是否使用过数字。若为真，则之前位使用过数字；若为假，则之前位没有使用过数字

class Solution {
public:
    int bfind(vector<string>& digits, const string& ch) {
        int ans = -1;
        for (int l = 0, r = digits.size() - 1; l <= r;) {
            int m = l + (r - l) / 2;
            if (digits[m] <= ch) {
                ans = m;
                l = m + 1;
            }
            else
                r = m - 1;
        }
        return ans;
    }

    int f(vector<string>& digits, string& num, int i, bool isLimit, bool isNum) {
        if (i == num.size()) return isNum;

        int ans = 0;
        // 之前从来没有选择过数字
        // 当前依然可以不要任何数字，累加后续的可能性
        if (isNum == 0) {
            ans += f(digits, num, i + 1, 0, 0);
        }

        if (isLimit) { //不能自由选择
            int idx = bfind(digits, to_string(num[i] - '0'));
            bool has_eq = (idx != -1 && digits[idx] == to_string(num[i] - '0'));
            // 计算严格小于 num[i] 的数字个数
            int less_cnt = 0;
            if (idx != -1) less_cnt = has_eq ? idx : idx + 1;

            ans += less_cnt * f(digits, num, i + 1, 0, 1);
            if (has_eq) {
                ans += f(digits, num, i + 1, 1, 1);
            }
        }
        else { //能自由选择
            ans += digits.size() * f(digits, num, i + 1, 0, 1);
        }

        return ans;
    }

    int atMostNGivenDigitSet(vector<string>& digits, int n) {
        string num = to_string(n);
        return f(digits, num, 0, 1, 0);//开局是受限制的
    }
};

//注：num[i] 是一个 char 类型，to_string(char) 会将其转换为对应的 ASCII 码的十进制字符串（例如 '1' 变成 "49"），而不是字符 "1"。
//应改用：   string ch(1, num[i]);  或者 to_string(num[i] - '0')



//题目三：
// 统计整数数目
// 给你两个数字字符串 num1 和 num2 ，以及两个整数max_sum和min_sum
// 如果一个整数 x 满足以下条件，我们称它是一个好整数
// num1 <= x <= num2
// min_sum <= digit_sum(x) <= max_sum
// 请你返回好整数的数目
// 答案可能很大，答案对 1000000007 取模
// 注意，digit_sum(x)表示x各位数字之和
// 测试链接 : https://leetcode.cn/problems/count-of-integers/

//分析：求 num1 ~ num2 上满足要求的答案，等价于 0~num2 上的答案 - 0~(num1-1) 上的答案  （类似前缀和的思想）
//由于num1,num2均有字符串存储，数据可能远超long long，这种情况下计算 0~(num1-1) 上的答案需要用高精度减法算出 num1 - 1
//太麻烦，所以转换一下：
//求出 0~num2 上的答案 - 0~num1上的答案，再单独验证num1：
//如果num1符合要求，则 ans+1 ; 如果num1不符合要求，则ans不变

class Solution {
public:
    const int MOD = 1e9 + 7;
    const int MAXN = 23;
    const int MAXM = 401;

    vector<vector<vector<int>>> dp;
    int mins, maxs;

    void build() {
        dp.assign(MAXN, vector<vector<int>>(MAXM, vector<int>(2, -1)));
    }

    bool check(string& num1) {
        int sum = 0;
        for (char ch : num1) {
            sum += ch - '0';
        }
        return sum >= mins && sum <= maxs;
    }

    // 递归含义：
    // 从num的高位出发，当前来到i位上
    // 之前决定的数字累加和是sum
    // 之前的决定已经比num小，后续可以自由选择数字，那么isLimit == 0
    // 之前的决定和num一样，后续不可以自由选择数字，那么isLimit == 1
    // 返回有多少种可能性
    int f(string& nums, int i, int sum, bool isLimit) {
        if (sum > maxs) return 0;
        //神之一剪: 如果剩下位(包括当前来到却未选数字的位置i)都能自由选择却小于mins, 说明之前的选择不可行
        if (sum + (nums.size() - i) * 9 < mins) return 0;
        if (i == nums.size()) return 1;

        if (dp[i][sum][isLimit] != -1) return dp[i][sum][isLimit];

        int ans = 0;
        int cur = nums[i] - '0';
        if (isLimit) {
            ans = f(nums, i + 1, sum + cur, 1);
            for (int pick = 0; pick < cur; pick++) {
                ans = (ans + f(nums, i + 1, sum + pick, 0)) % MOD;
            }
        }
        else {
            for (int pick = 0; pick <= 9; pick++) {
                ans = (ans + f(nums, i + 1, sum + pick, 0)) % MOD;
            }
        }
        dp[i][sum][isLimit] = ans;
        return ans;
    }

    int count(string num1, string num2, int min_sum, int max_sum) {
        mins = min_sum;
        maxs = max_sum;
        build();
        int ans = 0;
        ans = f(num2, 0, 0, 1);
        build();
        ans = (ans - f(num1, 0, 0, 1) + MOD) % MOD;
        if (check(num1)) {
            ans = (ans + 1) % MOD;
        }
        return ans;
    }
};



//题目四：
// 完全没有重复的数字个数
// 给定正整数n，返回在[1, n]范围内每一位都互不相同的正整数个数
// 测试链接 : https://leetcode.cn/problems/count-special-integers/
class Solution {
public:
    int f(string& num, int i, bool isLimit, bool isNum, int status, vector<vector<int>>& dp) {
        if (i == num.size()) return isNum;
        // 只有在不受限且已经填了数字的情况下，记忆化结果才是通用的 (重要)
        if (!isLimit && isNum && dp[i][status] != -1) return dp[i][status];

        int ans = 0;
        if (!isNum) {
            ans += f(num, i + 1, 0, 0, status, dp);
        }

        int up = isLimit ? (num[i] - '0') : 9;
        int start = isNum ? 0 : 1;
        for (int pick = start; pick <= up; pick++) {
            if (!(status & (1 << pick))) {
                bool nextLimit = isLimit && (pick == up);
                ans += f(num, i + 1, nextLimit, 1, status ^ (1 << pick), dp);
            }
        }
        
        // 只有在不受限且已经填了数字的情况下，才存入备忘录
        if (!isLimit && isNum)
            dp[i][status] = ans;
        return ans;
    }

    int countSpecialNumbers(int n) {
        string num = to_string(n);
        int len = num.size();
        vector<vector<int>> dp(len, vector<int>(1 << 10, -1));
        return f(num, 0, 1, 0, 0, dp);
    }
};

//小结:
//记忆化搜索存储的必须是“通用子问题”的解，
//而 isLimit=true 和 isNum=false 代表当前状态是“特殊”的，它们的解不能被其他路径复用。


//附：题目四变种：
// 至少有1位重复的数字个数
// 给定正整数n，返回在[1, n]范围内至少有1位重复数字的正整数个数
// 测试链接 : https://leetcode.cn/problems/numbers-with-repeated-digits/
class Solution {
public:
    int f(string& num, int i, bool isLimit, bool isNum, int status, vector<vector<int>>& dp) {
        if (i == num.size()) return isNum;
        if (!isLimit && isNum && dp[i][status] != -1) return dp[i][status];

        int ans = 0;
        if (!isNum) {
            ans += f(num, i + 1, 0, 0, status, dp);
        }

        int up = isLimit ? (num[i] - '0') : 9;
        int start = isNum ? 0 : 1;
        for (int pick = start; pick <= up; pick++) {
            if (!(status & (1 << pick))) {
                bool nextLimit = isLimit && (pick == up);
                ans += f(num, i + 1, nextLimit, 1, status ^ (1 << pick), dp);
            }
        }
        
        if (!isLimit && isNum)
            dp[i][status] = ans;
        return ans;
    }

    int numDupDigitsAtMostN(int n) {
        string num = to_string(n);
        int len = num.size();
        vector<vector<int>> dp(len, vector<int>(1 << 10, -1));
        return n - f(num, 0, 1, 0, 0, dp);
    }
};







//数位dp--下
// windy数
// 不含前导零且相邻两个数字之差至少为2的正整数被称为windy数
// windy想知道[a,b]范围上总共有多少个windy数
// 测试链接 : https://www.luogu.com.cn/problem/P2657
namespace test1 {
    #include <iostream>
    #include <vector>
    #include <string>
    using namespace std;

    vector<vector<int>> dp;

    void build() {
        dp.clear();
        dp.assign(11, vector<int>(10,-1));
    }

    int f(string& num, int i, bool isLimit, bool isNum, int prev) {
        if (i == num.size()) return isNum;
        if (!isLimit && isNum && dp[i][prev] != -1) return dp[i][prev];

        int ans = 0;
        if (!isNum) {
            ans += f(num, i + 1, 0, 0, 0);
        }

        int cur = num[i] - '0';
        int up = isLimit ? cur : 9;
        int start = isNum ? 0 : 1;
        for (int pick = start; pick <= up; pick++) {
            if (!isNum || pick <= prev - 2 || pick >= prev + 2) { //注意首位(!isNum)无需判断
                bool nextLimit = isLimit && (pick == up);
                ans += f(num, i + 1, nextLimit, 1, pick);
            }
        }

        if (!isLimit && isNum)
            dp[i][prev] = ans;
        return ans;
    }

    int main() {
        int a, b;
        cin >> a >> b;
        dp.reserve(11);
        int ans1 = 0, ans2 = 0;
        string num1 = to_string(a - 1);
        string num2 = to_string(b);
        build();
        ans2 = f(num2, 0, 1, 0, 0);
        build();
        ans1 = f(num1, 0, 1, 0, 0);

        cout << ans2 - ans1;

        return 0;
    }

}



//题目二：
// 萌数
// 如果一个数字，存在长度至少为2的回文子串，那么这种数字被称为萌数
// 比如101、110、111、1234321、45568
// 求[l,r]范围上，有多少个萌数
// 由于答案可能很大，所以输出答案对1000000007求余
// 测试链接 : https://www.luogu.com.cn/problem/P3413

//分析：正难则反，求出 不含回文串长度>=2 的部分
//如果一个数包含回文串，则必定有一个位置i满足 s[i] == s[i-1] 或 s[i] == s[i-2]
//所以，如果一个数不是萌数，则必定存在一个事实：
//  对于每个位置i : s[i] != s[i-1] 且 s[i] != s[i-2]

namespace test2 {
    #include <iostream>
    #include <vector>
    #include <string>
    using namespace std;

    const int MAXN = 1001;
    const int MOD = 1e9 + 7;
    vector<vector<vector<int>>> dp;

    void build() {
        dp.clear();
        dp.assign(MAXN, vector<vector<int>>(12, vector<int>(12, -1)));
    }

    int f(string& num, int i, int prepre, int pre, bool isLimit, bool isNum) {
        if (i == num.size()) return isNum;
        if (!isLimit && isNum && dp[i][prepre][pre] != -1) return dp[i][prepre][pre];

        int ans = 0;
        if (!isNum) {
            ans = (ans + f(num, i + 1, 10, 11, 0, 0)) % MOD;
        }

        int cur = num[i] - '0';
        int up = isLimit ? cur : 9;
        int start = isNum ? 0 : 1;
        for (int pick = start; pick <= up; pick++) {
            if (pick != prepre && pick != pre) {
                bool nextLimit = isLimit && (pick == up);
                ans = (ans + f(num, i + 1, pre, pick, nextLimit, 1)) % MOD;
            }
        }

        if (!isLimit && isNum) {
            dp[i][prepre][pre] = ans;
        }
        return ans;
    }

    int compute(string& num) {
        if (num[0] == '0') return 0;

        long long ret = 0;
        long long base = 1;
        int n = num.size();
        for (int i = n - 1; i >= 0; i--) {
            ret = (ret + base * (num[i] - '0')) % MOD;
            base = (base * 10) % MOD;
        }
        build();
        //初始传入 prepre = 10, pre = 11 , 避免误判
        return (ret - f(num, 0, 10, 11, 1, 0) + MOD) % MOD;
    }

    bool check(string& num) {
        int n = num.size();
        for (int prepre = -2, pre = -1, i = 0; i < n; i++, prepre++, pre++) {
            if (prepre >= 0 && num[i] == num[prepre]) {
                return true;
            }
            if (pre >= 0 && num[i] == num[pre]) {
                return true;
            }
        }
        return false;
    }

    int main() {
        string l, r;
        cin >> l >> r;
        int p1 = compute(l);
        int p2 = compute(r);
        int ans = (p2 - p1 + MOD) % MOD;
        if (check(l)) {
            ans = (ans + 1) % MOD;
        }
        
        cout << ans;

        return 0;
    }
}



//题目三：
// 不含连续1的非负整数
// 给定一个正整数n，请你统计在[0, n]范围的非负整数中
// 有多少个整数的二进制表示中不存在连续的1
// 测试链接 : https://leetcode.cn/problems/non-negative-integers-without-consecutive-ones/
class Solution {
public:
    int f(string& num, int i, bool isPre, bool isLimit, vector<vector<int>>& dp) {
        if (i == num.size()) return 1;
        if (!isLimit && dp[i][isPre] != -1) return dp[i][isPre];

        int ans = 0;
        int cur = num[i] - '0';
        int up = isLimit ? cur : 1;
        for (int pick = 0; pick <= up; pick++) {
            if (isPre && pick) continue;

            bool nextLimit = isLimit && (pick == up);
            ans += f(num, i + 1, pick == 1, nextLimit, dp);
        }

        if (!isLimit)
            dp[i][isPre] = ans;
        return ans;
    }

    int findIntegers(int n) {
        string num = bitset<32>(n).to_string();
        int bitwidth = num.size();
        vector<vector<int>> dp(bitwidth, vector<int>(2, -1));
        return f(num, 0, false, true, dp);
    }
};



//题目四： 三个链接考察同一个算法
// 范围内的数字计数
// 给定两个正整数a和b，求在[a,b]范围上的所有整数中
// 1 <= a, b
// 某个数码d出现了多少次
// 测试链接 : https://leetcode.cn/problems/digit-count-in-range/

// 范围内的数字计数
// 给定两个正整数a和b，求在[a,b]范围上的所有整数中
// 每个数码(digit)各出现了多少次
// 1 <= a, b
// 测试链接 : https://www.luogu.com.cn/problem/P2602

// 数字1的个数
// 给定一个整数n
// 计算所有小于等于n的非负整数中数字1出现的个数
// 测试链接 : https://leetcode.cn/problems/number-of-digit-one/

class Solution {
public:
    int f(string& num, int i, int cnt, bool isLimit, vector<vector<int>>& dp) {
        if (i == num.size()) return cnt;
        if (!isLimit && dp[i][cnt] != -1) return dp[i][cnt];

        int ans = 0;
        int cur = num[i] - '0';
        int up = isLimit ? cur : 9;
        for (int pick = 0; pick <= up; pick++) {
            bool nextLimit = isLimit && (pick == up);
            if (pick == 1)
                ans += f(num, i + 1, cnt + 1, nextLimit, dp);
            else
                ans += f(num, i + 1, cnt, nextLimit, dp);
        }

        if (!isLimit)
            dp[i][cnt] = ans;

        return ans;
    }

    int countDigitOne(int n) {
        string num = to_string(n);
        int m = num.size();
        vector<vector<int>> dp(m, vector<int>(m, -1));
        return f(num, 0, 0, true, dp);
    }
};


