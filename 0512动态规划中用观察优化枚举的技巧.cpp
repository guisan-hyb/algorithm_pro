#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


//动态规划中用观察优化枚举的技巧 - 上

//动态规划的问题中，已经写完了记忆化搜索的版本，还要写出严格位置依赖的版本，意义何在？
//不仅可以进行空间压缩优化；关键还在于，很多时候通过进一步观察，可以优化枚举，让时间复杂度更好！
//这个话题分为上、下两节来讲述

//注意：
//优化枚举的技巧很多，如多重背包利用二进制分组、单调队列进行优化，本节课讲根据观察优化

//动态规划方法的复杂度大致可以理解为：O(状态数量 * 每个状态的枚举代价)
//当每个状态的枚举代价为O(1)，那么写出记忆化搜索的版本，就是时间复杂度最好的实现了
//但是当每个状态的枚举代价比较高的时候，记忆化搜索的版本可能不是最优解，可能存在进一步的优化
//之所以从记忆化搜索改出了严格位置依赖的版本，是为了建立空间感，让观察并优化枚举的分析变容易

//通过观察优化枚举的技巧包括：
//观察并优化转移方程（本节课、下节课）、观察并设计高效的查询结构（下节课）

//本节课的题目4、题目7，是最能体现观察并优化转移方程技巧的题目
//但题目4属于著名的 买卖股票系列问题 中的一个，所以索性把这个系列全讲了，请重点关注题目4、题目7

//注意：
//到目前为止讲过的题目，如果有复杂枚举行为，都无法进行优化，这是我精心设计过的题目讲述顺序
//讲过的解法都是最优解，放心！当然，斐波那契数列那个题，最优解是矩阵快速幂的解法，后续会讲述



//题目一：
// 买卖股票的最佳时机
// 给定一个数组 prices ，它的第 i 个元素 prices[i] 表示一支给定股票第 i 天的价格
// 你只能选择 某一天 买入这只股票，并选择在 未来的某一个不同的日子 卖出该股票
// 设计一个算法来计算你所能获取的最大利润
// 返回你可以从这笔交易中获取的最大利润
// 如果你不能获取任何利润，返回 0
// 测试链接 : https://leetcode.cn/problems/best-time-to-buy-and-sell-stock/
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.size() == 1) return 0;

        int ans = 0;
        int minPrice = prices[0];
        for (int i = 1; i < prices.size(); i++) {
            ans = max(ans, prices[i] - minPrice);
            minPrice = min(minPrice, prices[i]);
        }
        return ans;
    }
};



//题目二：
// 买卖股票的最佳时机 II
// 给你一个整数数组 prices ，其中 prices[i] 表示某支股票第 i 天的价格
// 在每一天，你可以决定是否购买和/或出售股票
// 你在任何时候 最多 只能持有 一股 股票
// 你也可以先购买，然后在 同一天 出售
// 返回 你能获得的 最大 利润
// 测试链接 : https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-ii/

//思路：把所有的涨幅都赚到 把所有的跌幅都躲过 就是最大利润

class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int ans = 0;
        int n = prices.size();
        int cur = prices[0];
        for (int i = 1; i < n; i++) {
            if (prices[i] > cur) {
                ans += prices[i] - cur;
                cur = prices[i];
            }
            else cur = prices[i];
        }
        return ans;
    }
};

//其实这么写就可以了。。。
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int ans = 0;
        for (int i = 1; i < prices.size(); i++) {
            ans += max(0, prices[i] - prices[i - 1]);
        }
        return ans;
    }
};



//题目三：
// 买卖股票的最佳时机 III
// 给定一个数组，它的第 i 个元素是一支给定的股票在第 i 天的价格。
// 设计一个算法来计算你所能获取的最大利润。你最多可以完成 两笔 交易
// 注意：你不能同时参与多笔交易，你必须在再次购买前出售掉之前的股票
// 测试链接 : https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-iii

