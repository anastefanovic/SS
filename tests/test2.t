.section s1:

	.global a

	a: .byte 0
	b: .word 0xFF
	c: .word 0, 0


	.global b


.section s2:

	.extern x
	
	.equ d, a-b+2
	.equ e, f-1
	
	sub $3, a
	sub b, a
	mov d, %r2h 

	f: .byte 0, 0

halt

