#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>

using namespace std;



//前两题都是很经典的区间覆盖问题

//题目一：
// 跳跃游戏II
// 给定一个长度为n的整数数组nums
// 你初始在0下标，nums[i]表示你可以从i下标往右跳的最大距离
// 比如，nums[0] = 3
// 表示你可以从0下标去往：1下标、2下标、3下标
// 你达到i下标后，可以根据nums[i]的值继续往右跳
// 返回你到达n-1下标的最少跳跃次数
// 测试用例可以保证一定能到达
// 测试链接 : https://leetcode.cn/problems/jump-game-ii/

//法一：动态规划
class Solution {
public:
    int f(vector<int>& nums, int i, vector<int>& dp) {
        if (i >= nums.size()) return INT_MAX / 2;
        if (i == nums.size() - 1) return 0;
        if (dp[i] != -1) return dp[i];

        int ans = INT_MAX / 2;
        for (int j = 1; j <= nums[i]; j++) {
            ans = min(ans, f(nums, i + j, dp) + 1);
        }

        dp[i] = ans;
        return ans;
    }

    int jump(vector<int>& nums) {
        int n = nums.size();
        vector<int> dp(n, -1);
        return f(nums, 0, dp);
    }
};

//法二：贪心
class Solution {
public:
    int jump(vector<int>& nums) {
        int ans = 0;
        int cur_end = 0;//当前步以内，最右到哪
        int next_end = 0;//如果再走一步，（当前步+1）以内，最右在哪
        
        for (int i = 0; i < nums.size(); i++) {
            // 来到i下标
            // cur包括了i所在的位置，不用付出额外步数
            // cur没有包括i所在的位置，需要付出额外步数
            if (cur_end < i) {
                ans++;
                cur_end = next_end;
            }
            next_end = max(next_end, i + nums[i]);
        }

        return ans;
    }
};



//题目二：
// 灌溉花园的最少水龙头数目
// 在x轴上有一个一维的花园，花园长度为n，从点0开始，到点n结束
// 花园里总共有 n + 1 个水龙头，分别位于[0, 1, ... n]
// 给你一个整数n和一个长度为n+1的整数数组ranges
// 其中ranges[i]表示
// 如果打开点i处的水龙头，可以灌溉的区域为[i-ranges[i], i+ranges[i]]
// 请你返回可以灌溉整个花园的最少水龙头数目
// 如果花园始终存在无法灌溉到的地方请你返回-1
// 测试链接 : https://leetcode.cn/problems/minimum-number-of-taps-to-open-to-water-a-garden/

// right[i] = j
// 所有左边界在i的水龙头里，影响到的最右右边界是j
//这就把问题转化成了“跳跃游戏 II”

//法一：动态规划
//定义 dp[i] 为覆盖区间i~n 所需的最少水龙头数目。
class Solution {
public:
    int minTaps(int n, vector<int>& ranges) {
        int m = ranges.size();
        vector<int> right(m, 0);//所有左边界在i的水龙头里，影响到的最右右边界是j
        for (int i = 0; i < m; i++) {
            int l = max(0, i - ranges[i]);
            int r = min(m, i + ranges[i]);
            right[l] = max(right[l], r);
        }

        vector<int> dp(m + 1, INT_MAX / 2);
        dp[n] = 0;
        for (int i = n; i >= 0; i--) {
            for (int j = 0; j <= right[i]; j++) {
                dp[i] = min(dp[i], dp[j] + 1);
            }
        }

        return dp[0] == INT_MAX / 2 ? -1 : dp[0];
    }
};

