.pos 0x100
		ld $a, r0		# r0 = address of a
		ld (r0), r1		# r1 = a
		inc r1			# r1 = r1 + 1
		inca r1			# r1 = r1 + 4
		shl $0x6, r1	# r1 = r1 << 6
		ld (r0), r2		# r2 = a
		and r2, r1		# r1 = r1 & a
		shr $0x3, r1	# r1 = r1 >> 3
		ld $k, r3		# r3 = address of a
		st r1, (r3)		# a = r1
        halt            # halt

.pos 0x1000
k:               .long 20        # k
.pos 0x2000
a:               .long 32        # a