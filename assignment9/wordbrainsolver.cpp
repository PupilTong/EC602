#include<iostream>
#include <unordered_map>
#include <map>
#include<vector>
#include<string>
#include<fstream>
#include<stack>
using std::unordered_map;
using std::map;
using std::vector;
using std::string;
using std::stack;
using std::cout;
using std::endl;
using std::ifstream;
using std::cin;
class Node
{
//0 top, 1 tr, 2 right, 3 rb, 4 bottom, 5 bl, 6 left,7  lt
public:
    vector<int> neighbors;
    char index;
    bool isValid=true;
    Node(char c){
        this->index=c;
    }
};
class Grid{
private:
public:
    int colLength = 0;
    int rowHeight=0;
    vector<Node> nodes;
    Grid(string gridString){
        int row=0,col=0;
        for(auto c : gridString){
            if(c!='\n')nodes.push_back(Node(c));
        }
        for(int i=0;i<gridString.size();i++){
            if (gridString[i] != '\n') {
                //create links between nodes
                if(col!=0){
                    nodes[row*colLength + col-1].neighbors.push_back(i - row);//set previous link to right
                    nodes[i-row].neighbors.push_back(row*colLength + col-1);//set this node link to left
                }
                if(row!=0){
                    nodes[(row-1)*colLength + col].neighbors.push_back(i - row);
                    nodes[i - row].neighbors.push_back((row-1)*colLength + col);//top link
                    if(col<colLength - 1){
                        nodes[(row-1)*colLength+col + 1].neighbors.push_back(i-row);
                        nodes[i-row].neighbors.push_back((row-1)*colLength  + col+1);//top right link
                    }
                }
                if(col!=0&&row!=0){
                    nodes[(row-1)*colLength + col-1].neighbors.push_back(i-row);
                    nodes[i-row].neighbors.push_back((row-1)*colLength + col-1);//top left link
                }
                col++;
            }
            else{
                this->colLength = col;
                col=0;
                row++;
            }
        }
        this->rowHeight = row;
    }
    void UnlinkInValid(){
        for(int c=0;c<colLength;c++){
            auto temp_one_col = stack<char>();
            for(int r=0; r<rowHeight; r++){
                auto node=nodes[r*colLength + c];
                if(node.isValid)
                temp_one_col.push(node.index);
            }
            for(int r=rowHeight - 1; r>=0; r--){
                if(temp_one_col.size()>0){
                    nodes[r*colLength + c].index = temp_one_col.top();
                    nodes[r*colLength + c].isValid = true;
                    temp_one_col.pop();
                }
                else{
                    nodes[r*colLength + c].isValid = false;
                }

            }
        }
    }
};
vector<string> wordRequirement;
class TireNode{
private:
public:
    unordered_map<char,TireNode*> value;
    bool isWordEnd=false;
    void Append(string word,const unordered_map<char,bool> & possibleChar,int currentDepth,int currentWordReq){
        if(word.size()==0){
            this->isWordEnd=true;
        }
        else{
            auto c = word.front();
            if(possibleChar.find(c)!=possibleChar.end()){
                auto currentRChar = wordRequirement[currentWordReq][currentDepth];
                if(currentRChar=='*'||currentRChar!=c){
                    word.erase(word.begin());
                    if(value.find(c)!=value.end()){//find it
                        value[c]->Append(word,possibleChar,currentDepth + 1,currentWordReq);
                    }
                    else{
                        value[c] = new TireNode();
                        value[c]->Append(word,possibleChar,currentDepth + 1,currentWordReq);
                    }
                }
            }
        }
    }
};

