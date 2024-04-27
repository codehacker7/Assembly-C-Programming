.pos 0x100
        ld $a,r0
        ld (r0),r1 #r1 = value of a
        ld $c,r2
        ld (r2,r1,4),r3 #r3 = c[a]
        inc r1
        ld (r2,r1,4),r4 #r4 = c[a + 1]
        add r3,r4   #r4 = c[a] + c[a + 1]
        ld $i,r5
        st r4,(r5) 
        ld $0xff,r0
        ld (r5),r1 #r1 = i 
        and r0,r1 #r1 = c[a] + c[a + 1]
        ld $n,r2
        st r1,(r2)

.pos 0x1000
a:               .long 0x00000000         # a
.pos 0x2000
n:               .long 1        # n
.pos 0x3000
i:               .long 2        # i
.pos 0x400
c:              
    .long 0            # array[0]
    .long 1            # array[1]
    .long 2            # array[2]
    .long 3            # array[3]
    .long 4            # array[4]
    .long 5            # array[5]