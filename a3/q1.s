
.pos 0x100
        ld $h, r0           # r0 = address of h
        ld 12(r0), r1       # r1 = h[3]
        ld $u, r2           # r2 = address of u
        st r1, 0(r2)        # u = h[3]

        ld 0(r2), r1        # r1 = u
        ld (r0, r1, 4), r1  # r1 = h[u]
        st r1, 0(r2)        # u = h[u]

        ld $g, r0           # r0 = address of g
        ld $d, r1           # r1 = address of d
        st r0, 0(r1)        # d = &g

        ld $5, r2           # r2 = 5
        ld 0(r1), r3        # r3 = address of value in d
        st r2, 0(r3)        # *d = 5

        ld $h, r0           # r0 = address of h
        ld $1,r1            # r1 = 1
        ld (r0,r1,4), r1    # r1 = h[1]
        shl $2,r1           # r1 = 4 * r1
        add r0,r1           # r1 = &h[h[1]]
        ld $d,r3            # r3 = &d
        st r1,(r3)          # d = &h[h[1]]

        ld $d,r0            #r0 = &d
        ld (r0),r0          # r0 = contains the memory location address to which d is pointing =  d = &h[h[1]]
        ld (r0),r0          # r0 = *d
        ld $h,r1            # r1 = &h
        ld $4,r2            # r2 = 4
        ld (r1,r2,4),r3     # r3 = h[4]
        add r0,r3           # r3 = *d + h[4]
        ld $d,r0            # r0 = d
        ld (r0),r0          # r0 = contains the memory location address to which d is pointing
        st r3,(r0)          # *d = *d + h[4]
        halt

.pos 0x200
u:               .long 0x0                # u
g:               .long 0x8                # g
d:               .long 0x0                # d
h:               .long 0x0                # h[0]
                 .long 0x1                # h[1]
                 .long 0x2                # h[2]
                 .long 0x3                # h[3]
                 .long 0x4                # h[4]
                 .long 0x5                # h[5]
                 .long 0x6                # h[6]
                 .long 0x7                # h[7]
                 .long 0x8                # h[8]
                 .long 0x9                # h[9]