//法一：记忆化搜索
class Solution {
public:
    int f(vector<int>& prices, int i, int cnt, int holding,
        vector<vector<vector<int>>>& dp) {
        if (i == prices.size()) return 0;
        if (dp[i][cnt][holding] != -1) return dp[i][cnt][holding];

        int ans;
        if (holding) {
            // 卖出 或 继续持有
            ans = max(prices[i] + f(prices, i + 1, cnt + 1, 0, dp),  // 卖出，交易数+1
                f(prices, i + 1, cnt, 1, dp));                  // 继续持有
        }
        else {
            // 跳过 或 买入（仅当交易数 < 2 时可买入）
            ans = f(prices, i + 1, cnt, 0, dp);                       // 跳过
            if (cnt < 2) {
                ans = max(ans, -prices[i] + f(prices, i + 1, cnt, 1, dp));  // 买入
            }
        }

        return dp[i][cnt][holding] = ans;
    }

    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        vector<vector<vector<int>>> dp(n, vector<vector<int>>(3, vector<int>(2, -1)));
        return f(prices, 0, 0, 0, dp);
    }
};

//法二：动态规划-不优化枚举
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int ans = 0;
        int n = prices.size();
        // dp1[i] : 0...i范围上发生一次交易，不要求在i的时刻卖出，最大利润是多少
        vector<int> dp1(n, 0);
        for (int i = 1, minPrice = prices[0]; i < n; i++) {
            minPrice = min(minPrice, prices[i]);
            dp1[i] = max(dp1[i - 1], prices[i] - minPrice);
        }

        // dp2[i] : 0...i范围上发生两次交易，并且第二次交易在i时刻卖出，最大利润是多少
        vector<int> dp2(n, 0);
        for (int i = 1; i < n; i++) {
            for (int j = 0; j <= i; j++) {
                dp2[i] = max(dp2[i], dp1[j] + prices[i] - prices[j]);
            }
            ans = max(ans, dp2[i]);
        }

        return ans;
    }
};

//法三：动态规划-优化枚举
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int ans = 0;
        int n = prices.size();
        // dp1[i] : 0...i范围上发生一次交易，不要求在i的时刻卖出，最大利润是多少
        vector<int> dp1(n, 0);
        for (int i = 1, minPrice = prices[0]; i < n; i++) {
            minPrice = min(minPrice, prices[i]);
            dp1[i] = max(dp1[i - 1], prices[i] - minPrice);
        }

        // best[i] : 0...i范围上，所有的dp1[i]-prices[i]，最大值是多少
        // 这是数组的设置完全是为了替代最后for循环的枚举行为
        vector<int> best(n, 0);
        best[0] = dp1[0] - prices[0];
        for (int i = 1; i < n; i++) {
            best[i] = max(best[i - 1], dp1[i] - prices[i]);
        }

        // dp2[i] : 0...i范围上发生两次交易，并且第二次交易在i时刻卖出，最大利润是多少
        vector<int> dp2(n, 0);
        for (int i = 0; i < n; i++) {
            // 不需要枚举了
            // 因为，best[i]已经揭示了，0...i范围上，所有的dp1[i]-prices[i]，最大值是多少
            dp2[i] = best[i] + prices[i];
            ans = max(ans, dp2[i]);
        }

        return ans;
    }
};

//法四：法三基础上把代码结构优化了
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int ans = 0;
        int n = prices.size();
        vector<int> dp1(n, 0);
        vector<int> best(n, 0);
        vector<int> dp2(n, 0);
        best[0] = dp1[0] - prices[0];
        for (int i = 1, minPrice = prices[0]; i < n; i++) {
            minPrice = min(minPrice, prices[i]);
            dp1[i] = max(dp1[i - 1], prices[i] - minPrice);
            best[i] = max(best[i - 1], dp1[i] - prices[i]);
            dp2[i] = best[i] + prices[i];
            ans = max(ans, dp2[i]);
        }

        return ans;
    }
};

