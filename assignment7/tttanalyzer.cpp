#define Assignment7
#ifdef Assignment7
#include <iostream>
#include <map>
#include <string>
#include <vector>
// #include "movedef.h"
using std::endl;
using std::exception;
using std::map;
using std::string;
using std::vector;
struct Move {
  int r;
  int c;
  char player;
};

using std::cout;
using std::string;
using std::vector;
enum class Status {
  tie = 't',
  xWin = 'x',
  oWin = 'o',
  gameContinue = 'c',
  inValid = 'i',
  error = 'e'
};
enum class RowStatus {
  xWin = 'x',
  oWin = 'o',
  empty = '#',
  gameContinue = 'c',
};
class Row {
 private:
  char* rowRaw[3];
  RowStatus status = RowStatus::gameContinue;

 public:
  Row() {}
  Row(char* c0, char* c1, char* c2) { set(c0, c1, c2); }
  void set(char* c0, char* c1, char* c2) {
    rowRaw[0] = c0;
    rowRaw[1] = c1;
    rowRaw[2] = c2;
    updateStatus();
  }
  void updateStatus() {
    if ((*rowRaw[0] == *rowRaw[1]) && (*rowRaw[1] == *rowRaw[2])) {
      status = static_cast<RowStatus>(*(rowRaw[0]));
    } else {
      status = RowStatus::gameContinue;
    }
  }
  char GetStatus() { return static_cast<char>(status); }
  ~Row() {}
};

class tttBoard {
 private:
  Row rows[8];
  Status status = Status::inValid;
  char board[9];
  int xStepCount = 0;
  int oStepCount = 0;

  void UpdateRowsStatues() {
    for (auto row : rows) {
      row.updateStatus();
    }
  }
  void setBoard(string boardString) {
    for (int i = 0; i < 9; i++) {
      switch (boardString[i]) {
        case 'x':
          xStepCount++;
          break;
        case 'o':
          oStepCount++;
          break;
        case '#':
          break;
        default:
          throw "illegalInput";
      }
      board[i] = boardString[i];
    }
  }
  void CalculateStatus() {
    int xwin = 0, owin = 0;
    status = Status::inValid;
    if (!((xStepCount - oStepCount) == 1 || xStepCount == oStepCount)) return;
    for (auto r : rows) {
      switch (r.GetStatus()) {
        case 'x':
          xwin += 1;
          break;
        case 'o':
          owin += 1;
          break;
      }
    }
    if (xwin) {
      if (owin) return;
      if (!(xStepCount > oStepCount)) return;
      if (xwin > 2 && rows[6].GetStatus() != 'x' && rows[7].GetStatus() != 'x')
        return;
      status = Status::xWin;
      return;
    }
    if (owin) {
      if (!(xStepCount == oStepCount)) return;
      if (owin != 1) return;
      status = Status::oWin;
      return;
    }
    if (oStepCount + xStepCount == 9) {
      status = Status::tie;
      return;
    }
    status = Status::gameContinue;
  }
  void UpdateStatus() {
    try {
      UpdateRowsStatues();
      CalculateStatus();
    } catch (exception& e) {
      status = Status::error;
    }
  }
  void UpdateStatus(int row, int col) {
    rows[row].updateStatus();
    rows[3 + col].updateStatus();
    if (row == col) {
      rows[6].updateStatus();
    }
    if (row + col == 2) {
      rows[7].updateStatus();
    }
    CalculateStatus();
  }

 public:
  explicit tttBoard(string boardString) {
    setBoard(boardString);
    // rows
    rows[0] = Row(&board[0], &board[1], &board[2]);
    rows[1] = Row(&board[3], &board[4], &board[5]);
    rows[2] = Row(&board[6], &board[7], &board[8]);
    // cols
    rows[3] = Row(&board[0], &board[3], &board[6]);
    rows[4] = Row(&board[1], &board[4], &board[7]);
    rows[5] = Row(&board[2], &board[5], &board[8]);
    // Xs
    rows[6] = Row(&board[0], &board[4], &board[8]);
    rows[7] = Row(&board[6], &board[4], &board[2]);
    UpdateStatus();
  }
  char Set(string boardString) {
    setBoard(boardString);
    UpdateStatus();
    return static_cast<char>(status);
  }
  char Set(Move m) {
    status = Status::error;
    if (m.player != 'x' || m.player != 'o') return 'e';
    if (m.r * m.c > 8 || m.r * m.c < 0) return 'e';
    if (board[m.r * 3 + m.c] != '#') return 'e';
    board[m.r * 3 + m.c] = m.player;
    UpdateStatus(m.r, m.c);
    return static_cast<char>(status);
  }
  char GetStatus() { return static_cast<char>(status); }
  ~tttBoard() {}
};
char tttresult(string board) { return tttBoard(board).GetStatus(); }

char tttresult(vector<Move> board) {
  auto b = tttBoard("#########");
  for (auto m : board) {
    b.Set(m);
    if (b.GetStatus() != 'c') return b.GetStatus();
  }
  return b.GetStatus();
}

const char base3[3] = {'#', 'x', 'o'};
vector<string> get_all_boards() {
  vector<string> boards;
  string board;
  int test;
  for (int i = 0; board != "ooooooooo"; i++) {
    board = "";
    for (int j = i; j > 0; j /= 3) {
      board = base3[j % 3] + board;
    }
    while (board.length() < 9) {
      board = '#' + board;
    }
    boards.push_back(board);
    test = i;
  }
  return boards;
}

void ttt_tally() {
  auto m = map<char, int>();
  for (auto s : get_all_boards()) {
    m[tttBoard(s).GetStatus()] += 1;
  }
  for (auto&& [k, v] : m) {
    cout << k << " " << v << endl;
  }
}

// This version of main interactively
// tests either:
//
//  string tttresult "s"
//  vector tttresult "v"
// or
//  get_all_boards "a"
//
// MAIN

int main(void) {
  int n;
  std::string board;
  Move m;
  std::vector<Move> moves;
  std::vector<std::string> boards;
  std::string asktype;

  while (std::cin >> asktype) {
    if (asktype == "v") {  // test tttresult vector
      moves.clear();
      std::cin >> n;
      for (int i = 0; i < n; i++) {
        std::cin >> m.r >> m.c >> m.player;
        moves.push_back(m);
      }
      std::cout << tttresult(moves) << "\n";
    } else if (asktype == "s") {  // test tttresult string
      std::cin >> board;
      std::cout << tttresult(board) << "\n";
    } else if (asktype == "a") {  // test get_all_boards
      boards = get_all_boards();
      for (auto b : boards) {
        std::cout << b << "\n";
      }
    } else if (asktype == "t") {  // test ttt_tally
      ttt_tally();
    } else {
      return 0;
    }
  }
  return 0;
}
#endif
