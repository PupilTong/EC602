from sys import argv
import copy
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
    def Append(self, word,possibleChar):
        if(word.__len__()==0):
            self.isWordEnd=True
        else:
            c = word[0]
            if( c in possibleChar):
                word = word[1:]
                #word.erase(word.begin())
                if(self.value.__contains__(c)):#find it
                    self.value[c].Append(word,possibleChar)
                else:
                    self.value[c] = TireNode()
                    self.value[c].Append(word,possibleChar)


tiresList = []
resultList = {}
wordRequirement = []

def grid_search_atom(grid, startIndex,  result_string,tire, currentString,currentDepth, currentRequirement):
    global resultList
    global tiresList
    global wordRequirement
    grid=copy.deepcopy(grid)
    current_Node = grid.nodes[startIndex]
    grid.nodes[startIndex].isValid=False
    currentNodeRequirement = wordRequirement[currentRequirement][currentDepth]
    if(currentNodeRequirement!='*'):
        if(currentNodeRequirement!=current_Node.index):
            return
    if(current_Node.index not in tire.value):
        return
    else:
        tire = tire.value[current_Node.index]
    currentString  += current_Node.index
    if(tire.isWordEnd):
        result_string.append(currentString)
        if(currentRequirement == wordRequirement.__len__() - 1):
            a=""
            for w  in result_string:
                a +=w + " "
            a = a[0:a.__len__()]
            resultList[a]=True
        else:
            grid.UnlinkInValid()
            for i in range(grid.nodes.__len__()):
                if(grid.nodes[i].isValid):
                    grid_search_atom(grid,i,result_string,tiresList[currentRequirement + 1],"",0,currentRequirement + 1)
        result_string.pop()
    else:
        for neighbourIndex  in current_Node.neighbors :
            if(neighbourIndex!=-1):
                neigbour = grid.nodes[neighbourIndex]
                if(neigbour.isValid==True):
                    grid_search_atom(grid,neighbourIndex,result_string, tire,currentString,currentDepth + 1, currentRequirement)
    pass

def get_tires(file_name,words,grid):
    tires = []
    word_lengths={}
    possibleChar = {}
    for node in grid.nodes:
        possibleChar[node.index] = True
    for word in words:
        word_lengths[word.__len__()] = TireNode()
    #small first
    f = open(file_name,'r')
    line = f.readline()
    while line:
        line_length = line.__len__() - 1
        if(  line_length in word_lengths):
            word_lengths[line_length].Append(line[0:line_length],possibleChar)
        line = f.readline()
    f.close()

    for word in words:
        tires.append(word_lengths[word.__len__()])
    return tires
    pass
def search_and_print(grid_string,words,small_list,large_list):
    grid = Grid(grid_string)
    global wordRequirement 
    global tiresList 
    global resultList
    wordRequirement = words
    tiresList = []
    resultList = {}
    tiresList = get_tires(small_list,words,grid)
    for i in range(grid.nodes.__len__()):
        t=[]
        grid_search_atom(grid,i,t,tiresList[0],"",0,0)

    #get_answerList(resultTree,words,resultList)
    for answer in resultList:
        print(answer)
    
    if(resultList.__len__()==0):#cannot find
        tiresList=[]
        tiresList = get_tires(large_list,words,grid)
        for i in range(grid.nodes.__len__()):
            t=[]
            grid_search_atom(grid,i,t,tiresList[0],"",0,0)
        for answer in resultList:
            print(answer)
    #check & generate
    print(".")
    pass
if __name__ == "__main__":
    small_list = argv[1]
    large_list = argv[2]
    grid_size = 0
    grid_string = ""
    while(True):
        try:
            line = input()
            if(line==':w'):
                break
        except EOFError:
            break
            pass
        words = []
        if(grid_size == 0 ):
            grid_string = ""
            grid_size = line.__len__() 
            grid_string += line + "\n"
        else:
            if(grid_size == 1):
                words = line.split()
                search_and_print(grid_string,words,small_list,large_list)
            else:
                grid_string += line + "\n"
            grid_size -=1
        #print(words)
        #print(grid_string)
    pass