//法二：贪心
class Solution {
public:
    int minTaps(int n, vector<int>& ranges) {
        int m = ranges.size();//m = n + 1
        vector<int> right(m, 0);//所有左边界在i的水龙头里，影响到的最右右边界是j
        for (int i = 0; i < m; i++) {
            int l = max(0, i - ranges[i]);
            int r = min(m, i + ranges[i]);
            right[l] = max(right[l], r);
        }

        int ans = 0;
        int cur_end = 0;//当前ans数量的水龙头，打开了，最右影响的边界
        int next_end = 0;//如果再多打开一个水龙头，最右影响的边界
        for (int i = 0; i < n; i++) {
            next_end = max(next_end, right[i]);
            if (cur_end == i) {
                if (i < next_end) {
                    ans++;
                    cur_end = next_end;
                }
                else {
                    return -1;
                }
            }
        }

        return ans;
    }
};



//题目三：
// 字符串转化
// 给出两个长度相同的字符串str1和str2
// 请你帮忙判断字符串str1能不能在 零次 或 多次 转化后变成字符串str2
// 每一次转化时，你可以将str1中出现的所有相同字母变成其他任何小写英文字母
// 只有在字符串str1能够通过上述方式顺利转化为字符串str2时才能返回true
// 测试链接 : https://leetcode.cn/problems/string-transforms-into-another-string/
// 测试链接 : https://www.lintcode.com/problem/3869

//思路：
//特判：str1==str2 ; str2的字母种类是否为26
//如果str1和str2相同位置上的字母不是同一类型，则一定不能
//例如：
//str1: accba
//str2: bttab

//法一：最优解，分类讨论
class Solution {
public:
    bool canConvert(string& str1, string& str2) {
        if (str1 == str2) return true;

        int kind = 0;
        vector<int> map(26);//同一个表复用两次
        for (int i = 0; i < str2.size(); i++) {
            if (map[str2[i] - 'a']++ == 0) { //记录str2中字母种类
                kind++;
            }
        }

        if (kind == 26) return false;//必定产生连锁反应，不可能做到

        fill(map.begin(), map.end(), -1);//重置map

        for (int i = 0; i < str1.size(); i++) {
            int cur = str1[i] - 'a';
            if (map[cur] != -1 && str2[map[cur]] != str2[i]) { //map用来记录str1中当前字母之前的位置
                return false;
            }
            map[cur] = i;
        }

        return true;
    }
};

//法二：暴力：BFS
class Solution2 {
public:
    bool canConvert(string& str1, string& str2) {
        if (str1 == str2) return true;

        unordered_set<string> cnt;
        queue<string> q;
        //初始化源点
        q.push(str1);
        cnt.insert(str1);

        while (!q.empty())
        {
            string cur = q.front();
            q.pop();

            //暴力枚举
            for (char from = 'a'; from <= 'z'; from++) {
                for (char to = 'a'; to <= 'z'; to++) {
                    if (from == to) continue;

                    bool isChanged = false;//判断是否有更改的情况
                    string nxt = cur;
                    for (int i = 0; i < cur.size(); i++) {
                        if (cur[i] == 'a') {
                            nxt[i] = to;
                            isChanged = true;
                        }
                    }

                    if (!isChanged) continue;
                    if (nxt == str2) return true;
                    //更新
                    if (!cnt.count(nxt)) {
                        q.push(nxt);
                        cnt.insert(nxt);
                    }
                }
            }
        }

        return false;
    }
};

//法三：暴力：DFS
class Solution3 {
public:
    bool f(string& s1, string& s2, unordered_set<string>& visited) {
        if (s1 == s2) return true;

        for (char from = 'a'; from <= 'z'; from++) {
            for (char to = 'a'; to <= 'z'; to++) {
                if (from == to) continue;

                bool isChanged = false;
                string nxt = s1;
                for (int i = 0; i < s1.size(); i++) {
                    if (s1[i] == from) {
                        nxt[i] = to;
                        isChanged = true;
                    }
                }

                if (!isChanged) continue;

                if (!visited.count(nxt)) {
                    visited.insert(nxt);
                    if (f(nxt, s2, visited)) return true;
                }
                //注意这里不需要 visited.erase() 来回溯
                //因为如果从nxt出发找不到解，那么从别的路出发走到nxt再调用f()也同样找不到解
            }
        }

        return false;
    }

