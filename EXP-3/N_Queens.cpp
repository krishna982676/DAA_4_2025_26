#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<string> board;
    vector<vector<string>> result;
    int sz;

    bool isValid(int x, int y) {
        for (int i = 0; i < x; i++) {
            if (board[i][y] == 'Q') return false;
        }
        for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--) {
            if (board[i][j] == 'Q') return false;
        }
        for (int i = x - 1, j = y + 1; i >= 0 && j < sz; i--, j++) {
            if (board[i][j] == 'Q') return false;
        }
        return true;
    }

    void solve(int row) {
        if (row == sz) {
            result.push_back(board);
            return;
        }
        for (int col = 0; col < sz; col++) {
            if (isValid(row, col)) {
                board[row][col] = 'Q';
                solve(row + 1);
                board[row][col] = '.';
            }
        }
    }

    vector<vector<string>> solveNQueens(int n) {
        sz = n;
        board = vector<string>(n, string(n, '.'));
        solve(0);
        return result;
    }
};

int main() {
    int n;
    cout << "Enter value of n: ";
    cin >> n;

    Solution obj;
    vector<vector<string>> ans = obj.solveNQueens(n);

    cout << "Total solutions: " << ans.size() << "\n\n";

    for (int i = 0; i < ans.size(); i++) {
        cout << "Solution " << i + 1 << ":\n";
        for (auto &row : ans[i]) {
            cout << row << "\n";
        }
        cout << "\n";
    }

    return 0;
}
