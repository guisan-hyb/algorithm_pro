#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


//状压dp--上
//设计一个整型可变参数status,利用status的位信息，来表示：
//某个样本是否还能使用，然后利用这个信息进行尝试
//写出尝试的递归函数 -> 记忆化搜索 -> 严格位置依赖的动态规划 -> 空间压缩等优化

//如果有k个样本，那么表示这些样本的状态，数量是2^k
//所以可变参数status的范围：0 ~ (2^k)-1

//样本每增加一个，状态的数量是指数级增长的，所以状压dp能解决的问题往往样本数量都不大
//一般样本数量在20个以内(10^6)，如果超过这个数量，计算量（指令条数）会超过 10^7 ~ 10^8

//如果样本数量大到状压dp解决不了，或者任何动态规划都不可行，那么双向广搜(折半dfs)是一个备选思路


//题目一：
// 我能赢吗
// 给定两个整数n和m
// 两个玩家可以轮流从公共整数池中抽取从1到n的整数（不放回）
// 抽取的整数会累加起来（两个玩家都算）
// 谁在自己的回合让累加和 >= m，谁获胜
// 若先出手的玩家能稳赢则返回true，否则返回false
// 假设两位玩家游戏时都绝顶聪明，可以全盘为自己打算
// 测试链接 : https://leetcode.cn/problems/can-i-win/

//注意：这道题有两个可变参数status,rest
//但最关键的可变参数就1个，即status，表示还有哪些数字可以使用
//另一个可变参数rest是被status决定的，所以只需要对status做缓存表

//核心：任何动态规划都是这样！只关注最关键的可变参数，被决定的可变参数不用管！不重要！

class Solution {
public:
    // 如果1~7范围的数字都能选，那么status的状态为：
    // 1 1 1 1 1 1 1 1
    // 7 6 5 4 3 2 1 0
    // 0位弃而不用
    // 如果1~7范围的数字，4、2已经选了不能再选，那么status的状态为：
    // 1 1 1 0 1 0 1 1
    // 7 6 5 4 3 2 1 0
    // 0位弃而不用
    // f的含义 :
    // 数字范围1~n，当前的先手，面对status给定的数字状态
    // 在累加和还剩rest的情况下
    // 返回当前的先手能不能赢，能赢返回true，不能赢返回false
    int f(int maxChoosableInteger, int status, int rest, vector<char>& dp) {
        if (rest <= 0) return false;
        if (dp[status] != -1) return dp[status];

        int ans = 0;
        for (int i = 1; i <= maxChoosableInteger; i++) {
            if ((status & (1 << i)) && !f(maxChoosableInteger, status ^ (1 << i), rest - i, dp)) {
                ans = 1;
                break;
            }
        }
        dp[status] = ans;
        return ans;
    }

    bool canIWin(int maxChoosableInteger, int desiredTotal) {
        if (desiredTotal == 0) return true;

        // 如果1~n数字全加起来
        // 累加和和是n * (n+1) / 2，都小于m
        // 那么不会有赢家，也就意味着先手不会获胜
        if ((1 + maxChoosableInteger) * maxChoosableInteger / 2 < desiredTotal) return false;

        vector<char> dp(1 << (maxChoosableInteger + 1), -1);
        int status = (1 << (maxChoosableInteger + 1)) - 1;
        return f(maxChoosableInteger, status, desiredTotal, dp);
    }
};



//题目二：
// 火柴拼正方形
// 你将得到一个整数数组 matchsticks
// 其中 matchsticks[i] 是第 i 个火柴棒的长度
// 你要用 所有的火柴棍 拼成一个正方形
// 你 不能折断 任何一根火柴棒，但你可以把它们连在一起，而且每根火柴棒必须 使用一次
// 如果你能拼出正方形，则返回 true ，否则返回 false
// 测试链接 : https://leetcode.cn/problems/matchsticks-to-square/
class Solution {
public:
    // len : 每条边规定的长度
    // status : 表示哪些数字还可以选
    // cur : 当前要解决的这条边已经形成的长度
    // rest : 一共还有几条边没有解决
    // 返回 : 能否用光所有火柴去解决剩下的所有边
    // 因为调用子过程之前，一定保证每条边累加起来都不超过limit
    // 所以status是决定cur和rest的，关键可变参数只有status
    int f(vector<int>& nums, int len, int status, int curLen, int rest, vector<char>& dp) {
        if (rest == 0) return status == 0;
        if (dp[status] != -1) return dp[status];

        int ans = 0;
        for (int i = 0; i < nums.size(); i++) {
            if (ans) break;

            if (status & (1 << i)) {
                if (curLen + nums[i] < len) {
                    ans = f(nums, len, status ^ (1 << i), curLen + nums[i], rest, dp);
                }
                else if (curLen + nums[i] == len) {
                    ans = f(nums, len, status ^ (1 << i), 0, rest - 1, dp);
                }
            }
        }
        dp[status] = ans;
        return ans;
    }