    bool canConvert(string& str1, string& str2) {
        if (str1 == str2) return true;

        unordered_set<string> visited;
        visited.insert(str1);
        return f(str1, str2, visited);
    }
};



//题目四：
// 过河问题
// 一共n人出游，他们走到一条河的西岸，想要过河到东岸
// 每个人都有一个渡河时间ti，西岸有一条船，一次最多乘坐两人
// 如果船上有一个人，划到对岸的时间，等于这个人的渡河时间
// 如果船上有两个人，划到对岸的时间，等于两个人的渡河时间的最大值
// 返回最少要花费多少时间，才能使所有人都过河
// 测试链接 : https://www.luogu.com.cn/problem/P1809

//思路：
//定义dp[i]: 0~i 范围上的人过河所需的时间
//先按照时间从小到大排序，依次过河；
//如果剩一个人，让最小的回来接他；
//如果剩两个人，让最小的回来接，剩下的那两个人（即最大的两个人）过去，让次小的回来，最小的两个过去； 
//从这两个情况中选一个最短的情况

namespace test4 {
    #include <iostream>
    #include <vector>
    #include <climits>
    #include <algorithm>
    using namespace std;

    const int MAXN = 100005;
    int n;
    vector<int> dp(MAXN, INT_MAX / 2);
    vector<int> arr(MAXN, 0);

    int main() {
        cin >> n;
        for (int i = 0; i < n; i++) {
            cin >> arr[i];
        }

        sort(arr.begin(), arr.begin() + n);
        
        //初始化
        dp[0] = arr[0];
        dp[1] = arr[1];
        dp[2] = arr[0] + arr[1] + arr[2];

        for (int i = 3; i < n; i++) {
            dp[i] = min(arr[i] + arr[0] + dp[i - 1],
                arr[1] + arr[0] + arr[i] + arr[1] + dp[i - 2]);
        }

        cout << dp[n - 1] << endl;

        return 0;
    }
}



//题目五：
// 超级洗衣机
// 假设有n台超级洗衣机放在同一排上
// 开始的时候，每台洗衣机内可能有一定量的衣服，也可能是空的
// 在每一步操作中，你可以选择任意 m (1 <= m <= n) 台洗衣机
// 与此同时将每台洗衣机的一件衣服送到相邻的一台洗衣机
// 给定一个整数数组machines代表从左至右每台洗衣机中的衣物数量
// 请给出能让所有洗衣机中剩下的衣物的数量相等的最少的操作步数
// 如果不能使每台洗衣机中衣物的数量相等则返回-1
// 测试链接 : https://leetcode.cn/problems/super-washing-machines/

// 可以看作一个水坝放水，多少时间能填充满周围的洼地，答案就是只用看这个水坝的水流出去的时间，不用管洼地之间的水相互传递，因为当水坝的水不动的时候，水就一定是静止的了

//思路：
//先计算出平均值
//对于i号位置，计算出其左边与其右边的需求  (>0则需要衣服，<0则需要吐出衣服)，得出i号位置的瓶颈
//由于每一轮可以有多台洗衣机操作，因此最大瓶颈就是最终答案

class Solution {
public:
    int findMinMoves(vector<int>& machines) {
        int n = machines.size();
        int sum = 0;
        for (auto& ele : machines) sum += ele;

        if (sum % n != 0) return -1;

        int ans = 0;
        int average = sum / n;
        int lSum = 0, rSum = sum;
        for (int i = 0; i < n; i++) {
            rSum -= machines[i];
            int lneed = i * average - lSum;
            int rneed = (n - i - 1) * average - rSum;

            if (lneed > 0 && rneed > 0) {
                ans = max(ans, lneed + rneed);
            }
            else {
                ans = max(ans, max(abs(lneed), abs(rneed)));
            }

            lSum += machines[i];
        }

        return ans;
    }
};

