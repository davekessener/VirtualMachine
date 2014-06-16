.equ HL r15
  ld [r11], r3
  ld HL, :whateva
  ld r0, $
  ld r1, r10
:whateva
  ei
  hlt
.db 0,1,2,3

