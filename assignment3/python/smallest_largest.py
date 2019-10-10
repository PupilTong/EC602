# Copyright 2019 haoyangwang haoyangw@bu.edu
# Copyright 2019 KefanZhang kefan29@bu.edu
    #https://www.technical-recipes.com/2012/converting-between-binary-and-decimal-representations-of-ieee-754-floating-point-numbers-in-c/
    #https://docs.python.org/2/library/struct.html
def largest_double():
    #return struct.unpack('d',struct.pack('q',int('0x7fefffffffffffff',16)))[0]
    #fromhex()@ https://docs.python.org/3/library/stdtypes.html
    return float.fromhex('0x1.fffffffffffffp+1023')
    pass

def smallest_double():
    #return struct.unpack('d',struct.pack('q',int('0x0000000000000001',16)))[0]
    return float.fromhex('0x0.0000000000001p-1022')
    pass
  
def largest_single():
    #return struct.unpack('f',struct.pack('I',int('0x7f7fffff',16)))[0]
    return float.fromhex('0x1.fffffep127')
    pass

def smallest_single():
    #return struct.unpack('f',struct.pack('I',int('0x00000001',16)))[0]
    return float.fromhex('0x0.000001p-125')
    pass


def main():
  print(largest_double())
  print(smallest_double())
  print(largest_single())
  print(smallest_single())

if __name__ == '__main__':
  main()