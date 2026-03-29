#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include <climits>
using namespace std;



//区间dp: 大范围的问题拆分成若干小范围的问题来求解

//可能性展开的常见方式：
//1.基于两侧端点讨论的可能性展开
//2.基于范围上划分点的可能性展开


//区间dp--上


//题目一：
// 让字符串成为回文串的最少插入次数
// 给你一个字符串 s
// 每一次操作你都可以在字符串的任意位置插入任意字符
// 请你返回让s成为回文串的最少操作次数
// 测试链接 : https://leetcode.cn/problems/minimum-insertion-steps-to-make-a-string-palindrome/
//
//法一：递归
class Solution
{
public:
    //递归定义：
    // s[l....r]这个范围上的字符串，整体都变成回文串
    // 返回至少插入几个字符
    int f(string& s, int i, int j)
    {
        //base case:
        if (i == j) return 0;
        if (i + 1 == j) return s[i] == s[j] ? 0 : 1;

        if (s[i] == s[j])
            return f(s, i + 1, j - 1);
        else
            return min(f(s, i, j - 1), f(s, i + 1, j)) + 1;
    }

    int minInsertions(string s)
    {
        int n = s.size();
        return f(s, 0, n - 1);
    }
};

//法二：记忆化搜索
class Solution 
{
public:
    int f(string& s, int i, int j, vector<vector<int>>& dp)
    {
        if (i == j)
        {
            dp[i][j] = 0;
            return 0;
        }
        if (i + 1 == j)
        {
            dp[i][j] = s[i] == s[j] ? 0 : 1;
            return dp[i][j];
        }

        if (dp[i][j] != -1) return dp[i][j];

        if (s[i] == s[j])
            dp[i][j] = f(s, i + 1, j - 1, dp);
        else
            dp[i][j] = min(f(s, i + 1, j, dp), f(s, i, j - 1, dp)) + 1;
        return dp[i][j];
    }

    int minInsertions(string s)
    {
        int n = s.size();
        vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));
        return f(s, 0, n - 1, dp);
    }
};

//法三：动态规划
class Solution 
{
public:
    int minInsertions(string s) 
    {
        int n = s.size();
        vector<vector<int>> dp(n, vector<int>(n, 0));
        //初始化：
        //for (int i = 0; i < n; i++)
            //dp[i][i] = 0;
        for (int i = 0; i < n - 1; i++)
            dp[i][i + 1] = s[i] == s[i + 1] ? 0 : 1;

        for (int l = n - 3; l >= 0; l--)
        {
            for (int r = l + 2; r < n; r++)//l必须小于等于r
            {
                if (s[l] == s[r])
                    dp[l][r] = dp[l + 1][r - 1];
                else
                    dp[l][r] = min(dp[l][r - 1], dp[l + 1][r]) + 1;
            }
        }
        return dp[0][n - 1];
    }
};

//法四：动态规划+空间压缩
class Solution
{
public:
    int minInsertions(string s)
    {
        int n = s.size();
        if (n < 2) return 0;
        vector<int> dp(n, 0);
        //初始化：这里是第n-2层，对s[n-2,n-1]初始化
        dp[n - 1] = s[n - 2] == s[n - 1] ? 0 : 1;

        for (int l = n - 3; l >= 0; l--)
        {
            int leftdown = 0;
            int backup = 0;
            for (int r = l + 1; r < n; r++)
            {
                backup = dp[r];

                if (s[l] == s[r])
                    dp[r] = leftdown;
                else
                    dp[r] = min(dp[r - 1], dp[r]) + 1;

                leftdown = backup;
            }
        }
        return dp[n - 1];
    }
};



