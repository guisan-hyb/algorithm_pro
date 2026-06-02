#define _CRT_SECURE_NO_WARNINGS


#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <unordered_map>
#include <queue>
#include <algorithm>

using namespace std;

//贪心经典题目专题1

//狭义的贪心
//每一步都做出在当前状态下最好或最优的选择，从而希望最终的结果是最好或最优的算法

//广义的贪心
//通过分析题目自身的特点和性质，只要发现让求解答案的过程得到加速的结论，都算广义的贪心

//贪心是最符合自然智慧的思想，一般分析门槛不高
//理解基本的排序、有序结构，有基本的逻辑思维就能理解
//但是贪心的题目，千题千面，极难把握
//难度在于证明局部最优可以得到全局最优，好在！我们有对数器！贪心专题2、3，这两节大量使用对数器

//有关贪心的若干现实 & 提醒
//1.不要取纠结严格证明，每个题都去追求严格证明，浪费时间、收益很低，而且千题千面。玄学！
//2.一定要掌握用对数器验证的技巧，这是解决贪心问题的关键
//3.解法几乎只包含贪心思路的题目，代码量都不大
//4.大量积累贪心的经验，重点不是证明，而是题目的特征，以及贪心方式的特征，做好总结方便借鉴
//5.关注题目的数据量，题目的解可能来自贪心，也很有可能不是，如果数据量允许，能不用贪心就不同（稳）
//6.贪心在笔试中出现概率不低，但是面试中出现概率较低，原因是 淘汰率 vs 区分度
//7.广义的贪心无处不在，可能和别的思路结合，一般都可以通过自然智慧想明白，依然不纠结证明



//题目一：
// 最大数
// 给定一组非负整数nums
// 重新排列每个数的顺序（每个数不可拆分）使之组成一个最大的整数
// 测试链接 : https://leetcode.cn/problems/largest-number/
class Solution {
public:
    string largestNumber(vector<int>& nums) {
        vector<string> arr;
        for (auto& ele : nums) {
            arr.push_back(to_string(ele));
        }
        sort(arr.begin(), arr.end(), [](string& a, string& b) {
            return a + b > b + a;
            });

        if (arr[0] == "0") return "0";

        string ans;
        for (auto& ele : arr) {
            ans += ele;
        }

        return ans;
    }
};



//题目二：
// 两地调度
// 公司计划面试2n个人，给定一个数组 costs
// 其中costs[i]=[aCosti, bCosti]
// 表示第i人飞往a市的费用为aCosti，飞往b市的费用为bCosti
// 返回将每个人都飞到a、b中某座城市的最低费用
// 要求每个城市都有n人抵达
// 测试链接 : https://leetcode.cn/problems/two-city-scheduling/

//法一：贪心
class Solution {
public:
    int twoCitySchedCost(vector<vector<int>>& costs) {
        int n = costs.size();
        int sum = 0;
        vector<int> arr;
        for (auto& ele : costs) {
            sum += ele[0];
            arr.push_back(ele[1] - ele[0]);
        }
        sort(arr.begin(), arr.end());

        for (int i = 0; i < n / 2; i++) {
            sum += arr[i];
        }
        return sum;
    }
};

//法二：动态规划
class Solution {
public:
    int f(vector<vector<int>>& costs, int i, int a, int b, vector<vector<vector<int>>>& dp) {
        int n = costs.size();
        if (i == n) return (a == n / 2 && b == n / 2) ? 0 : INT_MAX / 2;
        if (a > n / 2 || b > n / 2) return INT_MAX / 2;
        if (dp[i][a][b] != -1) return dp[i][a][b];

        int ans = INT_MAX / 2;
        int p1 = f(costs, i + 1, a + 1, b, dp) + costs[i][0];
        int p2 = f(costs, i + 1, a, b + 1, dp) + costs[i][1];
        ans = min(p1, p2);

        dp[i][a][b] = ans;
        return ans;
    }

    int twoCitySchedCost(vector<vector<int>>& costs) {
        int n = costs.size();
        int m = n / 2;
        vector<vector<vector<int>>> dp(n, vector<vector<int>>(m + 1, vector<int>(m + 1, -1)));
        return f(costs, 0, 0, 0, dp);
    }
};

//法三：动态规划优化成二维
class Solution {
public:
    int f(vector<vector<int>>& costs, int i, int a, vector<vector<int>>& dp) {
        int n = costs.size();
        int m = n / 2;
        int b = i - a;

        if (i == n) return (a == m && b == m) ? 0 : INT_MAX / 2;
        if (a > m || b > m) return INT_MAX / 2;
        if (dp[i][a] != -1) return dp[i][a];

        int p1 = f(costs, i + 1, a + 1, dp) + costs[i][0];
        int p2 = f(costs, i + 1, a, dp) + costs[i][1];

        return dp[i][a] = min(p1, p2);
    }

    int twoCitySchedCost(vector<vector<int>>& costs) {
        int n = costs.size();
        int m = n / 2;
        vector<vector<int>> dp(n, vector<int>(m + 1, -1));
        return f(costs, 0, 0, dp);
    }
};



//题目三；
// 吃掉N个橘子的最少天数
// 厨房里总共有 n 个橘子，你决定每一天选择如下方式之一吃这些橘子
// 1）吃掉一个橘子
// 2) 如果剩余橘子数 n 能被 2 整除，那么你可以吃掉 n/2 个橘子
// 3) 如果剩余橘子数 n 能被 3 整除，那么你可以吃掉 2*(n/3) 个橘子
// 每天你只能从以上 3 种方案中选择一种方案
// 请你返回吃掉所有 n 个橘子的最少天数
// 测试链接 : https://leetcode.cn/problems/minimum-number-of-days-to-eat-n-oranges/
class Solution {
public:
    unordered_map<int, int> dp;

