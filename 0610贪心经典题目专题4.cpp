#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_map>
#include <queue>

using namespace std;


//题目一：
// 数组的最小偏移量
// 给你一个由n个正整数组成的数组nums
// 你可以对数组的任意元素执行任意次数的两类操作：
// 如果元素是偶数，除以2
// 例如如果数组是[1,2,3,4]
// 那么你可以对最后一个元素执行此操作，使其变成[1,2,3,2]
// 如果元素是奇数，乘上2
// 例如如果数组是[1,2,3,4]
// 那么你可以对第一个元素执行此操作，使其变成[2,2,3,4]
// 数组的偏移量是数组中任意两个元素之间的最大差值
// 返回数组在执行某些操作之后可以拥有的最小偏移量
// 测试链接 : https://leetcode.cn/problems/minimize-deviation-in-array/

//思路：
//先让所有的奇数获得一次乘2的机会
//从最大值讨论：如果最大值为偶数 -> 求差值，让现在最大值除以2；如果最大值为奇数 -> 停止

class Solution {
public:
    int minimumDeviation(vector<int>& nums) {
        set<int> cnt;
        for (auto& ele : nums) {
            if (ele % 2 == 0) cnt.insert(ele);
            else cnt.insert(ele * 2);
        }

        int ans = *cnt.rbegin() - *cnt.begin();
        while (ans > 0 && *cnt.rbegin() % 2 == 0) {
            int maxVal = *cnt.rbegin();
            cnt.erase(maxVal);
            cnt.insert(maxVal / 2);
            ans = min(ans, *cnt.rbegin() - *cnt.begin());
        }

        return ans;
    }
};



//题目二：
// 森林中的兔子
// 森林中有未知数量的兔子
// 你问兔子们一个问题: "还有多少只兔子与你颜色相同?"
// 你将答案收集到了一个数组answers中
// 你可能没有收集到所有兔子的回答，可能只是一部分兔子的回答
// 其中answers[i]是第i只兔子的答案
// 所有兔子都不会说错，返回森林中兔子的最少数量
// 测试链接 : https://leetcode.cn/problems/rabbits-in-forest/

//核心: 分组

//写法一：O(nlogn)
class Solution {
public:
    int numRabbits(vector<int>& answers) {
        sort(answers.begin(), answers.end());
        int n = answers.size();
        int ans = 0;
        for (int i = 0, j = 1; i < n; j++) {
            int cur = answers[i];
            while (j < n && answers[j] == cur) j++;
            ans += (j - i + cur) / (cur + 1) * (cur + 1);
            i = j;
        }
        return ans;
    }
};

//写法二：O(n)
class Solution {
public:
    int numRabbits(vector<int>& answers) {
        unordered_map<int, int> cnt;
        for (auto& ele : answers) {
            cnt[ele]++;
        }
        int ans = 0;
        for (auto& [val, freq] : cnt) {
            ans += (freq + val) / (val + 1) * (val + 1);
        }
        return ans;
    }
};



//题目三：
// 使数组相似的最少操作次数
// 给你两个正整数数组 nums 和 target ，两个数组长度相等
// 在一次操作中，你可以选择两个 不同 的下标 i 和 j
// 其中 0 <= i, j < nums.length ，并且：
// 令 nums[i] = nums[i] + 2 且
// 令 nums[j] = nums[j] - 2
// 如果两个数组中每个元素出现的频率相等，我们称两个数组是 相似 的
// 请你返回将 nums 变得与 target 相似的最少操作次数
// 测试数据保证nums一定能变得与target相似
// 测试链接 : https://leetcode.cn/problems/minimum-number-of-operations-to-make-arrays-similar/

//核心：
// 分离奇偶 -> 题中数值只±2，说明奇偶性不改变
// 每次进行操作产生的值的变化为4: 上升2,下降2  (会使整个数组的绝对差值总和精确地减少 4)
//附: 题目说测试数据保证这两个数组相似，说明两数组的奇偶个数一致

class Solution {
public:
    long long makeSimilar(vector<int>& nums, vector<int>& target) {
        int n = nums.size();
        //使数组奇偶分离并返回奇数个数
        auto f = [](vector<int>& arr) {
            int oddsize = 0;
            for (int i = 0; i < arr.size(); i++) {
                if ((arr[i] & 1) == 1) {
                    swap(arr[oddsize++], arr[i]);
                }
            }
            return oddsize;
        };

        int ret = f(nums);
        f(target);
        //sort -> [...)
        sort(nums.begin(), nums.begin() + ret);
        sort(nums.begin() + ret, nums.end());
        sort(target.begin(), target.begin() + ret);
        sort(target.begin() + ret, target.end());

        long long ans = 0;
        for (int i = 0; i < n; i++) {
            ans += abs(nums[i] - target[i]);
        }

        return ans / 4;
    }
};



