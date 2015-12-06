;*********************************************************************************
;* File Name          : checktimer.s
;* Author             : Daniel Nisch
;* Version            : V1.6
;* Date               : 09.12.2014
;* Description        : Die Zeit wird nach der verstrichenen Zeit mit dem
;					  : Zeitgeber aktuallisiert.
;
;**********************************************************************************

	IMPORT	tlast
	IMPORT	tusec
	IMPORT	t10msec
	IMPORT	t100msec
	IMPORT	t1sec
	IMPORT	t10sec
	IMPORT	t1min
	IMPORT	t10min
	IMPORT	timer
	IMPORT	test
		
			AREA myconst,DATA,READONLY
	
t10mseck 	EQU 10
t100mseck	EQU 10
t1seck		EQU 10
t10seck		EQU 6
t1mink		EQU 10
t10mink		EQU 6

	AREA |.text|, CODE, READONLY, ALIGN = 3

;--------------------------------------------
; checktimer subroutine
;--------------------------------------------
		EXPORT checktimer [CODE]

checktimer PROC
		push {r11, r12, lr}

		MOV r11, #0		;r11 = 0 zum zurücksetzten der Zeit falls Grenzwert erreicht.
		
		LDR r0, =tlast		;laden des Timerstands beim letzten checktimer.
		LDR r1, [r0]
		
		LDR r2, =timer		;laden des aktuellen Stands des timers.
		LDRH r10, [r2]
		
		CMP r10, r1		;Überprufen ob Overflow stattgefunden hat
		BMI overflow
		
		SUB r2, r10, r1		;Berechnen der verstrichenen Zeit ohne Overflow.
		b keinOverflow
		
overflow		;Berechnen der verstrichenen Zeit bei Overflow.
		MOV r3, #65536
		SUB r2, r3, r1
		ADD r2, r2, r10

keinOverflow
;Verstrichene Zeit wird in ms umgerechnet und aktuallisiert.
		LDR r3, =t10msec		;laden der Speicheradressen die Wahrscheinlich benötigt werden.
		LDR r5, =t100msec
		LDR r7, =t1sec
		LDR r8,=test
		MOV r9, #2
		STR r9, [r8]
		
		LDR r4, [r3]		;Laden der letzten ms Werte.
		LDR r6, [r5]
		
		LDR r1, =tusec
		LDR r1, [r1]
		ADD r2, r2, r1

		CMP r2, #1000		;überprüfung, ob Zeit aktuallisiert ist.
		BMI fertig
		MOV r9, #1
		STR r9, [r8]
		
weiter	
		CMP r2, #1000		;überprüfung, ob Zeit aktuallisiert ist.
		BMI fertig

		SUB r2, r2, #1000		;runterzählen der verstrichenen Zeit
		
		CMP r12,#4
		BMI lx
		MOV r12, #4
lx		
		ADD r4, r4, #1		;erhoehen der Zeit
		
		CMP r4, #t10mseck		;Ist der Wert 10
		BEQ erhoehenMsMs
		b weiter
		
		
erhoehenMsMs
		MOV r4, #0		;t10msec wird zurück auf 0 gesetzt.
		ADD r6, r6, #1		;erhoehen der Zeit
		
		CMP r6, #t100mseck		;Ist der Wert 10
		BEQ erhoehenMsS
		b weiter
		
erhoehenMsS
		MOV r6, #0
		LDR r8, [r7]
		ADD r8, r8, #1		;erhoehen der Zeit
		STR r8, [r7]		;speichern des neuen Sekunden Wertes.
		
		CMP r8, #t1seck		;Ist der Wert 10
		BEQ erhoehenSecSec
		b weiter

erhoehenSecSec
		MOV r12, #1

		STR r11, [r7]		;speichern des neuen Sekunden Wertes (0);
		LDR r8, =t10sec	
		LDR r9, [r8]
		ADD r9, r9, #1		;erhoehen der Zeit
		STR r9, [r8]
		
		CMP r9, #t10seck		;Ist der Wert 6
		BEQ erhoehenSecMin
		b weiter
		
erhoehenSecMin
		MOV r12, #2

		STR r11, [r8]
		LDR r8, =t1min
		LDR r9, [r8]
		ADD r9, r9, #1		;erhoehen der Zeit
		STR r9, [r8]
		
		CMP r9, #t1mink		;Ist der Wert 10
		BEQ erhoehenMinMin
		b weiter	
		
erhoehenMinMin
		MOV r12, #3
		
		STR r11, [r8]
		LDR r8, =t10min
		LDR r9, [r8]
		ADD r9, r9, #1		;erhoehen der Zeit
		STR r9, [r8]
		
		CMP r9, #t10mink		;Ist der Wert 6
		BNE weiter
		
		STR r11, [r8]
		b weiter

fertig	
		STR r4, [r3]		;speichern der Werte
		STR r6, [r5]
		
		LDR r1, =tusec
		STR r2, [r1] 
		
		STR r10, [r0]		;speichern des Timers als neuer tlast.
		
		MOV r10, r12		;r10 wird auf den Stand gesetzt welche Zeiten aktuallisiert werden müssen.
		
		pop {r11, r12, lr}
		BX LR
		
		ENDP
	
		ALIGN
       
		END