#define _CRT_SECURE_NO_WARNINGS
//对数器的实现
//1.你想要测的方法a
//2.时间复杂度不好但是容易实现的方法b
//3.实现一个随机样本产生器
//4.把方法a和方法b跑相同的随机样本，看看得到的结果是否一样
//5.如果有一个随机样本使得比对结果不一致，打印样本进行人工干预，改对方法a和方法b
//6.当样本数量很多时比对测试仍然正确，则可以确定方法a已经正确

//关键是第5步，找到一个数据量小的错误样本
//然后把错误例子代入代码一步一步排查
//print大法、断点技术都可以

//对数器的门槛其实是比较高的，因为往往需要在两种不同思路下实现功能相同的两个方法，暴力一个、想象中的最优解是另一个
//以后很多题目都会用到对数器，几乎可以验证任何方法，尤其是在验证贪心、观察规律方面很有用


//eg:可以翻转1次的情况下子数组最大累加和

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
