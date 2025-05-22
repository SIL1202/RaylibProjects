#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

void printBoard(const vector<int> &board, int n) {
  cout << "Found a solution!\n";
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (board[row] == col) {
        cout << " Q ";
      } else {
        cout << " . ";
      }
    }
    cout << endl;
  }
}

bool isValid(const vector<int> &board, int row, int col) {
  for (int i = 0; i < row; i++) {
    if (board[i] == col || abs(board[i] - col) == abs(i - row))
      return false;
  }
  return true;
}

bool solve(int row, vector<int> &board, int n) {
  if (row == n) {
    printBoard(board, n); // 找到解就印出
    return true;          // 回傳 true 表示找到解
  }

  for (int col = 0; col < n; col++) {
    if (isValid(board, row, col)) {
      board[row] = col;
      if (solve(row + 1, board, n)) {
        return true; // 如果已找到解，向上回傳
      }
      board[row] = -1; // 回溯
    }
  }

  return false; // 無法在這一層找到合法解
}

int main() {
  int n;
  cout << "Enter the number of queens (n): ";
  cin >> n;

  vector<int> board(n, -1);

  if (!solve(0, board, n)) {
    cout << "No solution exists for n = " << n << endl;
  }

  return 0;
}