//题目四：
// 知识竞赛
// 最近部门要选两个员工去参加一个需要合作的知识竞赛，
// 每个员工均有一个推理能力值ai，以及一个阅读能力值bi
// 如果选择第i个人和第j个人去参加竞赛，
// 两人在推理方面的能力为X = (ai + aj)/2
// 两人在阅读方面的能力为Y = (bi + bj)/2
// 现在需要最大化他们表现较差一方面的能力
// 即让min(X,Y) 尽可能大，问这个值最大是多少
// 测试链接 : https://www.nowcoder.com/practice/2a9089ea7e5b474fa8f688eae76bc050

//核心：按照 |能力a - 能力b| 从小到大排序
// 用maxA,maxB维护信息，枚举右，维护左

namespace test4 {
    #include <iostream>
    #include <vector>
    #include <algorithm>
    using namespace std;

    const int MAXN = 200005;
    vector<vector<int>> ability(MAXN, vector<int>(2, 0));
    int n;

    int f() {
        //数组固定大小为MAXN，所以不能全部都排序
        sort(ability.begin(), ability.begin() + n, [](vector<int>& a, vector<int>& b) {
            return abs(a[0] - a[1]) < abs(b[0] - b[1]);
        });

        int ret = 0;
        int maxA = 0, maxB = 0;
        for (int i = 0; i < n; i++) {
            if (ability[i][0] < ability[i][1]) {
                ret = max(ret, ability[i][0] + maxA);
            }
            else{
                ret = max(ret, ability[i][1] + maxB);
            }
            maxA = max(maxA, ability[i][0]);
            maxB = max(maxB, ability[i][1]);
        }

        return ret;
    }

    int main() {
        cin >> n;
        for (int i = 0; i < n; i++) {
            cin >> ability[i][0] >> ability[i][1];
        }

        double ans = (double)f();

        cout << ans / 2.0 << endl;

        return 0;
    }
}



//题目五：
// 将数组分成几个递增序列
// 给你一个有序的正数数组 nums 和整数 K
// 判断该数组是否可以被分成一个或几个 长度至少 为 K 的 不相交的递增子序列
// 数组中的所有数字，都要被，若干不相交的递增子序列包含
// 测试链接 : https://leetcode.cn/problems/divide-array-into-increasing-sequences/

//核心：因为划分成的数组单增，所以对于出现频次最高的数，要分别分派到不同的数组中

namespace test5 {
    #include<iostream>
    #include <vector>
    #include <utility>
    #include <cstdlib>
    #include <algorithm>
    using namespace std;

    //暴力（回溯）
    //我们需要在暴力搜索时维护每个子序列的状态（即当前子序列的最后一个元素是什么，以及当前子序列的长度是多少）
    //对于数组中的每一个元素，我们有两种选择：
    //加入某个已有的子序列：前提是该元素大于该子序列的最后一个元素。
    //新建一个子序列：将该元素作为新子序列的第一个元素。
    //当所有元素分配完毕时，检查所有子序列的长度是否都大于等于 K
    class Solution1 {
    public:
        bool f(vector<int>& nums, int k, int i, vector<pair<int,int>>& subs) {
            if (i == nums.size()) {
                for (auto& [endNum, cnt] : subs) {
                    if (cnt < k) return false;
                }
                return true;
            }

            //1.当前位置的数加入到已有的子序列
            for (int idx = 0; idx < subs.size(); idx++) {
                int curEndNum = subs[idx].first;
                int curCnt = subs[idx].second;

                if (nums[i] <= curEndNum) continue;

                subs[idx].first = nums[i];
                subs[idx].second = curCnt + 1;

                if (f(nums, k, i + 1, subs)) return true;

                //回溯
                subs[idx].first = curEndNum;
                subs[idx].second = curCnt;
            }

            //2.当前位置的数直接新起一个子序列
            subs.push_back({ nums[i],1 });
            if (f(nums, k, i + 1, subs)) return  true;
            subs.pop_back();

            return false;
        }

        bool canDivideIntoSubsequences(vector<int>& nums, int k) {
            int n = nums.size();
            sort(nums.begin(), nums.end());
            vector<pair<int, int>> subs;//记录路径信息：每个子序列的状态
            //因为子序列确保单增，所以只需记录末位元素和数组大小
            return f(nums, k, 0, subs);//回溯
        }
    };

    //贪心
    class Solution2 {
    public:
        bool canDivideIntoSubsequences(vector<int>& nums, int k) {
            sort(nums.begin(), nums.end());
            int n = nums.size();
            int maxcnt = 1, curcnt = 1;
            for (int i = 1; i < n; i++) {
                if (nums[i] != nums[i - 1]) {
                    maxcnt = max(maxcnt, curcnt);
                    curcnt = 1;
                }
                else {
                    curcnt++;
                }
            }
            maxcnt = max(maxcnt, curcnt);//别忘了最后一次

            int numOfOneGroup = n / maxcnt;
            // 向下取整如果满足 >= k
            // 那么所有的递增子序列长度一定 >= k
            return numOfOneGroup >= k;
        }
    };

