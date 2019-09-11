#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Sep  9 15:12:13 2019

@author: ece-student
"""
def is_happy(x):
    x=str(x)
    r=0
    for i in range(0,len(x)):
        r+=int(x[i])**2;
    if r == 1:
        return True
    elif r == 4:
        return False
    elif r == 0 :
        print("error! 0")
    else:
        return is_happy(r)


def product_of_positives(seq):
  """return the product of all the positive numbers in the sequence seq"""
  r=0
  for i in range(0,len(seq)):
      if seq[i]>0:
          r*= seq[i]
  return r

def proper_divisors(n):
    r=[]
    for i in range(1,n):
        if(n%i==0):
            r.append(i)
    return r
"""return all proper divisors of n in a tuple and in numerical order"""

if __name__ == '__main__':
	# your test code here.
    """    
    print("test")
    for i in range(1,101):
        if(is_happy(i)):
            print(i)
            """        
    print(proper_divisors(20))
    pass