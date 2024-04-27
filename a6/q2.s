
 
.pos 0x1000
		ld $i, r0			# r0 = address of i
        ld $0,r0            # r0 = i = 0
        ld $n, r1           # r1 = &n
        ld (r1),r1          # r1 = n
		ld $a, r2			# r2 = address of a
        ld $b, r3           # r3 = address of b
        ld $0, r4			# r4 = 0 = c

loop:   mov r1,r5           # r5 = n
        not r5
        inc r5              # r5 = -n
        mov r0,r6           # r6 = i 
        add r6,r5           # r5 = i - n
        beq r5,end 
        br do_stuff


do_stuff: ld (r2,r6,4),r5   # r5 = a[i]
          ld (r3,r6,4),r7   # r7 = b[i]
          not r7        
          inc r7            # r7 = -b[i]
          add r5,r7         # r7 = a[i] - b[i]
          bgt r7,add_c
          inc r0            #i++
          br loop


add_c:  inc r4 #c++
        inc r0 #i++
        br loop
          

end:  ld $c,r7
      st r4,(r7)
      ld $i,r5
      st r0,(r5)
      halt


.pos 0x2000
i:		.long -1
n:      .long 5
a:		.long 10
		.long 20
		.long 30
		.long 40
		.long 50
b:		.long 11
        .long 20
        .long 28
        .long 44
        .long 48
c:      .long 0