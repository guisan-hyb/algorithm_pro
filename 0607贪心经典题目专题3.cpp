#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;



//题目一：
// 最短无序连续子数组
// 给你一个整数数组nums，你需要找出一个 连续子数组
// 如果对这个子数组进行升序排序，那么整个数组都会变为升序排序
// 请你找出符合题意的最短子数组，并输出它的长度
// 测试链接 : https://leetcode.cn/problems/shortest-unsorted-continuous-subarray/

//法一：朴素做法
//先排序，然后lr左右双指针，如果排序数组和原数组相等就l++或者r++，最后l和r的差就是子数组最短长度
class Solution {
public:
    int findUnsortedSubarray(vector<int>& nums) {
        int n = nums.size();
        vector<int> arr(nums);
        sort(arr.begin(), arr.end());
        int l = 0, r = n - 1;
        for (int i = 0; i < n; i++) {
            if (l > r) break;
            if (arr[l] == nums[l]) l++;
            if (arr[r] == nums[r]) r--;
        }
        int ans = r - l + 1;
        return ans >= 0 ? ans : 0;
    }
};

//法二：O(n)做法
class Solution {
public:
    int findUnsortedSubarray(vector<int>& nums) {
        int n = nums.size();
        int l = n, r = -1;
        
        int mins = INT_MAX, maxs = INT_MIN;
        // max > 当前数，认为不达标
        // 从左往右遍历，记录最右不达标的位置
        for (int i = 0; i < n; i++) {
            if (maxs > nums[i]) {
                r = i;
            }
            maxs = max(maxs, nums[i]);
        }

        // min < 当前数，认为不达标
        // 从右往左遍历，记录最左不达标的位置
        for (int i = n - 1; i >= 0; i--) {
            if (mins < nums[i]) {
                l = i;
            }
            mins = min(mins, nums[i]);
        }

        return max(0, r - l + 1);
    }
};



//题目二：
// 最小区间
// 你有k个非递减排列的整数列表
// 找到一个最小区间，使得k个列表中的每个列表至少有一个数包含在其中
// 测试链接 : https://leetcode.cn/problems/smallest-range-covering-elements-from-k-lists/

//核心思路
//把「每个列表至少有一个数包含在其中」的区间叫做合法区间。
//
//先求出最左边的合法区间，然后求出第二个合法区间，第三个合法区间，依此类推。
//
//比如示例 1，最左边的合法区间是[0, 5]。
//
//枚举所有合法区间的左端点，或者枚举所有合法区间的右端点。其中第一个最短的合法区间就是答案。

class Solution {
public:
    struct Node {
        int v;//值
        int i;//当前值所在数组
        int j;//当前值所在数组的什么位置

        Node(int a, int b, int c) :v(a), i(b), j(c) {}
    };

    struct cmp {
        bool operator()(const Node& a, const Node& b) const {
            if (a.v != b.v) return a.v < b.v;
            if (a.i != b.i) return a.i < b.i;
            return a.j < b.j;
        }
    };

    vector<int> smallestRange(vector<vector<int>>& nums) {
        int k = nums.size();
        set<Node, cmp> cnt;
        for (int i = 0; i < k; i++) {
            cnt.insert({ nums[i][0],i,0 });
        }

        int r = INT_MAX;
        int ans_l = 0, ans_r = 0;
        while (cnt.size() == k) {
            Node mins = *cnt.begin();
            Node maxs = *cnt.rbegin();
            cnt.erase(cnt.begin());

            if (maxs.v - mins.v < r) {
                r = maxs.v - mins.v;
                ans_l = mins.v;
                ans_r = maxs.v;
            }

            if (mins.j + 1 < nums[mins.i].size()) {
                cnt.insert({ nums[mins.i][mins.j + 1],mins.i,mins.j + 1 });
            }
        }

        return { ans_l,ans_r };
    }
};



//题目三：
// 组团买票
// 景区里一共有m个项目，景区的第i个项目有如下两个参数：
// game[i] = { Ki, Bi }，Ki、Bi一定是正数
// Ki代表折扣系数，Bi代表票价
// 举个例子 : Ki = 2, Bi = 10
// 如果只有1个人买票，单张门票的价格为 : Bi - Ki * 1 = 8
// 所以这1个人游玩该项目要花8元
// 如果有2个人买票，单张门票的价格为 : Bi - Ki * 2 = 6
// 所以这2个人游玩该项目要花6 * 2 = 12元
// 如果有5个人买票，单张门票的价格为 : Bi - Ki * 5 = 0
// 所以这5个人游玩该项目要花5 * 0 = 0元
// 如果有更多人买票，都认为花0元(因为让项目倒贴钱实在是太操蛋了)
// 于是可以认为，如果有x个人买票，单张门票的价格为 : Bi - Ki * x
// x个人游玩这个项目的总花费是 : max { x * (Bi - Ki * x), 0 }
// 单位一共有n个人，每个人最多可以选1个项目来游玩，也可以不选任何项目
// 所有员工将在明晚提交选择，然后由你去按照上面的规则，统一花钱购票
// 你想知道自己需要准备多少钱，就可以应付所有可能的情况，返回这个最保险的钱数
// 数据量描述 : 
// 1 <= M、N、Ki、Bi <= 10^5
// 测试链接 : https://www.luogu.com.cn/problem/P12331

