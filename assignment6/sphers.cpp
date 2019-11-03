// Copyright 2019 zhou shen zhoushen@bu.edu

#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <boost/algorithm/string.hpp>
//sudo apt-get install libboost-dev

#define EasyWay

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using namespace std;

class Vector3D{
    private:
    public:
        double _x;
        double _y;
        double _z;

        Vector3D(){
            _x = 0;
            _y = 0;
            _z = 0;
        }
        Vector3D(double x, double y, double z){
            _x = x;
            _y = y;
            _z = z;
        }
        ~Vector3D(){

        }
        double operator* (Vector3D b){
            return _x * b._x + _y * b._y + _z * b._z;
        }
        Vector3D operator* (double b){
            return Vector3D(_x * b, _y * b, _z * b);
        }
        Vector3D operator+ (Vector3D b){
            return Vector3D(_x + b._x, _y + b._y, _z + b._z);
        }
        Vector3D operator- (Vector3D b){
            return Vector3D(_x - b._x, _y - b._y, _z - b._z);
        }
        Vector3D CrossProdect(Vector3D v){
            return Vector3D(_y*v._z - _z*v._y, _z*v._x - _x*v._z, _x*v._y - _y*v._x );
        }
        double Sum(){
            return _x + _y + _z;
        }
        double Pow2(){
            return _x*_x + _y*_y + _z*_z;
        }
};
#ifndef EasyWay
class Matrix3x3{
    private:
        vector<vector<double>> _data;
        int _row,_col;
    public:
        Matrix3x3(){
            
        };
        Matrix3x3(int row, int col){
            _row = row;
            _col = col;
            _data.resize(row);
            for (int i = 0; i < row; i++)
            {
                _data[i].resize(col);
            }
        };
        double Determinant(){
            //O(n!) function
             return _data[0][0]*_data[1][1]*_data[2][2] + _data[0][1]*_data[1][2]*_data[2][0] + _data[0][2]*_data[1][0]*_data[2][1]
                -_data[0][2]*_data[1][1]*_data[2][0] - _data[0][0]*_data[1][2]*_data[2][1] - _data[0][1]*_data[1][0]*_data[2][2];
        }
        double Determinant2x2(int row, int col){
            auto temp = vector<vector<double>>();
            temp.resize(2);
            temp[0].resize(2);
            temp[1].resize(2);
            int a=0;
            for (int i = 0; i < _row; i++){
                int b=0;
                if(i==row)continue;
                for (int j = 0; j < _col; j++){
                    if(j==col)continue;
                    temp.at(a).at(b)=_data[i][j];
                    b++;
                }
                a++;
            }
            return  (((row+col)%2==1)?-1:1) * (temp[0][0]*temp[1][1] - temp[0][1]*temp[1][0]);
        }
        double Get(int row, int col){
            if(row<_row && col< _col){
                return _data[row][col];
            }
            else{
                return 0;
            }
            
        }
        Matrix3x3 inverse(){
            auto det = Determinant();
            Matrix3x3 temp = Matrix3x3(3,3);
            for (int i = 0; i < _row; i++){
                for (int j = 0; j < _col; j++){
                    temp._data[j][i] = Determinant2x2(i,j) / det;
                }
                
            }
            return temp;
            
        }
        void TransportMe(){
            for (int i = 0; i < _row; i++){
                for (int j = 0; j < _col; j++){
                    if(i<j)continue;
                    double temp = _data[i][j];
                    _data[i][j]=_data[j][i];
                    _data[j][i]=temp;
                }
            }
        }
        void Set(int row, int col,double value){
            _data[row][col] = value;
        }
        Matrix3x3 operator/ (double div){
            for (int i = 0; i < _row; i++){
                for(int j = 0; j < _col ; j++){
                    _data[i][j] = _data[i][j] / div;
                }
            }
            
        }
        Vector3D operator* (Vector3D mul){
            double x,y,z;
            x = mul._x*_data[0][0] + mul._y*_data[0][1] + mul._z*_data[0][2];
            y = mul._x*_data[1][0] + mul._y*_data[1][1] + mul._z*_data[1][2];
            z = mul._x*_data[2][0] + mul._y*_data[2][1] + mul._z*_data[2][2];
            return Vector3D(x,y,z);
        }
        ~Matrix3x3(){

        }
};
#endif
class Ball{
private:
public:
    Vector3D _s;
    Vector3D _v;
    double _m;//mass
    double _r;
    int _life;
    string _name;
    ~Ball(){
    }

