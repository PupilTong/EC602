# EC602 Midterm One: Python

# you may make a total of 15 point changes
# in the grading scheme.
from random import uniform as w
grade = {'basesix': 25,
         'all_there': 10,
         'kv_match': 10,
         'half_same': 10,
         'Point': 5,
         'Rect': 20,
         'queenmoves': 20,
         'asciisort': 0}

print(sum(grade.values()))


def basesix(x):
    """convert the integer x into a base-6 string representation of x"""
    s=""

    if(x==0):
        return "0"
    if(x<0):
        s="-"
    x = abs(x)
    while(x>0):
        s=str(x%6)+s
        x=int(x/6)
    return s


def all_there(pool, fish):
    """return True if every element in the sequence fish exists in the sequence pool, 
    return False otherwise"""
    if(len(pool)==0 and len(fish)==0):
        return True
    r=False
    for j in fish:
        r=False
        for i in pool:
            if(i==j):
                r=True
        if(not r):
            return False
    return True


def kv_match(D):
    """for the dictionary D, return the number of keys 
    which are equal to their values"""
    count = 0
    for key,item in D.items():
        if(str(key)==str(item)):
            count = count+1
        
    return count

def half_same(x):
    """return True if the floating point number x can be represented
    using the half-precision number format (16 bits) without any loss of information.
    Assume that x is a double-precision number (64 bits)"""
    x=abs(x)
    #float.hex(x)
    s=float.hex(float(x))
    s=s.split("p")
    if(len(s[0])>6):
        if(ord(s[0][6])>ord("d")):
            return False
        if(s[0][7:]!="0000000000"):
            return False
    if(int(s[1][1:])>15 or int(s[1][1:])<-14):
        return False
    return True
        


class Point():
    """represent a point P(x,y) on the plane"""
    _x=0
    _y=0
    def __init__(self,x=0,y=0):
        self._x=x
        self._y=y
    pass
    def __add__(self, point2):
        r = Point()
        r._x=self._x+point2._x
        r._y=self._y+point2._y
        return r
    def __repr__(self):
        return "P("+str(self._x)+","+str(self._y)+")"


class Rect():
    """represent a rectangle on the plane.
    Rect(left,right,bottom,top) is the syntax to create."""
    _left =0
    _right=0
    _bottom=0
    _top =0
    def __init__(self, left=0,right=0,bottom=0,top=0):
        self._left=left
        self._right=right
        self._bottom=bottom
        self._top=top
    pass
    def __add__(self,rec):
        return Rect(self._left+rec._left,self._right+rec._right,self._bottom+rec._bottom,self._top+rec._top)
    def __repr__(self):
        return "Rect(left=" +str(self._left) + ",right=" + str(self._right) + ",bottom=" +str(self._bottom) + ",top=" + str(self._top) +")"
    def __contains__(self, key):
        if(key._x<self._right and key._x > self._left and key._y<self._top and key._y > self._bottom):
            return True
        return False
def queenmoves(board):
    """board is 8 line string with representing a chessboard
    with a single queen on the board, marked with the letter Q

    Return a new chess board with the same format, but with
    all places on the chess board that the queen can move to
    marked with an "X"

    A queen can move diagonally, vertically, or horizontally
    an arbitrary number of spaces."""
    s=board.split()
    col=0
    row=0
    result=""
    for line in s:
        if(line!="********"):
            for c in line:
                if(c=='Q'):
                    break
                col = col + 1
            break
        row = row+1
    for r in range(8):
        for c in range(8):
            if(r==row and c==col):
                result = result + "Q"
            elif(r==row or c == col):
                result = result + "X"
            elif(abs(r-row)==abs(c-col)):
                result = result +"X"
            else:
                result = result+"*"
        if(r!=7):
            result = result + "\n"
    
    return result


def asciisort(L):
    """sort the list L according to the numerical(ASCII) values
    of its elements as strings"""
    L = L.sort(key=lambda x:ord(str(x)[0]))
    return L

example_board = """********
********
******Q*
********
********
********
********
********"""


def main():
    print('basesix test')
    for i in range(100):
        if int(basesix(i), 6) != i:
            print(f'basesix({i}) is not working')

    print('all_there test')
    print(all_there((1, 2, 3), (3, 4)))       # False
    print(all_there(range(10), range(3, 7)))  # True
    print(all_there("abcd", "BC"))            # False
    print(all_there([], tuple()))             # True
    print(all_there("abcdef", "aaaee"))       # True

    print('kv_match')
    print(kv_match({1: 2, 'a': "a", (1, 2): (1, 2)}))  # 2
    print(kv_match({1: 2, 2: 1}))                      # 0
    print(kv_match({x: x for x in range(100)}))        # 100
    print(kv_match({1: {}, 2: {}, "abc": "abc"}))      # 1

    print('half_same')
    print(half_same(1/5))         # False
    print(half_same(0))           # True
    print(half_same(1023))        # True
    print(half_same(-0.125))      # True
    print(half_same(1.23456789))  # False
    print(half_same(0.5**12))     # True
    print(half_same(100_000))     # False

    print('queenmoves test')
    print(example_board)
    print(repr(example_board))
    print(queenmoves(example_board))

    print('Rect/Point test')
    try:
        special_points = [Point(4, 6), Point(
            5, 8), Point(5, 5), Point(6, 8), Point()]
        r = Rect(4, 6, 1, 8)
        random_points = [Point(w(3.5, 6.5), w(0, 10)) for x in range(10)]
        points = special_points+random_points
        print('testing', points)
        for q in points:
            if q in r:
                print(q, 'is inside of', r)
            else:
                print(q, 'is outside of', r)
        print([r])
    except:
        print('Something is not right with Rect/Point.')
        print('Get rid of try/except to learn more')

    print('asciisort test')
    L = [9, 32, "a", 1, 7, -102, "xyz", 0.1, "0.1", 0.1, "1/10", "X", "\n"]
    print(L)
    n = asciisort(L)
    print(n, L)


if __name__ == '__main__':
    main()