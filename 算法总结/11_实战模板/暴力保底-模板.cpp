#include <algorithm>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <unordered_set>
#include <vector>
// 暴力保底模板（CSP T4/T5、笔试"先拿部分分"）
// 原则：先写出一定能过的暴力，再想优化；永远不要交白卷。
//
// 常用暴力的复杂度与适用数据量：
// 子集枚举 O(2^n)          n <= 20
// 全排列   O(n!)           n <= 9 ~ 10
// DFS 回溯 O(分支^深度)    看剪枝
// O(n^2) 模拟              n <= 5000
// O(n^3) 枚举              n <= 300 ~ 500
using namespace std;

// ---------- 1. 子集枚举：n <= 20 ----------
template <typename Check>
long long enumerateSubsets(const vector<int>& values, Check check) {
    int n = static_cast<int>(values.size());
    long long answer = LLONG_MIN;
    for (int mask = 0; mask < (1 << n); mask++) {
        long long sum = 0;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                sum += values[i];
            }
        }
        if (check(sum)) {
            answer = max(answer, sum);
        }
    }
    return answer;
}

// ---------- 2. DFS 子集（带剪枝） ----------
void dfsSubset(const vector<int>& values, int index, int currentSum,
               int limit, int& best) {
    if (currentSum > limit) {
        return;
    }
    if (index == static_cast<int>(values.size())) {
        best = max(best, currentSum);
        return;
    }

    dfsSubset(values, index + 1, currentSum, limit, best);
    dfsSubset(values, index + 1, currentSum + values[index], limit, best);
}

// ---------- 3. 全排列：next_permutation ----------
vector<vector<int>> allPermutations(vector<int> values) {
    sort(values.begin(), values.end());
    vector<vector<int>> result;
    do {
        result.push_back(values);
    } while (next_permutation(values.begin(), values.end()));
    return result;
}

// ---------- 4. DFS 全排列（可选重复元素去重） ----------
void dfsPermutation(vector<int>& values, int index,
                    vector<vector<int>>& result) {
    if (index == static_cast<int>(values.size())) {
        result.push_back(values);
        return;
    }

    unordered_set<int> usedValues;
    for (int i = index; i < static_cast<int>(values.size()); i++) {
        if (usedValues.count(values[i]) != 0) {
            continue;
        }
        usedValues.insert(values[i]);
        swap(values[index], values[i]);
        dfsPermutation(values, index + 1, result);
        swap(values[index], values[i]);
    }
}

// ---------- 5. O(n^2) 模拟：最大子段和 ----------
int maxSubarrayBrute(const vector<int>& values) {
    int n = static_cast<int>(values.size());
    int answer = INT_MIN;
    for (int left = 0; left < n; left++) {
        int sum = 0;
        for (int right = left; right < n; right++) {
            sum += values[right];
            answer = max(answer, sum);
        }
    }
    return answer;
}

// ---------- 6. O(n^2) 数对枚举：统计满足条件的数对 ----------
long long countPairsBrute(const vector<int>& values, int limit) {
    long long count = 0;
    int n = static_cast<int>(values.size());
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (abs(values[i] - values[j]) <= limit) {
                count++;
            }
        }
    }
    return count;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> values = {3, 1, 4, 1, 5};

    long long bestSubset = enumerateSubsets(values, [](long long sum) {
        return sum <= 10;
    });
    cout << "子集和不超过 10 的最大值 = " << bestSubset << '\n';

    int bestDfs = INT_MIN;
    dfsSubset(values, 0, 0, 10, bestDfs);
    cout << "DFS 子集结果 = " << bestDfs << '\n';

    vector<int> permutationSource = {1, 2, 3};
    vector<vector<int>> permutations = allPermutations(permutationSource);
    cout << "全排列数量 = " << permutations.size() << '\n';

    cout << "最大子段和（暴力）= " << maxSubarrayBrute(values) << '\n';
    cout << "距离不超过 2 的数对 = " << countPairsBrute(values, 2) << '\n';
    return 0;
}