    Ball(string name){
        _s._x = 0;
        _s._y = 0;
        _s._z = 0;
        _v._x = 0;
        _v._y = 0;
        _v._z = 0;
        _r = 0;
        _m = 0;
        _name = name;
        _life = INT32_MAX;
    }
    Ball(double x, double y, double z, double vx, double vy, double vz, double r, double m, string name,int life){
        _s._x = x;
        _s._y = y;
        _s._z = z;
        _v._x = vx;
        _v._y = vy;
        _v._z = vz;
        _r = r;
        _m = m;
        _name = name;
        _life = life;
    }    
    Ball(Vector3D p, Vector3D v, double r, double m, string name,int life){
        _s = p;
        _v = v;
        _r = r;
        _m = m;
        _name = name;
    }
    double GetEnergy(){
        return 0.5*_m*(_v*_v);
    }
    Vector3D GetPosition(double t){
        return _v*t + _s;
    }
    void Move(double t){
        _s = _s + _v * t;
    }
    void Collision(Ball& y){
        auto temp_v   = _v    - ((_s-y._s)   * ( ((2*y._m) / (_m+y._m)) * ( ((_v-y._v)*(_s-y._s)) / ((_s-y._s)*(_s-y._s))) ));
        auto temp_y_v = y._v  - ((y._s - _s) * ( ((2*_m)   / (_m+y._m)) * ( ((y._v-_v)*(y._s-_s)) / ((y._s-_s)*(y._s-_s))) ));
        _v = temp_v;
        y._v = temp_y_v;
        _life--;
        y._life--;
    }
#ifndef EasyWay
    void ContainerCollision(){
        auto linearlyIndependent = vector<Vector3D>();
        linearlyIndependent.resize(3);
        linearlyIndependent[0] = _s;
        if(_s._x != 0){
            linearlyIndependent[1] = Vector3D(-(_s._y+_s._z)/_s._x,1,1);
        }
        else{
            if(_s._y!=0){
                linearlyIndependent[1] = Vector3D(1,-(_s._x + _s._z)/_s._y,1);
            }
            else{
                linearlyIndependent[1] = Vector3D(1,1,-(_s._x+_s._y)/_s._z);
            }
        }
        linearlyIndependent[2]= _s.CrossProdect(linearlyIndependent[1]);
        auto linearlyIndependentGroup = Matrix3x3(3,3);
        for (int i = 0; i < 3; i++){
            linearlyIndependentGroup.Set(i,0,linearlyIndependent[i]._x);
            linearlyIndependentGroup.Set(i,1,linearlyIndependent[i]._y);
            linearlyIndependentGroup.Set(i,2,linearlyIndependent[i]._z);
        }
        linearlyIndependentGroup.TransportMe();
        auto solve = linearlyIndependentGroup.inverse() * _v;
        solve._x = -solve._x;
        _v = linearlyIndependentGroup * solve;
        _life = _life - 1;
    }
#else
    void ContainerCollision(){
        //auto y = Vector3D(0 ,0 ,0);
        auto temp_v   = _v    - (_s  *  (2 * ( (_v*_s) / (_s*_s))));
        _v = temp_v;
        _life--;
    }
#endif
    ///Calcuate Collision time of this ball and conatiner in specific radius
    double PredictContainerCollision(double containerRadius){
        auto container = Ball("container");
        container._r = - containerRadius;
        return PredictCollision(*this, container,true);
    }
    ///Calcuate Collision time of two balls
    static double PredictCollision(Ball& x, Ball& y,bool isContainer=false){
        double a = (x._v - y._v).Pow2();
        double b = 2 * ((x._v - y._v) * (x._s - y._s));
        double c = (x._s - y._s).Pow2() - ((x._r+ y._r)*(x._r+ y._r));
        double b2mines4ac = b*b - 4*a*c;
        double t1,t2;
        if((b >= 0 && !isContainer) || b2mines4ac < 0|| a == 0){
            return -1;
        }
        else{
            t1 = (-b + sqrt(b2mines4ac)) /(2*a);
            t2 = (-b - sqrt(b2mines4ac)) /(2*a);
            //t2 < t1
            if(t1 < 0&& t2 < 0){
                return -1;//no answer
            }
            if(t1<=0)t1=t2+1;
            if(t2<=0)t2=t1+1;
            return t1<t2?t1:t2;
        }
    }
    void PrintOutInfo(){
        cout<< _name << " m=" << _m << " R=" << _r << " p=(" << _s._x <<"," << _s._y << "," << _s._z <<") v=(" << _v._x <<"," << _v._y <<"," <<_v._z <<")" <<endl;
    }
};
vector<vector<double>> timeTable;
void InitTimeTable(vector<Ball>& balls,int number,int containerR){
    timeTable = vector<vector<double>>();
    timeTable.resize(number);
    for(int i=0;i<number;i++)timeTable[i].resize(i+1);
    for (int i = 0; i < number; i++){
        for (int j = 0; j < number; j++){
            if(i<j)continue;
            if(balls.at(i)._life == 0 || balls.at(j)._life ==0){
                timeTable[i][j]=-1;
                continue;
            }
            if(i==j){
                timeTable[i][j] = balls[i].PredictContainerCollision(containerR);
            }            
            else{
                timeTable[i][j] = balls[i].PredictCollision(balls[i],balls[j]);
            }
            
        }
        
    }
}
void UpdateTimeTable(vector<Ball>& balls,int ballIndex,int number, int containerR){
    for (int i = 0; i < number; i++){
        if(balls[i]._life<=0 || balls[ballIndex]._life <= 0){
            timeTable[ballIndex][i]=-1;
            timeTable[i][ballIndex]=-1;
            continue;
        };
        if(i==ballIndex){
            timeTable[i][i] = balls.at(i).PredictContainerCollision(containerR);
        }
        else{
            if(i <ballIndex){
                timeTable[ballIndex][i] = balls.at(ballIndex).PredictCollision(balls.at(ballIndex),balls.at(i));
            }
            else{
                timeTable[i][ballIndex] = balls.at(ballIndex).PredictCollision(balls.at(ballIndex),balls.at(i));
            }
        }
    }
}
void TimePass(double sub,int tableRow, int tableCol){
    for (int i = 0; i < tableRow; i++){
        for (int j = 0; j < tableCol; j++){
            if(i<j)continue;
            if(timeTable[i][j]>=0){
                timeTable[i][j] -= sub;
            }
        }
    }
}
void MoveAll(vector<Ball>& balls,double t){
    for (int i = 0; i < balls.size(); i++)
    {
        balls.at(i).Move(t);
    }
}
void GetNextBallsCollTime(int number,int& ballA,int& ballB, double& time){
    time = -1;
    for (int i = 0; i < number; i++){
        for (int j = 0; j < number; j++){
            if(i<j)continue;
            if(timeTable[i][j]>0){
                if (time < 0){
                    time = timeTable[i][j];
                    ballA = i;
                    ballB = j;
                }
                else{
                    if(timeTable[i][j]<time){
                        time = timeTable[i][j];
                        ballA = i;
                        ballB = j;
                    }
                }
                
            }
        }
    }
    
}

