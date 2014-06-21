.inc "system.s"

; void vga_scroll(void)
:vga_scroll
	push O_VID_MEM
	push O_VID_MEM+VID_C
	push VID_C*(VID_R-1)
	call :memmove
	add SP,3
	push O_VID_MEM+VID_C*(VID_R-1)
	ld A,:__vga_stdcolor
	ld A,[A]
	or A,' '
	push A
	push VID_C
	call :memset
	add SP,3
	ld A,:__vga_csr_y
	jz [A],:__vga_scroll_skip
	dec [A]
  :__vga_scroll_skip
  	ret

;void vga_updateCursor(void)
:vga_updateCursor
	ld A,:__vga_csr_x
	push [A]
	ld A,:__vga_csr_y
	push [A]
	call :vga_setCursor
	add SP,2
	ret

;void vga_setCursor(uint x, uint y)
:vga_setCursor
	out DV_VGA,VGA_SETPOS
	out DV_VGA,[SP+2]
	out DV_VGA,[SP+1]
	ret

;void vga_cls(void)
:vga_cls
	push O_VID_MEM
	ld A,:__vga_stdcolor
	ld A,[A]
	or A,' '
	push A
	push VID_C*VID_R
	call :memset
	add SP,3
	ld A,:__vga_csr_x
	ld [A],0
	push [A]
	ld A,:__vga_csr_y
	ld [A],0
	push [A]
	call :vga_setCursor
	add SP,2
	ret

; void vga_putch(char c)
:vga_putch
	push IX
	ld IX,SP
	push HL
	push DE
	ld DE,[IX+2]
	jeq DE,'\b',:__vga_putch_b
	jeq DE,'\t',:__vga_putch_t
	jeq DE,'\n',:__vga_putch_n
	jeq DE,'\r',:__vga_putch_r
; default:
	ld HL,:__vga_csr_y
	ld A,[HL]
	mul A,VID_C
	ld HL,:__vga_csr_x
	add A,[HL]
	ld HL,O_VID_MEM
	add HL,A
	ld A,:__vga_stdcolor
	ld A,[A]
	or A,DE
	ld [HL],A
	ld HL,:__vga_csr_x
	inc [HL]
	jmp :__vga_putch_eswitch
; case '\b':
  :__vga_putch_b
	ld HL,:__vga_csr_x
	jz [HL],:__vga_putch_b_skip
	dec [HL]
  :__vga_putch_b_skip
  	jmp :__vga_putch_eswitch
; case '\t':
  :__vga_putch_t
  	ld HL,:__vga_csr_x
	ld A,[HL]
	add A,8
	and A,~7
	ld [HL],A
	jmp :__vga_putch_eswitch
; case '\n':
  :__vga_putch_n
  	ld HL,:__vga_csr_y
	inc [HL]
	log [HL]
; case '\r':
  :__vga_putch_r
  	ld HL,:__vga_csr_x
	ld [HL],0
; end of switch
  :__vga_putch_eswitch
; if(vga_csr_x >= VID_C)
	ld HL,:__vga_csr_x
	ld A,[HL]
	inc A
	sub A,VID_C
	and A,0x8000
	jnz A,:__vga_putch_skip1
	ld [HL],0
	ld HL,:__vga_csr_y
	inc [HL]
  :__vga_putch_skip1
  	ld HL,:__vga_csr_y
	ld DE,[HL]
	sub DE,VID_R
	and DE,0x8000
	jnz DE,:__vga_putch_skip2
	call :vga_scroll
  :__vga_putch_skip2
  	call :vga_updateCursor
	pop DE
	pop HL
	pop IX
	ret

; void vga_puts(const char *str)
:vga_puts
	push HL
	ld HL,[SP+2]
  :__vga_puts_loop
  	jz [HL],:__vga_puts_end
	push [HL]
	call :vga_putch
	inc SP
	inc HL
	jmp :__vga_puts_loop
  :__vga_puts_end
  	pop HL
  	ret

:__vga_csr_x
.db 0
:__vga_csr_y
.db 0
:__vga_stdcolor
.db 0