map<string,bool>  resultList;
vector<TireNode>  tiresList;
void grid_search_atom(Grid grid, int startIndex,  vector<string> * result_string,const TireNode * tire, string currentString,int currentDepth, int  currentRequirement){
    auto current_Node = grid.nodes[startIndex];
    grid.nodes[startIndex].isValid=false;//set search flag
    auto currentNodeRequirement = wordRequirement[currentRequirement][currentDepth];
    if(currentNodeRequirement!='*')
        if(currentNodeRequirement!=current_Node.index)
            return;
    if(tire->value.find(current_Node.index)==tire->value.end()){
        //find failed
        return;
    }else{
        tire = tire->value.at(current_Node.index);
    }
    currentString  += current_Node.index;
    if(tire->isWordEnd){
        result_string->push_back(currentString);
        if(currentRequirement == wordRequirement.size() - 1){
            string a="";
            for(auto & w : *result_string){
                a +=w + " ";
            }
            a.pop_back();
            resultList[a]=true;
        }
        else{
            grid.UnlinkInValid();
            for(int i=0;i<grid.nodes.size();i++){
                if(grid.nodes[i].isValid){
                    grid_search_atom(grid,i,result_string,&(tiresList[currentRequirement + 1]),"",0,currentRequirement + 1);
                }
            }
        }
        result_string->pop_back();
    }
    else{
        for(auto & neighbourIndex : current_Node.neighbors){
                auto neigbour = grid.nodes[neighbourIndex];
                if(neigbour.isValid==true){//un searched;
                    //then go searching
                    grid_search_atom(grid,neighbourIndex,result_string, tire,currentString,currentDepth + 1, currentRequirement);
                }
        }
    }
}
/*
void search_grid(Grid & g, vector<TireNode> & tire,int currentDepth, vector<string> thisNodeResult){
        for(int i=0; i<g.nodes.size();i++){
            if(g.nodes[i].isValid){
                auto result_grid = vector<Grid> () ;
                auto result_string = vector<string> ();
                grid_search_atom(g,i,&result_grid,&result_string,&(tire[currentDepth]),"",0,currentDepth);
                for(int j=0; j < result_grid.size();j++){
                    result_grid[j].UnlinkInValid();
                    thisNodeResult.push_back(result_string[j]);
                    if(currentDepth != tire.size() - 1){
                        search_grid(result_grid[j],tire,currentDepth + 1,thisNodeResult);
                    }
                    else{
                        string r="";
                        for(auto & word : thisNodeResult){
                            r += word + " ";
                        }
                        r.pop_back();
                        resultList[r]=true;
                    }
                    thisNodeResult.pop_back();
                }
            }
        }
}*/
void get_tires(char* fileName, const Grid & g){
    unordered_map<char,bool> possibleChar;
    unordered_map<char,bool> possibleL;
    for(auto & node : g.nodes){
        possibleChar.insert_or_assign(node.index,true);
    }
    ifstream file;
    for( auto & word : wordRequirement){
        tiresList.push_back(TireNode());
        possibleL[word.size()]=true;
    }
    file.open(fileName,std::ios::in);
    string line;
    while(getline(file,line)){
        auto ll =line.size();
        if(possibleL.find(ll)!=possibleL.end()){
            for(int i=0;i<wordRequirement.size();i++){
                if(ll==wordRequirement[i].size()){
                    tiresList[i].Append(line,possibleChar,0,i);
                }
            }
        }
    }
    file.close();
}
void search_and_print(string grid_string,char* small_list,char * large_list){
    auto grid = Grid(grid_string);
    tiresList.clear();
    resultList.clear();
    get_tires(small_list,grid);
    //search_grid(grid,tires,0,vector<string>());
    for(int i=0;i<grid.nodes.size();i++){
        auto t = vector<string>();
        grid_search_atom(grid,i,&t,&tiresList[0],"",0,0);
    }

    for(auto & answer : resultList){
        cout<<answer.first<<endl;
    }

    if(resultList.size()==0){//cannot find
        tiresList.clear();
        resultList.clear();
        get_tires(large_list,grid);
        for(int i=0;i<grid.nodes.size();i++){
            auto t = vector<string>();
            grid_search_atom(grid,i,&t,&tiresList[0],"",0,0);
        }
        for(auto &  answer : resultList){
            cout<<answer.first<<endl;
        }
    }
    cout<<"."<<endl;
}

int main(int argc, char **argv){
    auto small_list = argv[1];
    auto large_list = argv[2];
    //vector<string> input;

    string grid_string = "";
    int grid_size = 0;
    while (true){
        string line;
        getline(cin, line);
        //if(line.size()!=0)line.pop_back();
         if(grid_size == 0 ){
            wordRequirement.clear();
            grid_string = "";
            grid_size = line.size();
            grid_string += line + "\n";
        }  else{
            if(grid_size == 1){
                wordRequirement.push_back("");
                for(auto & c : line ){
                    if(c!=' ')wordRequirement.back()+=c;
                    else wordRequirement.push_back("");
                }
                search_and_print(grid_string,small_list,large_list);
                wordRequirement.clear();
                resultList.clear();
            }else{
                grid_string += line + "\n";
            }
            grid_size -=1;
        }

        //input.push_back(line);
        if(!cin)break;
        if(!cin.good())break;
    }
    //input.pop_back();
    return 0;
    /*
    int grid_size = 0;
    string grid_string = "";
    vector<string> words;
    for (auto & line : input){
        if(grid_size == 0 ){
            words.clear();
            grid_string = "";
            grid_size = line.size();
            grid_string += line + "\n";
        }  else{
            if(grid_size == 1){
                words.push_back("");
                for(auto & c : line ){
                    if(c!=' ')words.back()+=c;
                    else words.push_back("");
                }
                search_and_print(grid_string,words,small_list,large_list);
                cout<<"."<<endl;
            }else{
                grid_string += line + "\n";
            }
            grid_size -=1;
        }
    }
    */
}