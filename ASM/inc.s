.equ PC r15
.equ SP r14
.equ  F r13
.equ  A r12
.equ HL r11
.equ BC r10

.equ P_SCREEN 0
.equ P_KEYBOARD 1

.equ SCREEN_WIDTH 256
.equ SCREEN_HEIGHT 256

.equ F_EQ 0
.equ F_LT 1
.equ F_GT 2
.equ F_LE 3
.equ F_GE 4

.mac outc 2
{
  ld a,%1
  out %0,a
}

.mac jeq 3
{
  ld a,%0
  cmp a,%1
  btst a,F_EQ
  jz a,$+5
  jmp %2
}

.mac jnflag 4
{
  ld a,%0
  cmp a,%1
  btst a,%3
  jz a,%2
}

.mac jne 3
{
  jnflag %0,%1,%2,F_EQ
}

.mac jlt 3
{
  jnflag %0,%1,%2,F_GE
}

.mac jle 3
{
  jnflag %0,%1,%2,F_GT
}

.mac jgt 3
{
  jnflag %0,%1,%2,F_LE
}

.mac jge 3
{
  jnflag %0,%1,%2,F_LT
}

