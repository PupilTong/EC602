//Copyright 2019 haoyangw haoyangw@bu.edu
// Definitions:
// Rs = factor by which float is better than int at representing small numbers
// Rm = factor by which float is better than int at representing large numbers
// Ri = factor by which int is better than float at representing integers
//
// Formulas:
//
// Rs = 1 / smallest_float_greater_than_zero
// Rm = maximum_float_value / largest_int_value
//
// Ri = largest_int_value / N
// where N is the largest integer such that all integers 1,2,3,...,N can be
// represented without loss of accuracy by a float of this size.

#include <iostream>
#include <cstdint>
#include <cfloat>
#include <cmath>
//using half_float::half;
int main(){


  double Rs,Ri,Rm;


  Rs=Ri=Rm=0;//remove this line

  // calculate Rs, Ri, and Rm for half/binary16 vs int16_t

  float smallest_float_greater_than_zero_16 ;
  float maximum_float_value_16;
  float N_16;
  int16_t maximum_int_value_16 = 0x7fff;
  (*((uint32_t*)(&smallest_float_greater_than_zero_16))) = 0x38800000;
  (*((uint32_t*)(&maximum_float_value_16))) = 0x477fe000;
  N_16 = 2047.0;
  
  Rs = 1.0l / smallest_float_greater_than_zero_16;

  Rm = maximum_float_value_16 / maximum_int_value_16;

  Ri = maximum_int_value_16 / N_16;

  std::cout<< "16 : Ri= " << Ri << " Rm= " << Rm << " Rs= " << Rs << std::endl;
//======================================================================================================

  float smallest_float_greater_than_zero_32 ;
  float maximum_float_value_32;
  float N_32;
  int32_t maximum_int_value_32 = 0x7fffffff;
  (*((uint32_t*)(&smallest_float_greater_than_zero_32))) = 0x00800000;
  (*((uint32_t*)(&maximum_float_value_32))) = 0x7f7fffff;
  N_32 = (0x01<<24)-1;

  Rs = 1.0l / smallest_float_greater_than_zero_32;

  Rm = maximum_float_value_32 / maximum_int_value_32;

  Ri = maximum_int_value_32 / N_32;
  //=======================================================================================================
  // calculate Rs, Ri, and Rm for float/single/binary32 vs int32_t

  double smallest_float_greater_than_zero_64 ;
  double maximum_float_value_64;
  double N_64;
  int64_t maximum_int_value_64 = 0x7fffffffffffffff;
  (*((uint64_t*)(&smallest_float_greater_than_zero_64))) = 0x0010000000000000;
  (*((uint64_t*)(&maximum_float_value_64))) = 0x7fefffffffffffff;
  N_64 = (0x01UL<<53)-1;
  std::cout<< "32 : Ri= " << Ri << " Rm= " << Rm << " Rs= " << Rs << std::endl;

  Rs = 1.0l / smallest_float_greater_than_zero_64;

  Rm = maximum_float_value_64 / maximum_int_value_64;

  Ri = maximum_int_value_64 / N_64;

  // calculate Rs, Ri, and Rm for double/binary64 vs int64_t

  std::cout<< "64 : Ri= " << Ri << " Rm= " << Rm << " Rs= " << Rs << std::endl;
  
  return 0;
}