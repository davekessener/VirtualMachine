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
getp	PROC	; int getp(char *r0, int r1) // extracts r1th bit from r0
	push	{r2,r3,lr}
	and	r2,r1,#31	; r1 % 32 == which bit is wanted
	lsr	r1,r1,#5	; r1 / 32 == which int is the bit in
	lsl	r1,r1,#2	; return (*((int *)(r0 + r1 * 4)) >> r2) & 1
	ldr	r3,[r0,r1]
	lsr	r0,r3,r2
	and	r0,r0,#1
	pop	{r2,r3,pc}
	ENDP

; # ---------------------------------------------------------------------------
resetp	PROC	; void resetp(char *r0, int r1) // resets r1th bit from r0
	push	{r2,r3,lr}
	and	r2,r1,#31	; see getp
	lsr	r1,r1,#5
	lsl	r1,r1,#2
	mov	r3,#1		; int r3 = ~(1 << r2)
	lsl	r3,r3,r2
	mvn	r3,r3
	ldr	r2,[r0,r1]	; r0[r1] &= r3
	and	r2,r2,r3
	str	r2,[r0,r1]
	pop	{r2,r3,pc}
	ENDP

; # ---------------------------------------------------------------------------
sieb	PROC	; int sieb(char *r0, int r1)
		; // calculates sieb for r1 numbers in r0, 1-bit per number
		; // returns number of found primes
	push	{r2,r3,lr}

	mov	r2,#2		; 2 is the first prime
s_loop	mul	r12,r2,r2	; for1(r2 = 2 ; r2 * r2 < r1 ; ++r2)
	cmp	r12,r1
	bge	sl_end

	push	{r0,r1}		; if(getp(r0, r2)) {
	mov	r1,r2
	bl	getp
	mov	r12,r0
	pop	{r0,r1}

	cmp	r12,#1
	bne	sl_skip

	mov	r3,r2		; for2(r3 = r2 ; r2 * r3 < r1 ; ++r3)
sl_loop	mul	r12,r2,r3
	cmp	r12,r1
	bge	sl_skip

	push	{r0,r1}		; resetp(r0, r2 * r3);
	mov	r1,r12
	bl	resetp
	pop	{r0,r1}

	add	r3,r3,#1
	b	sl_loop		; end for2

sl_skip	add	r2,r2,#1	; end if
	b	s_loop		; end for1

sl_end	mov	r2,#2		; for3(r2 = 2, r3 = 0 ; r2 > r1 ; ++r2)
	mov	r3,#0
c_loop	cmp	r2,r1
	bge	cl_end

	push	{r0,r1}		; if(getp(r0, r2))
	mov	r1,r2
	bl	getp
	mov	r12,r0
	pop	{r0,r1}

	cmp	r12,#1
	bne	cl_skip

	add	r3,r3,#1	; ++r3

cl_skip	add	r2,r2,#1
	b	c_loop		; end for3

cl_end	mov	r0,r3		; return r3

	pop	{r2,r3,pc}
	ENDP

; # ---------------------------------------------------------------------------
writep	PROC	; void writep(char *r0, int *r1, int r2)
		; // copies all primes - as indicated by the sieb in r0 -
		; // to r1. it checks the first r2 bits of r0
	push	{r3,lr}

	mov	r3,#2		; for(r3 = 2 ; r3 < r2 ; ++r3)
w_loop	cmp	r3,r2
	bge	wl_end

	push	{r0,r1}		; if(getp(r0, r3))
	mov	r1,r3
	bl	getp
	mov	r12,r0
	pop	{r0,r1}

	cmp	r12,#1
	bne	w_skip

	str	r3,[r1]		; *r1++ = r3
	add	r1,r1,#4

w_skip	add	r3,r3,#1
	b w_loop		; end for

wl_end	pop	{r3,pc}
	ENDP

	END

