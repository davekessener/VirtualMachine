		IMPORT	initialize
		IMPORT	checktimer

		AREA mydata,DATA,ALIGN = 2

		INCLUDE	header.inc

states			DCD 0,0,0
time			DCD 0
state			DCD 0

		EXPORT	states
		EXPORT	time
		EXPORT	state

		AREA |.text|, CODE, READONLY, ALIGN = 3

		EXPORT	main [CODE]

main	PROC
		
		BL		initialize			; initialize()

		LDR		r1,=states
		LDR		r2,=time
		LDR		r3,=state

loop								; while(TRUE) {
		BL		checktimer
		LDR		r12,[r2]
		ADD		r0,r0,r12
		STR		r0,[r2]				; time += checktimer()

		LDR		r0,[r3]
		LDR		r0,[r1,r0,asl #2]
		BLX		r0
		STR		r0,[r3]				; state = states[state]();

		B		loop				; }
		ENDP

