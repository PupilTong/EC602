import numpy as np
import sys
#epsilon = 0.00000000001
epsilon = 0.000001
precision = 4
maxValidNumber = 1e+160
class Ball:
    _s = np.array([])
    _v = np.array([])
    _r = 0
    _m = 0
    _life = 0
    _bounces = 0
    _name = ""
    def __init__(self,x,y,z,vx,vy,vz,r,m,name,life):
        self._s = np.array([x,y,z])
        self._v = np.array([vx,vy,vz])
        self._r = float(r)
        self._m = float(m)
        self._life = int(life)
        self._name = name
    pass
    def GetEnergy(self):
        return 0.5*self._m*np.dot(self._v,self._v)
    def Move(self,t):
        self._s = self._s + self._v * t
    pass
    def Collision(self,y):
        temp_v   = self._v - ((self._s-y._s) * ( ((2*y._m)   / (self._m+y._m)) * ( np.dot(self._v-y._v,self._s-y._s) / np.dot(self._s-y._s,self._s-y._s)) ))
        temp_y_v = y._v    - ((y._s-self._s) * ( ((2*self._m)/ (self._m+y._m)) * ( np.dot(y._v-self._v,y._s-self._s) / np.dot(y._s-self._s,y._s-self._s)) ))
        self._v = temp_v
        y._v = temp_y_v
        y._bounces+=1
        self._bounces+=1
    pass
    def ContainerCollision(self):
        self._v   = self._v    - (self._s  *  (2 * ( np.dot(self._v,self._s) / np.dot(self._s,self._s))))
        self._bounces +=1
    pass
    #Calcuate Collision time of this ball and containerR in specific radius
    def PredictContainerCollision(self, containerRadius,antiDuplicateFlag):
        container = Ball(0,0,0,0,0,0,-containerRadius,0,"container",1)
        container._r = - containerRadius
        return self.PredictCollision(x=self,y=container,antiDuplicate=antiDuplicateFlag,isContainer=True)
    #Calcuate Collision time of two balls
    def PredictCollision(self,x,y,antiDuplicate,isContainer=False):
        a = float(np.sum(np.power(x._v - y._v,2)))
        b = float(2 * np.dot(x._v - y._v,x._s - y._s))
        c = float(np.sum(np.power(x._s - y._s,2)) - (x._r+ y._r)**2)
        b2mines4ac = b*b - 4*a*c
        if (b >= 0 and (not isContainer)) or b2mines4ac < 0 or a == 0 :
            return -1
        else:
            t1 = (-b + b2mines4ac**0.5) /(2*a)
            t2 = (-b - b2mines4ac**0.5) /(2*a)
            if(abs(t1)<epsilon):
                t1=0
            if(abs(t2)<epsilon):
                t2=0
            if(antiDuplicate):
                if(t1<epsilon):#t1 inValid
                    t1= maxValidNumber
                if(t2<epsilon):#t2 inValid
                    t2= maxValidNumber
            else:
                if(t1<-epsilon):#t1 inValid
                    t1= maxValidNumber
                if(t2<-epsilon):#t2 inValid
                    t2= maxValidNumber
            if(t1>=maxValidNumber and t2>=maxValidNumber):#both inValid
                return -1#
            if(t1<t2):
                return t1
            else:
                return t2

    def PrintOutInfo(self):      
        #print(self._name," m=" + str(self._m) + " R=" + str(np.round(self._r,6)) + " p=" + str(tuple(np.round(self._s,precision))) + "v=" + str(tuple(np.round(self._v)))," bounces=",str(self._bounces))
        print(self._name,"m=" + str(self._m) + " R=" + str(self._r) + " p=" + str(tuple(self._s)) + " v=" + str(tuple(self._v))," bounces=",str(self._bounces))
    pass
    
    