    int minDays(int n) {
        if (n <= 1) return n;
        if (dp.count(n)) return dp[n];

        // 1) 吃掉一个橘子
        // 2) 如果n能被2整除，吃掉一半的橘子，剩下一半
        // 3) 如果n能被3正数，吃掉三分之二的橘子，剩下三分之一
        // 因为方法2）和3），是按比例吃橘子，所以必然会非常快
        // 所以，决策如下：
        // 可能性1：为了使用2）方法，先把橘子吃成2的整数倍，然后直接干掉一半，剩下的n/2调用递归
        // 即，n % 2 + 1 + minDays(n/2)
        // 可能性2：为了使用3）方法，先把橘子吃成3的整数倍，然后直接干掉三分之二，剩下的n/3调用递归
        // 即，n % 3 + 1 + minDays(n/3)
        // 至于方法1)，完全是为了这两种可能性服务的，因为能按比例吃，肯定比一个一个吃快(显而易见的贪心)
        int ans = min(n % 2 + 1 + minDays(n / 2), n % 3 + 1 + minDays(n / 3));
        return dp[n] = ans;
    }
};



//题目四：
// 会议室II
// 给你一个会议时间安排的数组 intervals
// 每个会议时间都会包括开始和结束的时间intervals[i]=[starti, endi]
// 返回所需会议室的最小数量
// 测试链接 : https://leetcode.cn/problems/meeting-rooms-ii/
// 测试链接 : https://www.nowcoder.com/practice/1ae8d0b6bb4e4bcdbf64ec491f63fc37
// 测试链接 : https://leetcode.cn/problems/divide-intervals-into-minimum-number-of-groups/
namespace test4 {
#include <functional>
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
    using namespace std;

    int n;
    vector<vector<int>> arr;
    priority_queue<int, vector<int>, greater<>> heap;

    int main() {
        cin >> n;
        for (int i = 0; i < n; i++) {
            int s, e;
            cin >> s >> e;
            arr.push_back({ s, e });
        }
        sort(arr.begin(), arr.end(), [](vector<int>& a, vector<int>& b) {
            return a[0] < b[0];
            });

        int ans = 0;
        for (int i = 0; i < n; i++) {
            while (!heap.empty() && arr[i][0] >= heap.top()) {
                heap.pop();
            }
            heap.push(arr[i][1]);
            ans = max(ans, (int)heap.size());
        }

        cout << ans << endl;

        return 0;
    }
}


//互相重叠的区间，一定不能分到同一组里。所以有多少重叠的区间，就等于需要分为多少组。
class Solution {
public:
    int minGroups(vector<vector<int>>& intervals) {
        sort(intervals.begin(), intervals.end(), [](vector<int>& a, vector<int>& b) {
            return a[0] < b[0];
            });

        int ans = 0;
        priority_queue<int, vector<int>, greater<>> heap;
        for (auto& ele : intervals) {
            while (!heap.empty() && ele[0] > heap.top()) {
                heap.pop();
            }
            heap.push(ele[1]);
            ans = max(ans, (int)heap.size());
        }
        return ans;
    }
};



//题目五：
// 课程表III
// 这里有n门不同的在线课程，按从1到n编号
// 给你一个数组courses
// 其中courses[i]=[durationi, lastDayi]表示第i门课将会持续上durationi天课
// 并且必须在不晚于lastDayi的时候完成
// 你的学期从第 1 天开始
// 且不能同时修读两门及两门以上的课程
// 返回你最多可以修读的课程数目
// 测试链接 : https://leetcode.cn/problems/course-schedule-iii/
class Solution {
public:
    int scheduleCourse(vector<vector<int>>& courses) {
        //按截止日期从小到大排序
        sort(courses.begin(), courses.end(), [](vector<int>& a, vector<int>& b) {
            return a[1] < b[1];
            });

        int time = 0;
        priority_queue<int> heap;
        for (auto& ele : courses) {
            if (time + ele[0] <= ele[1]) {
                time += ele[0];
                heap.push(ele[0]);
            }
            else {
                if (!heap.empty() && heap.top() > ele[0]) {
                    time = time - heap.top() + ele[0];
                    heap.pop();
                    heap.push(ele[0]);
                }
            }
        }

        return heap.size();
    }
};



//题目六：
// 连接棒材的最低费用(leetcode测试)
// 你有一些长度为正整数的棍子
// 这些长度以数组sticks的形式给出
// sticks[i]是第i个木棍的长度
// 你可以通过支付x+y的成本将任意两个长度为x和y的棍子连接成一个棍子
// 你必须连接所有的棍子，直到剩下一个棍子
// 返回以这种方式将所有给定的棍子连接成一个棍子的最小成本
// 测试链接 : https://leetcode.cn/problems/minimum-cost-to-connect-sticks/
// 测试链接 : https://www.luogu.com.cn/problem/P1090

//经典的 Huffman Coding 问题  哈夫曼编码

namespace test6 {
    #include <iostream>
    #include <vector>
    #include <queue>
    using namespace std;

    int n;
    priority_queue<int, vector<int>, greater<>> heap;

    int main() {
        cin >> n;
        for (int i = 0; i < n; i++) {
            int a;
            cin >> a;
            heap.push(a);
        }

        int sum = 0;
        while (heap.size() > 1) {
            int cur = heap.top();
            heap.pop();
            cur += heap.top();
            heap.pop();

            sum += cur;
            heap.push(cur);
        }

        cout << sum;

        return 0;
    }
}