//法五：动态规划-优化枚举-空间压缩
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        int ans = 0;
        int dp1 = 0, best = -prices[0], dp2 = 0;
        for (int i = 1, minPrice = prices[0]; i < n; i++) {
            minPrice = min(minPrice, prices[i]);
            dp1 = max(dp1, prices[i] - minPrice);
            best = max(best, dp1 - prices[i]);
            dp2 = best + prices[i];
            ans = max(ans, dp2);
        }
        return ans;
    }
};



//题目四：
// 买卖股票的最佳时机 IV
// 给你一个整数数组 prices 和一个整数 k ，其中 prices[i] 是某支给定的股票在第 i 天的价格
// 设计一个算法来计算你所能获取的最大利润。你最多可以完成 k 笔交易
// 也就是说，你最多可以买 k 次，卖 k 次
// 注意：你不能同时参与多笔交易，你必须在再次购买前出售掉之前的股票
// 测试链接 : https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-iv/

//法一：记忆化搜索
class Solution {
public:
    int f(vector<int>& prices, int k, int i, int cnt, int holding, vector<vector<vector<int>>>& dp) {
        if (i == prices.size()) return 0;
        if (dp[i][cnt][holding] != -1) return dp[i][cnt][holding];

        int ans;
        if (holding) {
            // 卖出 或 继续持有
            ans = max(prices[i] + f(prices, k, i + 1, cnt + 1, 0, dp),  // 卖出，交易数+1
                f(prices, k, i + 1, cnt, 1, dp));                  // 继续持有
        }
        else {
            // 跳过 或 买入（仅当交易数 < k 时可买入）
            ans = f(prices, k, i + 1, cnt, 0, dp);                       // 跳过
            if (cnt < k) {
                ans = max(ans, -prices[i] + f(prices, k, i + 1, cnt, 1, dp));  // 买入
            }
        }

        return dp[i][cnt][holding] = ans;
    }

    int maxProfit(int k, vector<int>& prices) {
        int n = prices.size();
        vector<vector<vector<int>>> dp(n, vector<vector<int>>(k + 1, vector<int>(2, -1)));
        return f(prices, k, 0, 0, 0, dp);
    }
};

//法二：动态规划-不优化枚举
class Solution {
public:
    int stock2(vector<int>& prices) {
        int ans = 0;
        for (int i = 1; i < prices.size(); i++) {
            ans += max(0, prices[i] - prices[i - 1]);
        }
        return ans;
    }

    int maxProfit(int k, vector<int>& prices) {
        int n = prices.size();
        if (k >= n / 2) {
            // 这是一个剪枝
            // 如果k >= n / 2，那么代表所有上坡都可以抓到
            // 所有上坡都可以抓到 == 交易次数无限，所以回到股票问题2
            return stock2(prices);
        }

        //dp[i][j]: 最多进行i次交易，在prices 0~j 范围上，最大利润
        // 
        //max:
        //1.第j天不卖：dp[i][j-1]
        //2.第j天卖：max(dp[i-1][p]+prices[j]-prices[p]) 0<=p<j
        vector<vector<int>> dp(k + 1, vector<int>(n, 0));
        for (int i = 1; i <= k; i++) {
            for (int j = 1; j < n; j++) {
                dp[i][j] = dp[i][j - 1];
                for (int p = 0; p < j; p++) {
                    dp[i][j] = max(dp[i][j], dp[i - 1][p] + prices[j] - prices[p]);
                }
            }
        }

        return dp[k][n - 1];
    }
};

//法三：动态规划-优化枚举
class Solution {
public:
    int stock2(vector<int>& prices) {
        int ans = 0;
        for (int i = 1; i < prices.size(); i++) {
            ans += max(0, prices[i] - prices[i - 1]);
        }
        return ans;
    }

