// Copyright 2019 haoyangw haoyangw@bu.edu
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "movedef.h"
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;

class tttBoard {
 public:
  vector<char> rst;
  string bd;
  void UpdateStatus() {
    int xwin = 0, owin = 0, xStepCount = 0, oStepCount = 0, d;
    for (int i = 0; i < 9; i++) {
      if (bd.at(i) != 'x' && bd.at(i) != 'o' && bd.at(i) != '#') {
        this->status = 'e';
        return;
      }
      if (i < 3)
        rst.at(i) = RSt(bd.at(i * 3), bd.at(i * 3 + 1), bd.at(i * 3 + 2));
      else if (i < 6)
        rst.at(i) = RSt(bd.at(i - 3), bd.at(i), bd.at(i + 3));
      if (bd.at(i) == 'x') xStepCount++;
      if (bd.at(i) == 'o') oStepCount++;
    }
    rst.at(6) = RSt(bd.at(0), bd.at(4), bd.at(8));
    rst.at(7) = RSt(bd.at(6), bd.at(4), bd.at(2));
    for (auto r : rst) {
      if (r == 'x') xwin++;
      if (r == 'o') owin++;
    }
    d = xStepCount - oStepCount;
    if ((owin && xwin) || (d != 1 && d != 0)) return;
    status = owin ? (xStepCount == oStepCount ? 'o' : 'i')
             : ((xwin) ? (xStepCount - oStepCount == 1 ? 'x' : 'i')
                : (oStepCount + xStepCount == 9 ? 't' : 'c'));
  }
  char RSt(char a, char b, char c) {
    return (a == b && a == c) ? a : 'c';
  }

  char status = 'i';
  explicit tttBoard(string boardString) {
    rst.resize(8);
    bd = boardString;
    UpdateStatus();
  }
  void Set(Move m) {
    // if (m.player == '#' || m.r > 2 || m.c > 2 || board.at(m.r * 3 + m.c) !=
    // '#')
    bd.at(m.r * 3 + m.c) = m.player;
    UpdateStatus();
  }
};
char tttresult(string board) {
  if (board.size() != 9) return 'e';
  return tttBoard(board).status;
}

char tttresult(vector<Move> board) {
  auto b = tttBoard("#########");
  for (auto m : board) {
    b.Set(m);
    if (b.status != 'c') break;
  }
  return b.status;
}

const vector<char> base3 = {'#', 'x', 'o'};
vector<string> get_all_boards() {
  vector<string> boards;
  string board;
  for (int i = 0; board != "ooooooooo"; i++) {
    board.clear();
    for (int j = i; j > 0; j /= 3) board = base3.at(j % 3) + board;
    while (board.length() < 9) board = '#' + board;
    boards.push_back(board);
  }
  return boards;
}

void ttt_tally() {
  auto m = map<char, int>();
  for (auto s : get_all_boards()) {
    char t = tttBoard(s).status;
    (*((m.insert(std::make_pair(t, char()))).first)).second += 1;
  }
  for (auto om : m) cout << om.first << " " << om.second << endl;
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

int main() {
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