//题目二：
// 预测赢家
// 给你一个整数数组 nums 。玩家 1 和玩家 2 基于这个数组设计了一个游戏
// 玩家 1 和玩家 2 轮流进行自己的回合，玩家 1 先手
// 开始时，两个玩家的初始分值都是 0
// 每一回合，玩家从数组的任意一端取一个数字
// 取到的数字将会从数组中移除，数组长度减1
// 玩家选中的数字将会加到他的得分上
// 当数组中没有剩余数字可取时游戏结束
// 如果玩家 1 能成为赢家，返回 true
// 如果两个玩家得分相等，同样认为玩家 1 是游戏的赢家，也返回 true
// 你可以假设每个玩家的玩法都会使他的分数最大化
// 测试链接 : https://leetcode.cn/problems/predict-the-winner/
//
//法一：递归
class Solution 
{
public:
    //递归定义：当前 玩家1 在nums[l~r]上选 ， 获得的最大分数
    //          两个玩家都绝顶聪明
    int f(vector<int>& nums, int l, int r)
    {
        if (l == r) return nums[l];
        if (l + 1 == r) return max(nums[l], nums[r]);
        
        //可能性1：玩家1选左边
        //玩家二则在 nums[l+1~r] 上选，他可能选nums[l+1]，也可能选nums[r]
        //由于玩家二也绝顶聪明
        //所以他会给玩家1剩下最小的情况
        int p1 = nums[l] + min(f(nums, l + 2, r), f(nums, l + 1, r - 1));
        //可能性2: 玩家1选右边
        //分析同上
        int p2 = nums[r] + min(f(nums, l + 1, r - 1), f(nums, l, r - 2));

        return max(p1, p2);
    }

    bool predictTheWinner(vector<int>& nums) 
    {
        int n = nums.size();
        int sum = 0;
        for (auto& ele : nums)
            sum += ele;

        int first = f(nums, 0, n - 1);
        int second = sum - first;
        return first >= second;
    }
};

//法二：记忆化搜素
class Solution 
{
public:
    int f(vector<int>& nums, int l, int r, vector<vector<int>>& dp)
    {
        if (dp[l][r] != -1) return dp[l][r];

        int ans = 0;
        if (l == r) ans = nums[l];
        else if (l + 1 == r) ans = max(nums[l], nums[r]);
        else
        {
            int p1 = nums[l] + min(f(nums, l + 2, r, dp), f(nums, l + 1, r - 1, dp));
            int p2 = nums[r] + min(f(nums, l + 1, r - 1, dp), f(nums, l, r - 2, dp));
            ans = max(p1, p2);
        }
        dp[l][r] = ans;
        return ans;
    }

    bool predictTheWinner(vector<int>& nums) 
    {
        int n = nums.size();
        int sum = 0;
        for (auto& ele : nums)
            sum += ele;
        vector<vector<int>> dp(n, vector<int>(n, -1));
        int first = f(nums, 0, n - 1, dp);
        int second = sum - first;
        return first >= second;
    }
};

//法三：动态规划
class Solution
{
public:
    bool predictTheWinner(vector<int>& nums) 
    {
        int sum = 0;
        for (auto& ele : nums)
            sum += ele;
        int n = nums.size();
        int first = 0, second = 0;

        vector<vector<int>> dp(n, vector<int>(n, 0));
        //初始化：
        for (int i = 0; i < n; i++)
            dp[i][i] = nums[i];
        for (int i = 0; i < n - 1; i++)
            dp[i][i + 1] = max(nums[i], nums[i + 1]);

        for (int l = n - 3; l >= 0; l--)
        {
            for (int r = l + 2; r < n; r++)
            {
                int p1 = nums[l] + min(dp[l + 2][r], dp[l + 1][r - 1]);
                int p2 = nums[r] + min(dp[l + 1][r - 1], dp[l][r - 2]);
                dp[l][r] = max(p1, p2);
            }
        }

        first = dp[0][n - 1];
        second = sum - first;
        return first >= second;
    }
};



//题目三：
// 多边形三角剖分的最低得分
// 你有一个凸的 n 边形，其每个顶点都有一个整数值
// 给定一个整数数组values，其中values[i]是第i个顶点的值(顺时针顺序)
// 假设将多边形 剖分 为 n - 2 个三角形
// 对于每个三角形，该三角形的值是顶点标记的乘积
// 三角剖分的分数是进行三角剖分后所有 n - 2 个三角形的值之和
// 返回 多边形进行三角剖分后可以得到的最低分
// 测试链接 : https://leetcode.cn/problems/minimum-score-triangulation-of-polygon/

//本题是 基于范围上划分点的可能性展开 
//       子问题-->在某一区间上枚举所有符合要求的点，划分成两部分(两个子区间)，再在两个子区间重复操作

//核心：如何枚举划分三角形
// 选定任意两个点作底边，这两个点一定在某一个三角形当中，
// 然后枚举所有点作为这个三角形的顶点
// 对于每一个枚举的三角形，都分别以这个三角形的两条边（不是最开始选的底边）作为其他三角形的底边，再以同样逻辑枚举其他三角形的顶点


