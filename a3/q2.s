
.pos 0x100

    ld $0, r1           # r1 = 0
    ld $tmp, r0         # r0 = &tmp
    st r1, (r0)        # tmp = 0
    ld $tos, r0         # r0 = &tos
    st r1, (r0)         # tos = 0
    ld $a, r2           # r2 = &a
    ld  (r2), r3        # r3 = a[0]
    ld $s, r4           # r4 = &s
    ld (r0), r5        # r5 = tos
    st r3, (r4, r5, 4)  # s[tos] = a[0]
    inc r1              # r1 = 1
    st r1, 0(r0)        # tos = 1
    ld 4(r2), r3        # r3 = a[1]
    ld (r0), r5        # r5 = tos
    st r3, (r4, r5, 4)  # s[tos] = a[1]
    inc r1              # r1 = 2
    st r1, (r0)        # tos = 2
    ld 8(r2), r3        # r3 = a[2]
    ld (r0), r5        # r5 = tos
    st r3, (r4, r5, 4)  # s[tos] = a[2]
    inc r1              # r1 = 3
    st r1, (r0)        # tos = 3
    dec r1              # r1 = 2
    st r1, (r0)         # tos = 2
    ld (r0), r2         # r2 = tos
    ld (r4, r2, 4), r3  # r3 = s[tos]
    ld $tmp, r5         # r5 = address of tmp
    st r3,  (r5)        # tmp = s[tos]
    dec r1              # r1 = 1
    st r1,  (r0)        # tos = 1
    ld  (r0), r2        # r2 = tos
    ld (r4, r2, 4), r3  # r3 = s[tos]
    ld $tmp, r5         # r5 = address of tmp
    ld  (r5), r6        # r6 = tmp
    add r3, r6          # r6 = tmp + s[tos]
    st r6,  (r5)        # tmp = tmp + s[tos]
    dec r1              # r1 = 0
    st r1,  (r0)        # tos = 0
    ld  (r0), r2        # r2 = tos
    ld (r4, r2, 4), r3  # r3 = s[tos]
    ld $tmp, r5         # r5 = address of tmp
    ld  (r5), r6        # r6 = tmp
    add r3, r6          # r6 = tmp + s[tos]
    st r6,  (r5)        # tmp = tmp + s[tos]
    halt

.pos 0x200
tos:.long 0             # tos
tmp:.long 0             # tmp
a:  .long 0             # a[0]
    .long 1             # a[1]
    .long 2             # a[2]
s:  .long 0             # s[0]
    .long 1             # s[1]
    .long 2             # s[2]
    .long 3             # s[3]
    .long 4             # s[4]