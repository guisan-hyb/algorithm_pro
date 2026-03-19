#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;



//本节课讲述：最长递增子序列  最长不下降子序列  的最优解，以及一些扩展题目

//注意：本节课讲的是最优解，时间复杂度是O(n*logn)，空间复杂度是O(n)，好实现、理解难度不大
//这个问题也可以用线段树来求解，时间和空间复杂度和本节课讲的最优解没有区别，但需要理解线段树

//这个算法也叫做LIS算法


//题目一：
// 最长递增子序列和最长不下降子序列
// 给定一个整数数组nums
// 找到其中最长严格递增子序列长度、最长不下降子序列长度
// 测试链接 : https://leetcode.cn/problems/longest-increasing-subsequence/
//
//法一：动态规划  O(n^2)
//  dp[i]: 以i位置结尾的数做结尾的最长递增子序列长度
// 
//答疑
//问：什么样的题目适合「选或不选」，什么样的题目适合「枚举选哪个」？
//答：我分成两类问题：
//相邻无关子序列问题（比如 0 - 1 背包），适合「选或不选」。每个元素互相独立，只需依次考虑每个元素选或不选。
//相邻相关子序列问题（比如本题），适合「枚举选哪个」。我们需要知道子序列中的相邻两个数的关系。对于本题来说，枚举 nums[i] 必选，然后枚举前一个必选的数，方便比大小。
//    如果硬要用「选或不选」，需要额外记录上一个选的数的下标，算法总体的空间复杂度为 O(n^2)，而枚举选哪个只需要 O(n) 的空间。

class Solution 
{
public:
    int lengthOfLIS(vector<int>& nums) 
    {
        int n = nums.size();
        vector<int> dp(n, 0);
        int ans = 0;
        for (int i = 0; i < n; i++)
        {
            dp[i] = 1;
            for (int j = 0; j < i; j++)
            {
                if (nums[j] < nums[i])
                    dp[i] = max(dp[i], 1 + dp[j]);
            }
            ans = max(ans, dp[i]);
        }
        return ans;
    }
};

//法二：最优解  时间复杂度O(n * logn)
//贪心+二分
//依然着眼于某个上升子序列的 结尾的元素，
//如果 已经得到的上升子序列的结尾的数越小，那么遍历的时候后面接上一个数，会有更大的可能构成一个长度更长的上升子序列。
class Solution 
{
public:
    int f(vector<int>& ends, int len, int x)
    {
        int ans = -1;
        for (int l = 0, r = len - 1; l <= r;)
        {
            int m = l + (r - l) / 2;
            if (ends[m] >= x)
            {
                ans = m;
                r = m - 1;
            }
            else
                l = m + 1;
        }
        return ans;
    }

    int lengthOfLIS(vector<int>& nums)
    {
        // len表示ends数组目前的有效区长度
        // ends[0...len-1]是有效区，有效区内的数字一定严格升序
        int n = nums.size();
        int ans = 0;
        vector<int> ends(n, 0);
        int len = 0;
        for (int i = 0, find = -1; i < n; i++)
        {
            find = f(ends, len, nums[i]);
            if (find != -1)
            {
                ends[find] = nums[i];
            }
            else
                ends[len++] = nums[i];
        }
        return len;
    }
};

