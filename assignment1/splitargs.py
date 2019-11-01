
import sys
for i in range(1,len(sys.argv)):
    sys.stdout.write(sys.argv[i] + '\n')
    #print(sys.argv[i] + '\n',file=sys.stderr)
    if i>3:
        break
if len(sys.argv)>4:
    for i in range(5,len(sys.argv)):
        sys.stderr.writelines(sys.argv[i] + '\n')
        
        