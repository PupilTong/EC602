// Copyright 2019 haoyangwang haoyangw@bu.edu
// Copyright 2019 songgeng songgeng@bu.edu
// Copyright 2019 kefanzhang kefan29@bu.edu
#include <sys/types.h>
#include<sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::map;
using std::stack;
using std::string;
using std::unordered_map;
using std::vector;
class Node {
  // 0 top, 1 tr, 2 right, 3 rb, 4 bottom, 5 bl, 6 left,7  lt
 public:
  vector<int> neighbors;
  char index;
  bool isValid = true;
  explicit Node(char c) {
    this->index = c;
  }
};
class Grid {
 public:
  int colLength = 0;
  int rowHeight = 0;
  vector<Node> nodes;
  explicit Grid(string gridString) {
    int row = 0, col = 0;
    for (auto c : gridString) {
      if (c != '\n') nodes.push_back(Node(c));
    }
    for (int i = 0; i < gridString.size(); i++) {
      if (gridString[i] != '\n') {
        // create links between nodes
        if (col != 0) {
          nodes[row * colLength + col - 1].neighbors.push_back(
            i - row);  // set previous link to right
          nodes[i - row].neighbors.push_back(row * colLength + col -
                                             1);  // set this node link to left
        }
        if (row != 0) {
          nodes[(row - 1) * colLength + col].neighbors.push_back(i - row);
          nodes[i - row].neighbors.push_back((row - 1) * colLength +
                                             col);  // top link
          if (col < colLength - 1) {
            nodes[(row - 1) * colLength + col + 1].neighbors.push_back(i - row);
            nodes[i - row].neighbors.push_back((row - 1) * colLength + col +
                                               1);  // top right link
          }
        }
        if (col != 0 && row != 0) {
          nodes[(row - 1) * colLength + col - 1].neighbors.push_back(i - row);
          nodes[i - row].neighbors.push_back((row - 1) * colLength + col -
                                             1);  // top left link
        }
        col++;
      } else {
        this->colLength = col;
        col = 0;
        row++;
      }
    }
    this->rowHeight = row;
  }
  void UnlinkInValid() {
    for (int c = 0; c < colLength; c++) {
      auto temp_one_col = stack<char>();
      for (int r = 0; r < rowHeight; r++) {
        auto node = nodes[r * colLength + c];
        if (node.isValid) temp_one_col.push(node.index);
      }
      for (int r = rowHeight - 1; r >= 0; r--) {
        if (temp_one_col.size() > 0) {
          nodes[r * colLength + c].index = temp_one_col.top();
          nodes[r * colLength + c].isValid = true;
          temp_one_col.pop();
        } else {
          nodes[r * colLength + c].isValid = false;
        }
      }
    }
  }
};
class TireNode {
 private:
 public:
  unordered_map<char, TireNode*> value;
  bool isWordEnd = false;
  void Append(string word, const unordered_map<char, bool>& possibleChar) {
    auto c = word.front();
    if (word.size() == 0) {
      this->isWordEnd = true;
    } else {
      if (possibleChar.find(c) != possibleChar.end()) {
        word.erase(word.begin());
        if (value.find(c) != value.end()) {  // find it
          value[c]->Append(word, possibleChar);
        } else {
          value[c] = new TireNode();
          value[c]->Append(word, possibleChar);
        }
      }
    }
  }
};

