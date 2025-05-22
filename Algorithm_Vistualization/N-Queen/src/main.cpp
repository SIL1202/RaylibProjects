#include <cmath>
#include <iostream>
#include <unistd.h> // 用於 sleep()
#include <vector>

using namespace std;

void printBoard(const vector<int> &board, int n) {
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (board[row] == col) {
        cout << " Q ";
      } else {
        cout << " . ";
      }
    }
    cout << '\n';
  }
}

bool isValid(const vector<int> &board, int row, int col) {
  for (int i = 0; i < row; i++) {
    if (board[i] == col || abs(board[i] - col) == abs(i - row))
      return false;
  }
  return true;
}

void solve(int row, vector<int> &board, int n) {
  if (row == n) {
    printBoard(board, n); // find solution
    cout << '\n';
  }

  for (int col = 0; col < n; col++) {
    if (isValid(board, row, col)) {
      board[row] = col;
      solve(row + 1, board, n);
      board[row] = -1; // backtracking
    }
  }
}

int main() {
  int n;
  cout << "Enter the size of grid: ";
  cin >> n;

  vector<int> board(n, -1); // -1 表示該行尚未放皇后

  solve(0, board, n);

  return 0;
}

// {1, 3, 0, 2}
// . Q . .
// . . . Q
// Q . . .
// . . Q .

//
//     0   1   2   3   4
//   +------------------
// 0 | \               /
// 1 |     \       /
// 2 |         Q
// 3 |     /       \
// 4 | /               \
