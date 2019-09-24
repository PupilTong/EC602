// Copyright 2019 haoyangw haoyangw@bu.edu
#include <string>
#include <vector>
using std::vector;
using std::string;


bool is_happy(int inputnumber) {
    vector<int> numberArray;
    for(int i = 0; inputnumber > 0; i++) {
        numberArray.insert(numberArray.begin(),  inputnumber%10);
        inputnumber /= 10;
    }
    int r = 0;
    for(int i = 0;  i < numberArray.size(); i++){
        r += numberArray.at(i) * numberArray.at(i);
    }
    if (r==1){
        return true;
    }
    else if(r==4){
        return false;
    }
    else{
        return is_happy(r);
    }
    return false;

}
double product_of_positives(vector<double> seq) {
    double r=1; 
    for(int i=0;  i < seq.size(); i++){
        if(seq.at(i)>0){
            r*=seq.at(i);
        }
    }
    return r;
}

int product_of_positives(vector<int> seq) {
    int r=1; 
    for(int i=0;  i<seq.size(); i++){
        if(seq.at(i)>0){
            r*=seq.at(i);
        }
    }
    return r;
}

vector<int> proper_divisors(int n) {
    vector<int> r = {1};
    for(int i=2; i<n/2+1; i++){
        if(n%i==0){
            r.insert(r.end(), i);
        }
    }
    return r;
}


vector<int> fullAdder(int a, int b,  int cin){
    vector<int> r = vector<int>({0, 0}); 
    r.at(1) = a+b+cin;
    r.at(0) = r.at(1)%10;
    r.at(1) /= 10;
    return r;
}
string add(const string& num1,  const string& num2) {
    vector<int> adderOut = vector<int>({0, 0}); 
    string r;
    int a, b;
    for(int i = 0;  i < (num1.length()>num2.length()?num1.length():num2.length()) + 1; i++){
        if(i >= num1.length()){
            a = 0;
        }
        else{
            a = num1.at(num1.length() - 1 - i) - '0';
        }
        if(i >= num2.length()){
            b = 0;
        }
        else{
            b = num2.at(num2.length() - 1 - i) - '0';
        }

        adderOut = fullAdder(a, b, adderOut.at(1));
        r.insert(r.begin(), adderOut.at(0) + '0');
    }
    if(r.at(0)=='0'){
        r.erase(r.begin());
    }
    return r; 
    
}
/*
int main(){
    //std::cout << add("9999", "999"); 
    proper_divisors(12); 
    getchar(); 
    return 0; 
}*/

