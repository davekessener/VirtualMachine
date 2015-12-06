		IMPORT	checkkey
		IMPORT	setled
		IMPORT	displaytime

		IMOPRT	time

		AREA mydata,DATA,ALIGN = 2

		INCLUDE	header.inc

		AREA |.text|, CODE, READONLY, ALIGN = 3

		EXPORT	state_init		[CODE]
		EXPORT	state_running	[CODE]
		EXPORT	state_hold		[CODE]

; # ===========================================================================

state_init								; state_t state_init(void)
		PROC

		push	{r1-3,lr}				; {

		on_key	#KEY_S7,init_norm		; if(checkkey(KEY_S7)) {

		do_led	#LED_D20,#ON			; setled(LED_D20, ON)

		assign	=time,#0				; time = 0

		mov		r0,#STATE_RUNNING		; return STATE_RUNNING

		B		init_return

init_norm								; } else {
		mov		r0,#STATE_INIT			; return STATE_INIT;

init_return								; }
		pop		{r1-3,pc}				; }

		ENDP

; # ===========================================================================

state_running							; state_t state_running(void)
		PROC

		push	{r1-3,lr}				; {

		on_key	#KEY_S5,run_other		; if(checkkey(KEY_S5)) {

		do_led	#LED_D20,#OFF			; setled(LED_D20, OFF)

		assign	=time,#0				; time = 0

		mov		r0,#STATE_INIT			; return STATE_INIT

		B		run_return				; }

run_other								; else
		on_key	#KEY_S6,run_norm		; if(checkkey(KEY_S6)) {

		do_led	#LED_D19,#ON			; setled(LED_D19, ON)

		mov		r0,#STATE_HOLD			; return STATE_HOLD

		B		run_return

run_norm								; } else {
		mov		r0,#STATE_RUNNING		; return STATE_RUNNING

run_return								; }

		push	{r0}
		ldr		r0,=time
		ldr		r0,[r0]
		BL		displaytime
		pop		{r0}					; displaytime(time)

		pop		{r1-3,pc}				; }

		ENDP

; # ===========================================================================

state_hold								; state_t state_hold(void)
		PROC

		push	{r1-3,lr}				; {

		on_key	#KEY_S5,hold_other		; if(checkkey(KEY_S5)) {

		do_led	#LED_D19,#OFF			; setled(LED_D19, OFF)
		do_led	#LED_D20,#OFF			; setled(LED_D20, OFF)

		ldr		r0,=time
		ldr		r0,[r0]
		BL		displaytime				; displaytime(time)

		mov		r0,#STATE_INIT			; return STATE_INIT

		B		hold_return				; }

hold_other								; else
		on_key	#KEY_S7,hold_norm		; if(checkkey(KEY_S7)) {

		do_led	#LED_D19,#OFF			; setled(LED_D19, OFF)

		mov		r0,#STATE_RUNNING		; return STATE_RUNNING

		B		hold_return

hold_norm								; } else {
		mov		r0,#STATE_HOLD			; return STATE_HOLD

hold_return								; }
		pop		{r1-3,pc}				; }

		ENDP

