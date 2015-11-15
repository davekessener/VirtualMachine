; # ===========================================================================
	AREA MyData, DATA, ALIGN = 2
Base
p_buf	FILL 125,0xFF,1 ; 997 bits needed -> 125 bytes
primes	SPACE 1024	; 1KB should be enough (256dw for 168 primes)

; # ===========================================================================
	AREA |.text|, CODE, READONLY, ALIGN = 3

	EXPORT	main
	EXTERN	Init_TI_Board
; # ---------------------------------------------------------------------------
main	PROC
	bl	Init_TI_Board

	ldr	r0,=p_buf
	mov	r1,#998
	bl	sieb

	ldr	r0,=p_buf
	ldr	r1,=primes
	mov	r2,#998
	bl	writep

m_end	b	m_end
	ENDP

; # ---------------------------------------------------------------------------
getp	PROC
	push	{r2,r3,lr}
	and	r2,r1,#31
	lsr	r1,r1,#5
	ldr	r3,[r0,r1]
	lsr	r0,r3,r2
	and	r0,r0,#1
	pop	{r2,r3,pc}
	ENDP

; # ---------------------------------------------------------------------------
resetp	PROC
	push	{r2,r3,lr}
	and	r2,r1,#31
	lsr	r1,r1,#5
	mov	r3,#1
	lsl	r3,r3,r2
	mvn	r3,r3
	ldr	r2,[r0,r1]
	and	r2,r2,r3
	str	r2,[r0,r1]
	pop	{r2,r3,pc}
	ENDP

; # ---------------------------------------------------------------------------
sieb	PROC
	push	{r2,r3,lr}

	mov	r2,#2		; 2 is the first prime
s_loop	mul	r12,r2,r2
	cmp	r12,r1
	bge	sl_end

	push	{r0,r1}
	mov	r1,r2
	bl	getp
	mov	r12,r0
	pop	{r0,r1}

	cmp	r12,#1
	bne	sl_skip

	mov	r3,r2
sl_loop	mul	r12,r2,r3
	cmp	r12,r1
	bge	sl_skip

	push	{r0,r1}
	mov	r1,r12
	bl	resetp
	pop	{r0,r1}

	add	r3,r3,#1
	b	sl_loop

sl_skip	add	r2,r2,#1
	b	s_loop

sl_end	mov	r2,#2
	mov	r3,#0
c_loop	cmp	r2,r1
	bge	cl_end

	mov	r12,r2
	add	r2,r2,#1

	push	{r0,r1}
	mov	r1,r12
	bl	getp
	mov	r12,r0
	pop	{r0,r1}

	cmp	r12,#1
	bne	c_loop

	add	r3,r3,#1

	b	c_loop

cl_end	mov	r0,r3

	pop	{r2,r3,pc}
	ENDP

; # ---------------------------------------------------------------------------
writep	PROC
	push	{r3,lr}

	mov	r3,#2
w_loop	cmp	r3,r2
	bge	wl_end

	push	{r0,r1}
	mov	r1,r3
	bl	getp
	mov	r12,r0
	pop	{r0,r1}

	cmp	r12,#1
	bne	w_skip

	str	r3,[r1]
	add	r1,r1,#4

w_skip	add	r3,r3,#1
	b w_loop

wl_end	pop	{r3,pc}
	ENDP

	END

