;*********************************************************************************
;* File Name          : displaytime.s
;* Author             : Daniel Nisch
;* Version            : V1.6
;* Date               : 09.12.2014
;* Description        : Zeitanzeige.
;					  : Für die Stoppuhr wird die Zeit auf dem tft-Bildschirm 
;					  : von dem TI-Board dargestellt.
;
;**********************************************************************************

	IMPORT	puts				; C output function
	IMPORT	tftPut
					
;********************************************
; Data section, aligned on 4-byte boundery
;********************************************

	AREA MyData, DATA, align = 2
displayT0 	DCD 23, t10msec, 0
displayT 	DCD 20, t1sec, 22, t100msec, 23, t10msec, 0
displayT2 	DCD 19, t10sec, 20, t1sec, 22, t100msec, 23, t10msec, 0
displayT3 	DCD 17, t1min, 19, t10sec, 20, t1sec, 22, t100msec, 23, t10msec, 0
displayT4 	DCD 16, t10min, 17, t1min, 19, t10sec, 20, t1sec, 22, t100msec, 23, t10msec, 0
	
tlast 		DCD 0
tusec		DCD	0
t10msec 	DCD 0
t100msec 	DCD 0
t1sec 		DCD 0
t10sec 		DCD 0
t1min 		DCD 0
t10min 		DCD 0
	
	EXPORT tlast
	EXPORT tusec
	EXPORT t10msec
	EXPORT t100msec
	EXPORT t1sec
	EXPORT t10sec
	EXPORT t1min
	EXPORT t10min

;********************************************
; Code section, aligned on 8-byte boundery
;********************************************
	
	AREA |.text|, CODE, READONLY, ALIGN = 3

;--------------------------------------------
; displaytime subroutine
;--------------------------------------------
		EXPORT displaytime [CODE]
		
displaytime proc
		push {r1, r12, LR}

;Aussuchen des richtigen displayT(x) zur aktuallisierung der notwendigen zahlen.
		CMP r10, #1		;jedem displayT ist eine Zahl von 0 bis 3 zugeordnet.
		BNE weiter		;wenn r10 nicht 1 ist dann prüfe nächstes.
		LDR r5, =displayT2
		b start
		
weiter	CMP r10, #2
		BNE weiter2
		LDR r5, =displayT3
		b start
		
weiter2	CMP r10, #3
		BNE weiter3
		LDR r5, =displayT4
		b start
weiter3 CMP r10, #4
		BNE weiter4
		LDR r5, =displayT0
		b start
weiter4 
		LDR r5, =displayT		;wenn es keines der oberen Zahlen war dann 0.
		
start		
		MOV r6, #0		;r6, r7 sind zum Hochrechnen auf dem displayT.
		MOV r7, #4		;durch die versetzung der Zahl und der Adresse wird versetzt angefangen.
		MOV r4, #23		;Spalte der letzten Zahl die aktuallisiert wird.
		MOV r1, #2		;Zeile 2 auf dem Display.
		

aktuallisiere

		LDR r0, [r5, r6]		;Laden der Spaltenzahl
		LDR r2, [r5, r7]		;Laden der Adresse der Zeit. 
		LDR r2, [r2]		;Laden vom Zeitwert.
		ADD r2, r2, #48		;+48 um den ASCII Wert der Zahl zu erhalten.
		
		ADD r6, r6, #8		;Ein Schritt weiter auf dem DisplayT gehen.
		ADD r7, r7, #8		;+8, damit der die versetzung in r6 wieder eine Zahl steht.
		
		push {r0, r1, r4 - r7, r10, r11}
		BL tftPut		;Aktualisieren der Aktuellen Zahl mit Unterprogramm
		pop {r0, r1, r4 - r7, r10, r11}
		
		CMP r0, r4		;Überprüfung ob alle Zahlen aktuallisiert wurden
		BNE aktuallisiere
		
		MOV r10, #0		;zurücksetzen der Auswahl von DisplayT
		
		pop	{r1, r12, LR}
		BX LR
		
		ENDP
		ALIGN
       
		END