    int maxProfit(int k, vector<int>& prices) {
        int n = prices.size();
        if (k >= n / 2) {
            return stock2(prices);
        }

        vector<vector<int>> dp(k + 1, vector<int>(n, 0));
        for (int i = 1, best = 0; i <= k; i++) {
            //best维护：所有dp[i-1][p]-prices[p]的最大值   0<=p<j
            best = dp[i - 1][0] - prices[0];
            for (int j = 1; j < n; j++) {
                dp[i][j] = max(dp[i][j - 1], best + prices[j]);
                best = max(best, dp[i - 1][j] - prices[j]);
            }
        }

        return dp[k][n - 1];
    }
};

//法四：动态规划-优化枚举-空间压缩
class Solution {
public:
    int stock2(vector<int>& prices) {
        int ans = 0;
        for (int i = 1; i < prices.size(); i++) {
            ans += max(0, prices[i] - prices[i - 1]);
        }
        return ans;
    }

    int maxProfit(int k, vector<int>& prices) {
        int n = prices.size();
        if (k >= n / 2) {
            return stock2(prices);
        }

        vector<int> dp(n, 0);
        for (int i = 1; i <= k; i++) {
            int best = dp[0] - prices[0];
            for (int j = 1; j < n; j++) {
                int tmp = dp[j];
                dp[j] = max(dp[j - 1], best + prices[j]);
                best = max(best, tmp - prices[j]);
            }
        }

        return dp[n - 1];
    }
};



//题目五：
// 买卖股票的最佳时机含手续费
// 给定一个整数数组 prices，其中 prices[i]表示第 i 天的股票价格
// 整数 fee 代表了交易股票的手续费用
// 你可以无限次地完成交易，但是你每笔交易都需要付手续费
// 如果你已经购买了一个股票，在卖出它之前你就不能再继续购买股票了。
// 返回获得利润的最大值
// 注意：这里的一笔交易指买入持有并卖出股票的整个过程，每笔交易你只需要为支付一次手续费
// 测试链接 : https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-with-transaction-fee/
//
//记忆化搜索
class Solution {
public:
    int f(vector<int>& prices, int fee, int i, int hold, vector<vector<int>>& dp) {
        if (i == prices.size()) {
            return 0;
        }

        if (dp[i][hold] != -1) return dp[i][hold];

        int ans = 0;
        if (hold) {
            ans = max(f(prices, fee, i + 1, 1, dp), f(prices, fee, i + 1, 0, dp) + prices[i]);
        }
        else {
            ans = max(f(prices, fee, i + 1, 0, dp), f(prices, fee, i + 1, 1, dp) - prices[i] - fee);
        }
        dp[i][hold] = ans;
        return ans;
    }

    int maxProfit(vector<int>& prices, int fee) {
        int n = prices.size();
        vector<vector<int>> dp(n, vector<int>(2, -1));
        return f(prices, fee, 0, 0, dp);
    }
};



//题目六：
// 买卖股票的最佳时机含冷冻期
// 给定一个整数数组prices，其中第  prices[i] 表示第 i 天的股票价格
// 设计一个算法计算出最大利润
// 在满足以下约束条件下，你可以尽可能地完成更多的交易（多次买卖一支股票）:
// 卖出股票后，你无法在第二天买入股票 (即冷冻期为 1 天)
// 注意：你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）
// 测试链接 : https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-with-cooldown/
class Solution {
public:
    int f(vector<int>& prices, int i, int hold, vector<vector<int>>& dp) {
        if (i >= prices.size()) {
            return 0;
        }
        if (dp[i][hold] != -1) return dp[i][hold];

        int ans = 0;
        if (hold) {
            ans = max(f(prices, i + 1, 1, dp), f(prices, i + 2, 0, dp) + prices[i]);
        }
        else {
            ans = max(f(prices, i + 1, 0, dp), f(prices, i + 1, 1, dp) - prices[i]);
        }
        dp[i][hold] = ans;
        return ans;
    }

    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        vector<vector<int>> dp(n, vector<int>(2, -1));
        return f(prices, 0, 0, dp);
    }
};