//法一：记忆化搜素
class Solution 
{
public:
    int f(vector<int>& values, int i, int j, vector<vector<int>>& dp)
    {
        if (dp[i][j] != -1) return dp[i][j];

        int ans = 0x3f3f3f3f;
        if (i == j || i + 1 == j) ans = 0;
        else
        {
            for (int k = i + 1; k < j; k++)
            {
                ans = min(ans, values[i] * values[j] * values[k] + f(values, i, k, dp) + f(values, k, j, dp));
            }
        }
        dp[i][j] = ans;
        return ans;
    }

    int minScoreTriangulation(vector<int>& values)
    {
        int n = values.size();
        vector<vector<int>> dp(n, vector<int>(n, -1));
        return f(values, 0, n - 1, dp);
    }
};

//法二：动态规划
//  每个格子还是依赖左侧的格子和下方的格子
class Solution 
{
public:
    int minScoreTriangulation(vector<int>& values)
    {
        int n = values.size();
        vector<vector<int>> dp(n, vector<int>(n, INT_MAX));
        //初始化：
        for (int i = 0; i < n; i++)
            dp[i][i] = 0;
        for (int i = 0; i < n - 1; i++)
            dp[i][i + 1] = 0;
         
        for (int l = n - 3; l >= 0; l--)
        {
            for (int r = l + 2; r < n; r++)
            {
                for (int k = l + 1; k < r; k++)
                {
                    dp[l][r] = min(dp[l][r], values[l] * values[k] * values[r] + dp[l][k] + dp[k][r]);
                }
            }
        }
        return dp[0][n - 1];
    }
};



//题目四：
// 切棍子的最小成本
// 有一根长度为n个单位的木棍，棍上从0到n标记了若干位置
// 给你一个整数数组cuts，其中cuts[i]表示你需要将棍子切开的位置
// 你可以按顺序完成切割，也可以根据需要更改切割的顺序
// 每次切割的成本都是当前要切割的棍子的长度，切棍子的总成本是历次切割成本的总和
// 对棍子进行切割将会把一根木棍分成两根较小的木棍
// 这两根木棍的长度和就是切割前木棍的长度
// 返回切棍子的最小总成本
// 测试链接 : https://leetcode.cn/problems/minimum-cost-to-cut-a-stick/

//注意：本题需要进行离散化处理
// n的大小很大，且cuts数组中的元素并没有那么多
// 如果直接创建n*n的dp表，内存会溢出

//思路：
//将cuts数组排序，并在开头加上0，结尾加上n（添加辅助点,便于计算成本）
//在1~m上展开，遍历cuts中每一个点，
//划分成两部分，再重复该操作

//法一：记忆化搜素
class Solution 
{
public:
    //递归定义：
    //有切点[l~r]，决定一个顺序，让切点都切完，总代价最小
    int f(vector<int>& nums, int l, int r, vector<vector<int>>& dp)
    {
        if (dp[l][r] != -1) return dp[l][r];

        if (l > r) return 0;
        int ans = INT_MAX;
        if (l == r) ans = nums[r + 1] - nums[l - 1];//只剩一个点要切
        else
        {
            for (int k = l; k <= r; k++)
            {
                ans = min(ans, nums[r + 1] - nums[l - 1] + f(nums, l, k - 1, dp) + f(nums, k + 1, r, dp));
            }
        }
        dp[l][r] = ans;
        return ans;
    }

    int minCost(int n, vector<int>& cuts) 
    {
        sort(cuts.begin(), cuts.end());
        int m = cuts.size();
        vector<int> nums(m + 2);
        nums[0] = 0;
        for (int i = 0; i < m; i++)
        {
            nums[i + 1] = cuts[i];
        }
        nums[m + 1] = n;

        vector<vector<int>> dp(m + 2, vector<int>(m + 2, -1));
        return f(nums, 1, m, dp);
    }
};

