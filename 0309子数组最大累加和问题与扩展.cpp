#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;




//子数组最大累加和问题与扩展--上

//子数组最大累加和问题是一个非常经典的问题，也比较简单
//但是 扩展出的问题很多 ，在笔试、面试中特别常见
//扩展出的问题很多非常有趣，解法也比较巧妙，分上下两期讲述



//题目一：
// 子数组最大累加和
// 给你一个整数数组 nums
// 返回非空子数组的最大累加和
// 测试链接 : https://leetcode.cn/problems/maximum-subarray/
//
//法一：动态规划
//dp[i] : 子数组必须以i位置的数做结尾，往左能延伸出来的最大累加和
//    -->>经验总结：动态规划子数组问题定义时关键词“往左/右延伸”
//思路及状态转移方程：
//1.nums[i]不往左延伸：dp[i]=nums[i]
//2.nums[i]往左延伸：dp[i]=nums[i]+dp[i-1]
class Solution 
{
public:
    int maxSubArray(vector<int>& nums)
    {
        int n = nums.size();
        // dp[i] : 子数组必须以i位置的数做结尾，往左能延伸出来的最大累加和
        vector<int> dp(n, 0);
        dp[0] = nums[0];
        int ans = dp[0];
        for (int i = 1; i < n; i++)
        {
            dp[i] = max(nums[i], nums[i] + dp[i - 1]);
            ans = max(ans, dp[i]);
        }
        return ans;
    }
};

//法二：动态规划+空间压缩
class Solution 
{
public:
    int maxSubArray(vector<int>& nums) 
    {
        int n = nums.size();
        int cur = 0;
        int ans = nums[0];
        for (int i = 0; i < n; i++)
        {
            cur = max(nums[i], cur + nums[i]);
            ans = max(ans, cur);
        }
        return ans;
    }
};


// 如下代码为附加问题的实现
    // 子数组中找到拥有最大累加和的子数组  （重要）
    // 并返回如下三个信息:
    // 1) 最大累加和子数组的开头left
    // 2) 最大累加和子数组的结尾right
    // 3) 最大累加和子数组的累加和sum
    // 如果不止一个子数组拥有最大累加和，那么找到哪一个都可以
//思路：贪心/动态规划
class Solution
{
private:
    int left, right, sum;

public:
    void extra(vector<int>& nums)
    {
        left = right = 0;
        sum = INT_MIN;
        int n = nums.size();
        int pre = INT_MIN;
        for (int l = 0, r = 0; r < n; r++)
        {
            if (pre >= 0)
            {
                // 吸收前面的累加和有利可图
                // 那就不换开头
                pre += nums[r];
            }
            else
            {
                // 吸收前面的累加和已经无利可图
                // 那就换开头
                pre = nums[r];
                l = r;
            }

            if (pre > sum)
            {
                sum = pre;
                right = r;
                left = l;
            }
        }
    }
};



//题目二：
// 数组中不能选相邻元素的最大累加和
// 给定一个数组，可以随意选择数字
// 但是不能选择相邻的数字，返回能得到的最大累加和
// 测试链接 : https://leetcode.cn/problems/house-robber/
//
//法一：记忆化搜索
class Solution
{
public:
    //递归定义：nums[0,...i]累加最大和
    //1.要nums[i]
    //2.不要nums[i]
    int f(vector<int>& nums, int i, vector<int>& dp)
    {
        //base case
        if (i == 0) return nums[0];
        if (i == 1) return max(nums[0], nums[1]);//易错点，注意

        if (dp[i] != -1) return dp[i];

        int p1 = 0, p2 = 0;
        p1 = f(nums, i - 1, dp);
        p2 = nums[i] + f(nums, i - 2, dp);
        dp[i] = max(p1, p2);
        return dp[i];
    }

    int rob(vector<int>& nums)
    {
        int n = nums.size();
        vector<int> dp(n, -1);
        return f(nums, n - 1, dp);
    }
};

