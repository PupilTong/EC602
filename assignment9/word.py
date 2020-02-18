import copy
from sys  import argv
import sys
class Node:
#0 top, 1 tr, 2 right, 3 rb, 4 bottom, 5 bl, 6 left,7  lt
    def __init__(self,c):
        self.neighbors = []
        self.index=''
        self.isValid=True
        self.index=c
        for i  in range(8):
            self.neighbors.append(-1)
    pass
class Grid:
    def __init__(self,gridString):
        self.colLength = 0
        self.rowHeight=0
        self.nodes=[]

        row=0
        col=0
        for c in gridString:
            if(c!='\n'):
                self.nodes.append(Node(c))
        for i in range(gridString.__len__()):
            if (gridString[i] != '\n'):
                #create links between self.nodes
                colLength = self.colLength
                if(col!=0):
                    self.nodes[row*colLength + col-1].neighbors[2]=i - row#set previous link to right
                    self.nodes[i-row].neighbors[6] = row*colLength + col-1#set this node link to left
                if(row!=0):
                    self.nodes[(row-1)*colLength + col].neighbors[4]=i - row
                    self.nodes[i - row].neighbors[0]=(row-1)*colLength + col#top link
                    if(col<colLength - 1):
                        self.nodes[(row-1)*colLength+col + 1].neighbors[5]=i-row
                        self.nodes[i-row].neighbors[1]=(row-1)*colLength  + col+1#top right link
                if(col!=0 and row!=0):
                    self.nodes[(row-1)*colLength + col-1].neighbors[3]=i-row
                    self.nodes[i-row].neighbors[7]=(row-1)*colLength + col-1#top left link
                col+=1
            else:
                self.colLength = col
                col=0
                row+=1
        self.rowHeight = row
    def UnlinkInValid(self):
        colLength = self.colLength
        rowHeight = self.rowHeight
        for c in range(colLength):
            temp_one_col = []
            for  r in range(rowHeight):
                node=self.nodes[r*colLength + c]
                if(node.isValid):
                    temp_one_col.append(node.index)
            for r in range (rowHeight -1, -1,-1):
                if(temp_one_col.__len__()>0):
                    self.nodes[r*colLength + c].index = temp_one_col[temp_one_col.__len__() - 1]
                    self.nodes[r*colLength + c].isValid = True
                    temp_one_col.pop()
                else:
                    self.nodes[r*colLength + c].isValid = False
class TireNode(object):
    #unordered_map<char,TireNode*> value
    def __init__(self):
        self.value = {}
        self.isWordEnd=False
        pass
    def Append(self, word):
        if(word.__len__()==0):
            self.isWordEnd=True
        else:
            c = word[0]
            word = word[1:]
            #word.erase(word.begin())
            if(self.value.__contains__(c)):#find it
                self.value[c].Append(word)
            else:
                self.value[c] = TireNode()
                self.value[c].Append(word)
class AnswerTree:
    def __init__(self, v = ""):
        self.value = v
        self.children = []

def testFunction_PrintGrid(g:Grid):
    for i in range(g.nodes.__len__()):
        node = g.nodes[i]
        if(node.isValid):
            validFlag = " "
        else:
            validFlag = "*"
        print(node.index + validFlag,end="")
        if(i%g.rowHeight==g.rowHeight - 1):
            print()
    
def grid_search_atom(g, startIndex:int,result_grid,result_string,tire, currentString=""):
    grid = copy.deepcopy(g)
    startIndex = int(startIndex)
    current_Node = grid.nodes[startIndex]
    grid.nodes[startIndex].isValid=False#set search flag
    if(not (current_Node.index in tire.value)):
        #find failed
        return 
    else:
        newtire = tire.value[current_Node.index]
    currentString  += current_Node.index
    if(newtire.isWordEnd):
        result_string.append(currentString)
        result_grid.append(copy.deepcopy(grid))
        return
    for neighbourIndex in current_Node.neighbors :
        if(neighbourIndex!=-1):#check if neighbor is exist
            neigbour = grid.nodes[int(neighbourIndex)]
            if(neigbour.isValid==True):#un searched
                #then go searching
                grid_search_atom(grid,neighbourIndex,result_grid,result_string, newtire,currentString)
                
def search_grid(g:Grid, tire,result, currentDepth):
    result_grid = []
    result_string = []
    for i  in range(g.nodes.__len__()):
        if(g.nodes[i].isValid):
            grid_search_atom(g,i,result_grid,result_string,tire[currentDepth],"")
    for  i in range(result_grid.__len__()):
        result_grid[i].UnlinkInValid()
        result.append(AnswerTree(result_string[i]))
        if(currentDepth != tire.__len__()- 1):
            search_grid(result_grid[i],tire,result[result.__len__() - 1].children,currentDepth + 1)
def get_answerList(result,words,resultList,currentString, depth):
    pass
def get_tires(file_name,words):
    tires = []
    word_lengths={}
    for word in words:
        word_lengths[word.__len__()] = TireNode()
    #small first
    f = open(file_name,'r')
    line = f.readline()
    while line:
        line_length = line.__len__() - 1
        if(  line_length in word_lengths):
            word_lengths[line_length].Append(line[0:line_length])
        line = f.readline()
    f.close()

    for word in words:
        tires.append(word_lengths[word.__len__()])
    return tires
    pass
def search_and_print(grid_string,words,small_list,large_list):
    grid = Grid(grid_string)
    tires = []
    resultTree = []
    resultList = []
    tires = get_tires(small_list,words)
    search_grid(grid, tires,resultTree,0)

    #get_answerList(resultTree,words,resultList)
    for answer in resultList:
        print(answer)
    
    if(resultList.__len__()==0):#cannot find
        tires = get_tires(large_list,words)
        search_grid(grid, tires,resultTree,0)
        resultList = get_answerList(resultTree,words)
        for answer in resultList:
            print(answer)
    #check & generate
    pass
if __name__ == "__main__":
    # a = TireNode()
    # a.Append("post")
    # a.Append("pots")
    # a.Append("stop")
    # a.Append("opts")
    # b = TireNode()
    # b.Append("bedroom")
    # b.Append("vampire")
    # b.Append("vitamin")
    # g = Grid("vanmo\nipveo\ntoarr\ntsmed\nmiipb\n")
    # tireList = []
    # tireList.append(a)
    # tireList.append(b)
    # tireList.append(b)
    # tireList.append(b)
    # result = []
    # search_grid(g,tireList,result,0)
    small_list = argv[1]
    large_list = argv[2]
    puzzles_lines = sys.stdin.read().split("\n")
    grid_size = 0
    grid_string = ""
    for i in range(puzzles_lines.__len__()):
        words = []
        if(grid_size == 0 ):
            grid_string = ""
            grid_size = puzzles_lines[i].__len__() 
            grid_string += puzzles_lines[i] + "\n"
        else:
            if(grid_size == 1):
                words = puzzles_lines[i].split()
                search_and_print(grid_string,words,small_list,large_list)
            else:
                grid_string += puzzles_lines[i] + "\n"
            grid_size -=1
        #print(words)
        #print(grid_string)
    pass