//这个题也就是说 怎么安排会有最大盈利

//假设 B = 10, K = 2
//如果以报团视角看：
// 1个人：8元
// 2个人：12元
// 3个人：12元
// 4个人：8元
// 5个人：0元
// 
//以个人角度看：
//第1个人付款 8元
//第2个人付款 4元
//第3个人付款 0元
//第4个人付款 -4元
//第5个人付款 -8元
//
//也就是说，第i个人应付款 Bi-(p+1)*ki-p*ki  p代表之前的总人数
// (p+1)*ki 表示当前一共减免的费用
// p*ki 表示因为新来的人导致票价下降，从而补偿的费用

namespace test3 {
    #include <iostream>
    #include <vector>
    #include <set>
    #include <queue>
    #include <algorithm>
    #include <cstdio>

    using namespace std;

    //暴力
    class Solution1 {
    public:
        int f(vector<vector<int>>& games, int n, int i, vector<int>& cnt) {
            if (i == n) {
                int ans = 0;
                for (int j = 0; j < games.size(); j++) {
                    ans += max(0, games[j][1] - games[j][0] * cnt[j]) * cnt[j];
                }
                return ans;
            }
            else {
                int ans = f(games, n, i + 1, cnt);//当前这个人不参与任何项目
                for (int j = 0; j < games.size(); j++) {
                    cnt[j]++;
                    ans = max(ans, f(games, n, i + 1, cnt));
                    cnt[j]--;
                }
                return ans;
            }
        }

        int maxProfit(vector<vector<int>> games, int n) {
            int m = games.size();
            vector<int> cnt(m, 0);//不同项目的人数
            return f(games, n, 0, cnt);
        }
    };

    class Solution2 {
    public:
        struct Info
        {
            int bi;
            int ki;
            int people;

            Info(int b,int k,int p): bi(b),ki(k),people(p) {}

            // bi - (people + 1) * ki : 当前的人，门票原价减少了，当前的门票价格
            // people * ki : 当前人的到来，之前的所有人，门票价格都再减去ki
            int singleProfit() const {
                return max(0, bi - (people + 1) * ki - people * ki);
            }
        };

        struct cmp
        {
            bool operator()(const Info& a, const Info& b) const {
                return a.singleProfit() < b.singleProfit();
            }
        };

        int maxProfit(vector<vector<int>>& games, int n) {
            int m = games.size();
            priority_queue<Info, vector<Info>, cmp> heap;
            for (int i = 0; i < m; i++) {
                heap.push({ games[i][1],games[i][0],0 });
            }

            int ans = 0;
            for (int i = 0; i < n; i++) {
                if (heap.top().singleProfit() <= 0) break;

                Info tmp = heap.top();
                heap.pop();
                ans += tmp.singleProfit();
                tmp.people++;
                heap.push(tmp);
            }

            return ans;
        }
    };

    vector<vector<int>> randomGames(int m, int v) {
        vector<vector<int>> ret(m, vector<int>(2, 0));
        for (int i = 0; i < m; i++) {
            ret[i][0] = rand() % v + 1;
            ret[i][1] = rand() % v + 1;
        }
        return ret;
    }