//法二：动态规划
class Solution 
{
public:
    int rob(vector<int>& nums)
    {
        int n = nums.size();
        if (n == 1) return nums[0];
        if (n == 2) return max(nums[0], nums[1]);

        vector<int> dp(n, 0);
        //初始化：
        dp[0] = nums[0];
        dp[1] = max(nums[0], nums[1]);

        for (int i = 2; i < n; i++)
        {
            dp[i] = max(dp[i - 1], nums[i] + dp[i - 2]);
        }
        return dp[n - 1];
    }
};

//法三：动态规划 + 空间压缩
class Solution 
{
public:
    int rob(vector<int>& nums) 
    {
        int n = nums.size();
        if (n == 1) return nums[0];
        if (n == 2) return max(nums[0], nums[1]);

        int prepre = nums[0];
        int pre = max(nums[0], nums[1]);
        for (int i = 2; i < n; i++)
        {
            int cur = max(pre, nums[i] + prepre);
            prepre = pre;
            pre = cur;
        }
        return pre;
    }
};



//题目三：
// 环形数组的子数组最大累加和
// 给定一个数组nums，长度为n
// nums是一个环形数组，下标0和下标n-1是连在一起的
// 返回环形数组中，子数组最大累加和
// 测试链接 : https://leetcode.cn/problems/maximum-sum-circular-subarray/
//
//分析：
//1.如果子数组没有跨越边界，只有中间一段，就是普通的最大子数组和
//2.如果子数组 a 跨越边界，考虑不在子数组中的数，它们组成了一个中间的子数组 b。
//  那么有 sum(a)+sum(b)=sum(nums)=常数。所以 b 的元素和越小，a 的元素和就越大，所以相当于求最小子数组和。
//  用整个数组的元素和，减去最小子数组和，就得到了跨越边界的最大子数组和了。
class Solution 
{
public:
    int maxSubarraySumCircular(vector<int>& nums) 
    {
        int n = nums.size();
        int maxSum = nums[0], minSum = nums[0], all = nums[0];
        for (int i = 1, maxPre = nums[0], minPre = nums[0]; i < n; i++)
        {
            maxPre = max(nums[i], nums[i] + maxPre);
            minPre = min(nums[i], nums[i] + minPre);
            
            maxSum = max(maxSum, maxPre);
            minSum = min(minSum, minPre);
            all += nums[i];
        }
        return all == minSum ? maxSum : max(maxSum, all - minSum);
        //特判：eg: [-2,-5,-3]，最大累加和为-2，minSum = -10，all = -10
        //由于题目要求不能返回空字符串(累加和为0)，所以得特判
    }
};



//题目四：
// 环形数组中不能选相邻元素的最大累加和
// 给定一个数组nums，长度为n
// nums是一个环形数组，下标0和下标n-1是连在一起的
// 可以随意选择数字，但是不能选择相邻的数字
// 返回能得到的最大累加和
// 测试链接 : https://leetcode.cn/problems/house-robber-ii/
//
//核心：将首节点/尾节点分类讨论，将环形拆成两个不包含首或尾的队列，然后分别动态规划
//1.不要nums[0]，nums[1,...,n-1]范围上的经典打家劫舍问题
//2.要nums[0]，nums[2,...,n-2]范围上的经典打家劫舍问题
class Solution
{
public:
    int f(vector<int>& nums, int l, int r)
    {
        if (l > r) return 0;
        if (l == r) return nums[l];

        int prepre = nums[l];
        int pre = max(nums[l], nums[l + 1]);
        for (int i = l + 2; i <= r; i++)
        {
            int cur = max(pre, nums[i] + prepre);
            prepre = pre;
            pre = cur;
        }
        return pre;
    }

    int rob(vector<int>& nums)
    {
        int n = nums.size();
        if (n == 1)  return nums[0];
        if (n == 2) return max(nums[0], nums[1]);
        return max(f(nums, 1, n - 1), f(nums, 2, n - 2) + nums[0]);
    }
};