//法二：动态规划
class Solution 
{
public:
    int minCost(int n, vector<int>& cuts) 
    {
        sort(cuts.begin(), cuts.end());
        int m = cuts.size();
        vector<int> nums(m + 2);
        nums[0] = 0;
        for (int i = 0; i < m; i++)
        {
            nums[i + 1] = cuts[i];
        }
        nums[m + 1] = n;

        vector<vector<int>> dp(m + 2, vector<int>(m + 2, 0));
        //初始化：
        for (int i = 1; i <= m; i++)
            dp[i][i] = nums[i + 1] - nums[i - 1];

        for (int l = m - 1; l >= 1; l--)
        {
            for (int r = l + 1; r <= m; r++)
            {
                dp[l][r] = INT_MAX;
                for (int k = l; k <= r; k++)
                    dp[l][r] = min(dp[l][r], nums[r + 1] - nums[l - 1] + dp[l][k - 1] + dp[k + 1][r]);
            }
        }
        return dp[1][m];
    }
};



//题目五：
// 戳气球
// 有 n 个气球，编号为0到n-1，每个气球上都标有一个数字，这些数字存在数组nums中
// 现在要求你戳破所有的气球。戳破第 i 个气球
// 你可以获得 nums[i - 1] * nums[i] * nums[i + 1] 枚硬币
// 这里的 i - 1 和 i + 1 代表和 i 相邻的两个气球的序号
// 如果 i - 1或 i + 1 超出了数组的边界，那么就当它是一个数字为 1 的气球
// 求所能获得硬币的最大数量
// 测试链接 : https://leetcode.cn/problems/burst-balloons/

//本题细节：讨论k时不能根据最先打爆来定义，因为一旦这样定义，下一轮递归的边界情况不好讨论：不知道边界外是否打爆
//所以要反着定义：选的K最后被打爆
//有点像 打砖块 那个题

class Solution 
{
public:
    //递归定义：
    // arr[l...r]这些气球决定一个顺序，获得最大得分返回！
    // 一定有 : arr[l-1]一定没爆！
    // 一定有 : arr[r+1]一定没爆！
    // 尝试每个气球最后打爆
    int f(vector<int>& arr, int l, int r, vector<vector<int>>& dp)
    {
        if (dp[l][r] != -1) return dp[l][r];

        if (l > r) return 0;
        int ans = 0;
        if (l == r) ans = arr[l] * arr[l - 1] * arr[l + 1];
        else
        {
            for (int k = l; k <= r; k++)// k位置的气球最后打爆
                ans = max(ans, arr[l - 1] * arr[k] * arr[r + 1] + f(arr, l, k - 1, dp) + f(arr, k + 1, r, dp));
        }
        dp[l][r] = ans;
        return ans;
    }

    int maxCoins(vector<int>& nums) 
    {
        int n = nums.size();
        vector<int> arr(n + 2);
        arr[0] = 1;
        for (int i = 0; i < n; i++)
            arr[i + 1] = nums[i];
        arr[n + 1] = 1;

        vector<vector<int>> dp(n + 2, vector<int>(n + 2, -1));
        return f(arr, 1, n, dp);
    }
};

//法二：动态规划
class Solution
{
public:
    int maxCoins(vector<int>& nums)
    {
        int n = nums.size();
        vector<int> arr(n + 2);
        arr[0] = 1;
        for (int i = 0; i < n; i++)
            arr[i + 1] = nums[i];
        arr[n + 1] = 1;

        vector<vector<int>> dp(n + 2, vector<int>(n + 2, 0));
        //初始化：
        for (int i = 1; i <= n; i++)
            dp[i][i] = arr[i - 1] * arr[i] * arr[i + 1];

        for (int l = n - 1; l >= 1; l--)
        {
            for (int r = l + 1; r <= n; r++)
            {
                for (int k = l; k <= r; k++)
                    dp[l][r] = max(dp[l][r], arr[l - 1] * arr[k] * arr[r + 1] + dp[l][k - 1] + dp[k + 1][r]);
            }
        }
        return dp[1][n];
    }
};



