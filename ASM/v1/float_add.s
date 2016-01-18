;float.c:l31: float __fsadd (float a1, float a2)
;	---------------------------------
; Function __fsadd
; ---------------------------------
:___fsadd_start
:___fsadd
	push	ix
	ld	ix,0
	add	ix,sp
	ld	hl,-27
	add	hl,sp
	ld	sp,hl
;float.c:l36: char sign = 0;
	ld	(ix-21),0x00
;float.c:l38: fl1.f = a1;
	ld	hl,0x000F
	add	hl,sp
	ld	a,(ix+4)
	ld	(hl),a
	inc	hl
	ld	a,(ix+5)
	ld	(hl),a
	inc	hl
	ld	a,(ix+6)
	ld	(hl),a
	inc	hl
	ld	a,(ix+7)
	ld	(hl),a
;float.c:l39: fl2.f = a2;
	ld	hl,0x000B
	add	hl,sp
	ld	a,(ix+8)
	ld	(hl),a
	inc	hl
	ld	a,(ix+9)
	ld	(hl),a
	inc	hl
	ld	a,(ix+10)
	ld	(hl),a
	inc	hl
	ld	a,(ix+11)
	ld	(hl),a
;float.c:l42: if (!fl1.l)
	ld	hl,0x000F
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	b,(hl)
	inc	hl
	ld	d,(hl)
	inc	hl
	ld	c,(hl)
	ld	a,e
	or	a,b
	or	a,d
	or	a,c
	jr	NZ,:l00102
;float.c:l43: return (fl2.f);
	ld	hl,0x000B
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ld	l,c
	ld	h,b
	jp	:l00135
:l00102
;float.c:l44: if (!fl2.l)
	ld	hl,0x000B
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ld	a,c
	or	a,b
	or	a,e
	or	a,d
	jr	NZ,:l00104
;float.c:l45: return (fl1.f);
	ld	hl,0x000F
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ld	l,c
	ld	h,b
	jp	:l00135
:l00104
;float.c:l47: exp1 = EXP (fl1.l);
	ld	hl,0x000F
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ld	a,0x17
	push	af
	inc	sp
	push	de
	push	bc
	call	__rrulong_rrx_s
	pop	af
	pop	af
	inc	sp
	ld	c,l
	ld	b,0x00
	ld	(ix-18),c
	ld	(ix-17),b
;float.c:l48: exp2 = EXP (fl2.l);
	ld	hl,0x000B
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ld	a,0x17
	push	af
	inc	sp
	push	de
	push	bc
	call	__rrulong_rrx_s
	pop	af
	pop	af
	inc	sp
	ld	c,l
	ld	b,0x00
	ld	(ix-20),c
	ld	(ix-19),b
;float.c:l50: if (exp1 > exp2 + 25)
	ld	a,(ix-20)
	add	a,0x19
	ld	c,a
	ld	a,(ix-19)
	adc	a,0x00
	ld	b,a
	ld	a,c
	sub	a,(ix-18)
	ld	a,b
	sbc	a,(ix-17)
	jp	P,:l00106
;float.c:l51: return (fl1.f);
	ld	hl,0x000F
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ld	l,c
	ld	h,b
	jp	:l00135
:l00106
;float.c:l52: if (exp2 > exp1 + 25)
	ld	a,(ix-18)
	add	a,0x19
	ld	c,a
	ld	a,(ix-17)
	adc	a,0x00
	ld	b,a
	ld	a,c
	sub	a,(ix-20)
	ld	a,b
	sbc	a,(ix-19)
	jp	P,:l00108
;float.c:l53: return (fl2.f);
	ld	hl,0x000B
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ld	l,c
	ld	h,b
	jp	:l00135
:l00108
;float.c:l55: mant1 = MANT (fl1.l);
	ld	hl,0x000F
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	a,e
	and	a,0x7F
	ld	e,a
	ld	d,0x00
	ld	(ix-4),c
	ld	(ix-3),b
	ld	a,e
	or	a,0x80
	ld	(ix-2),a
	ld	(ix-1),d
;float.c:l56: mant2 = MANT (fl2.l);
	ld	hl,0x000B
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	a,e
	and	a,0x7F
	ld	e,a
	ld	d,0x00
	ld	(ix-8),c
	ld	(ix-7),b
	ld	a,e
	or	a,0x80
	ld	(ix-6),a
	ld	(ix-5),d
;float.c:l58: if (SIGN (fl1.l))
	ld	hl,0x000F
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ld	a,d
	rlc	a
	and	a,0x01
	jr	Z,:l00110
;float.c:l59: mant1 = -mant1;
	xor	a,a
	sbc	a,(ix-4)
	ld	(ix-4),a
	ld	a,0x00
	sbc	a,(ix-3)
	ld	(ix-3),a
	ld	a,0x00
	sbc	a,(ix-2)
	ld	(ix-2),a
	ld	a,0x00
	sbc	a,(ix-1)
	ld	(ix-1),a
:l00110
;float.c:l60: if (SIGN (fl2.l))
	ld	hl,0x000B
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ld	a,d
	rlc	a
	and	a,0x01
	jr	Z,:l00112