double GetSystemEnergy(vector<Ball>& balls){
    double energy = 0;
    for(int i=0;i<balls.size();i++){
        if(balls.at(i)._life>0){
            energy += balls.at(i).GetEnergy();
        }
    }

    return energy;
}
Vector3D GetSystemMomentum(vector<Ball>& balls){
    auto momentum = Vector3D();
    for(int i=0;i<balls.size();i++){
        if(balls.at(i)._life>0){
            momentum = momentum + (balls.at(i)._v * balls.at(i)._m);
        }
    }
    return momentum;

}





int main(int argc, char **argv) {
  // get container radius and life from arguments
  int containerR = 0;
  int life = 0;
  // exclude argv bracket_left 0 bracket_right, since it is for the name of the program
  for(int i = 1; i < argc; ++i){
    if(i == 1){
      containerR = stoi(argv[i]);  //atof(): convert string to double
      //cout << *(argv+i) << "\n";
    }
    else if(i == 2){
      life = stoi(argv[i]);
    }
    else{
      cerr << *(argv+i) << ", Too many arguments" << "\n";
      exit(0);  //program is terminated here
    }
  }

  double currentTime = 0;
  double nextEventTimeSpan;
  int eventBallA, eventBallB;

  // Ask user to type in parameters
  cout << "Please enter the mass, radius, x/y/z position, x/y/z velocity" << endl;
  cout << "and name of each sphere." <<endl;
  cout << "When complete, use EOF / Ctrl-D to stop entering" << endl;

  // Vector<> for balls
  vector<Ball> inputedBalls; 

  int lineLen = 0;
  string thisBall;
  while(getline(cin, thisBall)){
    //get each element from thisBall
    vector<string> line;

    boost::split(line, thisBall, boost::is_any_of("\t "), boost::token_compress_on);

    //check for invalid input
    if(line.size() != 9){
      cout << "invalid input" <<endl;
      exit(0);
    }


    //cout << line[i] << endl;
    string name = line[8];
    double curMass = stod(line[0]);
    double curRadius = stod(line[1]);

    double posX = stod(line[2]);
    double posY = stod(line[3]);
    double posZ = stod(line[4]);

    double velX = stod(line[5]);
    double velY = stod(line[6]);
    double velZ = stod(line[7]);

    //initialize each ball
    auto ball = Ball(posX, posY, posZ, velX, velY, velZ, curRadius, curMass, name, life);
    inputedBalls.push_back(ball);
  }

  cout<< "test" << endl;
  InitTimeTable(inputedBalls,inputedBalls.size(),containerR);
  cout<< "test2" << endl;
  GetNextBallsCollTime(inputedBalls.size(),eventBallB,eventBallA,nextEventTimeSpan);

  while (nextEventTimeSpan > 0){
    currentTime += nextEventTimeSpan;
    MoveAll(inputedBalls, nextEventTimeSpan);
    //===========================================================================
    if(eventBallA==eventBallB){
      inputedBalls.at(eventBallA).ContainerCollision();
    }
    else{
      inputedBalls.at(eventBallA).Collision(inputedBalls.at(eventBallB));
    }
    
    //===========================================================================
    TimePass(nextEventTimeSpan,inputedBalls.size(),inputedBalls.size());
    //===========================================================================
    if(eventBallA!=eventBallB){
      UpdateTimeTable(inputedBalls,eventBallA,inputedBalls.size(),containerR);
    }
    UpdateTimeTable(inputedBalls,eventBallB,inputedBalls.size(),containerR);
    //===========================================================================

    cout<< "time of event: " << currentTime << endl;
    //===========================================================================
    if(eventBallA!=eventBallB){
      cout<< "colliding " << inputedBalls.at(eventBallA)._name << " " << inputedBalls.at(eventBallB)._name<<endl;
    }else{
      cout<< "colliding " << inputedBalls.at(eventBallA)._name << " " << "container"<<endl;
    }
    
    //===========================================================================
    inputedBalls.at(eventBallA).PrintOutInfo();
    inputedBalls.at(eventBallB).PrintOutInfo();
    cout << "energy: "<<GetSystemEnergy(inputedBalls)<<endl;
    auto momentum = GetSystemMomentum(inputedBalls);
    cout <<"momentum: ("<<momentum._x <<"," <<momentum._y <<"," <<momentum._z <<")"<<endl;
    GetNextBallsCollTime(inputedBalls.size(),eventBallB,eventBallA,nextEventTimeSpan);
  }
  

  return 0;
}