    bool makesquare(vector<int>& matchsticks) {
        int sum = 0;
        for (auto& ele : matchsticks) {
            sum += ele;
        }

        if (sum % 4 != 0) return false;
        int n = matchsticks.size();
        vector<char> dp(1 << n, -1);
        int status = (1 << n) - 1;
        return f(matchsticks, sum / 4, status, 0, 4, dp);
    }
};



//题目三：
// 划分为k个相等的子集
// 给定一个整数数组  nums 和一个正整数 k，
// 找出是否有可能把这个数组分成 k 个非空子集，其总和都相等。
// 测试链接 : https://leetcode.cn/problems/partition-to-k-equal-sum-subsets/

//法一：状压dp记忆化搜索
//与上一题一样
class Solution {
public:
    int f(vector<int>& nums, int limit, int status, int curSum, int restGroup, vector<char>& dp) {
        if (restGroup == 0) return status == 0;
        if (dp[status] != -1) return dp[status];

        int ans = 0;
        for (int i = 0; i < nums.size(); i++) {
            if (status & (1 << i)) {
                if (curSum + nums[i] < limit) {
                    ans = f(nums, limit, status ^ (1 << i), curSum + nums[i], restGroup, dp);
                }
                else if (curSum + nums[i] == limit) {
                    ans = f(nums, limit, status ^ (1 << i), 0, restGroup - 1, dp);
                }

                if (ans) break;
            }
        }
        dp[status] = ans;
        return ans;
    }

    bool canPartitionKSubsets(vector<int>& nums, int k) {
        int sum = 0;
        for (auto& ele : nums) sum += ele;
        if (sum % k != 0) return false;

        int n = nums.size();
        int status = (1 << n) - 1;
        vector<char> dp(1 << n, -1);
        return f(nums, sum / k, status, 0, k, dp);
    }
};

//法二：暴力递归+良好剪枝
class Solution {
public:
    // group里面是各个集合已经有的累加和
    // 随着递归的展开，group里的累加和会变化
    // 所以这是一个带路径的递归，而且路径信息比较复杂(group数组)
    // 无法改成动态规划，但是利用剪枝策略可以通过
    // group[0....index]这些数字，填入每个集合，一定要都使用
    // 每个集合的累加和一定都要是target，返回能不能做到
    bool f(vector<int>& group, int aim, vector<int>& nums, int idx) {
        if (idx < 0) return true;

        for (int i = 0; i < group.size(); i++) {
            if (group[i] + nums[idx] <= aim) {
                group[i] += nums[idx];
                if (f(group, aim, nums, idx - 1)) return true;

                group[i] -= nums[idx];//回溯
                while (i + 1 < group.size() && group[i] == group[i + 1]) i++;//剪枝
            }
        }

        return false;
    }

    bool canPartitionKSubsets(vector<int>& nums, int k) {
        int sum = 0;
        for (auto& ele : nums) sum += ele;
        if (sum % k != 0) return false;

        sort(nums.begin(), nums.end());
        int n = nums.size();
        vector<int> group(k, 0);
        return f(group, sum / k, nums, n - 1);
    }
};



//题目四：
// 售货员的难题 - TSP问题
// 某乡有n个村庄(1<=n<=20)，有一个售货员，他要到各个村庄去售货
// 各村庄之间的路程s(1<=s<=1000)是已知的
// 且A村到B村的路程，与B到A的路大多不同(有向带权图)
// 为了提高效率，他从商店出发到每个村庄一次，然后返回商店所在的村，
// 假设商店所在的村庄为1
// 他不知道选择什么样的路线才能使所走的路程最短
// 请你帮他选择一条最短的路
// 测试链接 : https://www.luogu.com.cn/problem/P1171
namespace test4 {
    #include <iostream>
    #include <vector>
    #include <climits>
    using namespace std;

