.inc "base.inc"

:main
  mcpy :cont,:src,$$-:src
  jmp :cont

:src
.org 0x300
:cont
  call :init_int
  ei
 :wait
  wait
  jmp :wait

.inc "vga.s"

:init_int
  ld r0,O_INT_TABLE+1
  ld [r0],:int_kbd
  out DV_VGA,VGA_SETSCREEN
  out DV_VGA,O_VID_MEM
  out DV_KBD,KBD_ENABLE
  ret

:int_kbd
  di
  in DV_KBD,r0
  jeq r0,'q',:end
  push r0
  call :vga_putch
  ld r0,0
  pop [r0]
  ei
  ret

:end
  hlt

:text
.db "Hello, World!",0