//题目五：
// 打家劫舍 IV
// 沿街有一排连续的房屋。每间房屋内都藏有一定的现金
// 现在有一位小偷计划从这些房屋中窃取现金
// 由于相邻的房屋装有相互连通的防盗系统，所以小偷不会窃取相邻的房屋
// 小偷的 窃取能力 定义为他在窃取过程中能从单间房屋中窃取的 最大金额
// 给你一个整数数组 nums 表示每间房屋存放的现金金额
// 第i间房屋中放有nums[i]的钱数
// 另给你一个整数k，表示小偷需要窃取至少 k 间房屋
// 返回小偷需要的最小窃取能力值
// 测试链接 : https://leetcode.cn/problems/house-robber-iv/
//
//法一：动态规划
class Solution
{
public:
    int f(vector<int>& nums, int able)
    {
        int n = nums.size();
        if (n == 1) return able >= nums[0] ? 1 : 0;
        if (n == 2) return able >= min(nums[0], nums[1]) ? 1 : 0;

        int ans = 0;
        vector<int> dp(n, 0);
        dp[0] = able >= nums[0] ? 1 : 0;
        dp[1] = able >= min(nums[0], nums[1]) ? 1 : 0;

        for (int i = 2; i < n; i++)
        {
            int p1 = dp[i - 1];
            int p2 = able >= nums[i] ? 1 + dp[i - 2] : dp[i - 2];
            dp[i] = max(p1, p2);
        }
        return dp[n - 1];
    }

    int minCapability(vector<int>& nums, int k)
    {
        int max = 0, min = INT_MAX;
        for (auto& ele : nums)
        {
            if (ele > max) max = ele;
            if (ele < min) min = ele;
        }

        int ans = 0;
        for (int l = min, r = max; l <= r;)
        {
            int m = l + (r - l) / 2;
            if (f(nums, m) >= k)
            {
                ans = m;
                r = m - 1;
            }
            else
                l = m + 1;
        }

        return ans;
    }
};

//法二：动态规划+空间压缩
class Solution
{
public:
    int f(vector<int>& nums, int able)
    {
        int n = nums.size();
        if (n == 1) return able >= nums[0] ? 1 : 0;
        if (n == 2) return able >= min(nums[0], nums[1]) ? 1 : 0;

        int prepre = able >= nums[0] ? 1 : 0;
        int pre = able >= min(nums[0], nums[1]) ? 1 : 0;

        for (int i = 2; i < n; i++)
        {
            int cur = max(pre, (able >= nums[i] ? 1 : 0) + prepre);
            prepre = pre;
            pre = cur;
        }
        return pre;
    }

    int minCapability(vector<int>& nums, int k)
    {
        int max = 0, min = INT_MAX;
        for (auto& ele : nums)
        {
            if (ele > max) max = ele;
            if (ele < min) min = ele;
        }

        int ans = 0;
        for (int l = min, r = max; l <= r;)
        {
            int m = l + (r - l) / 2;
            if (f(nums, m) >= k)
            {
                ans = m;
                r = m - 1;
            }
            else
                l = m + 1;
        }

        return ans;
    }
};

//法三：小贪心优化
class Solution
{
public:
    //小贪心优化：
    //由于偷的收益都是1
    //与其抉择不如直接偷
    int f(vector<int>& nums, int able)
    {
        int n = nums.size();
        int ans = 0;
        for (int i = 0; i < n; i++)
        {
            if (able >= nums[i])
            {
                ans++;
                i++;
            }
        }
        return ans;
    }

    int minCapability(vector<int>& nums, int k)
    {
        int max = 0, min = INT_MAX;
        for (auto& ele : nums)
        {
            if (ele > max) max = ele;
            if (ele < min) min = ele;
        }

        int ans = 0;
        for (int l = min, r = max; l <= r;)
        {
            int m = l + (r - l) / 2;
            if (f(nums, m) >= k)
            {
                ans = m;
                r = m - 1;
            }
            else
                l = m + 1;
        }

        return ans;
    }
};