    const int MAXN = 21;
    int n;
    vector<vector<int>> matrix(MAXN, vector<int>(MAXN, 0));//邻接矩阵
    vector<vector<int>> dp;

    //status中：1表示已经访问过，0表示未访问过
    //从哪个村庄出发结果都一样，这里默认从村庄0开始访问
    //
    //注意：i代表当前所在的村庄，这个参数不能被状态信息推导出来，是“关键的”
    int f(int status, int i) {
        if (status == (1 << n) - 1) return matrix[i][0];

        if (dp[status][i] != -1) return dp[status][i];

        int ans = INT_MAX;
        for (int k = 1; k < n; k++) {
            if ((status & (1 << k)) == 0) {
                ans = min(ans, f(status | (1 << k), k) + matrix[i][k]);
            }
        }
        dp[status][i] = ans;
        return ans;
    }

    int main() {
        ios::sync_with_stdio(false);
        cin.tie(nullptr);

        cin >> n;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cin >> matrix[i][j];
            }
        }
        dp.assign(1 << n, vector<int>(n, -1));
        
        cout << f(1, 0);

        return 0;
    }
}

namespace test4_ {
#include <iostream>
#include <cstring>
#include <climits>
#include <algorithm>

    using namespace std;

    const int MAXN = 21;
    int n;
    int matrix[MAXN][MAXN]; 

    int dp[1 << 20][MAXN];

    int f(int status, int i) {
        if (status == (1 << n) - 1) return matrix[i][0];

        if (dp[status][i] != -1) return dp[status][i];

        int ans = INT_MAX;
        for (int k = 1; k < n; k++) {
            if ((status & (1 << k)) == 0) {
                ans = min(ans, f(status | (1 << k), k) + matrix[i][k]);
            }
        }

        return dp[status][i] = ans;
    }

    int main() {
        // 优化输入输出流速度
        ios::sync_with_stdio(false);
        cin.tie(nullptr);

        cin >> n;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cin >> matrix[i][j];
            }
        }

        memset(dp, -1, sizeof(dp));

        cout << f(1, 0) << endl;

        return 0;
    }
}








//状压dp--下
//本节课继续讲述4个状压dp问题，以及重要技巧：如何在位状态上，枚举所有子集的状态（题目4）


//题目一：
// 每个人戴不同帽子的方案数
// 总共有 n 个人和 40 种不同的帽子，帽子编号从 1 到 40
// 给你一个整数列表的列表 hats ，其中 hats[i] 是第 i 个人所有喜欢帽子的列表
// 请你给每个人安排一顶他喜欢的帽子，确保每个人戴的帽子跟别人都不一样，并返回方案数
// 由于答案可能很大，答案对 1000000007 取模
// 测试链接 : https://leetcode.cn/problems/number-of-ways-to-wear-different-hats-to-each-other

//法一：状压dp
class Solution {
public:
    const int MOD = 1e9 + 7;

    int f(vector<int>& hat, int n, int status, int hatIdx, vector<vector<int>>& dp) {
        if (status == (1 << n) - 1) return 1;
        if (hatIdx == hat.size()) return 0;

        if (dp[status][hatIdx] != -1) return dp[status][hatIdx];

        int hatInfo = hat[hatIdx];
        //1.当前帽子不给任何人
        int ans = f(hat, n, status, hatIdx + 1, dp) % MOD;
        //2.当前帽子可能给某个人
        for (int i = 0; i < n; i++) {
            if ((hatInfo & (1 << i)) && !(status & (1 << i))) {
                ans = (ans + f(hat, n, status | (1 << i), hatIdx + 1, dp)) % MOD;
            }
        }

        dp[status][hatIdx] = ans;
        return ans;
    }