//题目六：
// 布尔运算
// 给定一个布尔表达式和一个期望的布尔结果 result
// 布尔表达式由 0 (false)、1 (true)、& (AND)、 | (OR) 和 ^ (XOR) 符号组成
// 布尔表达式一定是正确的，不需要检查有效性
// 但是其中没有任何括号来表示优先级
// 你可以随意添加括号来改变逻辑优先级
// 目的是让表达式能够最终得出result的结果
// 返回最终得出result有多少种不同的逻辑计算顺序
// 测试链接 : https://leetcode.cn/problems/boolean-evaluation-lcci/
class Solution 
{
public:
    //递归定义：
    //[l~r]范围上计算结果 == want 的所有可能
    // 0/1  逻  0/1   逻       0/1
    //  l  l+1  l+2  l+3........r
    int f(string& s, int want, int l, int r, vector<vector<vector<int>>>& dp)
    {
        if (dp[l][r][want] != -1) return dp[l][r][want];

        if (l > r) return 0;
        int ans = 0;
        if (l == r) ans = (s[l] - '0') == want ? 1 : 0;
        else
        {
            for (int k = l + 1; k <= r - 1; k += 2)//k判定符号
            {
                if (s[k] == '&')
                {
                    if (want == 1)
                        ans += f(s, 1, l, k - 1, dp) * f(s, 1, k + 1, r, dp);
                    else if (want == 0)
                        ans += f(s, 0, l, k - 1, dp) * f(s, 0, k + 1, r, dp)
                        + f(s, 0, l, k - 1, dp) * f(s, 1, k + 1, r, dp)
                        + f(s, 1, l, k - 1, dp) * f(s, 0, k + 1, r, dp);
                }
                else if (s[k] == '|')
                {
                    if (want == 1)
                        ans += f(s, 1, l, k - 1, dp) * f(s, 1, k + 1, r, dp)
                        + f(s, 1, l, k - 1, dp) * f(s, 0, k + 1, r, dp)
                        + f(s, 0, l, k - 1, dp) * f(s, 1, k + 1, r, dp);
                    else if (want == 0)
                        ans += f(s, 0, l, k - 1, dp) * f(s, 0, k + 1, r, dp);
                }
                else if (s[k] == '^')
                {
                    if (want == 1)
                        ans += f(s, 1, l, k - 1, dp) * f(s, 0, k + 1, r, dp)
                        + f(s, 0, l, k - 1, dp) * f(s, 1, k + 1, r, dp);
                    else if (want == 0)
                        ans += f(s, 1, l, k - 1, dp) * f(s, 1, k + 1, r, dp)
                        + f(s, 0, l, k - 1, dp) * f(s, 0, k + 1, r, dp);
                }
            }
        }
        dp[l][r][want] = ans;
        return ans;
    }

    int countEval(string s, int result) 
    {
        int n = s.size();
        vector<vector<vector<int>>> dp(n + 1, vector<vector<int>>(n + 1, vector<int>(2, -1)));
        return f(s, result, 0, n - 1, dp);
    }
};






//区间dp--下


//题目一：
// 完成配对需要的最少字符数量
// 给定一个由'['、']'、'('，')'组成的字符串
// 请问最少插入多少个括号就能使这个字符串的所有括号正确配对
// 例如当前串是 "([[])"，那么插入一个']'即可满足
// 输出最少需要插入多少个字符
// 测试链接 : https://www.nowcoder.com/practice/e391767d80d942d29e6095a935a5b96b

//本题： 基于两侧端点讨论的可能性展开 + 基于范围上划分点的可能性展开

namespace test1
{
    #include <iostream>
    #include <vector>
    #include <string>
    #include <climits>
    using namespace std;

    string s;

    //递归定义：
    //[l~r]上完成配对的最小数量
    int f(int l, int r, vector<vector<int>>& dp)
    {
        if (dp[l][r] != -1) return dp[l][r];

        if (l > r) return 0;
        int ans = INT_MAX;
        int p1 = INT_MAX;
        int p2 = INT_MAX;
        if (l == r) ans = 1;
        else
        {
            // 可能性1 : [l]、[r]本来就是配对的
            if ((s[l] == '[' && s[r] == ']') || (s[l] == '(' && s[r] == ')'))
                p1 = f(l + 1, r - 1, dp);
            //可能性2 : 基于每个可能的划分点，做左右划分
            //注意：这里可能性2不能放在else里面是因为有可能不选边界更优
            //例如：()()
            //可能性1得到答案等于2，而可能性2得到答案等于0
            for (int k = l; k < r; k++)
            {
                p2 = min(p2, f(l, k, dp) + f(k + 1, r, dp));
            }
        }
        ans = min(ans,min(p1, p2));
        dp[l][r] = ans;
        return ans;
    }

