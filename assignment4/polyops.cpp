//Copyright 2019 haoyangw haoyangw@bu.edu
#include <vector>
using std::vector;
///return:{result,carry out}
typedef vector<double> Poly;
Poly operator+ (const Poly &polya, const Poly &polyb) {
    Poly result;    
    vector<double> adderOut = vector<double>({0, 0}); 
    double a, b;
    for(int i = 0;  i < (polya.size() >polyb.size()?polya.size():polyb.size()) + 1; i++){
        if(i >= polya.size()){
            a = 0;
        }
        else{
            a = polya.at(i);
        }
        if(i >= polyb.size()){
            b = 0;
        }
        else{
            b = polyb.at(i);
        }
        result.push_back(a+b);
    }
    while(result.size()>1 && result.at(result.size() - 1)==0){
        result.pop_back();
    }
    return result; 
    

}
Poly operator << (const Poly &polya, const int &shift) {
    Poly result =  polya;
    for (int i = 0; i < shift; i++){
        result.insert(result.begin(),0.0);
        /* code */
    }
    return result;
}

Poly operator * (const Poly &polya, const double &b){
    Poly result;
    for(int i = 0; i < polya.size(); i++){
        result.push_back(polya.at(i)*b);
    }
    return result;
}

Poly operator * (const Poly &polya, const Poly &polyb){
    Poly result;
    for(int i = 0; i < polyb.size(); i++){
        result = result + ((polya * (polyb.at(i)))<<i);
    }
    return result;
}
// Add two polynomials, returning the result
Poly add_poly(const Poly &a,const Poly &b){
    //return a*b;
    return a+b;
}

// Multiply two polynomials, returning the result.
Poly multiply_poly(const Poly &a,const Poly &b){
    return a*b;
    //return a;
}

