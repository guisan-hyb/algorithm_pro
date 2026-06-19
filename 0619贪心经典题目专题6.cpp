#define _CRT_SECURE_NO_WARNINGS


#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <queue>
#include <limits>

using namespace std;



//题目一：
// 消灭怪物的最大数量
// 你正在玩一款电子游戏，在游戏中你需要保护城市免受怪物侵袭
// 给定两个大小为n的整数数组dist、speed
// 其中dist[i]是第i个怪物与城市的初始距离
// 其中speed[i]是第i个怪物的速度
// 你有一种武器，一旦充满电，就可以消灭一个怪物，但是，武器需要1的时间才能充电完成
// 武器在游戏开始时是充满电的状态，怪物从0时刻开始移动，一旦任何怪物到达城市，就输掉了这场游戏
// 如果某个怪物恰好在某一分钟开始时到达城市，这也会被视为输掉游戏
// 返回在你输掉游戏前可以消灭的怪物的最大数量，如果消灭所有怪兽了返回n
// 测试链接 : https://leetcode.cn/problems/eliminate-maximum-number-of-monsters/

//思路：谁最快到达城市就消灭谁

class Solution {
public:
    int eliminateMaximum(vector<int>& dist, vector<int>& speed) {
        int n = dist.size();
        vector<int> time(n, 0);
        for (int i = 0; i < n; i++) {
            time[i] = (dist[i] + speed[i] - 1) / speed[i];//向上取整
        }

        sort(time.begin(), time.end());

        for (int i = 0; i < n; i++) {
            if (time[i] <= i) {
                return i;
            }
        }

        return n;
    }
};



//题目二：
// 最大回文数字
// 给你一个仅由数字（0 - 9）组成的字符串num
// 请你找出能够使用num中数字形成的最大回文整数
// 并以字符串形式返回，该整数不含前导零
// 你无需使用num中的所有数字，但你必须使用至少一个数字，数字可以重新排列
// 测试链接 : https://leetcode.cn/problems/largest-palindromic-number/
class Solution {
public:
    string largestPalindromic(string num) {
        unordered_map<char, int> cnt;
        for (auto& ch : num) {
            cnt[ch]++;
        }

        string ans;
        char midCh = 0;
        for (char ch = '9'; ch >= '1'; ch--) {
            if (midCh == 0 && (cnt[ch] & 1) == 1) midCh = ch;
            for (int i = cnt[ch] / 2; i > 0; i--) {
                ans += ch;
            }
        }

        if (ans.empty() && midCh == 0) return "0";
        if (ans.empty() && midCh != 0) return string{ midCh };

        for (int i = cnt['0'] / 2; i > 0; i--) {
            ans += '0';
        }

        if (midCh == 0 && (cnt['0'] & 1) == 1) ans += '0';
        if (midCh != 0) ans += midCh;

        int curlen = ans.size();
        if ((midCh == 0 && (cnt['0'] & 1) == 1) || (midCh != 0)) {
            for (int i = curlen - 2; i >= 0; i--) {
                ans += ans[i];
            }
        }
        else {
            for (int i = curlen - 1; i >= 0; i--) {
                ans += ans[i];
            }
        }

        return ans;
    }
};



//题目三：
// 最大平均通过率
// 一所学校里有一些班级，每个班级里有一些学生，现在每个班都会进行一场期末考试
// 给你一个二维数组classes，其中classes[i]=[passi, totali]
// 表示你提前知道了第i个班级总共有totali个学生
// 其中只有 passi 个学生可以通过考试
// 给你一个整数extraStudents，表示额外有extraStudents个聪明的学生，一定能通过期末考
// 你需要给这extraStudents个学生每人都安排一个班级，使得所有班级的平均通过率最大
// 一个班级的 通过率 等于这个班级通过考试的学生人数除以这个班级的总人数
// 平均通过率 是所有班级的通过率之和除以班级数目
// 请你返回在安排这extraStudents个学生去对应班级后的最大平均通过率
// 测试链接 : https://leetcode.cn/problems/maximum-average-pass-ratio/

//思路：加入哪个班的收益最大，哪个班排前面

class Solution {
public:
    double maxAverageRatio(vector<vector<int>>& classes, int extraStudents) {
        auto cmp = [](const vector<double>& a, const vector<double>& b){
            double da = (a[0] + 1) / (a[1] + 1) - a[0] / a[1];
            double db = (b[0] + 1) / (b[1] + 1) - b[0] / b[1];
            return da < db;
        };

        priority_queue<vector<double>, vector<vector<double>>, decltype(cmp)> heap(cmp);
        for (auto& ele : classes) {
            heap.push({ (double)ele[0],(double)ele[1] });
        }

        while (extraStudents--) {
            auto cur = heap.top();
            heap.pop();
            cur[0] += 1, cur[1] += 1;
            heap.push({ cur[0],cur[1] });
        }

        double ans = 0;
        while (!heap.empty()) {
            auto cur = heap.top();
            heap.pop();

            ans += cur[0] * 1.0 / cur[1];
        }

        return ans / classes.size();
    }
};