map<string, bool> resultList;
vector<string> wordRequirement;
vector<TireNode> tiresList;
void grid_search_atom(Grid grid, int startIndex, vector<string>* result_string,
                      const TireNode* tire, string currentString,
                      int currentDepth, int currentRequirement) {
  auto current_Node = grid.nodes[startIndex];
  grid.nodes[startIndex].isValid = false;  // set search flag
  auto currentNodeRequirement =
    wordRequirement[currentRequirement][currentDepth];
  if (currentNodeRequirement != '*')
    if (currentNodeRequirement != current_Node.index) return;
  if (tire->value.find(current_Node.index) == tire->value.end()) {
    // find failed
    return;
  } else {
    tire = tire->value.at(current_Node.index);
  }
  currentString += current_Node.index;
  if (tire->isWordEnd) {
    result_string->push_back(currentString);
    if (currentRequirement == wordRequirement.size() - 1) {
      string a = "";
      for (auto& w : *result_string) {
        a += w + " ";
      }
      a.pop_back();
      resultList[a] = true;
    } else {
      grid.UnlinkInValid();
      for (int i = 0; i < grid.nodes.size(); i++) {
        if (grid.nodes[i].isValid) {
          grid_search_atom(grid, i, result_string,
                           &(tiresList[currentRequirement + 1]), "", 0,
                           currentRequirement + 1);
        }
      }
    }
    result_string->pop_back();
  } else {
    for (auto& neighbourIndex : current_Node.neighbors) {
      auto neigbour = grid.nodes[neighbourIndex];
      if (neigbour.isValid == true) {  // un searched;
        // then go searching
        grid_search_atom(grid, neighbourIndex, result_string, tire,
                         currentString, currentDepth + 1, currentRequirement);
      }
    }
  }
}
void get_tires(vector<TireNode>* tires, char* fileName, const Grid& g) {
  unordered_map<char, bool> possibleChar;
  for (auto& node : g.nodes) {
    possibleChar.insert_or_assign(node.index, true);
  }
  unordered_map<int, TireNode> tiresOrdersByDepth;
  ifstream file;
  for (auto& word : wordRequirement) {
    tiresOrdersByDepth[word.size()] = TireNode();
  }
  file.open(fileName, std::ios::in);
  string line;
  while (getline(file, line)) {
    int lineLength = line.size();
    if (tiresOrdersByDepth.find(lineLength) != tiresOrdersByDepth.end()) {
      tiresOrdersByDepth[lineLength].Append(line, possibleChar);
    }
  }
  file.close();
  for (auto& word : wordRequirement) {
    tires->push_back(tiresOrdersByDepth[word.size()]);
  }
}
void search_and_print(string grid_string, char* small_list, char* large_list) {
  auto grid = Grid(grid_string);
  tiresList.clear();
  resultList.clear();
  get_tires(&tiresList, small_list, grid);
  // search_grid(grid,tires,0,vector<string>());
  for (int i = 0; i < grid.nodes.size(); i++) {
    auto t = vector<string>();
    grid_search_atom(grid, i, &t, &tiresList[0], "", 0, 0);
  }

  for (auto& answer : resultList) {
    cout << answer.first << endl;
  }

  if (resultList.size() == 0) {  // cannot find
    tiresList.clear();
    resultList.clear();
    get_tires(&tiresList, large_list, grid);
    int shard_mem_id = shmget(IPC_PRIVATE, 256, IPC_CREAT|0666);
    //map<string,bool> temp;
    auto gridSize = grid.nodes.size();
    auto childpid = fork();

    char* child_result = (char*)shmat(shard_mem_id, NULL, 0);
    for (int i = childpid ? 0 : gridSize / 2 + 1;
         i < (childpid ? gridSize / 2 : gridSize); i++) {
      auto t = vector<string>();
      grid_search_atom(grid, i, &t, &tiresList[0], "", 0, 0);
    }
    int size = 0;
    if (!childpid) {  // child
      int index = 0;
      for (auto& key : resultList) {
        for(int i = 0;i<key.first.size();i++){
          child_result[i+index] = key.first[i];
        }
        child_result[key.first.size()]='\0';
        index +=key.first.size() + 1;
      }
      child_result[index]='\n';
      shmdt(child_result);
      exit(0);
    }
    wait(0);
    //child_result = (map<string,bool> *)shmat(shard_mem_id, NULL, 0);
    /*for (auto& childRS : *child_result) {
      resultList[childRS.first] = true;
    }*/
    string temp ;
    for(int i = 0;child_result[i]!='\n';i++){
      if(child_result[i]=='\0'){
        resultList[temp]=true;
        temp.clear();
      }
      else{
        temp += child_result[i];
      }
    }
    memset(child_result,0,256);
    shmdt(child_result);
    shmctl(shard_mem_id,IPC_RMID,0);
    for (auto& answer : resultList) {
      cout << answer.first << endl;
    }
  }
  cout << "." << endl;
}

int main(int argc, char** argv) {
  auto small_list = argv[1];
  auto large_list = argv[2];
  // vector<string> input;

  string grid_string = "";
  int grid_size = 0;
  while (true) {
    string line;
    getline(cin, line);
    // if(line.size()!=0)line.pop_back();
    if (grid_size == 0) {
      wordRequirement.clear();
      grid_string = "";
      grid_size = line.size();
      grid_string += line + "\n";
    } else {
      if (grid_size == 1) {
        wordRequirement.push_back("");
        for (auto& c : line) {
          if (c != ' ')
            wordRequirement.back() += c;
          else
            wordRequirement.push_back("");
        }
        search_and_print(grid_string, small_list, large_list);
        wordRequirement.clear();
        resultList.clear();
      } else {
        grid_string += line + "\n";
      }
      grid_size -= 1;
    }

    // input.push_back(line);
    if (!cin) break;
    if (!cin.good()) break;
  }
  // input.pop_back();
  return 0;
}
