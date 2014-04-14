.inc inc.s

:main
  ld r0,:heap_copy
  ld hl,:heap_init
  ld bc,:heap_init-HEAP_ADDR
  jz bc,$+14
  ld a,(r0)
  ld (hl),a
  inc r0
  inc hl
  dec bc
  jmp $-17
  call :heap_init
  hlt

:heap_copy

.org HEAP_ADDR-128

:heap_init
  ld hl,HEAP_ADDR
  ld (hl),HEAP_ADDR
  inc hl
  ld (hl),0xffff-HEAP_ADDR
  ret

:malloc
  ld hl,HEAP_ADDR
  inc r0
:malloc_loop
  ld r1,(hl)
  inc hl
  ld r2,(hl)
  dec hl
  jlt r0,r2,:malloc_fill
  jeq r0,r2,:malloc_tight
  jz r1,:malloc_error
  ld r3,hl
  ld hl,r1
  jmp :malloc_loop
:malloc_tight
  ld a,(hl)
  ld (r3),a
  ld (hl),r0
  ld r0,hl
  inc r0
  ret
:malloc_fill
  ld r3,hl
  add hl,r2
  sub hl,r0
  ld (hl),r0
  ld r0,hl
  inc r0
  sub hl,r3
  inc r3
  ld (r3),hl
  ret
:malloc_error
  hlt

:free
  jz r0,$+1
  ret

:head_end

