.inc inc.s

.equ S_W 32
.equ S_H 32

:main
	ld sp,:VID_RAM
	dec sp
	ld hl,:VID_RAM
	ld r0,hl
	ld bc,S_W*S_H-1
	ld (r0),0x0500
	ldir (r0+1)

	ld hl,:VID_RAM
	ld r0,'A'
	ld r1,0x1100
	ld bc,16
	call :write_s
	ld hl,:VID_RAM+S_W
	ld r0,'A'
	ld r1,0x0100
	ld bc,16
	call :write_s

	out P_SCREEN,5
	out P_SCREEN,:VID_RAM
	out P_SCREEN,8
	out P_SCREEN,9
	out P_SCREEN,0x010B
	out P_SCREEN,0x0A0A

	jmp $
	hlt

  :write_s
	ld (hl),r0
	inc hl
	dec bc
	add r0,r1
	jz bc,$+5
	jmp :write_s
	ret

:string
  .dw "Hello world!"
:string_end

.org 0x10000 - S_W*S_H
:VID_RAM
.orst

