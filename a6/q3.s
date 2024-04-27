.pos 0x1000

# average of 1 student
			ld $s, r1				
			ld 0(r1), r1			
            ld $n, r6              
			ld $0, r0				
			ld 0(r6), r6			
			bgt r6, average	
			halt

average:
            mov r0, r3		            
			mov r0, r5				
			shl $1, r3				
			shl $2, r5
			add r5, r3				

			ld $1, r5				
			add r3, r5				
			ld (r1, r5, 4), r5		


			ld $2, r7				
			add r3, r7				
			ld (r1, r7, 4), r7		
			add r7, r5				


			ld $3, r7				
			add r3, r7				
			ld (r1, r7, 4), r7		


			ld $4, r2				
			add r3, r2				
			ld (r1, r2, 4), r2		

			
			add r2, r7				
			add r7, r5				
			shr $2, r5				
			ld $5, r7				
			add r3, r7				
			st r5, (r1, r7, 4)		
			inc r0					
			mov r0, r5			
			not r5					
			inc r5					
			add r6, r5				
			bgt r5, average		
			ld $-1, r5				
			add r6, r5				
			beq r5, median			
			j sort					
			halt
		

sort:		
			ld $n, r3
            ld $0, r0								
			ld (r3), r3				
			ld $-1, r5							
			ld $local, r7				
			add r5, r3	
			st r3, 0(r7)	
            j compare

sorting:  
			ld $local, r3				
			ld 0(r3), r3
            inc r0								
			mov r0, r5				
			not r5					
			inc r5					
			add r3, r5				
			bgt r5, compare		
			ld $-1, r5				
			add r5, r3				
			ld $local, r7				
			st r3, 0(r7)			
			ld $0, r0				
			bgt r3, compare	
			j median				
			halt
			

compare:	
            mov r0, r3				
			mov r0, r5				
			shl $1, r3				
			shl $2, r5				
			add r5, r3				
			ld $5, r5				
			ld $11, r2				
			add r3, r5				
			add r3, r2				
			ld (r1, r5, 4), r7		
			ld (r1, r2, 4), r4		
			not r4					
			inc r4					
			add r4, r7			
			bgt r7, swap			
			j sorting				
			halt


swap:		
            mov r0, r3				
			mov r0, r5				
			shl $1, r3				
			shl $2, r5	
			add r5, r3				
			ld $6, r2				
			ld $6, r6				
			add r3, r6				
			add r3, r2				

swapping:	
            ld (r1, r3, 4), r7		
			ld (r1, r2, 4), r4		
			st r7, (r1, r2, 4)		
			st r4, (r1, r3, 4)		
			inc r3					
			inc r2					
			mov r3, r7				
			not r7					
			inc r7					
			add r6, r7				
			bgt r7, swapping		
			j sorting		
			halt

median:		
            ld $n, r0				
			ld 0(r0), r0			
			shr $1, r0				
			mov r0, r3				
			mov r0, r5				
			shl $1, r3				
			shl $2, r5				
			add r5, r3				
			ld (r1, r3, 4), r7		
			ld $m, r2				
			st r7,  0(r2)			
			halt

.pos 0x3000
n:      .long 5       # just one student
m:      .long 0       # put the answer here
s:      .long base    # address of the array

base:   .long 1111    # student ID
        .long 1      # grade 0
        .long 1      # grade 1
        .long 1      # grade 2
        .long 1      # grade 3
        .long 0       # computed average    

		.long 2222    # student ID
        .long 5      # grade 0
        .long 5      # grade 1
        .long 5      # grade 2
        .long 5      # grade 3
        .long 0       # computed average    

		.long 3333    # student ID
        .long 3      # grade 0
        .long 3      # grade 1
        .long 3     # grade 2
        .long 3     # grade 3
        .long 0       # computed average 

		.long 4444    # student ID
        .long 0      # grade 0
        .long 0      # grade 1
        .long 0     # grade 2
        .long 0     # grade 3
        .long 0       # computed average 

		.long 5555    # student ID
        .long 2      # grade 0
        .long 2      # grade 1
        .long 2     # grade 2
        .long 2     # grade 3
        .long 0       # computed average 



.pos 0x6000
local:			.long 0	