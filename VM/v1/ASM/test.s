.equ DV_VGA 0
.equ DV_KBD 1
.equ INT_TBL 0x0100
.equ VID_MEM 0xc000

.macro jeq %r,%v,%lbl
  push r10
  ld r10,%v
  xor r10,%r
  jz r10,::go
  pop r10
  jmp ::stay
::go
  pop r10
  jmp %lbl
::stay
.endmacro

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
  out DV_VGA,1
  out DV_VGA,VID_MEM
  ld r1,INT_TBL+1
  ld [r1],:kbint
  out DV_KBD,1
  ei
:idle
  wait
  jmp :idle

:kbint
  in DV_KBD,r1
  jeq r1,'Q'-('A'-'a'),:quit
  jeq r1,'b',:bye
  ret

:bye
  di
  ld r1,:btext
  ld r2,VID_MEM
 :bye_loop
    ld r3,[r1]
    ld [r2],r3
    jz r3,:bye_end
	inc r1
	inc r2
    jmp :bye_loop
 :bye_end
  out DV_VGA,1
  out DV_VGA,VID_MEM
  ei
  ret

:quit
  hlt

:text
.db "Hello, World!",0

:btext
.db "Goodbye, World!",0

