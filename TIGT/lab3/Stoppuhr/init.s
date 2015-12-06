;*********************************************************************************
;* File Name          : init.s
;* Author             : Daniel Nisch
;* Version            : V1.0
;* Date               : 09.12.2014
;* Description        : Initiallisierung Format.
;					  : Die Trennzeichen für die Zaidarstellung werden abgegeben. 
;					  : 
;**********************************************************************************

	IMPORT	puts				; C output function
	IMPORT	tftPut

;********************************************
; Code section, aligned on 8-byte boundery
;********************************************
	
	AREA |.text|, CODE, READONLY, ALIGN = 3

;--------------------------------------------
; displaytime subroutine
;--------------------------------------------
			EXPORT init [CODE]

init	PROC
	
		push {r10, r11, r12, LR}
		
		;Einmaliges darstellen der Trennzeichen beim Initiallisieren.
		MOV r0, #18
		MOV r1, #2
		MOV r2, #':'
		BL tftPut
		
		MOV r0, #21
		MOV r1, #2
		MOV r2, #'.'
		BL tftPut
		
		pop {r10, r11, r12, LR}
		BX LR

		ENDP
	
		ALIGN
       
		END