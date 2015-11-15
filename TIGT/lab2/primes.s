; # ===========================================================================
	AREA MyData, DATA, ALIGN = 2
Base
p_buf	FILL 125,-1,1 ; 997 bits needed -> 125 bytes
primes	SPACE 1024	; 1KB should be enough (256dw for 168 primes)

; # ===========================================================================
	AREA |.text|, CODE, READONLY, ALIGN = 3

	EXPORT	main
	EXTERN	Init_TI_Board
; # ---------------------------------------------------------------------------
main	PROC
	bl	Init_TI_Board

	ldr	r1,=p_buf
	ldr	r2,#998
	bl	sieb

	ldr	r1,=p_buf
	ldr	r2,=primes
	ldr	r3,#998
	bl	writep

m_end	b	m_end
	ENDP

; # ---------------------------------------------------------------------------
getp	PROC
	and	r6,r5,#31
	lsr	r5,r5,#5
	ldr	r0,[r1,r5]
	lsr	r0,r0,r6
	and	r0,r0,#1
	ENDP

; # ---------------------------------------------------------------------------
resetp	PROC
	and	r6,r5,#31
	lsr	r5,r5,#5
	ldr	r0,#1
	lsl	r0,r0,r6
	mvn	r0,r0
	ldr	r6,[r1,r5]
	and	r6,r6,r0
	str	r6,[r1,r5]
	ENDP

; # ---------------------------------------------------------------------------
sieb	PROC
	ldr	r3,#2		; 2 is the first prime
s_loop	mul	r10,r3,r3
	tst	r10,r2
	bge	sl_end

	ldr	r5,r3
	bl	getp
	tst	r0,#1
	bne	sl_skip

	ldr	r4,r3
sl_loop	mul	r5,r3,r4
	tst	r5,r2
	bge	sl_skip

	bl	resetp

	add	r4,r4,#1
	b	sl_loop

sl_skip	add	r3,r3,#1
	b	s_loop

sl_end	ldr	r3,#2
	ldr	r4,#0
c_loop	tst	r3,r2
	bge	cl_end

	ldr	r5,r3
	add	r3,r3,#1
	bl	getp
	tst	r0,#1
	bne	c_loop

	add	r4,r4,#1

	b	c_loop

cl_end	ldr	r0,r4
	ENDP

; # ---------------------------------------------------------------------------
writep	PROC
	ldr	r4,#2
w_loop	tst	r4,r3
	bge	wl_end

	ldr	r5,r4
	bl	getp
	tst	r0,#1
	gne	w_skip

	str	r4,[r2]
	add	r2,r2,#4
	add	r4,r4,#1

w_skip	b w_loop

wl_end	nop
	ENDP

	END

