import numpy as np
class Ball:
    _s = np.array([])
    _v = np.array([])
    _r = 0
    _m = 0
    _life = 0
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
        self._life-=1
        y._life-=1
    pass
    def ContainerCollision(self):
        self._v   = self._v    - (self._s  *  (2 * ( np.dot(self._v,self._s) / np.dot(self._s,self._s))))
        self._life -=1
    pass
    #Calcuate Collision time of this ball and conatiner in specific radius
    def PredictContainerCollision(self, containerRadius):
        container = Ball(0,0,0,0,0,0,-containerRadius,0,"container",1)
        container._r = - containerRadius
        return self.PredictCollision(x=self,y=container,isContainer=True)
    #Calcuate Collision time of two balls
    def PredictCollision(self,x,y,isContainer=False):
        a = float(np.sum(np.power(x._v - y._v,2)))
        b = float(2 * np.dot(x._v - y._v,x._s - y._s))
        c = float(np.sum(np.power(x._s - y._s,2)) - (x._r+ y._r)**2)
        b2mines4ac = b*b - 4*a*c
        if (b > 0 and (not isContainer)) or b2mines4ac < 0 or a == 0 :
            return -1
        else:
            t1 = (-b + b2mines4ac**0.5) /(2*a)
            t2 = (-b - b2mines4ac**0.5) /(2*a)
            #t2 < t1
            if (t1 < 0 and t2 < 0):
                return -1#no answer
            if t1<=0 :
                t1=t2+1
            if t2<=0 :
                t2=t1+1
            if t1<t2:
                return t1
            else:
                return t2
    def PrintOutInfo(self):
        print(" m=" + str(self._m) + " R=" + str(self._r) + " p=" + str(tuple(self._s)) + "v=" + str(tuple(self._v)))
    
    

def InitTimeTable(balls,containerR):
    for i in range(balls.__len__()):
        for j in range(balls.__len__()):
            if i<j:
                continue
            if balls[i]._life == 0 or balls[j]._life ==0:
                timeTable[str(i)+str(j)]=-1
                continue
            if i==j:
                timeTable[str(i)+str(j)] = balls[i].PredictContainerCollision(containerR)           
            else:
                timeTable[str(i)+str(j)] = balls[i].PredictCollision(balls[i],balls[j])
pass

def UpdateTimeTable(balls,ballIndex,containerR):
    for i in range(balls.__len__()):
        if balls[i]._life<=0:
            if i<ballIndex:
                timeTable[str(ballIndex)+str(i)]=-1
            else:
                timeTable[str(i)+str(ballIndex)]=-1
            continue
        if i==ballIndex:
            timeTable[str(i)+str(i)] = balls[i].PredictContainerCollision(containerR)
        else:
            if(i <ballIndex):
                timeTable[str(ballIndex)+str(i)] = balls[ballIndex].PredictCollision(balls[ballIndex],balls[i])
            else:
                timeTable[str(i)+str(ballIndex)] = balls[ballIndex].PredictCollision(balls[ballIndex],balls[i])
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
            if(timeTable[str(i)+str(j)]>0):
                if (next["nextEventTimeSpan"] < 0):
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
def mainStep():
    global timeTable
    timeTable = {}
    inputedBalls=[]
    conatinerR=0
    currentTime=0
    nextColl={
        "nextEventTimeSpan":0,
        "eventBallA":0,
        "eventBallB":0
    }



    one = Ball(0,0,0,0,0,1,1,20,"one",3)
    two = Ball(0,1,100,0,0,0,1,2,"two",3)
    inputedBalls.append(one)
    inputedBalls.append(two)
    conatinerR = 120



    InitTimeTable(inputedBalls,conatinerR)
    GetNextBallsCollTime(inputedBalls.__len__(),nextColl)
    while (nextColl["nextEventTimeSpan"] > 0):
        currentTime += nextColl["nextEventTimeSpan"]
        MoveAll(inputedBalls, nextColl["nextEventTimeSpan"])
        if(nextColl["eventBallA"]==nextColl["eventBallB"]):
            inputedBalls[nextColl["eventBallA"]].ContainerCollision()
        else:
            inputedBalls[nextColl["eventBallA"]].Collision(inputedBalls[nextColl["eventBallB"]])
        #===========================================================================
        TimePass(nextColl["nextEventTimeSpan"],inputedBalls.__len__(),inputedBalls.__len__())
        #===========================================================================
        if(nextColl["eventBallA"]!=nextColl["eventBallB"]):
            UpdateTimeTable(inputedBalls,nextColl["eventBallA"],conatinerR)
        UpdateTimeTable(inputedBalls,nextColl["eventBallB"],conatinerR)
        #===========================================================================

        print("time of event: " + str(currentTime))
        #===========================================================================
        if(nextColl["eventBallA"]!=nextColl["eventBallB"]):
            print("colliding " + inputedBalls[nextColl["eventBallA"]]._name + " " + inputedBalls[nextColl["eventBallB"]]._name)
        else:
            print( "colliding " + inputedBalls[nextColl["eventBallA"]]._name + " " + "container")
        #===========================================================================
        inputedBalls[nextColl["eventBallA"]].PrintOutInfo()
        inputedBalls[nextColl["eventBallB"]].PrintOutInfo()
        print("energy: "+str(GetSystemEnergy(inputedBalls)))
        momentum = GetSystemMomentum(inputedBalls)
        print("momentum:" + str(tuple(momentum)))
        GetNextBallsCollTime(inputedBalls.__len__(),nextColl)
        
pass
if __name__ == "__main__":
    mainStep()


        