//题目四：
// 雇佣K名工人的最低成本
// 有n名工人，给定两个数组quality和wage
// 其中quality[i]表示第i名工人的工作质量，其最低期望工资为wage[i]
// 现在我们想雇佣k名工人组成一个工资组
// 在雇佣一组k名工人时，我们必须按照下述规则向他们支付工资：
// 对工资组中的每名工人，应当按其工作质量与同组其他工人的工作质量的比例来支付工资
// 工资组中的每名工人至少应当得到他们的最低期望工资
// 给定整数k，返回组成满足上述条件的付费群体所需的最小金额
// 测试链接 : https://leetcode.cn/problems/minimum-cost-to-hire-k-workers/

//思路：
//通过观察发现满足两个规则的结论是：用一个比率a(性价比)记录：wage/quality，则a的最大值再乘以总质量即能完成两个规则
//所以要选择总质量最小的k个人


class Solution {
public:
    struct Node {
        int _quality;
        double _ratio;

        Node(int q = 0,double r = 0): _quality(q),_ratio(r) {}
    };

    struct cmp {
        bool operator()(const Node& a, const Node& b) const {
            return a._quality < b._quality;
        }
    };

    double mincostToHireWorkers(vector<int>& quality, vector<int>& wage, int k) {
        int n = quality.size();
        vector<Node> arr(n);
        for (int i = 0; i < n; i++) {
            arr[i] = { quality[i],(double)wage[i] / quality[i] };
        }

        // 根据比例排序，比例小的在前，比例大的在后
        sort(arr.begin(), arr.end(), [](const Node a, const Node b) {
            return a._ratio < b._ratio;
        });

        double ans = std::numeric_limits<double>::max();
        priority_queue <Node, vector<Node>, cmp> heap;
        int total_sum = 0;
        for (int i = 0; i < n; i++) {
            heap.push(arr[i]);
            total_sum += arr[i]._quality;

            if (heap.size() > k) {
                total_sum -= heap.top()._quality;
                heap.pop();
            }

            if (heap.size() == k) {
                ans = min(ans, total_sum * arr[i]._ratio);
            }
        }

        return ans;
    }
};



//题目五：
// 砍树
// 一共有n棵树，每棵树都有两个信息：
// 第一天这棵树的初始重量、这棵树每天的增长重量
// 你每天最多能砍1棵树，砍下这棵树的收益为：
// 这棵树的初始重量 + 这棵树增长到这一天的总增重
// 从第1天开始，你一共有m天可以砍树，返回m天内你获得的最大收益
// 测试链接 : https://pintia.cn/problem-sets/91827364500/exam/problems/type/7?problemSetProblemId=91827367873

//思路：贪心+动态规划
//dp[i][j]: 0~i位置上，还剩j天获得的最大收益
//1.不要i位置：dp[i-1][j]
//2.要i位置：dp[i-1][j-1]+[i]初始+[i]增长*(j-1)

namespace test5 {
    class Solution1 {
    public:
        int f(vector<vector<int>>& tree, int i, int day, vector<vector<int>>& dp) {
            if (i < 0 || day <= 0) return 0;
            if (dp[i][day] != -1) return dp[i][day];

            int ans = 0;
            int p1 = f(tree, i - 1, day, dp);
            int p2 = f(tree, i - 1, day - 1, dp) + tree[i][0] + tree[i][1] * (day - 1);
            ans = max(p1, p2);
            dp[i][day] = ans;

            return ans;
        }

        int compute(vector<vector<int>>& tree, int day) {
            sort(tree.begin(), tree.end(), [](const vector<int>& a, const vector<int>& b) {
                return a[1] < b[1];//按照增长率升序排序
            });

            int n = tree.size();
            vector<vector<int>> dp(n, vector<int>(day + 1, -1));
            return f(tree, n - 1, day, dp);
        }
    };

    class Solution2 {
    public:
        int f(vector<vector<int>>& tree, int day, int i, vector<int>& visited) {
            if (i == day) return 0;

            int ans = f(tree, day, i + 1, visited);//不砍树
            for (int j = 0; j < tree.size(); j++) {
                if (!visited[j]) {
                    visited[j] = true;
                    ans = max(ans, f(tree, day, i + 1, visited) + tree[j][0] + tree[j][1] * i);
                    visited[j] = false;
                }
            }

            return ans;
        }

        int compute(vector<vector<int>>& tree, int day) {
            int n = tree.size();
            vector<int> visited(n, 0);
            return f(tree, day, 0, visited);
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
        
        int N = 8;
        int V = 1000;
        int testTimes = 2000;
        cout << "测试开始" << endl;
        for (int i = 1; i <= testTimes; i++) {
            int n = rand() % N + 1;
            int day = rand() % N + 1;
            vector<vector<int>> tree = randomArray(n, V);
            Solution1 s1;
            Solution2 s2;
            int ans1 = s1.compute(tree, day);
            int ans2 = s2.compute(tree, day);
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