    int numberWays(vector<vector<int>>& hats) {
        int n = hats.size();
        //由于帽子数量>20，如果记录帽子的状态则有 2^40 次方
        //因此可以反过来考虑：帽子戴人
        int m = 0;//记录编号最大的帽子
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < hats[i].size(); j++) {
                m = max(m, hats[i][j]);
            }
        }

        //这里也可以用二维数组存储每个帽子哪些人可以戴
        //这里用位信息存储，防止MLE
        vector<int> hat(m + 1, 0);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < hats[i].size(); j++) {
                hat[hats[i][j]] |= (1 << i);
            }
        }

        //非特殊情况下（如状压dp--上--题目二，另外两个参数可以由关键参数决定）,
        //DP 状态的维度必须和递归函数的可变参数保持一致
        vector<vector<int>> dp(1 << n, vector<int>(m + 1, -1));
        int status = 0;
        return f(hat, n, status, 1, dp);
    }
};

//法二：状压dp+优化
//对于每个 hatInfo 只要我们提取每个1，就不用 for循环 了
//    -->> Brian算法
class Solution {
public:
    const int MOD = 1e9 + 7;

    int f(vector<int>& hat, int n, int status, int hatIdx, vector<vector<int>>& dp) {
        if (status == (1 << n) - 1) return 1;
        if (hatIdx == hat.size()) return 0;

        if (dp[status][hatIdx] != -1) return dp[status][hatIdx];

        int hatInfo = hat[hatIdx];
        //1.当前帽子不给任何人
        int ans = f(hat, n, status, hatIdx + 1, dp) % MOD;
        //2.当前帽子可能给某个人
        int rightone = 0;//记录hatInfo中最右侧的1
        while (hatInfo != 0) {
            rightone = hatInfo & (-hatInfo);
            if (!(status & rightone)) {
                ans = (ans + f(hat, n, status ^ rightone, hatIdx + 1, dp)) % MOD;
            }
            hatInfo ^= rightone;
        }

        dp[status][hatIdx] = ans;
        return ans;
    }

    int numberWays(vector<vector<int>>& hats) {
        int n = hats.size();
        int m = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < hats[i].size(); j++) {
                m = max(m, hats[i][j]);
            }
        }

        vector<int> hat(m + 1, 0);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < hats[i].size(); j++) {
                hat[hats[i][j]] |= (1 << i);
            }
        }

        vector<vector<int>> dp(1 << n, vector<int>(m + 1, -1));
        int status = 0;
        return f(hat, n, status, 1, dp);
    }
};



//题目二：
// 最优账单平衡
// 给你一个表示交易的数组 transactions
// 其中 transactions[i] = [fromi, toi, amounti]
// 表示 ID = fromi 的人给 ID = toi 的人共计 amounti
// 请你计算并返回还清所有债务的最小交易笔数
// 测试链接 : https://leetcode.cn/problems/optimal-account-balancing/
//
//思路：
//根据给出的数组最终可以化成一个负债表，我们只要负债表中不为0的数据
//计算出该负债表最多能分为几个和为0的子集，答案即为 n - sets
//(一个大小为m,和为0的集合的交易笔数为m-1 , 765. 情侣牵手，这道题：https://leetcode.cn/problems/couples-holding-hands/description/)
//如何计算划分出和为0的集合的最大数目？
//1.当前子集和为0：不要集合中的一个数据时所能划分的集合数目+1（无需枚举所有数据，答案都是一样的）
//2.当前子集和不为0：枚举所有数据，得到最大值

//注：参数sum由status决定，故dp表仍是一维

class Solution {
    vector<int> computeDebt(vector<vector<int>>& nums) {
        int n = nums.size();
        vector<int> help(n, 0);
        for (auto& ele : nums) {
            help[ele[0]] -= ele[2];
            help[ele[1]] += ele[2];
        }

        vector<int> debt;
        for (auto& ele : help) {
            if (ele != 0) {
                debt.push_back(ele);
            }
        }
        return debt;
    }

    int f(vector<int>& debt, int n, int status, int sum, vector<int>& dp) {
        if (dp[status] != -1) return dp[status];

        int ans = 0;
        //集合中数据大于1
        if (status & (status - 1)) { //学习这个trick
            if (sum == 0) {
                for (int i = 0; i < n; i++) {
                    if (status & (1 << i)) {
                        // 找到任何一个元素，去除这个元素
                        // 剩下的集合进行尝试，返回值 + 1
                        ans = f(debt, n, status ^ (1 << n), sum - debt[i], dp);
                        // 然后不需要再尝试下一个元素了，因为答案一定是一样的
                        // 所以直接break
                        break;
                    }
                }
            }
            else {
                for (int i = 0; i < n; i++) {
                    if (status & (1 << i)) {
                        ans = max(ans, f(debt, n, status ^ (1 << i), sum - debt[i], dp));
                    }
                }
            }
        }
        dp[status] = ans;
        return ans;
    }

