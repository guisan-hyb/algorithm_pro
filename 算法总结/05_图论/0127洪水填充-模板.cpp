// 洪水填充模板：岛屿数量 / 被围绕的区域 / 最大人工岛 / 打砖块（时光倒流）
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

std::array<int, 4> dx= {1, -1, 0, 0};
std::array<int, 4> dy= {0, 0, 1, -1};
// ================= 1. 通用染色骨架 =================
void dfsFill(vector<vector<char>>& g, int i, int j, char from, char to) {
    if (i < 0 || i >= (int)g.size() || j < 0 || j >= (int)g[0].size() || g[i][j] != from)
        return;
    g[i][j] = to;                                  // 标记，不撤销
    for (int d = 0; d < 4; d++) dfsFill(g, i + dx[d], j + dy[d], from, to);
}

// ================= 2. 岛屿数量 =================
int numIslands(vector<vector<char>> g) {
    int ans = 0;
    for (int i = 0; i < (int)g.size(); i++)
        for (int j = 0; j < (int)g[0].size(); j++)
            if (g[i][j] == '1') { ans++; dfsFill(g, i, j, '1', '0'); }
    return ans;
}

// ================= 3. 被围绕的区域 =================
void solveSurrounded(vector<vector<char>>& board) {
    int n = (int)board.size(), m = (int)board[0].size();
    for (int i = 0; i < n; i++) {
        if (board[i][0] == 'O') dfsFill(board, i, 0, 'O', 'F');
        if (board[i][m - 1] == 'O') dfsFill(board, i, m - 1, 'O', 'F');
    }
    for (int j = 0; j < m; j++) {
        if (board[0][j] == 'O') dfsFill(board, 0, j, 'O', 'F');
        if (board[n - 1][j] == 'O') dfsFill(board, n - 1, j, 'O', 'F');
    }
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            if (board[i][j] == 'O') board[i][j] = 'X';
            else if (board[i][j] == 'F') board[i][j] = 'O';
        }
}

// ================= 4. 最大人工岛 =================
int largestIsland(vector<vector<int>> grid) {
    int n = (int)grid.size(), m = (int)grid[0].size(), id = 2;
    function<void(int,int,int)> dfsId = [&](int i, int j, int ID) {
        if (i < 0 || i >= n || j < 0 || j >= m || grid[i][j] != 1) return;
        grid[i][j] = ID;
        for (int d = 0; d < 4; d++) dfsId(i + dx[d], j + dy[d], ID);
    };
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (grid[i][j] == 1) dfsId(i, j, id++);

    vector<int> sizeOf(id, 0);
    int ans = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (grid[i][j] > 1) ans = max(ans, ++sizeOf[grid[i][j]]);

    vector<bool> used(id, false);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            if (grid[i][j] != 0) continue;
            int cur = 1;
            for (int d = 0; d < 4; d++) {
                int x = i + dx[d], y = j + dy[d];
                if (x < 0 || x >= n || y < 0 || y >= m) continue;
                int ID = grid[x][y];
                if (ID > 1 && !used[ID]) { used[ID] = true; cur += sizeOf[ID]; }
            }
            for (int d = 0; d < 4; d++) {          // 还原临时标记
                int x = i + dx[d], y = j + dy[d];
                if (x >= 0 && x < n && y >= 0 && y < m && grid[x][y] > 1)
                    used[grid[x][y]] = false;
            }
            ans = max(ans, cur);
        }
    return ans;
}

// ================= 5. 打砖块（时光倒流） =================
class Bricks {
    int m, n;
    vector<vector<int>> g;
    int dfs(int i, int j) {                        // 返回新增的"坚固(2)"砖块数
        if (i < 0 || i >= m || j < 0 || j >= n || g[i][j] != 1) return 0;
        g[i][j] = 2;
        return 1 + dfs(i + 1, j) + dfs(i - 1, j) + dfs(i, j + 1) + dfs(i, j - 1);
    }
    bool worth(int i, int j) {
        return g[i][j] == 1 &&
               (i == 0 || (i > 0 && g[i - 1][j] == 2) || (i < m - 1 && g[i + 1][j] == 2) ||
                (j > 0 && g[i][j - 1] == 2) || (j < n - 1 && g[i][j + 1] == 2));
    }
public:
    vector<int> hitBricks(vector<vector<int>> grid, vector<vector<int>> hits) {
        g = grid; m = (int)g.size(); n = (int)g[0].size();
        vector<int> ans(hits.size(), 0);
        for (auto& h : hits) g[h[0]][h[1]]--;              // 先变成最终状态
        for (int j = 0; j < n; j++) dfs(0, j);             // 顶部坚固砖块染 2
        for (int i = (int)hits.size() - 1; i >= 0; i--) {  // 时光倒流
            int r = hits[i][0], c = hits[i][1];
            g[r][c]++;
            if (worth(r, c)) ans[i] = dfs(r, c) - 1;
        }
        return ans;
    }
};

int main() {
    vector<vector<char>> g = {{'1','1','0'},{'0','1','0'},{'0','0','1'}};
    std::cout << "岛屿数量 = " << numIslands(g) << "\n";                 // 2
    vector<vector<int>> grid = {{1,0},{0,1}};
    std::cout << "最大人工岛 = " << largestIsland(grid) << "\n";          // 3
    return 0;
}
