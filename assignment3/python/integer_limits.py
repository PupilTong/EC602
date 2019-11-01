Table = "{:<6} {:<22} {:<22} {:<22}"
print(Table.format('Bytes','Largest Unsigned Int','Minimum Signed Int','Maximum Signed Int'))
for i in range(1,9):
    print(Table.format(i,((0x01)<<(i*8))-1,-(int)((0x01)<<(i*8-1)),(int)((0x01)<<(i*8-1))-1))