    int minTransfers(vector<vector<int>>& transactions) {
        vector<int> debt = computeDebt(transactions);
        int n = debt.size();
        vector<int> dp(1 << n, -1);
        return n - f(debt, n, (1 << n) - 1, 0, dp);
    }
};



//题目三：
// 好子集的数目
// 给你一个整数数组 nums，好子集的定义如下：
// nums的某个子集，所有元素的乘积可以表示为一个或多个互不相同质数的乘积
// 比如nums = [1, 2, 3, 4]
// [2, 3]，[1, 2, 3]，[1, 3] 是好子集
// 乘积分别为6=2*3，6=2*3，3=3
// [1, 4]和[4]不是好子集，因为乘积分别为4=2*2和4=2*2
// 返回nums中不同的好子集的数目，答案对 1000000007 取模
// 如果两个子集删除的下标不同，那么它们被视为不同的子集
// 测试链接 : https://leetcode.cn/problems/the-number-of-good-subsets/

//思路：
//由于 “如果两个子集删除的下标不同，那么它们被视为不同的子集”，说明答案与子集顺序无关
//   ->维护一个数组cnt，统计不同元素的个数
//因为nums[i]<=30 ，可以用状态记录符合要求的情况下质数的情况：
//例如：质因子 5，3，2
//则状态有：   0  0  1
//             0  1  0
//             0  1  1
//             1  0  0
//             1  0  1
//             1  1  0
//             1  1  1
//
//定义dp[i][s]: 1~i范围的cnt上，质因数状态为s的子集数目
//1.要cnt[i]
//2.不要cnt[i]

class Solution {
public:
    const int MAXV = 30;
    const int LIMIT = 1 << 10;
    const int MOD = 1000000007;

    // 打个表来加速判断（学习这个trick）
    // 如果一个数字拥有某一种质数因子不只1个
    // 那么认为这个数字无效，状态全是0，0b0000000000
    // 如果一个数字拥有任何一种质数因子都不超过1个
    // 那么认为这个数字有效，用位信息表示这个数字拥有质数因子的状态
    // 比如12，拥有2这种质数因子不只1个，所以无效，用0b0000000000表示
    // 比如14，拥有2这种质数因子不超过1个，拥有7这种质数因子不超过1个，有效
    // 从高位到低位依次表示：...13 11 7 5 3 2
    // 所以用0b0000001001表示14拥有质数因子的状态
    // 质数: 29 23 19 17 13 11 7 5 3 2
    // 位置: 9  8  7  6  5  4  3 2 1 0
    vector<int> own = { 0b0000000000, // 0
            0b0000000000, // 1
            0b0000000001, // 2
            0b0000000010, // 3
            0b0000000000, // 4
            0b0000000100, // 5
            0b0000000011, // 6
            0b0000001000, // 7
            0b0000000000, // 8
            0b0000000000, // 9
            0b0000000101, // 10
            0b0000010000, // 11
            0b0000000000, // 12
            0b0000100000, // 13
            0b0000001001, // 14
            0b0000000110, // 15
            0b0000000000, // 16
            0b0001000000, // 17
            0b0000000000, // 18
            0b0010000000, // 19
            0b0000000000, // 20
            0b0000001010, // 21
            0b0000010001, // 22
            0b0100000000, // 23
            0b0000000000, // 24
            0b0000000000, // 25
            0b0000100001, // 26
            0b0000000000, // 27
            0b0000000000, // 28
            0b1000000000, // 29
            0b0000000111 // 30
    };

