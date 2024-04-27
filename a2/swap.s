.pos 0x100
                 ld $array, r0          # r0 = address of array[0]
                 ld $9,r1               #r1 = 9
                 ld (r0,r1,4), r1       # r1 = array[9]
                 ld $orig, r2           # r2 = address of orig
                 st r1, (r2)            # orig = array[9]
                 ld $array, r0          # r0 = address of array[0]
                 ld 0x8(r0), r1         # r1 = array[2]
                 ld $9,r2
                 st r1, 0x24(r0)        # array[9] = array[2]
                 ld $orig, r0           # r0 = address of orig
                 ld (r0), r1            # r1 = orig
                 ld $array, r2          # r2 = address of array
                 st r1, 0x8(r2)         # array[2] = orig
                 halt                   # halt



.pos 0x1000
orig:               .long 0x00000000         # orig
.pos 0x2000
array:              
    .long 0            # array[0]
    .long 1            # array[1]
    .long 2            # array[2]
    .long 3            # array[3]
    .long 4            # array[4]
    .long 5            # array[5]
    .long 6            # array[6]
    .long 7            # array[7]
    .long 8            # array[8]
    .long 9            # array[9]
    .long 10           # array[10]