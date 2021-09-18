.section data:

x : .word 0
y : .word 0

.equ equ_test, 4
.equ equ_test2, z - equ_test
.section text:

start: 
	   pushb %r4
	   mov %r6, x
       sub 4, %r6
	   mov x(%pc), y
	   mov start(%pc), z
	   jmp *start(%pc)
	   
	   popb x
	   movb %r5h, x
	   mov equ_test(%r1), 15
	   mov equ_test2(%r1), 20