//题目七：
// DI序列的有效排列
// 给定一个长度为n的字符串s，其中s[i]是:
// "D"意味着减少，"I"意味着增加
// 有效排列是对有n+1个在[0,n]范围内的整数的一个排列perm，使得对所有的i：
// 如果 s[i] == 'D'，那么 perm[i] > perm[i+1]
// 如果 s[i] == 'I'，那么 perm[i] < perm[i+1]
// 返回有效排列的perm的数量
// 因为答案可能很大，答案对 1000000007 取模
// 测试链接 : https://leetcode.cn/problems/valid-permutations-for-di-sequence/

//题目7 核心 就是 定义状态，我们只能想到 用二进制数做状压dp，但是数字太多 做不了。
// 老师 提炼出 数字是几 不重要，只要 知道后面有几个数字 小于 当前数字， 状态就确定了 这一关键信息。 
// 化繁为简。 
// 为什么这样能确定 可能的数量？因为 你 选了哪个数字，和 nextLess 是一一对应的。
// 一个可能的nextLess数字，就代表选了某一个数。 暂时就理解到这。

//法一：记忆化搜索
class Solution {
public:
    const int MOD = 1e9 + 7;

    // 猜法很妙！
    // 一共有n个数字，位置范围0~n-1
    // 当前来到i位置，i-1位置的数字已经确定，i位置的数字还没确定
    // i-1位置和i位置的关系，是s[i-1] : D、I
    // 0~i-1范围上是已经使用过的数字，i个
    // 还没有使用过的数字中，比i-1位置的数字小的，有less个
    // 还没有使用过的数字中，比i-1位置的数字大的，有n - i - less个
    // 返回后续还有多少种有效的排列

    //dp[i][less]: 当前正在填写第i个位置，且在剩余可选的数字中，比前一个位置数字(perm[i - 1])小的数字有less个时，后续还能产生的有效排列数量
    int f(string& s, int n, int i, int less, vector<vector<int>>& dp) {
        if (i == n) return 1;
        if (dp[i][less] != -1) return dp[i][less];
        int ans = 0;
        if (i == 0 || s[i - 1] == 'D') {
            for (int nxtless = 0; nxtless < less; nxtless++) {
                ans = (ans + f(s, n, i + 1, nxtless, dp)) % MOD;
            }
        }
        else if (s[i - 1] == 'I') {
            for (int nxtless = less; nxtless < n - i; nxtless++) {
                ans = (ans + f(s, n, i + 1, nxtless, dp)) % MOD;
            }
        }
        dp[i][less] = ans;
        return ans;
    }

    int numPermsDISequence(string s) {
        int n = s.size() + 1;
        vector<vector<int>> dp(n, vector<int>(n + 1, -1));
        return f(s, n, 0, n, dp);
    }
};

//法二：动态规划
class Solution {
public:
    int numPermsDISequence(string s) {
        const int MOD = 1e9 + 7;
        int n = s.size() + 1;
        vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));
        //初始化
        for (int less = 0; less <= n; less++)
            dp[n][less] = 1;

        for (int i = n - 1; i >= 0; i--) {
            for (int less = 0; less <= n; less++) {
                if (i == 0 || s[i - 1] == 'D') {
                    for (int nxtless = 0; nxtless < less; nxtless++) {
                        dp[i][less] = (dp[i][less] + dp[i + 1][nxtless]) % MOD;
                    }
                }
                else if (s[i - 1] == 'I') {
                    for (int nxtless = less; nxtless < n - i; nxtless++) {
                        dp[i][less] = (dp[i][less] + dp[i + 1][nxtless]) % MOD;
                    }
                }
            }
        }
        
        return dp[0][n];
    }
};

//浅谈 为什么记忆化搜索时是 n * n+1 个格子，动态规划时是 n+1 * n+1 个格子？
//因为记忆化搜索访问越界时有base case兜底来返回值，而动态规划没有
//所以动态规划要多准备一行存储越界时的值，即记忆化搜索中base case 返回的值
//而初始化的也正是这一部分

