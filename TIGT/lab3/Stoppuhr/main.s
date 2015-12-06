;*********************************************************************************
;* File Name          : main.s
;* Author             : Daniel Nisch
;* Version            : V1.4
;* Date               : 06.12.2014
;* Description        : Stoppuhr.
;		      		  : Eine Stoppuhr, mit den Funktionen start, stop und halten.
;				      : Die Zeit wird auf den Display ausgegeben und die LEDs
;					  : zeigen den Zustand an.
;					  : in dem main Programm wird zu den Unterprogrammen
;					  : gesprungen und daf�r gesorgt, dass die richtigen
;					  : Befehle f�r den jeweiligen Zustand ausgef�hrt werden.
;					  :
;**********************************************************************************

	IMPORT 	Init_TI_Board		; Initialize the serial line
	IMPORT	puts				; C output function
	IMPORT	checkkey
	IMPORT	checktimer
	IMPORT	displaytime
	IMPORT	setled
	IMPORT	init
	IMPORT	initHW
	IMPORT	tlast
	IMPORT	tusec
	IMPORT	t10msec
	IMPORT	t100msec
	IMPORT	t1sec
	IMPORT	t10sec
	IMPORT	t1min
	IMPORT	t10min

;********************************************
; Data section, aligned on 4-byte boundery
;********************************************
	AREA MyData, DATA, align = 2
test	DCD 1
	
	EXPORT test

;********************************************
; Code section, aligned on 8-byte boundery
;********************************************

	AREA |.text|, CODE, READONLY, ALIGN = 3

;--------------------------------------------
; main subroutine
;--------------------------------------------
	EXPORT main [CODE]
		
main	PROC

		BL	Init_TI_Board	; Initialize the serial line to TTY
							; for compatability to out TI-C-Board
		BL	initHW
		
		MOV r12, #2		;setzen des start Zustandes: stop.
		MOV r11, #3		;alle zahlen m�ssen auf dem Display aktuallisiert werden.
		MOV r10, #3
		bl init
		b auswahl

stop	CMP r11, #1		;�berprufen ob die Zeit schon auf 0 gesetzt wurde.
		BEQ fertig

		LDR r5, =tlast		;zur�cksetzen der Zeit
		MOV r6, #0
		STR r6, [r5]
		LDR r5, =tusec
		STR r6, [r5]
		LDR r5, =t10msec
		STR r6, [r5]
		LDR r5, =t100msec
		STR r6, [r5]
		LDR r5, =t1sec
		STR r6, [r5]
		LDR r5, =t10sec
		STR r6, [r5]
		LDR r5, =t1min
		STR r6, [r5]
		LDR r5, =t10min
		STR r6, [r5]
		
		MOV r10, #3		;alle zahlen m�ssen auf dem Display aktuallisiert werden.
		bl displaytime
		MOV r11, #1
		
fertig	b auswahl
		
running	
		bl checktimer

		LDR r1,=test
		LDR r1, [r1]
		CMP r1, #1
		BNE keineAenderung
		bl displaytime
keineAenderung

		b auswahl
		
hold
		bl checktimer
		
auswahl
		bl checkkey
		bl setled
		
		CMP r12, #2		;Pr�fung/Ausf�hrung des Zustands
		BLO hold
		BEQ stop
						;Bei running wird einfach weitergelaufen
		b running
		
forever	b	forever		; nowhere to retun if main ends		
		ENDP
	
		ALIGN
       
		END