    // 1....i范围的数字，每种数字cnt[i]个
    // 最终相乘的结果一定要让质因子的状态为s，且每种质因子只能有1个
    // 请问子集的数量是多少
    // s每一位代表的质因子如下
    // 质数: 29 23 19 17 13 11 7 5 3 2
    // 位置: 9  8  7  6  5  4  3 2 1 0
    int f(vector<int>& cnt, int i, int s, vector<vector<int>>& dp) {
        if (dp[i][s] != -1) return dp[i][s];

        int ans = 0;
        if (i == 1) { //1用来给答案乘倍率
            if (s == 0) {
                ans = 1;
                for (int j = 0; j < cnt[1]; j++) {
                    //假设有5个1，那么这5个1都有可能放到一个符合要求的集合中，这5个1有2^5中可能，因此要乘倍率
                    ans = (ans << 1) % MOD;
                }
            }
        }
        else {
            //1.不要i位置
            ans = f(cnt, i - 1, s, dp);
            //2.要i位置
            int cur = own[i];
            int times = cnt[i];
            if (cur != 0 && times != 0 && (s & cur) == cur)
                ans = ((long)f(cnt, i - 1, s ^ cur, dp) * times + ans) % MOD;
        }
        dp[i][s] = ans;
        return ans;
    }

    int numberOfGoodSubsets(vector<int>& nums) {
        vector<int> cnt(MAXV + 1, 0);//1~30
        for (auto& ele : nums) {
            cnt[ele]++;
        }

        int ans = 0;
        vector<vector<int>> dp(MAXV + 1, vector<int>(LIMIT, -1));
        for (int s = 1; s < LIMIT; s++) {
            ans = (ans + f(cnt, MAXV, s, dp)) % MOD;
        }
        return ans;
    }
};



//题目四：
// 分配重复整数
// 给你一个长度为n的整数数组nums，这个数组中至多有50个不同的值
// 同时你有m个顾客的订单quantity，其中整数quantity[i]是第i位顾客订单的数目
// 请你判断是否能将nums中的整数分配给这些顾客，且满足：
// 第i位顾客恰好有quantity[i]个整数、第i位顾客拿到的整数都是相同的
// 每位顾客都要满足上述两个要求，返回是否能都满足
// 测试链接 : https://leetcode.cn/problems/distribute-repeating-integers/

//核心trick!!!
//给出一个status,要求枚举它的所有子集状态：  （建议直接记住）
for (int j = status; j > 0; j = (j - 1) & status) {...}

//思路：
//用数组cnt维护：相同数的个数
//dp[s][i]: 用cnt i~m-1 的数据，能否满足该子集的要求  
//1.使用i位置的数据
//  遍历s状态的所有子集, 看看cnt[i]这个数据能否满足该子集的要求 (学习这个遍历方式)
//  若能，则调用dp[ s'][i+1]看能否满足；若不能，看看其他子集能否满足
//2.不使用i位置的数据 : dp[s][i+1]
//
//核心：
//1. 用一个结构维护[s的子集状态, 该子集需要的数据] -> 涉及一个trick (trick1)
//2. 如何高效遍历所有子集 -> 涉及上述trick (trick2)

class Solution {
public:
    int f(vector<int>& cnt, vector<int>& sum, int status, int i, vector<vector<char>>& dp) {
        if (status == 0) return 1;
        if (i == cnt.size()) return 0;
        if (dp[status][i] != -1) return dp[status][i];

        int ans = 0;
        //1.使用cnt[i]
        for (int j = status; j > 0; j = (j - 1) & status) {
            if (sum[j] <= cnt[i] && f(cnt, sum, status ^ j, i + 1, dp)) {
                ans = 1;
                break;
            }
        }
        //2.不使用cnt[i]
        if (!ans) {
            ans = f(cnt, sum, status, i + 1, dp);
        }

        dp[status][i] = ans;
        return ans;
    }

    bool canDistribute(vector<int>& nums, vector<int>& quantity) {
        sort(nums.begin(), nums.end());

        int freq = 1;
        vector<int> cnt;
        for (int i = 1; i < nums.size(); i++) {
            if (nums[i] != nums[i - 1]) {
                cnt.push_back(freq);
                freq = 1;
            }
            else
                freq++;
        }
        cnt.push_back(freq);

        int m = quantity.size();
        vector<int> sum(1 << m, 0);//统计每个子集所需的相同数字个数  学习这个trick1
        for (int i = 0; i < m; i++) {
            int singlePersonStatus = 1 << i;
            int singlePersonNeed = quantity[i];
            for (int j = 0; j < singlePersonStatus; j++) {
                sum[singlePersonStatus | j] = singlePersonNeed + sum[j];
            }
        }

        vector<vector<char>> dp(1 << m, vector<char>(cnt.size(), -1));
        int status = (1 << m) - 1;//1表示还需要去满足，0表示已经满足了
        return f(cnt, sum, status, 0, dp);
    }
};