//法三：动态规划+优化枚举(前缀和优化)
class Solution {
public:
    int numPermsDISequence(string s) {
        const int MOD = 1e9 + 7;
        int n = s.size() + 1;
        vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));
        //初始化
        for (int less = 0; less <= n; less++)
            dp[n][less] = 1;

        for (int i = n - 1; i >= 0; i--) {
            if (i == 0 || s[i - 1] == 'D') {
                dp[i][1] = dp[i + 1][0];
                for (int less = 2; less <= n; less++) {
                    dp[i][less] = (dp[i][less - 1] + dp[i + 1][less - 1]) % MOD;
                }
            }
            else if (s[i - 1] == 'I') {
                dp[i][n - i - 1] = dp[i + 1][n - i - 1];
                for (int less = n - i - 2; less >= 0; less--) {
                    dp[i][less] = (dp[i][less + 1] + dp[i + 1][less]) % MOD;
                }
            }
        }

        return dp[0][n];
    }
};







//动态规划中用观察优化枚举的技巧 - 下


//上节课讲了利用严格位置依赖的动态规划来 建立空间感 ，进而观察并优化枚举
//本节课继续讲述 观察并优化转移方程 来优化枚举（题目1、2）
//同时还要讲述 观察并设计高效的查询结构 来优化枚举（题目3、4）


//题目一：
// 规划兼职工作
// 你打算利用空闲时间来做兼职工作赚些零花钱，这里有n份兼职工作
// 每份工作预计从startTime[i]开始、endTime[i]结束，报酬为profit[i]
// 返回可以获得的最大报酬
// 注意，时间上出现重叠的 2 份工作不能同时进行
// 如果你选择的工作在时间X结束，那么你可以立刻进行在时间X开始的下一份工作
// 测试链接 : https://leetcode.cn/problems/maximum-profit-in-job-scheduling/

//动态规划+二分优化枚举
class Solution {
public:
    int binSearch(vector<vector<int>>& info, int i) {
        int ans = -1;
        int stime = info[i][0];
        for (int l = 0, r = i - 1; l <= r;) {
            int m = l + (r - l) / 2;
            if (info[m][1] <= stime) {
                ans = m;
                l = m + 1;
            }
            else {
                r = m - 1;
            }
        }
        return ans;
    }

    int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {
        int n = startTime.size();
        vector<vector<int>> info(n, vector<int>(3, 0));
        for (int i = 0; i < n; i++) {
            info[i][0] = startTime[i];
            info[i][1] = endTime[i];
            info[i][2] = profit[i];
        }

        //按 结束时间 升序排列
        sort(info.begin(), info.end(), [](vector<int>& a, vector<int>& b) {
            return a[1] < b[1];
            });

        //dp[i]:编号0~i任务中 能获得的最大报酬
        //1.要i位置的任务 -> 需要找到最近一个结束时间<=该任务开始时间的任务
        //2.不要i位置的任务 dp[i-1]
        vector<int> dp(n, 0);
        //初始化：
        dp[0] = info[0][2];
        for (int i = 1; i < n; i++) {
            int find = binSearch(info, i);
            int p = (find == -1 ? 0 : dp[find]) + info[i][2];
            
            dp[i] = max(dp[i - 1], p);
        }

        return dp[n - 1];
    }
};



//题目二：
// K个逆序对数组
// 逆序对的定义如下：
// 对于数组nums的第i个和第j个元素
// 如果满足0<=i<j<nums.length 且 nums[i]>nums[j]，则为一个逆序对
// 给你两个整数n和k，找出所有包含从1到n的数字
// 且恰好拥有k个逆序对的不同的数组的个数
// 由于答案可能很大，答案对 1000000007 取模
// 测试链接 : https://leetcode.cn/problems/k-inverse-pairs-array/