//题目六：
// 子矩阵最大累加和问题
// 给定一个二维数组grid，找到其中子矩阵的最大累加和
// 返回拥有最大累加和的子矩阵左上角和右下角坐标
// 如果有多个子矩阵都有最大累加和，返回哪一个都可以
// 测试链接 : https://leetcode.cn/problems/max-submatrix-lcci/
//
//核心：压缩数组，将二维矩阵压缩成一维数组，在这个一维数组上进行“经典子数组最大累加和问题--变式（见题目一的额外题目）”
class Solution 
{
public:
    vector<int> getMaxMatrix(vector<vector<int>>& matrix) 
    {
        int m = matrix.size();
        int n = matrix[0].size();
        vector<int> nums(n);
        int x1, y1, x2, y2, sum = INT_MIN;

        //第0行-第0行、第0行-第1行、...、第0行-第m-1行
        //第1行-第1行、第1行-第2行、第1行-第3行、...、第1行-第m-1行
        //......
        //第m-1行-第m-1行
        for (int up = 0; up < m; up++)
        {
            //up更新时，压缩数组重新开始
            nums.assign(n, 0);
            for (int down = up; down < m; down++)
            {
                //dp - “经典子数组最大累加和问题--变式（见题目一的额外题目）”
                for (int l = 0, r = 0, pre = INT_MIN; r < n; r++)
                {
                    nums[r] += matrix[down][r];

                    if (pre >= 0)
                        pre += nums[r];
                    else
                    {
                        pre = nums[r];
                        l = r;
                    }

                    if (pre > sum)
                    {
                        sum = pre;
                        x1 = up, x2 = down;
                        y1 = l, y2 = r;
                    }
                }
            }
        }

        return { x1,y1,x2,y2 };
    }
};






//子数组最大累加和问题与扩展-下



//题目一：
// 乘积最大子数组
// 给你一个整数数组 nums
// 请你找出数组中乘积最大的非空连续子数组
// 并返回该子数组所对应的乘积
// 测试链接 : https://leetcode.cn/problems/maximum-product-subarray/
//
//mindp[i] : 子数组必须以i位置的数做结尾，往左能延伸出来的最小乘积
//maxdp[i] : 子数组必须以i位置的数做结尾，往左能延伸出来的最大乘积
//1.不扩充：dp[i]
//2.[0,...,i-1]最大乘积：dp[i]*max
//3.[0,...,i-1]最小乘积：dp[i]*min eg:若nums[i]<0，可能乘一个负数变成最大乘积
//
//法一：动态规划
class Solution 
{
public:
    int maxProduct(vector<int>& nums)
    {
        int n = nums.size();
        int ans = nums[0];
        vector<int> maxdp(n, 0);
        vector<int> mindp(n, 0);
        //初始化
        maxdp[0] = nums[0], mindp[0] = nums[0];
        for (int i = 1; i < n; i++)
        {
            maxdp[i] = max(max(nums[i], maxdp[i - 1] * nums[i]), mindp[i - 1] * nums[i]);
            mindp[i] = min(min(nums[i], maxdp[i - 1] * nums[i]), mindp[i - 1] * nums[i]);
            if (maxdp[i] > ans) ans = maxdp[i];
        }
        return ans;
    }
};

//法二：动态规划+空间压缩
class Solution 
{
public:
    int maxProduct(vector<int>& nums) 
    {
        int n = nums.size();
        int premax = nums[0], premin = nums[0], ans = nums[0];
        for (int i = 1; i < n; i++)
        {
            int curmax = max(nums[i], max(premax * nums[i], premin * nums[i]));
            int curmin = min(nums[i], min(premax * nums[i], premin * nums[i]));
            premax = curmax;
            premin = curmin;
            ans = max(ans, premax);
        }
        return ans;
    }
};


//题目二：
// 子序列累加和必须被7整除的最大累加和
// 给定一个非负数组nums，
// 可以任意选择数字组成子序列，但是子序列的累加和必须被7整除
// 返回最大累加和
// 对数器验证
//
//法一：暴力验证（做对数器用）
class Solution
{
    //递归定义：nums[i,...,n-1]上的能被7整除的最大子序列和
    int f(vector<int>& nums, int i, int sum)
    {
        if (i == nums.size())
            return sum % 7 == 0 ? sum : 0;

        return max(f(nums, i + 1, sum), f(nums, i + 1, nums[i] + sum));
    }

    int maxSum(vector<int>& nums)
    {
        return f(nums, 0, 0);
    }
};

