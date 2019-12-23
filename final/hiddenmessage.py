#!/usr/bin/env python3
from sys import argv
if __name__ == "__main__":
    inputFile = argv[1]
    f = open(inputFile, 'r')
    codes = f.read().split()
    codes.append('-1')
    result = ''
    count = 0
    r = list(range(ord('a'),ord('z')+1))
    for code in codes:
        if(count == 25):
            result+= chr(r[0])
            r = list(range(ord('a'),ord('z')+1))
            count = 0
        if(code=='-1'):
            break
        if(int(code)<ord('a') or int(code)>ord('z')):
            result +=str(chr(int(code)))
        else:
            r.remove(int(code))
            count +=1
    print(result)
        
pass
