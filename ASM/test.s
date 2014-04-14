.inc inc.s

.equ SPRITE_WIDTH 8
.equ SPRITE_HEIGHT 8

:start
	ld r0,(SCREEN_WIDTH-SPRITE_WIDTH)/2
	ld r1,0
	ld r2,:sprite_pos
	ld r3,:sprite_neg
  :loop
	inc r1
	jge r1,0xff,:loop_end
	outc P_SCREEN,1
	out P_SCREEN,r0
	out P_SCREEN,r1
	outc P_SCREEN,SPRITE_WIDTH
	outc P_SCREEN,SPRITE_HEIGHT
	outc P_SCREEN,:sprite_pos
	jmp :loop
  :loop_end

:sprite_pos
  .dw -1,-1,-1,-1,-1,-1,-1,-1
  .dw -1,-1,-1,-1,-1,-1,-1,-1
  .dw -1,-1,-1,-1,-1,-1,-1,-1
  .dw -1,-1,-1,-1,-1,-1,-1,-1
  .dw -1,-1,-1,-1,-1,-1,-1,-1
  .dw -1,-1,-1,-1,-1,-1,-1,-1
  .dw -1,-1,-1,-1,-1,-1,-1,-1
  .dw -1,-1,-1,-1,-1,-1,-1,-1

:sprite_neg
  .res 8**2
;  .dw 0,0,0,0,0,0,0,0
;  .dw 0,0,0,0,0,0,0,0
;  .dw 0,0,0,0,0,0,0,0
;  .dw 0,0,0,0,0,0,0,0
;  .dw 0,0,0,0,0,0,0,0
;  .dw 0,0,0,0,0,0,0,0
;  .dw 0,0,0,0,0,0,0,0
;  .dw 0,0,0,0,0,0,0,0