//法二：动态规划（正式方法）
//dp[i][j]: nums前i个数里，形成的子序列的累加和%7==j  这样的子序列最大累加和是多少
class Solution
{
    int maxSum(vector<int>& nums)
    {
        int n = nums.size();
        // dp[i][j] : nums[0...i-1]
        // nums前i个数形成的子序列一定要做到，子序列累加和%7 == j
        // 这样的子序列最大累加和是多少
        // 注意 : dp[i][j] == -1代表不存在这样的子序列
        vector<vector<int>> dp(n + 1, vector<int>(7, 0));
        //初始化
        dp[0][0] = 0;
        fill(dp[0].begin() + 1, dp[0].end(), -1);

        for (int i = 1; i <= n; i++)
        {
            for (int j = 0; j < 7; j++)
            {
                int cur = nums[i - 1] % 7;
                int need = (j - need + 7) % 7;
                //1.不要第i-1个数
                dp[i][j] = dp[i - 1][j];
                //2.要第i-1个数
                if (dp[i - 1][need] != -1)
                    dp[i][j] = max(dp[i][j], dp[i - 1][need] + nums[i]);
            }
        }
        return dp[n][0];
    }
};



//题目三：
// 魔法卷轴
// 给定一个数组nums，其中可能有正、负、0
// 每个魔法卷轴可以把nums中连续的一段全变成0
// 你希望数组整体的累加和尽可能大
// 卷轴使不使用、使用多少随意，但一共只有2个魔法卷轴
// 请返回数组尽可能大的累加和
// 对数器验证
//
//思路：
//由于只有2个魔法卷轴，因此做使用0个、1个、2个魔法卷轴的讨论
//1.使用0个卷轴：直接相加，求出p1
//2.使用1个卷轴：定义dp[i]: 0~i范围上一定使用1次卷轴，0~i范围上整体最大累加和是多少
//  1) 要dp[i] dp[i]=nums[i]+dp[i-1]   (这里的dp[]就是prefix[])
//  2) 不要dp[i]，则问题在于如何求出卷轴啥时候停止 -->> 运用前缀和，到当前区域内最大前缀和时停止
//     -> dp[i]=maxPreSum
//  最后两种情况比大小即可，求出p2
//3.使用2个卷轴：
//  在定义了prefix[i]:0~i范围上一定使用1次卷轴，0~i范围上整体最大累加和是多少的基础上，
//  再定义suffix[i]:i+1~n范围上一定使用1次卷轴，i+1~n范围上整体最大累加和是多少
//  注：这两个数组错开，是因为如果连上的话，就相当于使用1个卷轴
//  最后依据分界点遍历，求出p3
//最后统计答案：p1,p2,p3比大小
class Solution
{
    int magicScroll(vector<int>& nums)
    {
        int n = nums.size();
        int p1, p2, p3 = INT_MIN;
        //不使用卷轴:
        for (auto& ele : nums) p1 += ele;

        //使用1个卷轴:
        vector<int> prefix(n, 0);// prefix[i] : 0~i范围上一定要用1次卷轴的情况下，0~i范围上整体最大累加和多少
        int maxPreSum = max(0, nums[0]);//记录当前最大前缀和  [注]：当为空数组时，累加和为0，这个也要考虑
        for (int i = 1, curSum = nums[0]; i < n; i++)
        {
            prefix[i] = max(nums[i] + prefix[i - 1], maxPreSum);
            curSum += nums[i];//记录当前前缀和
            maxPreSum = max(maxPreSum, curSum);
        }
        p2 = prefix[n - 1];

        //使用2个卷轴:
        vector<int> suffix(n, 0);// suffix[i] : i~n-1范围上一定要用1次卷轴的情况下，i~n-1范围上整体最大累加和多少
        int maxSufSum = max(nums[n - 1], 0);
        for (int i = n - 2, curSum = nums[n - 1]; i >= 0; i--)
        {
            suffix[i] = max(nums[i] + suffix[i + 1], maxSufSum);
            curSum += nums[i];
            maxSufSum = max(maxSufSum, curSum);
        }
        for (int i = 1; i < n; i++)//枚举所有划分点
        {
            // 枚举所有的划分点i
            // 0~i-1 左
            // i~n-1 右
            p3 = max(p3, prefix[i - 1] + suffix[i]);
        }

        return max(max(p1, p2), p3);
    }
};