    vector<int> randomArray(int n, int v) {
        vector<int> ret(n, 0);
        for (int i = 0; i < n; i++) {
            ret[i] = rand() % v + 1;
        }
        return ret;
    }

    int main() {
        srand((unsigned int)time(nullptr));

        int N = 8;
        int V = 100;
        int testTimes = 2000;
        cout << "测试开始" << endl;
        for (int i = 1; i <= testTimes; i++) {
            int n = rand() % N + 1;
            int k = rand() % n + 1;
            vector<int> nums = randomArray(n, V);
            Solution1 s1;
            Solution2 s2;
            int ans1 = s1.canDivideIntoSubsequences(nums, k);
            int ans2 = s2.canDivideIntoSubsequences(nums, k);

            if (ans1 != ans2) {
                cout << "出错了" << endl;
            }

            if (i % 100 == 0) {
                cout << "当前测试到第 " << i << " 组" << endl;
            }
        }
        cout << "测试结束" << endl;

        return 0;
    }
}

//【经验】：
//在遍历 subs 容器时使用了范围 for 循环（for (auto& ele : subs)），同时递归调用内部可能会对 subs 执行 push_back（新起子序列分支）。
//当 push_back 导致 vector 重新分配内存时，之前获得的引用 ele 会失效，后续对 ele 的访问（包括回溯时的恢复操作）将导致未定义行为，
//从而可能产生错误的结果。

//修复方法：将范围 for 循环改为基于下标的循环，每次通过下标重新获取元素引用，避免引用失效。



//题目六：
// 最低加油次数
// 汽车从起点出发驶向目的地，该目的地位于出发位置东面target英里处
// 沿途有加油站，用数组stations表示，其中 stations[i] = [positioni, fueli]
// 表示第i个加油站位于出发位置东面positioni英里处，并且有fueli升汽油
// 假设汽车油箱的容量是无限的，其中最初有startFuel升燃料
// 它每行驶1英里就会用掉1升汽油
// 当汽车到达加油站时，它可能停下来加油，将所有汽油从加油站转移到汽车中
// 为了到达目的地，汽车所必要的最低加油次数是多少？
// 如果无法到达目的地，则返回-1
// 注意：如果汽车到达加油站时剩余燃料为0，它仍然可以在那里加油
// 如果汽车到达目的地时剩余燃料为 0，仍然认为它已经到达目的地
// 测试链接 : https://leetcode.cn/problems/minimum-number-of-refueling-stops/

//法一：动态规划
// dp[i][j]: 0~i个加油站、恰好加了j次油能跑的最远距离
class Solution {
public:
    int f(vector<vector<int>>& stations, int startFuel, int i, int cnt, vector<vector<int>>& dp) {
        if (i < 0) return cnt == 0 ? startFuel : 0;
        if (dp[i][cnt] != -1) return dp[i][cnt];

        int ans = 0;
        //1.不加油
        int p1 = f(stations, startFuel, i - 1, cnt, dp);
        ans = max(ans, p1);

        //2.加油
        if (cnt > 0) {
            int p2 = f(stations, startFuel, i - 1, cnt - 1, dp);
            if (p2 >= stations[i][0]) {
                ans = max(ans, p2 + stations[i][1]);
            }
        }

        dp[i][cnt] = ans;
        return ans;
    }

    int minRefuelStops(int target, int startFuel, vector<vector<int>>& stations) {
        if (startFuel >= target) return 0;

        int n = stations.size();
        if (n == 0) return -1;

        vector<vector<int>> dp(n, vector<int>(n + 1, -1));
    
        for (int j = 0; j <= n; j++) {
            if (f(stations, startFuel, n - 1, j, dp) >= target) {
                return j;
            }
        }

        return -1;
    }
};


//法二：贪心
//思路：
//由于油箱的容量是无限的，所以：
//路上的不是加油站，而是一桶桶的油，每次经过的时候，就把油带上，
//当油不够的时候我们就取身上最大的那桶油加上，这样如果身上没油了，那么就到不了了

class Solution {
public:
    int minRefuelStops(int target, int startFuel, vector<vector<int>>& stations) {
        if (startFuel >= target) return 0;

        priority_queue<int> heap;//存储路过的加油站的油
        int cur = startFuel;
        int cnt = 0;

        for (int i = 0; i < stations.size(); i++) {
            if (cur < stations[i][0]) {
                while (!heap.empty() && cur < stations[i][0]) {
                    int tmpFuel = heap.top();
                    heap.pop();

                    cur += tmpFuel;
                    cnt++;
                    if (cur >= target) return cnt;
                }

                if (cur < stations[i][0]) return -1;
            }
            heap.push(stations[i][1]);
        }

        //走到这说明加油站全都去过一遍了
        while (!heap.empty()) {
            int tmpFuel = heap.top();
            heap.pop();

            cur += tmpFuel;
            cnt++;
            if (cur >= target) return cnt;
        }

        return -1;
    }
};

