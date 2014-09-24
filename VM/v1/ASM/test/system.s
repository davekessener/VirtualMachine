.inc "base.inc"

; void *memmove(uint8_t *dst, uint8_t *src, size_t count)
:memmove
	push IX
	ld IX,SP
	push HL
	push DE
	ld A,[IX+2]
	ld HL,[IX+3]
	ld DE,[IX+4]
	mcpy DE,HL,A
	add A,DE
	pop DE
	pop HL
	pop IX
	ret
;  :__memmove_loop
;  	jz [IX+2],:__memmove_end
;	dec [IX+2]
;	ld [DE],[HL]
;	inc DE
;	inc HL
;	jmp :__memmove_loop
;  :__memmove_end

; void *memset(uint8_t *dst, uint8_t val, size_t count)
:memset
	push IX
	ld IX,SP
	push HL
	ld HL,[IX+4]
	ld [HL],[IX+3]
	inc HL
	dec [IX+2]
	push HL
	push [IX+4]
	push [IX+2]
	call :memmove
	add SP,3
	pop HL
	pop IX
	ret

