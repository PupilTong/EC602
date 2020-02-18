#include<iostream>
#include <unordered_map>
#include <map>
#include<vector>
#include<string>
//#include<thread>
//#include<mutex>
using std::unordered_map;
using std::map;
using std::vector;
using std::string;
//using std::thread;
//using std::mutex;
using std::cout;
using std::endl;
class Node
{
//0 top, 1 tr, 2 right, 3 rb, 4 bottom, 5 bl, 6 left,7  lt
public:
    vector<int> neighbors;
    char index;
    bool isValid=true;
    Node(char c){
        this->neighbors.resize(8);
        this->index=c;
        for(auto & e : neighbors){
            e=-1;
        }
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
            if (gridString.at(i) != '\n') {
                //create links between nodes
                if(col!=0){
                    nodes.at(row*colLength + col-1).neighbors.at(2)=i - row;//set previous link to right
                    nodes.at(i-row).neighbors.at(6) = row*colLength + col-1;//set this node link to left
                }
                if(row!=0){
                    nodes.at((row-1)*colLength + col).neighbors.at(4)=i - row;
                    nodes.at(i - row).neighbors.at(0)=(row-1)*colLength + col;//top link
                    if(col<colLength - 1){
                        nodes.at((row-1)*colLength+col + 1).neighbors.at(5)=i-row;
                        nodes.at(i-row).neighbors.at(1)=(row-1)*colLength  + col+1;//top right link
                    }
                }
                if(col!=0&&row!=0){
                    nodes.at((row-1)*colLength + col-1).neighbors.at(3)=i-row;
                    nodes.at(i-row).neighbors.at(7)=(row-1)*colLength + col-1;//top left link
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
        /*
        for(int i=nodes.size()-1;i>=0; i --){
            if(nodes.at(i).isValid==false){
                auto temp_node = nodes.at(i);
                auto temp_node_top = nodes.at(i).neighbors.at(0);
                while(!nodes[temp_node_top].isValid)
            }
        }*/
        for(int c=0;c<colLength;c++){
            auto temp_one_col = vector<char>();
            for(int r=0; r<rowHeight; r++){
                auto node=nodes.at(r*colLength + c);
                if(node.isValid)
                temp_one_col.push_back(node.index);
            }
            for(int r=rowHeight - 1; r>=0; r--){
                if(temp_one_col.size()>0){
                    nodes.at(r*colLength + c).index = temp_one_col.back();
                    nodes.at(r*colLength + c).isValid = true;
                    temp_one_col.pop_back();
                }
                else{
                    nodes.at(r*colLength + c).isValid = false;
                }

            }
        }
    }
    Grid* fork() const{
         return new Grid(*this);
    }
};
class TireNode{
private:
public:
    unordered_map<char,TireNode*> value;
    bool isWordEnd=false;
    void Append(string word){
        auto c = word.front();
        if(word.size()==0){
            this->isWordEnd=true;
        }
        else{
            word.erase(word.begin());
            if(value.find(c)!=value.end()){//find it
                value.at(c)->Append(word);
            }
            else{
                value[c] = new TireNode();
                value[c]->Append(word);
            }
        }
    }
};
class AnswerTree{
public:
    string value;
    vector<AnswerTree> children;
    AnswerTree(string v){
        value = v;
    }
};
//tried to use multi-threading but it seems i have to add -lpthread while compile
/*
mutex locker;
void grid_search_atom(const Grid & grid, int startIndex, vector<Grid> * result_grid, vector<string> * result_string, int* threadCount){
    string resultString="";
    



    locker.lock();
    *threadCount+=1;
    locker.unlock();

}*/
/*
void grid_search(Grid & grid, const string & word, const TireNode& tire){
    int threadCount =0;
    vector<Grid> result_grid;
    vector<string> result_string;
    /*
    for(int i=0;i<grid.nodes.size();i++){
        //thread t(grid_search_atom, grid, i, &result_grid, &result_string, & threadCount);
        //t.detach();
        
    }
    while(threadCount!=grid.nodes.size()-1);
    *
}*/
void testFunction_PrintGrid(Grid & g){
    for(int i=0;i<g.nodes.size();i++){
        auto node = g.nodes.at(i);
        cout<<node.index<<((node.isValid)?" ":"*");
        if(i%g.rowHeight==g.rowHeight - 1)
        cout<<'\n';
    }
}
void getAnswer(vector<AnswerTree> & result, vector<string>* answer){
    
}
void grid_search_atom(Grid grid, int startIndex, vector<Grid> * result_grid, vector<string> * result_string,const TireNode * tire, string currentString){
    auto current_Node = grid.nodes.at(startIndex);
    grid.nodes.at(startIndex).isValid=false;//set search flag
    if(tire->value.find(current_Node.index)==tire->value.end()){
        //find failed
        return;
    }else{
        tire = tire->value.at(current_Node.index);
    }
    currentString  += current_Node.index;
    if(tire->isWordEnd){
        result_string->push_back(currentString);
        result_grid->push_back(grid);
        return;
    }
    for(auto & neighbourIndex : current_Node.neighbors){
        if(neighbourIndex!=-1){//check if neighbor is exist
            auto neigbour = grid.nodes.at(neighbourIndex);
            if(neigbour.isValid==true){//un searched;
                //then go searching
                grid_search_atom(grid,neighbourIndex,result_grid,result_string, tire,currentString);
            }
        }
    }
}
void search_grid(Grid & g, vector<TireNode*> & tire,vector<AnswerTree>* result,int currentDepth){
    vector<Grid>  result_grid;
     vector<string>  result_string;
    for(int i=0; i<g.nodes.size();i++){
        if(g.nodes.at(i).isValid){
            grid_search_atom(g,i,&result_grid,&result_string,tire.at(currentDepth),"");
        }
    }
    for(int i=0; i < result_grid.size();i++){
        result_grid.at(i).UnlinkInValid();
        result->push_back(AnswerTree(result_string.at(i)));
        if(currentDepth != tire.size() - 1){
            search_grid(result_grid.at(i),tire,&(result->back().children),currentDepth + 1);
        }
    }
}
int main(){
    auto a = TireNode();
    a.Append("post");
    a.Append("pots");
    a.Append("stop");
    a.Append("opts");
    auto b = TireNode();
    b.Append("bedroom");
    b.Append("vampire");
    b.Append("vitamin");
    auto g = Grid("vanmo\nipveo\ntoarr\ntsmed\nmiipb\n");
    vector<TireNode*> tireList;
    tireList.push_back(&a);
    tireList.push_back(&b);
    tireList.push_back(&b);
    tireList.push_back(&b);
    vector<AnswerTree> result;
    search_grid(g,tireList,&result,0);
    vector<string> answer;
    getAnswer(result,&answer);
    //testFunction_PrintGrid(g);
    /*
    vector<Grid> result_grid;
    vector<string> result_string;
    for(int i=0;i<g.nodes.size();i++){
        if(g.nodes.at(i).isValid){
            grid_search_atom(g,i,&result_grid,&result_string,&a,"");
        }
    }

    testFunction_PrintGrid(result_grid.at(0));
    cout<<endl;
    result_grid.at(0).UnlinkInValid();
    auto temp_grid = result_grid.at(0);
    result_grid.clear();
    result_string.clear();
    for(int i=0;i<g.nodes.size();i++){
        if(g.nodes.at(i).isValid){
            grid_search_atom(temp_grid,i,&result_grid,&result_string,&b,"");
        }
    }
    testFunction_PrintGrid(result_grid.at(0));
    /*
    for(int i=0; i < result_string.size();i++){
        cout<<result_string.at(i)<<endl;
        testFunction_PrintGrid(result_grid.at(i));
    }*/
}