def InitTimeTable(balls,containerR):
    for i in range(balls.__len__()):
        for j in range(balls.__len__()):
            if i<j:
                continue
            if balls[i]._life == 0 or balls[j]._life ==0:
                timeTable[str(i)+str(j)]=-1
                continue
            if i==j:
                timeTable[str(i)+str(j)] = balls[i].PredictContainerCollision(containerR,antiDuplicateFlag=True)           
            else:
                timeTable[str(i)+str(j)] = balls[i].PredictCollision(balls[i],balls[j],antiDuplicate=True)
pass

def UpdateTimeTable(balls,ballIndex,containerR,periousBall):
    for i in range(balls.__len__()):
        if(i==periousBall):
            isAntiDuplicate=True
        else:
            isAntiDuplicate=False
        if balls[i]._life<=0 or balls[ballIndex]._life<=0:
            if i<ballIndex:
                timeTable[str(ballIndex)+str(i)]=-1
            else:
                timeTable[str(i)+str(ballIndex)]=-1
            continue
        if i==ballIndex:
            timeTable[str(i)+str(i)] = balls[i].PredictContainerCollision(containerR,antiDuplicateFlag=isAntiDuplicate)
        else:
            if(i <ballIndex):
                timeTable[str(ballIndex)+str(i)] = balls[ballIndex].PredictCollision(balls[ballIndex],balls[i],antiDuplicate=isAntiDuplicate)
            else:
                timeTable[str(i)+str(ballIndex)] = balls[ballIndex].PredictCollision(balls[ballIndex],balls[i],antiDuplicate=isAntiDuplicate)
pass

def TimePass(sub,tableRow,tableCol):
    for  i in range(0,tableRow):
        for j in range(0,tableCol):
            if(i<j):
                continue
            if(timeTable[str(i)+str(j)]>=0):
                timeTable[str(i)+str(j)] -= sub
pass

def MoveAll(balls,t):
    for i in range(balls.__len__()):
        balls[i].Move(t)
pass

def GetNextBallsCollTime(number,next):
    next["nextEventTimeSpan"] = -1
    for i in range(0,number):
        for j in range(0,number):
            if(i<j):
                continue
            if(timeTable[str(i)+str(j)]>-epsilon):
                if (next["nextEventTimeSpan"] <= -epsilon):
                    next["nextEventTimeSpan"] = timeTable[str(i)+str(j)]
                    next["eventBallA"] = i
                    next["eventBallB"] = j
                else:
                    if(timeTable[str(i)+str(j)]<next["nextEventTimeSpan"]):
                        next["nextEventTimeSpan"] = timeTable[str(i)+str(j)]
                        next["eventBallA"] = i
                        next["eventBallB"] = j
pass

def GetSystemEnergy(balls):
    energy = 0
    for i in range(balls.__len__()):
        if(balls[i]._life>0):
            energy += balls[i].GetEnergy()
    return energy

def GetSystemMomentum(balls):
    momentum = np.array([0,0,0])
    for i in range(balls.__len__()):
        if(balls[i]._life>0):
            momentum = momentum + np.dot(balls[i]._v , balls[i]._m)
    return momentum

def correctParFunction(wrongPars):
    for i in range(len(wrongPars)):
        p1 = 0
        p2 = 2
        while p2 < len(wrongPars[i])-1:
            temp = wrongPars[i][p1]
            wrongPars[i][p1] = wrongPars[i][p2]
            wrongPars[i][p2] = temp
            p1 += 1
            p2 += 1
        temp = wrongPars[i][p1]
        wrongPars[i][p1] = wrongPars[i][p1+1]
        wrongPars[i][p1+1] = temp
    return wrongPars