//题目四：
// 三个无重叠子数组的最大和
// 给你一个整数数组 nums 和一个整数 k
// 找出三个长度为 k 、互不重叠、且全部数字和（3 * k 项）最大的子数组
// 并返回这三个子数组
// 以下标的数组形式返回结果，数组中的每一项分别指示每个子数组的起始位置
// 如果有多个结果，返回字典序最小的一个
// 测试链接 : https://leetcode.cn/problems/maximum-sum-of-3-non-overlapping-subarrays/

//思路：核心在于构造信息数组，再根据中间的子数组左右分析第一个、第三个子数组
//创建一个sum数组，sum[i]记录从i位置开始往后达成k长度的子串大小
//创建prefix数组，predix[i]记录0~i位置上最大子串(k长度)的开头下标  （predp）
//创建suffix数组，suffix[i]记录i~n-1位置上最大子串(长度)的开头下标  （sufdp）
//中间数组由k移动到n-k-1，移动时统计左边第一个数组情况、右边第三个数组情况

//小结：动态规划主打的就是一个尝试策略，这里因为是累加和且长度固定所以比较容易想到sum数组，
//          因为是三个数组所以划分三个区域，每个区域找最大值的左下标，枚举三个区域分界点

class Solution 
{
public:
    vector<int> maxSumOfThreeSubarrays(vector<int>& nums, int k) 
    {
        int n = nums.size();
        vector<int> sum(n, 0);
        vector<int> prefix(n, 0);
        vector<int> suffix(n, 0);

        for (int l = 0, r = 0, cursum = 0; r < n; r++)
        {
            cursum += nums[r];
            if (r - l + 1 == k)
            {
                sum[l] = cursum;
                cursum -= nums[l];
                l++;
            }
        }

        // prefix[k-1] = 0;   初始化
        for (int i = k; i < n; i++)
        {
            prefix[i] = sum[prefix[i - 1]] >= sum[i - k + 1] ? prefix[i - 1] : i - k + 1;
        }

        suffix[n - k] = n - k;//初始化
        for (int i = n - k - 1; i >= 0; i--)
        {
            suffix[i] = sum[i] >= sum[suffix[i + 1]] ? i : suffix[i + 1];
        }

        int a, b, c;
        int maxs = 0;
        for (int i = k, j = 2 * k - 1; j <= n - k - 1; i++, j++)
        {
            // 0.....i-1   i.....j  j+1.....n-1
            // 最好开头p      i开头     最好开头s
            int p = prefix[i - 1];
            int s = suffix[j + 1];
            int cursum = sum[p] + sum[i] + sum[s];
            if (cursum > maxs)
            {
                maxs = cursum;
                a = p;
                b = i;
                c = s;
            }
        }

        return { a,b,c };
    }
};



//题目五：
// 可以翻转1次的情况下子数组最大累加和
// 给定一个数组nums，
// 现在允许你随意选择数组连续一段进行翻转，也就是子数组逆序的调整
// 比如翻转[1,2,3,4,5,6]的[2~4]范围，得到的是[1,2,5,4,3,6]
// 返回必须随意翻转1次之后，子数组的最大累加和
// 对数器验证

//也等同于：可以翻转、可以不翻转的子数组最大累加和
// 
//思路：翻转后累加和最大的子数组，一定是由 原数组的一段(a) + a数组边界外的某一段子数组翻转 得来的
//定义：原数组(a)（a数组往右的最大子串）+a左侧部分的最大子串 即为最终答案，因为如果求出左侧部分最大字串，则一定能翻转过来
//      当然也可以定义为a数组往左最大字串+a右侧部分的最大字串

