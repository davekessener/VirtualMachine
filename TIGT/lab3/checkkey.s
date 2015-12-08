		IMPORT	portin

		AREA mydata,DATA,ALIGN = 2

		INCLUDE	header.inc

		AREA |.text|, CODE, READONLY, ALIGN = 3

		EXPORT	checkkey [CODE]

checkkey PROC				; bool_t checkkey(key_t k)

		push	{r1,lr}		; {

		ldr		r1,=portin
		ldrh	r1,[r1]
		mvn		r1,r1
		and		r0,r0,r1	; return *portin & k

		pop		{r1,pc}		; }
		
		ENDP

	END

