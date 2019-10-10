//Copyright 2019 haoyangw haoyangw@bu.edu
//Copyright 2019 kefanzhang kefan29@bu.edu

double largest_double(){
    double r;
    (*((unsigned long*)(&r))) = 0x7fefffffffffffff;
    return r;
}

double smallest_double(){

    double r;
    (*((unsigned long*)(&r))) = 0x0000000000000001;
    return r;
}
  
float largest_single(){
    float maximum_float_value_32;
    (*((int*)(&maximum_float_value_32))) = 0x7f7fffff;
    return maximum_float_value_32;
}

float smallest_single(){
    float smallest_float_greater_than_zero_32;
    (*((int*)(&smallest_float_greater_than_zero_32))) = 0x00000001;
    return smallest_float_greater_than_zero_32;
}