//附：
// 如果求最长不下降子序列（可以相等），那么使用如下的二分搜索 :
// ends[0...len-1]是不降序的
// 在其中找到>num的最左位置，如果不存在返回-1
// 如果求最长不下降子序列，就在lengthOfLIS中把bs1方法换成bs2方法
// 已经用对数器验证了，是正确的
class Solution
{
public:
    int f(vector<int>& ends, int len, int x)
    {
        int ans = -1;
        for (int l = 0, r = len - 1; l <= r;)
        {
            int m = l + (r - l) / 2;
            if (ends[m] > x)
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



//题目二：
// 俄罗斯套娃信封问题
// 给你一个二维整数数组envelopes ，其中envelopes[i]=[wi, hi]
// 表示第 i 个信封的宽度和高度
// 当另一个信封的宽度和高度都比这个信封大的时候
// 这个信封就可以放进另一个信封里，如同俄罗斯套娃一样
// 请计算 最多能有多少个信封能组成一组“俄罗斯套娃”信封
// 即可以把一个信封放到另一个信封里面，注意不允许旋转信封
// 测试链接 : https://leetcode.cn/problems/russian-doll-envelopes/
//
//思路：将所有信封按要求排序：1.按照宽度由小到大升序排序 2.按照高度由大到小降序排序
//      -->>转化成最长递增子序列问题
class Solution 
{
public:
    static bool cmp(vector<int>& a, vector<int>& b)
    {
        // 排序策略:
        // 宽度从小到大
        // 宽度一样，高度从大到小
        if (a[0] != b[0])
            return a[0] < b[0];
        return a[1] > b[1];
    }

    int f(const vector<int>& ends, int len, int x)
    {
        int ans = -1;
        for (int l = 0, r = len - 1; l <= r;)
        {
            int m = l + (r - l) / 2;
            if (ends[m] >= x)
            {
                ans = m;
                r = m - 1;
            }
            else
                l = m + 1;
        }
        return ans;
    }

    int maxEnvelopes(vector<vector<int>>& envelopes)
    {
        int n = envelopes.size();
        sort(envelopes.begin(), envelopes.end(), cmp);
        vector<int> ends(n);
        int len = 0;
        for (int i = 0; i < n; i++)
        {
            int find = f(ends, len, envelopes[i][1]);
            if (find == -1)
                ends[len++] = envelopes[i][1];
            else
                ends[find] = envelopes[i][1];
        }
        return len;
    }
};



//题目三：
// 使数组K递增的最少操作次数
// 给你一个下标从0开始包含n个正整数的数组arr，和一个正整数k
// 如果对于每个满足 k <= i <= n-1 的下标 i
// 都有 arr[i-k] <= arr[i] ，那么称 arr 是K递增的
// 每一次操作中，你可以选择一个下标i并将arr[i]改成任意正整数
// 请你返回对于给定的 k ，使数组变成K递增的最少操作次数
// 测试链接 : https://leetcode.cn/problems/minimum-operations-to-make-the-array-k-increasing/
//
//思路：分组+最长递增子序列
class Solution
{
public:
    int f(const vector<int>& ends, int len, int x)
    {
        int ans = -1;
        for (int l = 0, r = len - 1; l <= r;)
        {
            int m = l + (r - l) / 2;
            if (ends[m] > x)
            {
                ans = m;
                r = m - 1;
            }
            else
                l = m + 1;
        }
        return ans;
    }

    int LIS(vector<int>& nums)
    {
        int n = nums.size();
        vector<int> ends(n);
        int len = 0;
        for (int i = 0; i < n; i++)
        {
            int find = f(ends, len, nums[i]);
            if (find == -1)
                ends[len++] = nums[i];
            else
                ends[find] = nums[i];
        }
        return len;
    }

    int kIncreasing(vector<int>& arr, int k) 
    {
        int n = arr.size();
        vector<int> nums;
        int ans = 0;
        for (int i = 0; i < k; i++)
        {
            nums.assign({});
            for (int j = i; j < n; j += k)
            {
                nums.push_back(arr[j]);
            }
            ans += nums.size() - LIS(nums);
        }
        return ans;
    }
};



//扩展：ends数组的更新和查询是分离的
//      见 题目四、题目五


//题目四：
// 最长数对链
// 给你一个由n个数对组成的数对数组pairs
// 其中 pairs[i] = [lefti, righti] 且 lefti < righti
// 现在，我们定义一种 跟随 关系，当且仅当 b < c 时
// 数对 p2 = [c, d] 才可以跟在 p1 = [a, b] 后面
// 我们用这种形式来构造 数对链
// 找出并返回能够形成的最长数对链的长度
// 测试链接 : https://leetcode.cn/problems/maximum-length-of-pair-chain/
//
//法一：LIS
class Solution 
{
public:
    static bool cmp(vector<int>& a, vector<int>& b)
    {
        return a[0] < b[0];
    }

    int f(const vector<int>& ends, int len, int x)
    {
        int ans = -1;
        for (int l = 0, r = len - 1; l <= r;)
        {
            int m = l + (r - l) / 2;
            if (ends[m] >= x)
            {
                ans = m;
                r = m - 1;
            }
            else
                l = m + 1;
        }
        return ans;
    }

    int findLongestChain(vector<vector<int>>& pairs)
    {
        int n = pairs.size();
        vector<int> ends(n, 0);
        int len = 0;
        //按left升序排序
        sort(pairs.begin(), pairs.end(), cmp);

        //查询和比较分离：
        for (int i = 0; i < n; i++)
        {
            int left = pairs[i][0];
            int right = pairs[i][1];
            int find = f(ends, len, left);
            if (find == -1)
                ends[len++] = right;
            else
                ends[find] = min(ends[find], right);
        }
        return len;
    }
};

//法二：贪心（最优解）
//按结尾排序
class Solution
{
public:
    static bool cmp(vector<int>& a, vector<int>& b)
    {
        return a[1] < b[1];
    }

    int findLongestChain(vector<vector<int>>& pairs) 
    {
        sort(pairs.begin(), pairs.end(), cmp);
        int ans = 0;
        int pre = INT_MIN;
        for (auto& ele : pairs)
        {
            if (ele[0] > pre)
            {
                ans++;
                pre = ele[1];
            }
        }
        return ans;
    }
};



//题目五：
// 有一次修改机会的最长不下降子序列
// 给定一个长度为n的数组arr，和一个整数k
// 只有一次机会可以将其中连续的k个数全修改成任意一个值
// 这次机会你可以用也可以不用，请返回最长不下降子序列长度
// 1 <= k, n <= 10^5
// 1 <= arr[i] <= 10^6
// 测试链接 : https://www.luogu.com.cn/problem/P8776
//
//思路：
//让想要修改的连续k个数紧邻选中的数字： ...,a,k,k,k,k,b,...
//求出a左侧最大值小于b的最长不下降子序列长度 -- (1)
//修改的连续k个数对应答案 k -- (2)
//求出b右侧的最长不下降子序列长度 -- (3)
//ans = max(ans,(1)+(2)+(3))

namespace test_5
{
    #include <iostream>
    #include <vector>
    using namespace std;

    const int MAXN = 100005;
    int n, k;
    vector<int> nums(MAXN);//输入的数组
    vector<int> ends_(MAXN);
    vector<int> right_(MAXN);//辅助数组：以i位置开头,[i,...,n-1]上的最长不下降子序列长度

    // 求最长不下降子序列长度的二分
    // ends[0...len-1]是升序的，找到>num的最左位置
    // 不存在返回-1
    int f1(int len, int x)
    {
        int ans = -1;
        for (int l = 0, r = len - 1; l <= r;)
        {
            int m = l + (r - l) / 2;
            if (ends_[m] > x)
            {
                ans = m;
                r = m - 1;
            }
            else
                l = m + 1;
        }
        return ans;
    }

    // 求最长不上升子序列长度的二分
    // ends[0...len-1]是降序的，找到<num的最左位置
    // 不存在返回-1
    int f2(int len, int x)
    {
        int ans = -1;
        for (int l = 0, r = len - 1; l <= r;)
        {
            int m = l + (r - l) / 2;
            if (ends_[m] < x)
            {
                ans = m;
                r = m - 1;
            }
            else
                l = m + 1;
        }
        return ans;
    }

    void buildRight()//建立辅助数组right
    {
        //以i位置开头,[i,...,n-1]上的最长不下降子序列长度
        //等价于
        //以n-1位置开头, n-1,n-2,..,i位置上的最长不上升子序列长度

        // right[j] :
        // 一定以arr[j]做开头的情况下，arr[j...]上最长不下降子序列长度是多少
        // 关键逻辑 :
        // 一定以arr[i]做开头的情况下，arr[i...]上最长不下降子序列
        // 就是！从n-1出发来看(从右往左遍历)，以arr[i]做结尾的情况下的最长不上升子序列

        //借助ends数组实现

        int len = 0;
        for (int i = n - 1; i >= 0; i--)
        {
            int find = f2(len, nums[i]);
            if (find == -1)
            {
                ends_[len++] = nums[i];
                right_[i] = len;
            }
            else
            {
                ends_[find] = nums[i];
                right_[i] = find + 1;
            }
        }
    }

    int compute()
    {
        buildRight();
        int ans = 0;
        int len = 0;
        for (int i = 0, j = k; j < n; i++, j++)
        {
            // 在左边部分[0...i-1]中寻找长度
            // f1找到的是第一个 > nums[j] 的位置
            // 如果找到位置idx，说明长度为idx的序列可以接上，长度为idx
            // 如果没找到(-1)，说明所有元素都 <= nums[j]，整个左边序列都可以接上，长度为len
            int find = f1(len, nums[j]);
            int lans = (find == -1) ? len : find;
            ans = max(ans, lans + k + right_[j]);

            //更新ends数组
            find = f1(len, nums[i]);
            if (find != -1)
                ends_[find] = nums[i];
            else
                ends_[len++] = nums[i];
        }
        ans = max(ans, len + k);// 处理修改区间覆盖数组末尾的情况
        return ans;
    }

    int main()
    {
        cin >> n >> k;
        for (int i = 0; i < n; i++)
            cin >> nums[i];

        cout << compute();

        return 0;
    }
}

