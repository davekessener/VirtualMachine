.equ DV_VGA 0
.equ DV_KBD 1
.equ INT_TBL 0x0100
.equ VID_MEM 0xc000

:main
  ld r1,:text
  ld r2,VID_MEM
:loop
  ld r3,[r1]
  ld [r2],r3
  jz r3,:end
  inc r1
  inc r2
  jmp :loop
:end
  ld r1,1
  out DV_VGA,r1
  ld r1,VID_MEM
  out DV_VGA,r1
  ld r1,INT_TBL+1
  ld r2,:kbint
  ld [r1],r2
  ld r1,1
  out DV_KBD,r1
  ei
:idle
  jmp :idle

:kbint
  in DV_KBD,r1
  ld r2,'q'
  xor r1,r2
  jz r1,:quit
  ret

:quit
  hlt

:text
.db "Hello, World!",0