    int main()
    {
        cin >> s;
        int n = s.size();
        vector<vector<int>> dp(n, vector<int>(n, -1));
        cout << f(0, n - 1, dp);

        return 0;
    }
}



//题目二：
// 涂色 & 奇怪打印机
// 假设你有一条长度为5的木板，初始时没有涂过任何颜色
// 你希望把它的5个单位长度分别涂上红、绿、蓝、绿、红
// 用一个长度为5的字符串表示这个目标：RGBGR
// 每次你可以把一段连续的木板涂成一个给定的颜色，后涂的颜色覆盖先涂的颜色
// 例如第一次把木板涂成RRRRR
// 第二次涂成RGGGR
// 第三次涂成RGBGR，达到目标
// 返回尽量少的涂色次数
// 测试链接 : https://www.luogu.com.cn/problem/P4170
// 测试链接 : https://leetcode.cn/problems/strange-printer/
class Solution 
{
public:
    //递归定义：
    //[l~r]范围上刷成给定颜色至少涂几次
    int f(string& s, int l, int r, vector<vector<int>>& dp)
    {
        if (dp[l][r] != -1) return dp[l][r];

        if (l > r) return 0;
        int ans = INT_MAX;
        if (l == r) ans = 1;
        else
        {
            //可能性1：[l]、[r]相等
            //及其注意：这里不能定义为 f(l+1,r-1)+1
            //例如：aaaaa  如果按上述定义答案就不对
            if (s[l] == s[r])
                ans = f(s, l, r - 1, dp);
            //可能性2：[l]、[r]不等，枚举划分点
            else
            {
                for (int k = l; k < r; k++)
                {
                    ans = min(ans, f(s, l, k, dp) + f(s, k + 1, r, dp));
                }
            }
        }
        dp[l][r] = ans;
        return ans;
    }

    int strangePrinter(string s) 
    {
        int n = s.size();
        vector<vector<int>> dp(n, vector<int>(n, -1));

        return f(s, 0, n - 1, dp);
    }
};



//题目三：
// 合唱队
// 具体描述情打开链接查看
// 测试链接 : https://www.luogu.com.cn/problem/P3205

//分析：新数据添加构成了新的端点，因此对端点进行讨论 -> 基于两侧端点讨论的可能性展开

//eg: [a,b,c,d]
//1. a最后进来 - [b,c,d]
//   1) b最后进来：a<b
//   2) d最后进来：a<d
//2. d最后进来 - [a,b,c]
//   1) a最后进来：d>a
//   2) c最后进来：d>c

namespace test3
{
    #include <iostream>
    #include <vector>
    using namespace std;
    const int MOD = 19650827;
    int n;

    int main()
    {
        cin >> n;
        vector<int> nums(n + 1, 0);
        for (int i = 1; i <= n; i++)
            cin >> nums[i];

        // 人的编号范围 : 1...n
        // dp[l][r][0] : 形成l...r的状况的方法数，同时要求l位置的数字是最后出现的
        // dp[l][r][1] : 形成l...r的状况的方法数，同时要求r位置的数字是最后出现的
        vector<vector<vector<int>>> dp(n + 1, vector<vector<int>>(n + 1, vector<int>(2, 0)));
        //初始化:
        //只有两个数时，按照题目给出的排序规则，这两个数一定升序排列
        for (int i = 1; i <= n - 1; i++)
        {
            if (nums[i] < nums[i + 1])
            {
                dp[i][i + 1][0] = 1;
                dp[i][i + 1][1] = 1;
            }
        }

        for (int l = n - 2; l >= 1; l--)
        {
            for (int r = l + 2; r <= n; r++)
            {
                //1.左侧的数最后进来
                if (nums[l] < nums[l + 1])
                    dp[l][r][0] = (dp[l][r][0] + dp[l + 1][r][0]) % MOD;
                if (nums[l] < nums[r])
                    dp[l][r][0] = (dp[l][r][0] + dp[l + 1][r][1]) % MOD;

                //2.右侧的数最后进来
                if (nums[r] > nums[l])
                    dp[l][r][1] = (dp[l][r][1] + dp[l][r - 1][0]) % MOD;
                if (nums[r] > nums[r - 1])
                    dp[l][r][1] = (dp[l][r][1] + dp[l][r - 1][1]) % MOD;
            }
        }

        cout << (dp[1][n][0] + dp[1][n][1]) % MOD;

        return 0;
    }
}



