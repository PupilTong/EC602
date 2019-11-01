#include <string>
using std::string;
class StringIntenger:public string{
    public:
        StringIntenger(string num){
            for (int i = 0; i < num.length(); i++){
                push_back(num.at(i) - '0');
            }
        }
        StringIntenger(){
        }
        StringIntenger operator = (const string & num){
            return StringIntenger(num);
        }

        StringIntenger operator << (const int & shift){
            for(int i =0; i<shift;i++){
                push_back(0);
            }
            return *this;
        }
        StringIntenger operator * (const int& num2) {
            StringIntenger result = *this;
            int carry = 0, r = 0;
            for(int i=0; i<result.size();i++){
                r=OnebitMuliplier(result.at(result.size() - i -1),num2,carry);
                result.at(result.size() - i -1)=r;
            }
            if(carry!=0){
                result.insert(result.begin(),carry);
            }
            return result;
        }
        StringIntenger operator * (const StringIntenger& num2) {
            StringIntenger result;
            for(int i = 0; i < num2.size(); i++){
                result = result + (((*this) * (num2.at(num2.size() - i - 1)))<<i);
            }
            return result;
        }
        StringIntenger operator + (const StringIntenger & num2){
            StringIntenger result;
            StringIntenger num1 = *this;
            int a, b,carry =0,r;
            for(int i = 0;  i < (num1.length()>num2.length()?num1.length():num2.length()) + 1; i++){
                if(i >= num1.length()){
                    a = 0;
                }
                else{
                    a = num1.at(num1.length() - 1 - i);
                }
                if(i >= num2.length()){
                    b = 0;
                }
                else{
                    b = num2.at(num2.length() - 1 - i);
                }

                r = OnebitAdder(a, b,carry);
                result.insert(result.begin(), r);
            }
            while(result.size()>1 && result.at(0)==0){
                result.erase(result.begin());
            }
            return result; 
        }
        string ToString(){
            for(int i=0;i<length();i++){
                at(i)=at(i)+'0';
            }
            return data();
        }

    private:
        int OnebitMuliplier(int a, int b,  int& cin){
            int r = 0; 
            r = a*b+cin;
            cin = r / 10;
            r %= 10;
            return r;
        }
        int OnebitAdder(int a, int b, int & cin){
            int r=0;
            r = a+b+cin;
            cin = r / 10;
            r %= 10;
            return r;
        }
};

typedef string BigInt;
BigInt multiply_int(const BigInt &a,const BigInt &b){
    StringIntenger numa = a;
    StringIntenger numb = b;
    return (numa*numb).ToString();
}
