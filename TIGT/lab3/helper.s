         PRESERVE8

        IMPORT TFT_gotoxy
        IMPORT TFT_bigText
        IMPORT TFT_cursor_off
        IMPORT TFT_cls



rcc_base            EQU 0x40023800
APB1ENR             EQU 0x40
RCC_APB1Periph_TIM7 EQU 0x20

timer7_base EQU 0x40001400
TIMx_CR1    EQU 0x00
TIMx_CR2    EQU 0x04
TIMx_DIER   EQU 0x0C
TIMx_SR     EQU 0x10
TIMx_EGR    EQU 0x14
TIMx_CNT    EQU 0x24
TIMx_PSC    EQU 0x28
TIMx_ARR    EQU 0x2C


        GLOBAL timer
        GLOBAL portset
        GLOBAL portclr
        GLOBAL portin
        
timer       EQU timer7_base+TIMx_CNT

GPIOG_base  EQU 0x40021800
GPIOE_base  EQU 0x40021000
GPIO_IDR    EQU 0x10
GPIO_BSRRL  EQU 0x18
GPIO_BSRRH  EQU 0x1A

portset     EQU GPIOG_base+GPIO_BSRRL
portclr     EQU GPIOG_base+GPIO_BSRRH
portin      EQU GPIOE_base+GPIO_IDR


        AREA mycode,CODE
;
; Unterprogramm zum Initialisieren der Hardware
; LCD: Loeschen und Ausschalten des Cursors
; TIMER2: Starten, Kontinuierliches Zaehlen mit einer Frequenz von 100kHz
; Ueberlauf nach 715 Minuten
;

        GLOBAL initHW
initHW
        push   {lr}


;
; Init Timer7
;
        ldr    r0,=rcc_base
        ldr    r1,[r0,#APB1ENR]
        orr    r1,#RCC_APB1Periph_TIM7
        str    r1,[r0,#APB1ENR]

        ldr    r0,=timer7_base
        ldr    r1,=168000000/2/100000 - 1
        str    r1,[r0,#TIMx_PSC]
        ldr    r1,=0xffff
        str    r1,[r0,#TIMx_ARR]
        ldr    r1,=1
        str    r1,[r0,#TIMx_EGR]
        ldr    r1,=1
        str    r1,[r0,#TIMx_CR1]


;
;  Init LCD
;
        bl     TFT_cursor_off
        bl     TFT_cls

        pop    {pc}


;
; Display one char on TFT
; r0: x
; r1: y
; r2: char
;
        GLOBAL tftPut
tftPut
        push   {r0,r1,r2,r3,lr}

        
        bl     TFT_bigText

        
        pop    {r0,r1,r2,r3,pc}




;
; Loest trigger im trace aus
; r0: data
;
        GLOBAL trigger
trigger
        PUSH {R1,LR}
        LDR R1,=0xE0000004
        STR R0,[R1]
        pop {R1,PC}


        ALIGN
        END

