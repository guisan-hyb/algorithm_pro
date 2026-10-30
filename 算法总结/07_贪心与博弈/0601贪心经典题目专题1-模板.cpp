// 贪心专题1模板：最大数 / 两地调度 / 会议室 / 课程表 / 哈夫曼 / 吃橘子
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

// ---------- 1. 最大数（比较器：a+b > b+a） ----------
string largestNumber(vector<int> nums) {
    vector<string> arr;
    for (int x : nums) arr.push_back(to_string(x));
    sort(arr.begin(), arr.end(), [](const string& a, const string& b) { return a + b > b + a; });
    if (arr[0] == "0") return "0";
    string ans;
    for (auto& s : arr) ans += s;
    return ans;
}

// ---------- 2. 两地调度（排序差值） ----------
int twoCitySchedCost(vector<vector<int>>& costs) {
    int n = (int)costs.size(), sum = 0;
    vector<int> diff;
    for (auto& c : costs) { sum += c[0]; diff.push_back(c[1] - c[0]); }
    sort(diff.begin(), diff.end());
    for (int i = 0; i < n / 2; i++) sum += diff[i];
    return sum;
}

// ---------- 3. 会议室 II（开始时间排序 + 小根堆） ----------
int minMeetingRooms(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[0] < b[0]; });
    priority_queue<int, vector<int>, greater<>> heap;
    int ans = 0;
    for (auto& it : intervals) {
        while (!heap.empty() && it[0] >= heap.top()) heap.pop();
        heap.push(it[1]);
        ans = max(ans, (int)heap.size());
    }
    return ans;
}

// ---------- 4. 课程表 III（截止时间排序 + 大根堆替换） ----------
int scheduleCourse(vector<vector<int>> courses) {
    sort(courses.begin(), courses.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[1] < b[1]; });
    priority_queue<int> heap;
    int time = 0;
    for (auto& c : courses) {
        if (time + c[0] <= c[1]) { time += c[0]; heap.push(c[0]); }
        else if (!heap.empty() && heap.top() > c[0]) {
            time = time - heap.top() + c[0];
            heap.pop();
            heap.push(c[0]);
        }
    }
    return (int)heap.size();
}

// ---------- 5. 连接棒材的最低费用（哈夫曼） ----------
long long connectSticks(vector<int> sticks) {
    priority_queue<int, vector<int>, greater<>> heap(sticks.begin(), sticks.end());
    long long cost = 0;
    while (heap.size() > 1) {
        int a = heap.top(); heap.pop();
        int b = heap.top(); heap.pop();
        cost += a + b;
        heap.push(a + b);
    }
    return cost;
}

// ---------- 6. 吃掉 N 个橘子的最少天数（贪心 + 记忆化） ----------
unordered_map<int,int> memoEat;
int minDays(int n) {
    if (n <= 1) return n;
    if (memoEat.count(n)) return memoEat[n];
    return memoEat[n] = min(n % 2 + 1 + minDays(n / 2), n % 3 + 1 + minDays(n / 3));
}

int main() {
    std::cout << "最大数 = " << largestNumber({10, 2}) << "\n";        // 210
    vector<vector<int>> costs = {{10,20},{30,200},{400,50},{30,20}};
    std::cout << "两地调度 = " << twoCitySchedCost(costs) << "\n";              // 110
    std::cout << "最少会议室 = " << minMeetingRooms({{0,30},{5,10},{15,20}}) << "\n";  // 2
    std::cout << "最多课程 = " << scheduleCourse({{100,200},{200,1300},{1000,1250},{2000,3200}}) << "\n";  // 3
    std::cout << "连接棒材费用 = " << connectSticks({2,4,3}) << "\n";          // 14
    std::cout << "吃橘子天数 = " << minDays(10) << "\n";                        // 4
    return 0;
}
