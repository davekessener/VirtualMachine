.inc inc.s

:TXT_MODE_INIT
	ld 		hl,		VIDEO_RAM
	ld		r0,		:TXTCOLOR
	ld		r1,		(r0)
	ld 		r0,		hl
	ld 		(r0),	r1
	ld 		bc,		TXT_WIDTH*TXT_HEIGHT-1
	ldir 	(r0 + 1)

	out P_SCREEN,	SCREEN_C_TXT_MODE_ON
	out P_SCREEN,	VIDEO_RAM
	out P_SCREEN,	SCREEN_C_TXT_UPDATE
	out P_SCREEN,	SCREEN_C_TXT_CURSOR_ON

	ret

:putch
	ld hl,:TXTCOLOR
	ld r9,(hl)
	ld bc,:CURSOR
	ld hl,(bc)
	add hl,VIDEO_RAM
	ld bc,(sp+1)
	or bc,r9
	ld (hl),bc
	out P_SCREEN, SCREEN_C_TXT_UPDATE
	call :ADVANCE_CURSOR
	ret

:puts
	ld hl,(sp+1)
  :puts_loop
	ld bc,(hl)
	jz bc,:puts_loop_end
	inc hl
	push hl
	push bc
	call :putch
	pop bc
	pop hl
	jmp :puts_loop
  :puts_loop_end
	ret

:ADVANCE_CURSOR
	ld hl,:CURSOR
	ld bc,(hl)
	inc bc
	ld a,bc
	and a,0x1f
	jz a,$+5
	jmp $+8
	add bc,0x0100
	and bc,0x1fff
	ld (hl),bc
	out P_SCREEN, SCREEN_C_TXT_CURSOR_ADVANCE
	ret

:CURSOR
  .dw 0

:TXTCOLOR
  .dw 0x0A00