//题目四：
// 移除盒子
// 给出一些不同颜色的盒子boxes，盒子的颜色由不同的正数表示
// 你将经过若干轮操作去去掉盒子，直到所有的盒子都去掉为止
// 每一轮你可以移除具有相同颜色的连续 k 个盒子（k >= 1）
// 这样一轮之后你将得到 k * k 个积分
// 返回你能获得的最大积分总和
// 测试链接 : https://leetcode.cn/problems/remove-boxes/

//分析：
// 由于 相同颜色的盒子相邻的越多，得到的积分越多
//因此 dp[l][r][k] 定义为    boxes[l....r]范围上要去消除，前面跟着k个连续的和boxes[l]颜色一样的盒子
//1.直接消除最长前缀
//2.讨论前缀跟在后面某个区间，一起消掉

//本题亮点：跟着前缀信息一起消

// 时间复杂度O(n^4)
class Solution
{
public:
    // boxes[l....r]范围上要去消除，前面跟着k个连续的和boxes[l]颜色一样的盒子
    // 这种情况下，返回最大得分
    int f(vector<int>& boxes, int l, int r, int k, vector<vector<vector<int>>>& dp)
    {
        if (dp[l][r][k] != -1) return dp[l][r][k];
        
        if (l > r) return 0;
        if (l == r) return (k + 1) * (k + 1);//只剩一个盒子时，它要和前面的 k 个盒子一起消除

        int s = l;
        while (s + 1 <= r && boxes[s + 1] == boxes[l])
            s++;
        // boxes[l...s]都是一种颜色，boxes[s+1]就不是同一种颜色了
        // cnt是总前缀数量 : 之前的相同前缀(k个) + l...s这个颜色相同的部分(s-l+1个)
        int cnt = k + s - l + 1;
        int ans = 0;
        //可能性1：前缀先消
        ans = cnt * cnt + f(boxes, s + 1, r, 0, dp);
        //可能性2：讨论前缀跟着哪个后，一起消掉
        for (int m = s + 2; m <= r; m++)
        {
            if (boxes[m] == boxes[l] && boxes[m] != boxes[m - 1])
                // boxes[l] == boxes[m]是必须条件
                // boxes[m - 1] != boxes[m]是剪枝条件，避免不必要的调用
                ans = max(ans, f(boxes, s + 1, m - 1, 0, dp) + f(boxes, m, r, cnt, dp));
        }
        dp[l][r][k] = ans;
        return ans;
    }

    int removeBoxes(vector<int>& boxes) 
    {
        int n = boxes.size();
        vector<vector<vector<int>>> dp(n + 1, vector<vector<int>>(n + 1, vector<int>(n + 1, -1)));
        return f(boxes, 0, n - 1, 0, dp);
    }
};



//题目五：
// 合并石头的最低成本
// 有 n 堆石头排成一排，第 i 堆中有 stones[i] 块石头
// 每次 移动 需要将 连续的 k 堆石头合并为一堆，而这次移动的成本为这 k 堆中石头的总数
// 返回把所有石头合并成一堆的最低成本
// 如果无法合并成一堆返回-1
// 测试链接 : https://leetcode.cn/problems/minimum-cost-to-merge-stones/

// 时间复杂度O(n^3)
    // 优化策略来自于观察
    // l.....r最终会变成几份其实是注定的，根本就无法改变
    // 那么也就知道，满足(n - 1) % (k - 1) == 0的情况下，
    // 0....n-1最终一定是1份，也无法改变
    // 如果l.....r最终一定是1份
    // 那么要保证l.....m最终一定是1份，m+1...r最终一定是k-1份
    // 
    // 推广：
    // 如果l.....r最终一定是p份（p>1）
    // 那么要保证l.....m最终一定是1份，那么m+1...r最终一定是p-1份
    // 怎么保证的？枚举行为中，m += k-1很重要！
    // m每次跳k-1！
    // 如果l.....r最终一定是1份
    // 就一定能保证l.....m最终一定是1份
    // 也一定能保证m+1...r最终一定是k-1份
    // 不要忘了，加上最后合并成1份的代价
    // 如果l.....r最终一定是p份
    // 就一定能保证l.....m最终一定是1份
    // 也一定能保证m+1...r最终一定是p-1份
    // 不用加上最后合并成1份的代价