;float.c:l61: mant2 = -mant2;
	xor	a,a
	sbc	a,(ix-8)
	ld	(ix-8),a
	ld	a,0x00
	sbc	a,(ix-7)
	ld	(ix-7),a
	ld	a,0x00
	sbc	a,(ix-6)
	ld	(ix-6),a
	ld	a,0x00
	sbc	a,(ix-5)
	ld	(ix-5),a
:l00112
;float.c:l63: if (exp1 > exp2)
	ld	a,(ix-20)
	sub	a,(ix-18)
	ld	a,(ix-19)
	sbc	a,(ix-17)
	jp	P,:l00114
;float.c:l65: mant2 >>= exp1 - exp2;
	ld	a,(ix-18)
	sub	a,(ix-20)
	ld	c,a
	ld	a,(ix-17)
	sbc	a,(ix-19)
	ld	b,a
	push	bc
	ld	l,(ix-6)
	ld	h,(ix-5)
	push	hl
	ld	l,(ix-8)
	ld	h,(ix-7)
	push	hl
	call	__rrslong_rrx_s
	pop	af
	pop	af
	pop	af
	ld	b,h
	ld	c,l
	ld	(ix-8),c
	ld	(ix-7),b
	ld	(ix-6),e
	ld	(ix-5),d
	jr	:l00115
:l00114
;float.c:l69: mant1 >>= exp2 - exp1;
	ld	a,(ix-20)
	sub	a,(ix-18)
	ld	c,a
	ld	a,(ix-19)
	sbc	a,(ix-17)
	ld	b,a
	push	bc
	ld	l,(ix-2)
	ld	h,(ix-1)
	push	hl
	ld	l,(ix-4)
	ld	h,(ix-3)
	push	hl
	call	__rrslong_rrx_s
	pop	af
	pop	af
	pop	af
	ld	b,h
	ld	c,l
	ld	(ix-4),c
	ld	(ix-3),b
	ld	(ix-2),e
	ld	(ix-1),d
;float.c:l70: exp1 = exp2;
	ld	a,(ix-20)
	ld	(ix-18),a
	ld	a,(ix-19)
	ld	(ix-17),a
:l00115
;float.c:l72: mant1 += mant2;
	ld	a,(ix-4)
	add	a,(ix-8)
	ld	(ix-4),a
	ld	a,(ix-3)
	adc	a,(ix-7)
	ld	(ix-3),a
	ld	a,(ix-2)
	adc	a,(ix-6)
	ld	(ix-2),a
	ld	a,(ix-1)
	adc	a,(ix-5)
;float.c:l74: if (mant1 < 0)
	ld	(ix-1),a
	bit	7,a
	jr	Z,:l00119
;float.c:l76: mant1 = -mant1;
	xor	a,a
	sbc	a,(ix-4)
	ld	(ix-4),a
	ld	a,0x00
	sbc	a,(ix-3)
	ld	(ix-3),a
	ld	a,0x00
	sbc	a,(ix-2)
	ld	(ix-2),a
	ld	a,0x00
	sbc	a,(ix-1)
	ld	(ix-1),a
;float.c:l77: sign = 1;
	ld	(ix-21),0x01
	jr	:l00121
:l00119
;float.c:l79: else if (!mant1)
	ld	a,(ix-4)
	or	a,(ix-3)
	or	a,(ix-2)
	or	a,(ix-1)
	jr	NZ,:l00121
;float.c:l80: return (0);
	ld	hl,0x0000
	ld	e,l
	ld	d,h
	jp	:l00135