    int main() {
        srand((unsigned int)time(nullptr));

        int N = 8;
        int M = 8;
        int V = 20;
        int testTimes = 2000;
        cout << "测试开始" << endl;
        for (int i = 1; i <= testTimes; i++) {
            int n = rand() % N + 1;
            int m = rand() % M + 1;
            vector<vector<int>> games = randomGames(m, V);
            Solution1 s1;
            Solution2 s2;
            int ans1 = s1.maxProfit(games, n);
            int ans2 = s2.maxProfit(games, n);
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

namespace test_lg {
    #include <iostream>
    #include <queue>
    using namespace std;

    struct Info
    {
        int bi;
        int ki;
        int people;

        Info(int b,int k,int p): bi(b),ki(k),people(p) {}

        int singleProfit() const {
            return max(0, bi - ki * (people + 1) - ki * people);
        }
    };

    struct cmp
    {
        bool operator()(const Info& a, const Info& b) {
            return a.singleProfit() < b.singleProfit();
        }
    };

    int main(){
        int N, M;
        cin >> N >> M;
        priority_queue<Info, vector<Info>, cmp> heap;
        for (int i = 0; i < M; i++) {
            int k, b;
            cin >> k >> b;
            heap.push({ b,abs(k),0 });
        }

        long long ans = 0;
        for (int i = 0; i < N; i++) {
            if (heap.top().singleProfit() <= 0) break;

            Info cur = heap.top();
            heap.pop();
            ans += cur.singleProfit();
            cur.people++;
            heap.push(cur);
        }

        cout << ans << endl;

        return 0;
    }
}



//题目四：
// 平均值最小累加和
// 给定一个数组arr，长度为n
// 再给定一个数字k，表示一定要将arr划分成k个集合
// 每个数字只能进一个集合
// 返回每个集合的平均值都累加起来的最小值
// 平均值向下取整
// 1 <= n <= 10^5
// 0 <= arr[i] <= 10^5
// 1 <= k <= n
// 来自真实大厂笔试，没有在线测试，对数器验证

//贪心思想：让最小的k-1个数，每个数独自成一个集合

namespace test4 {
    //暴力
    class Solution1 {
    public:
        int f(vector<int>& arr, int i, vector<int>& sum, vector<int>& cnt) {
            if (i == arr.size()) {
                int ans = 0;
                for (int j = 0; j < sum.size(); j++) {
                    if (cnt[j] == 0) return INT_MAX;
                    ans += sum[j] / cnt[j];
                }
                return ans;
            }
            else {
                int ans = INT_MAX;
                for (int j = 0; j < sum.size(); j++) {
                    sum[j] += arr[i];
                    cnt[j]++;
                    ans = min(ans, f(arr, i + 1, sum, cnt));
                    sum[j] -= arr[i];
                    cnt[j]--;
                }
                return ans;
            }
        }

        int minAverageSum(vector<int>& arr, int k) {
            vector<int> sum(k, 0);
            vector<int> cnt(k, 0);
            return f(arr, 0, sum, cnt);
        }
    };

    class Solution2 {
    public:
        int minAverageSum(vector<int>& arr, int k) {
            sort(arr.begin(), arr.end());
            int ans = 0;
            for (int i = 0; i <= k - 2; i++) {
                ans += arr[i];
            }

            int sum = 0;
            for (int i = k - 1; i < arr.size(); i++) {
                sum += arr[i];
            }
            ans += sum / ((int)arr.size() - k + 1);

            return ans;
        }
    };

    vector<int> randomArray(int n, int v) {
        vector<int> ret(n);
        for (int i = 0; i < n; i++) {
            ret[i] = rand() % v;
        }
        return ret;
    }

    int main() {
        srand((unsigned int)time(nullptr));

        int N = 8;
        int V = 10000;
        int testTimes = 2000;
        cout << "测试开始" << endl;
        for (int i = 1; i <= testTimes; i++) {
            int n = rand() % N + 1;
            vector<int> arr = randomArray(n, V);
            int k = rand() % n + 1;
            Solution1 s1;
            Solution2 s2;
            int ans1 = s1.minAverageSum(arr, k);
            int ans2 = s2.minAverageSum(arr, k);
            if (ans1 != ans2) {
                cout << "出错了" << endl;
            }

            if (i % 100 == 0) {
                cout << "测试到第 " << i << " 组" << endl;
            }
        }
        cout << "测试结束" << endl;

        return 0;
    }
}



//题目五：
// 执行所有任务的最少初始电量
// 每一个任务有两个参数，需要耗费的电量、至少多少电量才能开始这个任务
// 返回手机至少需要多少的初始电量，才能执行完所有的任务
// 测试链接 : https://leetcode.cn/problems/minimum-initial-energy-to-finish-tasks/

//考虑两个任务<a0, a1> <b0, b1>, 应该先做哪个 ? 设T为当前能量.
//先做a, 要求 T - a0 >= b1, T >= a0 + b1
//先做b, 要求 T - b0 >= a1, T >= a1 + b0
//假设先做a更好, 就有a0 + b1 < a1 + b0, 即 a0 - a1 < b0 - b1.
//也就是说哪个任务应该先做, 完全依赖于a0 - a1
//所以按a0 - a1 排序, 依次执行就是最优方案.

namespace test5 {
    class Solution1 {
    public:
        int minimumEffort(vector<vector<int>>& tasks) {
            // 消耗电量 - 至少电量，越大的任务，越先倒推
            sort(tasks.begin(), tasks.end(), [](vector<int>& a, vector<int>& b) {
                return a[0] - a[1] > b[0] - b[1];
                });
            int ans = 0;
            for (auto& ele : tasks) {
                ans = max(ans + ele[0], ele[1]);
            }

            return ans;
        }
    };

    //暴力
    class Solution2 {
    public:
        int f(vector<vector<int>>& tasks, int i) {
            if (i == tasks.size()) {
                int ans = 0;
                for (auto& ele : tasks) {
                    ans = max(ans + ele[0], ele[1]);
                }
                return ans;
            }
            else {
                int ans = INT_MAX;
                for (int j = i; j < tasks.size(); j++) {
                    swap(tasks[i], tasks[j]);
                    ans = min(ans, f(tasks, i + 1));
                    swap(tasks[i], tasks[j]);
                }
                return ans;
            }
        }

        int minimumEffort(vector<vector<int>>& tasks) {
            return f(tasks, 0);
        }
    };
    
    vector<vector<int>> randomArray(int n, int v) {
        vector<vector<int>> ret(n, vector<int>(2, 0));
        for (int i = 0; i < n; i++) {
            ret[i][0] = rand() % v + 1;
            ret[i][1] = rand() % v + 1;
        }
        return ret;
    }

    int main() {
        srand((unsigned int)time(nullptr));

        int N = 10;
        int V = 20;
        int testTimes = 2000;
        cout << "测试开始" << endl;
        for (int i = 1; i <= testTimes; i++) {
            int n = rand() % N + 1;
            vector<vector<int>> tasks = randomArray(n, V);
            Solution1 s1;
            Solution2 s2;
            int ans1 = s1.minimumEffort(tasks);
            int ans2 = s2.minimumEffort(tasks);
            if (ans1 != ans2) {
                cout << "出错了" << endl;
            }

            if (i % 100 == 0) {
                cout << "测试到第 " << i << " 组" << endl;
            }
        }
        cout << "测试结束" << endl;

        return 0;
    }
}



//题目六：
// 两个0和1数量相等区间的最大长度
// 给出一个长度为n的01串，现在请你找到两个区间
// 使得这两个区间中，1的个数相等，0的个数也相等
// 这两个区间可以相交，但是不可以完全重叠，即两个区间的左右端点不可以完全一样
// 现在请你找到两个最长的区间，满足以上要求
// 返回区间最大长度
// 来自真实大厂笔试，没有在线测试，对数器验证

//找最两边第一个相同的字符，由于中间的部分共用，所以01长度必定相等

namespace test6 {
    //暴力
    class Solution1 {
    public:
        int len(vector<int>& arr) {
            unordered_map<int, unordered_map<int, int>> cnt;
            int n = arr.size();
            for (int i = 0; i < n; i++) {
                int zeros = 0, ones = 0;
                for (int j = i; j < n; j++) {
                    zeros += arr[j] == 0 ? 1 : 0;
                    ones += arr[j] == 1 ? 1 : 0;
                    cnt[zeros][ones]++;
                }
            }

            int ans = 0;
            for (auto& [zeros, inner] : cnt) {
                for (auto& [ones, nums] : inner) {
                    if (nums > 1) {
                        ans = max(ans, zeros + ones);
                    }
                }
            }
            return ans;
        }
    };

    class Solution2 {
    public:
        int len(vector<int>& arr) {
            int n = arr.size();
            int leftzero = -1;
            int leftone = -1;
            int rightzero = -1;
            int rightone = -1;

            for (int i = 0; i < n; i++) {
                if (arr[i] == 0) {
                    leftzero = i;
                    break;
                }
            }

            for (int i = 0; i < n; i++) {
                if (arr[i] == 1) {
                    leftone = i;
                    break;
                }
            }

            for (int i = n - 1; i >= 0; i--) {
                if (arr[i] == 0) {
                    rightzero = i;
                    break;
                }
            }

            for (int i = n - 1; i >= 0; i--) {
                if (arr[i] == 1) {
                    rightone = i;
                    break;
                }
            }

            int p1 = rightzero - leftzero;
            int p2 = rightone - leftone;
            return max(p1, p2);
        }
    };

    vector<int> randomArray(int n) {
        vector<int> ret(n);
        for (int i = 0; i < n; i++) {
            ret[i] = rand() % 2;
        }
        return ret;
    }

    int main() {
        srand((unsigned int)time(nullptr));

        int N = 500;
        int testTimes = 2000;
        cout << "测试开始" << endl;
        for (int i = 1; i <= testTimes; i++) {
            int n = rand() % N + 2;
            vector<int> arr = randomArray(n);
            Solution1 s1;
            Solution2 s2;
            int ans1 = s1.len(arr);
            int ans2 = s2.len(arr);
            if (ans1 != ans2) {
                cout << "出错了" << endl;
            }

            if (i % 100 == 0) {
                cout << "测试到第 " << i << " 组" << endl;
            }
        }
        cout << "测试结束" << endl;

        return 0;
    }
}