class Solution 
{
public:
    //递归定义：l~r上能合并成1份的最小代价
    int f(vector<int>& preSum, int k, int l, int r, vector<vector<int>>& dp)
    {
        if (l >= r) return 0;

        if (dp[l][r] != -1) return dp[l][r];

        int ans = INT_MAX;
        for (int m = l; m < r; m += k - 1)
        {
            ans = min(ans, f(preSum, k, l, m, dp) + f(preSum, k, m + 1, r, dp));
        }
        if ((r - l) % (k - 1) == 0)// 最终一定能划分成一份，那么就再加合并代价
            ans += preSum[r + 1] - preSum[l];

        dp[l][r] = ans;
        return ans;
    }

    int mergeStones(vector<int>& stones, int k) 
    {
        int n = stones.size();
        if ((n - 1) % (k - 1) != 0) return -1;
        //前缀和优化：
        vector<int> preSum(n + 1, 0);
        for (int i = 0; i < n; i++)
            preSum[i + 1] = preSum[i] + stones[i];

        vector<vector<int>> dp(n, vector<int>(n, -1));
        return f(preSum, k, 0, n - 1, dp);
    }
};



//题目六：
// 统计不同回文子序列
// 给你一个字符串s，返回s中不同的非空回文子序列个数
// 由于答案可能很大，答案对 1000000007 取模
// 测试链接 : https://leetcode.cn/problems/count-different-palindromic-subsequences/
//
//分析：
//dp[i][j]: i~j上不同的非空回文子序列个数
//1.s[i]!=s[j] : dp[i][j] = dp[i][j-1] + dp[i+1][j] - dp[i+1][j-1]  (容斥原理)
//2.s[i]==s[j] == a 这里假设等于字符a
//  1) i+1~j-1 上没有a : dp[i][j] = 2*dp[i+1][j-1] + 2  这里的2指a和aa
//       a....a
//	     i    j
//	    (i+1..j-1) + a(i+1..j-1)a + a + aa
 
//  2) i+1~j-1 上有一个a ：dp[i][j] = 2*dp[i+1][j-1] + 1  由于子范围上有一个a，所有两个端点处的a只会多一个aa情况
//      a.....a......a
//      i     lr     j
//      (i+1..j-1) + a(i+1..j-1)a + aa

//  3) i+1~j-1 上有多个a : dp[i][j] = 2*dp[i+1][j-1] - dp[l+1][r-1]  由于子范围上至少2个a，所以减去重复部分
//      a...a....这内部可能还有a但是不重要....a...a
//      i   l                             r   j
class Solution 
{
public:
    int countPalindromicSubsequences(string s) 
    {
        int n = s.size();
        const int MOD = 1e9 + 7;

        vector<int> last(256,-1);
        vector<int> lpre(n, -1); //i位置的左边和s[i]字符相等且最近的位置在哪，不存在就是 - 1
        for (int i = 0; i < n; i++)
        {
            lpre[i] = last[s[i]];
            last[s[i]] = i;
        }

        vector<int> rpre(n, -1); //i位置的右边和s[i]字符相等且最近的位置在哪，不存在就是n
        last.assign(256, n);
        for (int i = n - 1; i >= 0; i--)
        {
            rpre[i] = last[s[i]];
            last[s[i]] = i;
        }

        vector<vector<long>> dp(n, vector<long>(n, 0));
        //初始化：
        for (int i = 0; i < n; i++)
            dp[i][i] = 1;

        for (int i = n - 2; i >= 0; i--)
        {
            for (int j = i + 1; j < n; j++)
            {
                if (s[i] != s[j])
                    dp[i][j] = dp[i][j - 1] + dp[i + 1][j] - dp[i + 1][j - 1] + MOD;
                else
                {
                    int l = rpre[i];
                    int r = lpre[j];

                    if (l > r)// i...j的内部没有s[i]字符
                        dp[i][j] = 2 * dp[i + 1][j - 1] + 2;
                    else if (l == r)// i...j的内部有一个s[i]字符
                        dp[i][j] = 2 * dp[i + 1][j - 1] + 1;
                    else
                        dp[i][j] = 2 * dp[i + 1][j - 1] - dp[l + 1][r - 1] + MOD;
                }
                dp[i][j] %= MOD;
            }
        }
        return dp[0][n - 1];
    }
};

