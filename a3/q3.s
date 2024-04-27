ld $a,r0 #r0 = &a
ld $3,r1 #r1 = 3
st r1,(r0) # a = 3

ld $p,r1 #r1 = &p
st r0,(r1) # p = &a

ld (r1),r2
ld (r2),r2 #r2 = *p
dec r2 # r2 = *p -1

ld (r1),r3 #r3 = memory location address to which p is pointing
st r2,(r3)

ld $b,r0
ld $p,r1
st r0,(r1) # p = &b[0]
ld (r1),r1 #r1 = contains memory location to which p is pointing
ld $4,r2
add r2,r1  #r1 = r1 + 4 = p++
ld $p,r3
st r1,(r3)

ld $a,r3
ld (r3),r3
ld (r0,r3,4),r4 #r4 = b[a]
ld $p,r1
ld (r1),r1 #r1 = contains memory location to which p is pointing
st r4,(r1,r3,4) #p[a] = b[a]

ld $0,r3
ld (r0,r3,4),r4 #r4 = b[0]

ld $3,r5
shl $2,r5
add r5,r1 # r1 = r1 + 12 = &p[3]
st r4,(r1) # *(p+3) = b[0]
halt



.pos 0x200
a:  .long 0             # tos
p:  .long 2             # tmp
b:  .long 0             # b[0]
    .long 1             # b[1]
    .long 2             # b[2]
    .long 3             # b[3]
    .long 4             # b[4]