def mainStep():
    global timeTable
    timeTable = {}
    inputedBalls=[]
    currentTime=0
    nextColl={
        "nextEventTimeSpan":0,
        "eventBallA":0,
        "eventBallB":0
    }
    containerR = float(sys.argv[1])
    maxBounceTime = int(sys.argv[2])

    #containerR = 2000
    #maxBounceTime = 2

    print("Please enter the mass, radius, x/y/z position, x/y/z velocity and name of each sphere" \
          + "\nWhen complete, use EOF / Ctrl-D to stop entering")
    # 20 1  0 0 0    0 0 1 one
    # 2  5  0 1 100  0 0 0 two
    pars = sys.stdin.read()
    pars_arr = pars.split()
    ##
    counter = 0
    for par in pars_arr:
        if (counter + 1) % 9 != 0:
            pars_arr[counter] = float(par)
        counter += 1
    arrayOfallPars = []
    numOfBalls = len(pars_arr) / 9
    for i in range(int(numOfBalls)):
        cur = pars_arr[i*9 : i*9+9]
        arrayOfallPars.append(cur)
    correctedPar = correctParFunction(arrayOfallPars)

    for ball in range(int(numOfBalls)):
        thisball = Ball(*correctedPar[ball],maxBounceTime)
        inputedBalls.append(thisball)


    print("")
    print("Here are the initial conditions.")
    print("universe radius", str(containerR))
    print("max collisions" ,str(maxBounceTime))
    for b in inputedBalls:
        b.PrintOutInfo()

    print("energy:",str(GetSystemEnergy(inputedBalls)))
    momentum = GetSystemMomentum(inputedBalls)
    print("momentum:" ,str(tuple(momentum)))
    print("")
    print("Here are the events.")
    print("")





    InitTimeTable(inputedBalls,containerR)
    GetNextBallsCollTime(inputedBalls.__len__(),nextColl)
    while (nextColl["nextEventTimeSpan"] >= -epsilon):
        currentTime += nextColl["nextEventTimeSpan"]
        MoveAll(inputedBalls, nextColl["nextEventTimeSpan"])
        if(nextColl["eventBallA"]==nextColl["eventBallB"]):
            inputedBalls[nextColl["eventBallA"]].ContainerCollision()
        else:
            inputedBalls[nextColl["eventBallA"]].Collision(inputedBalls[nextColl["eventBallB"]])
        #===========================================================================

        print("time of event: " + str(round(currentTime,precision)))
        #===========================================================================
        if(nextColl["eventBallA"]!=nextColl["eventBallB"]):
            print("colliding " + inputedBalls[nextColl["eventBallA"]]._name + " " + inputedBalls[nextColl["eventBallB"]]._name)
        else:
            print( "reflecting " + inputedBalls[nextColl["eventBallA"]]._name)
        #===========================================================================
        for i in inputedBalls:
            if(i._life<=0):continue
            i.PrintOutInfo()
        #print("energy: "+str(round(GetSystemEnergy(inputedBalls),precision)))
        print("energy: "+str(GetSystemEnergy(inputedBalls)))
        momentum = GetSystemMomentum(inputedBalls)
        print("momentum: " + str(tuple(momentum)))

        print("")
        if(inputedBalls[nextColl["eventBallA"]]._life==1):
            print("disappear",inputedBalls[nextColl["eventBallA"]]._name)
            print("")
        if(nextColl["eventBallA"]!=nextColl["eventBallB"]):
            if(inputedBalls[nextColl["eventBallB"]]._life==1):
                print("disappear",inputedBalls[nextColl["eventBallB"]]._name)
                print("")

        inputedBalls[nextColl["eventBallA"]]._life-=1
        if(nextColl["eventBallA"]!=nextColl["eventBallB"]):
            inputedBalls[nextColl["eventBallB"]]._life-=1
        
        #===========================================================================
        TimePass(nextColl["nextEventTimeSpan"],inputedBalls.__len__(),inputedBalls.__len__())
        if(nextColl["eventBallA"]!=nextColl["eventBallB"]):
            UpdateTimeTable(inputedBalls,nextColl["eventBallA"],containerR,nextColl["eventBallB"])
        UpdateTimeTable(inputedBalls,nextColl["eventBallB"],containerR,nextColl["eventBallA"])
        #===========================================================================
        GetNextBallsCollTime(inputedBalls.__len__(),nextColl)
        
pass
if __name__ == "__main__":
    mainStep()



        

