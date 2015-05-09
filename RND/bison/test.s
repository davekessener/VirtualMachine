.inc "inc.s"
.equ msg "Hello world!"
.equ cur $

.defmacro jne %a, %b, %lbl
	ld t15, %a
	sub t15, %b
	jz t15, ::0
	jmp %lbl
::0
.endmacro

:main
	ld A,16
	ld B,8
	sub A,B
	ld r0,199
	mul r0,A
	jne A,BL,:end
.dw msg, '\0'
.db 0,1,'\n','\\','\''
:end
	hlt

