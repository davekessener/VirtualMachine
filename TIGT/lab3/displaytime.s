			IMPORT	tftPut

			AREA mydata,DATA,ALIGN = 2

			INCLUDE	header.inc

			MACRO	modulo	$rd,$rm,$rn
			udiv	$rd,$rm,$rn				; rd = rm / rn
			mls		$rd,$rd,$rn,$rm			; rd = rm - (rd * rn)
			MEND

disp		DCB	"  :  :  .  ",0
label		DCB	"HAW Timer",0
ttime		DCD 0

			EXPORT	state

			AREA |.text|, CODE, READONLY, ALIGN = 3

			EXPORT	displaytime[CODE]

displaytime PROC					; void displaytime(time_t t)

			push	{r1-6,lr}		; {

			ldr		r1,=ttime
			ldr		r2,[r1]
			cmp		r2,r0
			BEQ		skip			; if(t != ttime) {

			str		r0,[r1]			; ttime = t

			mov		r12,#100
			modulo	r3,r0,r12		; hs  = t % 100
			udiv	r0,r12			;  t /= 100
			mov		r12,#60
			modulo	r4,r0,r12		;  s  = t %  60
			udiv	r0,r12			;  t /= 60
			modulo	r5,r0,r12		;  m  = t %  60
			udiv	r0,r12			;  t /= 60
			mov		r12,#24
			modulo	r6,r0,r12		;  h  = t % 24

			ldr		r0,=disp+#0
			mov		r1,r6
			BL		write_2d		; write_2d(disp,      h)

			ldr		r0,=disp+#3
			mov		r1,r5
			BL		write_2d		; write_2d(disp + 3,  m)

			ldr		r0,=disp+#6
			mov		r1,r4
			BL		write_2d		; write_2d(disp + 6,  s)

			ldr		r0,=disp+#9
			mov		r1,r3
			BL		write_2d		; write_2d(disp + 9, hs)

			mov		r0,#4
			mov		r1,#1
			ldr		r2,=label
			BL		puts			; puts(4, 1, "HAW Timer")

			mov		r0,#3
			mov		r1,#3
			ldr		r2,=disp
			BL		puts			; puts(3, 3, disp)

skip								; }
			pop		{r1-6,pc}		; }
		
			ENDP


write_2d	PROC					; void write_2d(char *s, int v)

			push	{r2,r3,lr}		; {

			mov		r12,#10
			udev	r3,r1,r12
			modulo	r2,r3,r12
			add		r2,r2,#48		; // ASCII #48 == '0'
			strb	r2,[r0]			; s[0] = '0' + ((v / 10) % 10)

			modulo	r3,r1,r12
			add		r3,r3,#48
			strb	r3,[r0,#1]		; s[1] = '0' +  (v % 10)

			pop		{r2,r3,pc}		; }

			ENDP


puts		PROC					; void puts(int x, int y, const char *s)

			push	{lr}			; {

loop		ldrb	r12,[r2]		; while(*s) {
			CMP		r12,#0
			BEQ		end_s

			BL		tftPut			; tftPut(x, y, s)

			add		r0,r0,#1		; ++x
			add		r2,r2,#1		; ++s

			B		loop			; }

end_s		pop		{pc}			; }

			ENDP

