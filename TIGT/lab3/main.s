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
		LDR		r0,[r1,r0,lsl #2]
		BLX		r0
		STR		r0,[r3]				; state = states[state]();

		B		loop				; }
		ENDP

	END

;;******************** (C) COPYRIGHT HAW-Hamburg ********************************
;;* File Name          : main.s
;;* Author             : Alfred Lohmann
;;* Version            : V1.0
;;* Date               : 15.07.2012
;;* Description        : This is a simple main.
;;					  : The output is send to UART 1. Open Serial Window when 
;;					  : when debugging. Select UART #1 in Serial Window selection.
;;					  :
;;					  : Replace this main with yours.
;;
;;*******************************************************************************

	;IMPORT 	Init_TI_Board		; Initialize the serial line
	;IMPORT	puts				; C output function

;;********************************************
;; Data section, aligned on 4-byte boundery
;;********************************************
	
	;AREA MyData, DATA, align = 2
	
;text	DCB	"Hallo Welt",0

;;********************************************
;; Code section, aligned on 8-byte boundery
;;********************************************

	;AREA |.text|, CODE, READONLY, ALIGN = 3

;;--------------------------------------------
;; main subroutine
;;--------------------------------------------
	;EXPORT main [CODE]
	
;main	PROC

		;BL	Init_TI_Board	; Initialize the serial line to TTY
							;; for compatability to out TI-C-Board
		
		;LDR	r0,=text
		;BL	puts			; call C output method
		
		
;forever	b	forever		; nowhere to retun if main ends		
		;ENDP
	
		;ALIGN
       
		;END
		