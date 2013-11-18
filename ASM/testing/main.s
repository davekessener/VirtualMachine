.inc inc.s

.extern :TXT_MODE_INIT
.extern :putch
.extern :puts

  .res 14

  .dw :int_0,  :int_1,  :int_2,  :int_3,  :int_4,  :int_5,  :int_6,  :int_7
  .dw :int_8,  :int_9,  :int_10, :int_11, :int_12, :int_13, :int_14, :int_15
  .dw :int_16, :int_17, :int_18, :int_19, :int_20, :int_21, :int_22, :int_23
  .dw :int_24, :int_25, :int_26, :int_27, :int_28, :int_29, :int_30, :int_31

:main
	call :TXT_MODE_INIT
	ei
	jmp $
	hlt

:INT_0 ; Clock interrupt
	reti
:INT_1 ; Keyboard interrupt
	in r0,P_KEYBOARD
	ld a,r0
	and a,0x8000
	jz a,$+5
	jmp $+8
	push 'A'
	call :putch
	pop r0
	reti
:INT_2
	reti
:INT_3
	reti
:INT_4
	reti
:INT_5
	reti
:INT_6
	reti
:INT_7
	reti
:INT_8
	reti
:INT_9
	reti
:INT_10
	reti
:INT_11
	reti
:INT_12
	reti
:INT_13
	reti
:INT_14
	reti
:INT_15
	reti
:INT_16
	reti
:INT_17
	reti
:INT_18
	reti
:INT_19
	reti
:INT_20
	reti
:INT_21
	reti
:INT_22
	reti
:INT_23
	reti
:INT_24
	reti
:INT_25
	reti
:INT_26
	reti
:INT_27
	reti
:INT_28
	reti
:INT_29
	reti
:INT_30
	reti
:INT_31
	reti
:INT_32
	reti

