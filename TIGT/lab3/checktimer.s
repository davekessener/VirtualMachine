			IMPORT	timer

			AREA mydata,DATA,ALIGN = 2

			INCLUDE	header.inc

ttime		DCW	0

			AREA |.text|, CODE, READONLY, ALIGN = 3

			EXPORT	checktimer [CODE]

checktimer	PROC

			push	{r1-r4,lr}

			ldr		r1,=timer
			ldr		r2,[r1]			; t = *timer	// r2 == *timer
			ldr		r1,=ttime
			ldrh	r3,[r1]			; 				// r3 == ttime
			sub		r3,r2,r3		; r = t - ttime	// r3 == r
			mov		r12,#0xFFFF
			and		r3,r3,r12
			mov		r12,#1000
			udiv	r0,r3,r12		; // r0 = r / 1000
			mls		r4,r0,r12,r3	; // r4 [= r - r0 * 1000] = r % 1000
			sub		r4,r2,r4
			mov		r12,#0xFFFF
			and		r4,r4,r12
			strh	r4,[r1]			; ttime = t - (r % 1000)
									; return r / 1000 // r0 == r / 1000
			pop		{r1-r4,pc}
		
			ENDP

	END

