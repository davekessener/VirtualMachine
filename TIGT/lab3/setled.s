		IMPORT	portset
		IMPORT	portclr

		AREA mydata,DATA,ALIGN = 2

		INCLUDE	header.inc

		AREA |.text|, CODE, READONLY, ALIGN = 3

		EXPORT	setled [CODE]

setled	PROC

		push	{lr}

		CMP		r1,#FALSE
		BEQ		clear_led

		ldr		r1,=portset
		B		done

clear_led
		ldr		r1,=portclr

done	strh	r0,[r1]

		pop		{pc}
		
		ENDP

