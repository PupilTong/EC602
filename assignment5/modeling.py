#Copyright 2019 haoyangw haoyangw@bu.edu
class Polynomial(dict):

    def __init__(self, data=[]):
        if type(data) == Polynomial:
            for i, v in data.items():
                self[i] = v  # not allowed to use copy

        else:
            lendata = len(list(data))
            for i in range(lendata):
                self[lendata - i - 1] = data[i]

    def __add__(self, poly2):
        poly2 = Polynomial(poly2)
        result = Polynomial()
        for townum in self, poly2:
            for i, value in townum.items():
                if i in result:
                    result[i] += value
                else:
                    result[i] = value
        return result

    def __sub__(self,poly2):
        return self + (-poly2)

    def __neg__(self):
        for i,v in self.items():
            self[i]=-v
        return self

    def __mul__(self, poly2):
        poly2 = Polynomial(poly2)
        result = Polynomial()
        for i2 in poly2:
            for i1 in self:
                if i1 in result:
                    result[i1+i2] += self[i1]*poly2[i2]
                else:
                    result[i1+i2] = self[i1]*poly2[i2]
        return result

    def __getitem__(self, i):
        if i in self:
            return self.get(i)
        else:
            return 0
    
    def deriv(self):
        result = Polynomial()
        for i,v in self.items():
            if i!=0:
                result[i-1]=v*i
        return result

    def eval(self,x):
        result = 0
        for i,v in self.items():
            result += v*(x**i)
        return result

def main():
    a = Polynomial([1, 2, 3])
    a[-1] = 1
    b = Polynomial([2, 1])
    print(a.eval(2))
    pass


if __name__ == "__main__":
    main()