//法一：动态规划 -> 像 合唱团 那个题
class Solution {
public:
    int kInversePairs(int n, int k) {
        const int MOD = 1e9 + 7;
        //dp[i][j]: 1~i这些数字，恰好有j个逆序对的不同排列个数
        //假设最大的数字为e
        //1.e排在最后 -> dp[i-1][j]
        //2.e排在倒数第二 -> dp[i-1][j-1]
        //3.e排在倒数第三 -> dp[i-1][j-2]
        vector<vector<int>> dp(n + 1, vector<int>(k + 1, 0));
        //初始化：
        for (int i = 0; i <= n; i++)
            dp[i][0] = 1;

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= k; j++) {
                if (i > j) {
                    for (int p = 0; p <= j; p++) {
                        dp[i][j] = (dp[i][j] + dp[i - 1][p]) % MOD;
                    }
                }
                else {
                    for (int p = j - i + 1; p <= j; p++) {
                        dp[i][j] = (dp[i][j] + dp[i - 1][p]) % MOD;
                    }
                }
            }
        }

        return dp[n][k];
    }
};

//法二：动态规划+滑动窗口/前缀和优化
class Solution {
public:
    int kInversePairs(int n, int k) {
        const int MOD = 1e9 + 7;
        vector<vector<int>> dp(n + 1, vector<int>(k + 1, 0));
        //初始化：
        for (int i = 0; i <= n; i++) {
            dp[i][0] = 1;
        }

        for (int i = 1; i <= n; i++) {
            int slideWindow = 1;
            for (int j = 1; j <= k; j++) {
                if (i > j) {
                    slideWindow = (slideWindow + dp[i - 1][j]) % MOD;
                }
                else {
                    slideWindow = ((slideWindow + dp[i - 1][j]) % MOD - dp[i - 1][j - i] + MOD) % MOD;
                }
                dp[i][j] = slideWindow;
            }
        }

        return dp[n][k];
    }
};

//补一个记忆化搜索版本 - 没有优化
class Solution {
public:
    const int MOD = 1e9 + 7;

    int f(int i, int k, vector<vector<int>>& dp) {
        if (i < 0) return 0;
        if (k == 0) return 1;
        if (dp[i][k] != -1) return dp[i][k];

        int ans = 0;
        for (int p = k; p >= max(0, k - i + 1); p--) {
            ans = (ans + f(i - 1, p, dp)) % MOD;
        }
        dp[i][k] = ans;
        return ans;
    }

    int kInversePairs(int n, int k) {
        vector<vector<int>> dp(n + 1, vector<int>(k + 1, -1));
        return f(n, k, dp);
    }
};

//记忆化搜索+前缀和优化
class Solution {
public:
    const int MOD = 1e9 + 7;

    int f(int i, int k, vector<vector<int>>& dp) {
        if (i == 0) return k == 0 ? 1 : 0;
        if (k < 0) return 0;
        if (dp[i][k] != -1) return dp[i][k];

        int ans = (f(i, k - 1, dp) + f(i - 1, k, dp)) % MOD;
        if (k >= i) {
            ans = (ans - f(i - 1, k - i, dp) + MOD) % MOD;
        }

        dp[i][k] = ans;
        return ans;
    }

    int kInversePairs(int n, int k) {
        vector<vector<int>> dp(n + 1, vector<int>(k + 1, -1));
        return f(n, k, dp);
    }
};

//根据这个版本的记忆化，可以改出这一版的动态规划（前缀和优化）
class Solution {
public:
    const int MOD = 1e9 + 7;

    int kInversePairs(int n, int k) {
        vector<vector<int>> dp(n + 1, vector<int>(k + 1, 0));
        for (int i = 0; i <= n; i++) {
            dp[i][0] = 1;
        }

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= k; j++) {
                // 核心递推公式：dp[i][j] = dp[i][j-1] + dp[i-1][j] - dp[i-1][j-i]
                long long val = (long long)dp[i][j - 1] + dp[i - 1][j];

                // 当 j >= i 时，需要减去溢出窗口的那一项 dp[i-1][j-i]
                if (j >= i) {
                    val -= dp[i - 1][j - i];
                }

                dp[i][j] = (val % MOD + MOD) % MOD;
            }
        }

        return dp[n][k];
    }
};



//题目三四以后有缘再看吧