;float.c:l83: while (mant1<HIDDEN) {
:l00121
	ld	c,(ix-4)
	ld	b,(ix-3)
	ld	e,(ix-2)
	ld	d,(ix-1)
	ld	a,c
	sub	a,0x00
	ld	a,b
	sbc	a,0x00
	ld	a,e
	sbc	a,0x80
	ld	a,d
	sbc	a,0x00
	jr	NC,:l00126
;float.c:l84: mant1 <<= 1;
	ld	a,0x01
	push	af
	inc	sp
	ld	l,(ix-2)
	ld	h,(ix-1)
	push	hl
	ld	l,(ix-4)
	ld	h,(ix-3)
	push	hl
	call	__rlslong_rrx_s
	pop	af
	pop	af
	inc	sp
	ld	b,h
	ld	(ix-4),l
	ld	(ix-3),b
	ld	(ix-2),e
	ld	(ix-1),d
;float.c:l85: exp1--;
	ld	l,(ix-18)
	ld	h,(ix-17)
	dec	hl
	ld	(ix-18),l
	ld	(ix-17),h
	jr	:l00121
;float.c:l89: while (mant1 & 0xff000000) {
:l00126
	ld	c,(ix-4)
	ld	b,(ix-3)
	ld	e,(ix-2)
	ld	a,(ix-1)
	or	a,a
	jr	Z,:l00128
;float.c:l90: if (mant1&1)
	ld	a,(ix-4)
	and	a,0x01
	jr	Z,:l00125
;float.c:l91: mant1 += 2;
	ld	a,(ix-4)
	add	a,0x02
	ld	(ix-4),a
	ld	a,(ix-3)
	adc	a,0x00
	ld	(ix-3),a
	ld	a,(ix-2)
	adc	a,0x00
	ld	(ix-2),a
	ld	a,(ix-1)
	adc	a,0x00
	ld	(ix-1),a
:l00125
;float.c:l92: mant1 >>= 1;
	ld	a,0x01
	push	af
	inc	sp
	ld	l,(ix-2)
	ld	h,(ix-1)
	push	hl
	ld	l,(ix-4)
	ld	h,(ix-3)
	push	hl
	call	__rrslong_rrx_s
	pop	af
	pop	af
	inc	sp
	ld	b,h
	ld	(ix-4),l
	ld	(ix-3),b
	ld	(ix-2),e
	ld	(ix-1),d
;float.c:l93: exp1++;
	inc	(ix-18)
	jr	NZ,:l00161
	inc	(ix-17)
:l00161
	jp	:l00126
:l00128
;float.c:l97: mant1 &= ~HIDDEN;
	ld	c,(ix-4)
	ld	b,(ix-3)
	ld	e,(ix-2)
	ld	d,(ix-1)
	ld	(ix-4),c
	ld	(ix-3),b
	ld	a,e
	and	a,0x7F
	ld	(ix-2),a
	ld	(ix-1),d
;float.c:l100: if (exp1 >= 0x100)
	ld	a,(ix-18)
	sub	a,0x00
	ld	a,(ix-17)
	sbc	a,0x01
	jp	M,:l00133
;float.c:l101: fl1.l = (sign ? SIGNBIT : 0) | __INFINITY;
	ld	hl,0x000F
	add	hl,sp
	ld	(ix-27),l
	ld	(ix-26),h
	xor	a,a
	or	a,(ix-21)
	jr	Z,:l00137
	ld	bc,0x0000
	jr	:l00138
:l00137
	ld	bc,0x0000
:l00138
	ld	e,c
	ld	d,b
	ld	a,e
	or	a,0x80
	ld	c,a
	ld	a,d
	or	a,0x7F
	ld	b,a
	ld	l,(ix-27)
	ld	h,(ix-26)
	ld	(hl),e
	inc	hl
	ld	(hl),d
	inc	hl
	ld	(hl),c
	inc	hl
	ld	(hl),b
	jp	:l00134
:l00133
;float.c:l102: else if (exp1 < 0)
	bit	7,(ix-17)
	jr	Z,:l00130
;float.c:l103: fl1.l = 0;
	ld	hl,0x000F
	add	hl,sp
	ld	(hl),0x00
	inc	hl
	ld	(hl),0x00
	inc	hl
	ld	(hl),0x00
	inc	hl
	ld	(hl),0x00
	jp	:l00134
:l00130
;float.c:l105: fl1.l = PACK (sign ? SIGNBIT : 0 , exp1, mant1);
	ld	hl,0x000F
	add	hl,sp
	ld	(ix-27),l
	ld	(ix-26),h
	xor	a,a
	or	a,(ix-21)
	jr	Z,:l00139
	ld	(ix-25),0x00
	ld	(ix-24),0x00
	ld	(ix-23),0x00
	ld	(ix-22),0x80
	jr	:l00140
:l00139
	xor	a,a
	ld	(ix-25),a
	ld	(ix-24),a
	ld	(ix-23),a
	ld	(ix-22),a
:l00140
	ld	e,(ix-18)
	ld	d,(ix-17)
	ld	a,(ix-17)
	rla	
	sbc	a,a
	ld	c,a
	ld	b,a
	ld	a,0x17
	push	af
	inc	sp
	push	bc
	push	de
	call	__rlulong_rrx_s
	pop	af
	pop	af
	inc	sp
	ld	b,h
	ld	c,l
	ld	a,(ix-25)
	or	a,c
	ld	(ix-25),a
	ld	a,(ix-24)
	or	a,b
	ld	(ix-24),a
	ld	a,(ix-23)
	or	a,e
	ld	(ix-23),a
	ld	a,(ix-22)
	or	a,d
	ld	(ix-22),a
	ld	c,(ix-4)
	ld	b,(ix-3)
	ld	e,(ix-2)
	ld	d,(ix-1)
	ld	a,c
	or	a,(ix-25)
	ld	c,a
	ld	a,b
	or	a,(ix-24)
	ld	b,a
	ld	a,e
	or	a,(ix-23)
	ld	e,a
	ld	a,d
	or	a,(ix-22)
	ld	d,a
	ld	l,(ix-27)
	ld	h,(ix-26)
	ld	(hl),c
	inc	hl
	ld	(hl),b
	inc	hl
	ld	(hl),e
	inc	hl
	ld	(hl),d
:l00134
;float.c:l106: return (fl1.f);
	ld	hl,0x000F
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ld	l,c
	ld	h,b
:l00135
	ld	sp,ix
	pop	ix
	ret
___fsadd_end::

