;* File Name          : setled.s
;* Author             : Daniel Nisch
;* Version            : V1.3
;* Date               : 05.12.2014
;* Description        : Die LEDs werden dem Zustand entsprechend angepasst.
;
;**********************************************************************************

	IMPORT	portset
	IMPORT	portclr

	AREA |.text|, CODE, READONLY, ALIGN = 3

;--------------------------------------------
; setled subroutine
;--------------------------------------------
		EXPORT setled [CODE]

setled	PROC
		push {r1}
		
		LDR r0, =portclr		;laden der Adresse zum auschalten der LEDs.
		MOV r1, #-1		;alle bits werden 1 gesetzt.
		STRH r1, [r0]	;der Wert wird gespeichert und damit alle LEDs ausgeschlatet.
		
		CMP r12, #2
		BMI ledHold		;wenn der Zustand hold ist springe zu ledHold
		BEQ fertig		;wenn der Zustand stop ist muss keine LED angeschaltet werden. -> fertig.
		
		LDR r0, =portset		;laden der Adresse zum eindschalten der LEDs.
		MOV r1, #128		;das bit für die LED im Zustand running wird gesetzt
		STRH r1, [r0]		;der Wert wird gespeichert und damit die LED eingeschlatet. 
		b fertig
		
ledHold	LDR r0, =portset
		MOV r1, #192
		STRH r1, [r0]
		
fertig	
		pop{r1}
		BX LR
		
		ENDP
	
		ALIGN
       
		END