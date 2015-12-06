;*********************************************************************************
;* File Name          : checkkey.s
;* Author             : Daniel Nisch
;* Version            : V1.3
;* Date               : 05.12.2014
;* Description        : Tastenpr�fung.
;					  : F�r die Stoppuhr wird gepr�ft ob eine relevante Taste 
;					  : (S5 - S7) gedr�ckt wurde.
;
;**********************************************************************************

	IMPORT	portin
	IMPORT	tlast
	IMPORT	tusec
	IMPORT	t10msec
	IMPORT	timer

	AREA |.text|, CODE, READONLY, ALIGN = 3

;--------------------------------------------
; checkkey subroutine
;--------------------------------------------
		EXPORT checkkey  [CODE]

checkkey PROC
		push {r1}

		MOV r2, #0
		LDR r1, =portin		;Laden des Tastenzustands.
		LDRH r2, [r1]
		
;bestimmung der gedr�ckten relevanten Tasten.
		MOV r3, #224
		AND r2, r2, r3
		
		CMP r2, #224		;Pr�fung ob Taste gedr�ckt.
		BEQ fertig
		
		CMP r2, #96		;Pr�fung ob Taste S7 (running) gedr�ckt wurde.
		BNE weiter
		MOV r12, #3
		MOV r11, #0
		MOV r1, #-1
		
		LDR r7, =tlast
		LDR r6, [r5]
		CMP	r6, #0
		BNE j
		
		LDR r5, =tusec
		LDR r6, [r5]
		CMP	r6, #0
		BNE j
		
		LDR r5, =t10msec
		LDR r6, [r5]
		CMP	r6, #0
		BNE j
		
		MOV r8, #0
		LDR r8, =timer
		LDRH r8, [r8]
		STR r8, [r7]
j		
		b fertig
		
weiter	CMP r2, #192		;Pr�fung ob Taste S5 (stop) gedr�ckt wurde.
		BNE weiter2
		MOV r12, #2
		MOV r11, #3
		b fertig
		
weiter2	MOV r12, #1		;Wurde hold oder mehrere Tasten gedr�ckt wird
		MOV r11, #0		;der Zustand auf hold gesetzt.

fertig	
		pop{r1}
		BX LR
		
		ENDP
	
		ALIGN
       
		END