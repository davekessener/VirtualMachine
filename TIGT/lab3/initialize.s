		IMPORT	TI_Init_Board
		IMPORT	initHW

		IMPORT	state_init
		IMPORT	state_running
		IMPORT	state_hold

		IMOPRT	states
		IMOPRT	time
		IMOPRT	state

		AREA mydata,DATA,ALIGN = 2

		INCLUDE	header.inc

		AREA |.text|, CODE, READONLY, ALIGN = 3

		EXPORT	initialize [CODE]

initialize
		PROC									; void initialize(void)

		push	{r1,lr}							; {


		BL		TI_Init_Board					; TI_Init_Board()

		BL		initHW							; initHW()


		ldr		r1,=states

		ldr		r0,=state_init
		str		r0,[r1,#STATE_INIT*4]			; states[STATE_INIT] = state_init
		
		ldr		r0,=state_running
		str		r0,[r1,#STATE_RUNNING*4]		; states[STATE_RUNNING] = state_running
		
		ldr		r0,=state_hold
		str		r0,[r1,#STATE_HOLD*4]			; states[STATE_HOLD] = state_hold


		do_led	#LED_D19,#OFF					; setled(LED_D19, OFF)

		do_led	#LED_D20,#OFF					; setled(LED_D20, OFF)


		assign	=state,#STATE_INIT				; state = STATE_INIT
		
		assign	=time,#0						; time = 0


		BL		checktimer						; checktimer()

		mov		r0,#0
		BL		displaytime						; displaytime(0)


		pop		{r1,pc}							; }
		
		ENDP