// 法一：正式方法 O(n)
class Solution1
{
public:
    int maxSumReverse(vector<int>& nums)
    {
        int n = nums.size();
        if (n == 0) return 0;

        // sufDP[i]: 以i位置开头向右延伸能得到的最大子数组和
        vector<int> sufDP(n, 0);
        sufDP[n - 1] = nums[n - 1];
        for (int i = n - 2; i >= 0; i--)
        {
            sufDP[i] = max(nums[i], nums[i] + sufDP[i + 1]);
        }

        // preDP[i]: 以i位置结尾向左延伸能得到的最大子数组和
        vector<int> preDP(n, 0);
        preDP[0] = nums[0];
        for (int i = 1; i < n; i++)
        {
            preDP[i] = max(nums[i], nums[i] + preDP[i - 1]);
        }

        int ans = sufDP[0]; // 初始为不翻转的情况(即整个数组最大子数组和，sufDP[0]可能不包含前面的负数，需结合maxPreSum更新)
        // 实际上 sufDP[0] 只是必须以0开头的最大和，真正的不翻转最大和应该在循环中通过 maxPreSum 捕获

        int maxPreSum = preDP[0]; // 当前遇到的最大前缀累加和

        // 枚举所有划分点
        // 这里的逻辑是：尝试将数组分为两部分 [0...i-1] 和 [i...n-1]
        // 翻转中间某段可以使得原本不连续的 maxPreSum(某段结尾) 和 sufDP[i](某段开头) 连在一起
        for (int i = 1; i < n; i++)
        {
            // 尝试将左边某个最大子数组(maxPreSum) 和 右边某个最大子数组连起来
            ans = max(ans, sufDP[i] + maxPreSum);
            maxPreSum = max(maxPreSum, preDP[i]);
        }
        // 注意：最后还要检查一次 maxPreSum，这代表了不需要翻转（或者翻转区间在边缘无效）时，原数组的最大子数组和
        ans = max(ans, maxPreSum);

        return ans;
    }
};

// 法二：暴力方法
class Solution2
{
public:
    int maxSum(vector<int>& nums)
    {
        int ans = INT_MIN;
        int n = nums.size();
        if (n == 0) return 0;

        for (int i = 0, pre = 0; i < n; i++)
        {
            pre = max(nums[i], nums[i] + pre);
            ans = max(ans, pre);
        }
        return ans;
    }

    int maxSumReverse(vector<int>& nums)
    {
        int ans = INT_MIN;
        int n = nums.size();
        if (n == 0) return 0;

        for (int l = 0; l < n; l++)
        {
            for (int r = l; r < n; r++)
            {
                reverse(nums.begin() + l, nums.begin() + r + 1);
                ans = max(ans, maxSum(nums));
                reverse(nums.begin() + l, nums.begin() + r + 1);
            }
        }
        return ans;
    }
};

vector<int> randomArray(int len, int v)
{
    vector<int> nums(len);
    for (int i = 0; i < len; i++)
    {
        nums[i] = rand() % v - v / 2;
    }
    return nums;
}

int main()
{
    srand((unsigned int)time(0));
    int n = 50;
    int v = 200;
    int testTime = 5000;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++)
    {
        int len = rand() % 50 + 1;
        vector<int> nums = randomArray(len, v);

        Solution1 t1;
        Solution2 t2;
        int ans1 = t1.maxSumReverse(nums);
        int ans2 = t2.maxSumReverse(nums);
        if (ans1 != ans2)
        {
            cout << "error!" << endl;
            cout << "ans1: " << ans1 << ", ans2: " << ans2 << endl;
            // 打印出错的数组以便调试
            cout << "Array: ";
            for (int num : nums) cout << num << " ";
            cout << endl;
            break;
        }
    }
    cout << "测试结束" << endl;

    return 0;
}



//题目六：
// 删掉1个数字后长度为k的子数组最大累加和
// 给定一个数组nums，求必须删除一个数字后的新数组中
// 长度为k的子数组最大累加和，删除哪个数字随意
// 对数器验证

//思路：求出原数组中每一个k+1长度的子串累加和与该子串中的最小值，减去这个最小值
//      需要单调队列
class Solution
{
    int maxNums(vector<int>& nums, int k)
    {
        int n = nums.size();
        if (n <= k) return 0;

        int ans = INT_MIN;
        vector<int> upQue(n);//单调队列
        int l = 0, r = 0;

        for (int i = 0, sum = 0; i < n; i++)
        {
            while (l < r && nums[upQue[r]] >= nums[i])
                r--;
            upQue[r++] = i;
            
            sum += nums[i];
            if (i >= k)
            {
                ans = max(ans, sum - nums[upQue[l]]);
                if (upQue[l] == i - k) l++;
                sum -= nums[i - k];
            }
        }

        return ans;
    }
};

