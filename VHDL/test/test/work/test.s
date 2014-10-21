	.file	"test.vhdl"
	.section	.rodata
	.type	_UI00000000, @object
	.size	_UI00000000, 10
_UI00000000:
	.byte	116
	.byte	101
	.byte	115
	.byte	116
	.byte	46
	.byte	118
	.byte	104
	.byte	100
	.byte	108
	.byte	0
.globl work__nand2__a__OT__STB
	.align 4
	.type	work__nand2__a__OT__STB, @object
	.size	work__nand2__a__OT__STB, 16
work__nand2__a__OT__STB:
	.long	1
	.long	0
	.byte	1
	.zero	3
	.long	2
.globl work__nand2__y__OT__STB
	.align 4
	.type	work__nand2__y__OT__STB, @object
	.size	work__nand2__y__OT__STB, 16
work__nand2__y__OT__STB:
	.long	1
	.long	0
	.byte	1
	.zero	3
	.long	2
.globl work__nand2__a__OT__RTI
	.align 4
	.type	work__nand2__a__OT__RTI, @object
	.size	work__nand2__a__OT__RTI, 24
work__nand2__a__OT__RTI:
	.byte	35
	.byte	0
	.byte	0
	.byte	0
	.long	0
	.long	std__standard__bit_vector__RTI
	.long	work__nand2__a__OT__STB
	.long	2
	.long	8
	.type	work__nand2__a__RTISTR, @object
	.size	work__nand2__a__RTISTR, 2
work__nand2__a__RTISTR:
	.byte	97
	.byte	0
.globl work__nand2__a__RTI
	.align 4
	.type	work__nand2__a__RTI, @object
	.size	work__nand2__a__RTI, 16
work__nand2__a__RTI:
	.byte	16
	.byte	1
	.byte	5
	.byte	0
	.long	work__nand2__a__RTISTR
	.long	8
	.long	work__nand2__a__OT__RTI
	.type	work__nand2__b__RTISTR, @object
	.size	work__nand2__b__RTISTR, 2
work__nand2__b__RTISTR:
	.byte	98
	.byte	0
.globl work__nand2__b__RTI
	.align 4
	.type	work__nand2__b__RTI, @object
	.size	work__nand2__b__RTI, 16
work__nand2__b__RTI:
	.byte	16
	.byte	1
	.byte	5
	.byte	0
	.long	work__nand2__b__RTISTR
	.long	16
	.long	work__nand2__a__OT__RTI
.globl work__nand2__y__OT__RTI
	.align 4
	.type	work__nand2__y__OT__RTI, @object
	.size	work__nand2__y__OT__RTI, 24
work__nand2__y__OT__RTI:
	.byte	35
	.byte	0
	.byte	0
	.byte	0
	.long	0
	.long	std__standard__bit_vector__RTI
	.long	work__nand2__y__OT__STB
	.long	2
	.long	8
	.type	work__nand2__y__RTISTR, @object
	.size	work__nand2__y__RTISTR, 2
work__nand2__y__RTISTR:
	.byte	121
	.byte	0
.globl work__nand2__y__RTI
	.align 4
	.type	work__nand2__y__RTI, @object
	.size	work__nand2__y__RTI, 16
work__nand2__y__RTI:
	.byte	16
	.byte	1
	.byte	3
	.byte	0
	.long	work__nand2__y__RTISTR
	.long	24
	.long	work__nand2__y__OT__RTI
	.type	work__nand2__RTISTR, @object
	.size	work__nand2__RTISTR, 6
work__nand2__RTISTR:
	.byte	110
	.byte	97
	.byte	110
	.byte	100
	.byte	50
	.byte	0
	.align 4
	.type	work__nand2__RTIARRAY, @object
	.size	work__nand2__RTIARRAY, 16
work__nand2__RTIARRAY:
	.long	work__nand2__a__RTI
	.long	work__nand2__b__RTI
	.long	work__nand2__y__RTI
	.long	0
.globl work__nand2__RTI
	.align 4
	.type	work__nand2__RTI, @object
	.size	work__nand2__RTI, 28
work__nand2__RTI:
	.byte	4
	.byte	1
	.byte	0
	.byte	0
	.long	work__nand2__RTISTR
	.long	0
	.long	work__RTI
	.long	32
	.long	3
	.long	work__nand2__RTIARRAY
	.text
.globl work__nand2__ELAB
	.type	work__nand2__ELAB, @function
work__nand2__ELAB:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$40, %esp
	movl	8(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -28(%ebp)
	movl	$0, -24(%ebp)
.L3:
	cmpl	$2, -24(%ebp)
	jae	.L2
	movl	-24(%ebp), %edx
	movl	-28(%ebp), %eax
	movl	(%eax,%edx,4), %eax
	movl	$work__nand2__a__RTI, 4(%esp)
	movl	%eax, (%esp)
	call	__ghdl_signal_merge_rti
	addl	$1, -24(%ebp)
	jmp	.L3
.L2:
	movl	8(%ebp), %eax
	addl	$16, %eax
	movl	%eax, -20(%ebp)
	movl	$0, -16(%ebp)
.L5:
	cmpl	$2, -16(%ebp)
	jae	.L4
	movl	-16(%ebp), %edx
	movl	-20(%ebp), %eax
	movl	(%eax,%edx,4), %eax
	movl	$work__nand2__b__RTI, 4(%esp)
	movl	%eax, (%esp)
	call	__ghdl_signal_merge_rti
	addl	$1, -16(%ebp)
	jmp	.L5
.L4:
	movl	8(%ebp), %eax
	addl	$24, %eax
	movl	%eax, -12(%ebp)
	movl	$0, -8(%ebp)
.L7:
	cmpl	$2, -8(%ebp)
	jae	.L8
	movl	-8(%ebp), %edx
	movl	-12(%ebp), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	movb	$0, 4(%esp)
	movl	%eax, (%esp)
	call	__ghdl_signal_init_b2
	movl	-4(%ebp), %eax
	movl	$work__nand2__y__RTI, 4(%esp)
	movl	%eax, (%esp)
	call	__ghdl_signal_merge_rti
	addl	$1, -8(%ebp)
	jmp	.L7
.L8:
	leave
	ret
	.size	work__nand2__ELAB, .-work__nand2__ELAB
.globl work__nand2__ARCH__behav__INSTSIZE
	.section	.rodata
	.align 4
	.type	work__nand2__ARCH__behav__INSTSIZE, @object
	.size	work__nand2__ARCH__behav__INSTSIZE, 4
work__nand2__ARCH__behav__INSTSIZE:
	.long	36
	.type	work__nand2__ARCH__behav__P0__RTISTR, @object
	.size	work__nand2__ARCH__behav__P0__RTISTR, 3
work__nand2__ARCH__behav__P0__RTISTR:
	.byte	80
	.byte	48
	.byte	0
	.align 4
	.type	work__nand2__ARCH__behav__P0__RTIARRAY, @object
	.size	work__nand2__ARCH__behav__P0__RTIARRAY, 4
work__nand2__ARCH__behav__P0__RTIARRAY:
	.zero	4
.globl work__nand2__ARCH__behav__P0__RTI
	.align 4
	.type	work__nand2__ARCH__behav__P0__RTI, @object
	.size	work__nand2__ARCH__behav__P0__RTI, 28
work__nand2__ARCH__behav__P0__RTI:
	.byte	6
	.byte	2
	.byte	0
	.byte	0
	.long	work__nand2__ARCH__behav__P0__RTISTR
	.long	32
	.long	work__nand2__ARCH__behav__RTI
	.long	2
	.long	0
	.long	work__nand2__ARCH__behav__P0__RTIARRAY
	.type	work__nand2__ARCH__behav__RTISTR, @object
	.size	work__nand2__ARCH__behav__RTISTR, 6
work__nand2__ARCH__behav__RTISTR:
	.byte	98
	.byte	101
	.byte	104
	.byte	97
	.byte	118
	.byte	0
	.align 4
	.type	work__nand2__ARCH__behav__RTIARRAY, @object
	.size	work__nand2__ARCH__behav__RTIARRAY, 8
work__nand2__ARCH__behav__RTIARRAY:
	.long	work__nand2__ARCH__behav__P0__RTI
	.long	0
.globl work__nand2__ARCH__behav__RTI
	.align 4
	.type	work__nand2__ARCH__behav__RTI, @object
	.size	work__nand2__ARCH__behav__RTI, 28
work__nand2__ARCH__behav__RTI:
	.byte	5
	.byte	1
	.byte	0
	.byte	0
	.long	work__nand2__ARCH__behav__RTISTR
	.long	0
	.long	work__nand2__RTI
	.long	36
	.long	1
	.long	work__nand2__ARCH__behav__RTIARRAY
	.text
	.type	work__nand2__ARCH__behav__P0__PROC, @function
work__nand2__ARCH__behav__P0__PROC:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$104, %esp
	movl	8(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -36(%ebp)
	movl	$0, -32(%ebp)
.L11:
	cmpl	$2, -32(%ebp)
	jae	.L10
	movl	-32(%ebp), %ecx
	movl	-32(%ebp), %edx
	movl	-36(%ebp), %eax
	movl	(%eax,%edx,4), %eax
	movzbl	(%eax), %eax
	movb	%al, -46(%ebp,%ecx)
	addl	$1, -32(%ebp)
	jmp	.L11
.L10:
	leal	-46(%ebp), %eax
	movl	%eax, -56(%ebp)
	movl	$work__nand2__a__OT__STB, -52(%ebp)
	movl	8(%ebp), %eax
	addl	$16, %eax
	movl	%eax, -28(%ebp)
	movl	$0, -24(%ebp)
.L13:
	cmpl	$2, -24(%ebp)
	jae	.L12
	movl	-24(%ebp), %ecx
	movl	-24(%ebp), %edx
	movl	-28(%ebp), %eax
	movl	(%eax,%edx,4), %eax
	movzbl	(%eax), %eax
	movb	%al, -58(%ebp,%ecx)
	addl	$1, -24(%ebp)
	jmp	.L13
.L12:
	leal	-58(%ebp), %eax
	movl	%eax, -68(%ebp)
	movl	$work__nand2__a__OT__STB, -64(%ebp)
	call	__ghdl_stack2_mark
	movl	%eax, -44(%ebp)
	leal	-68(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	-56(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	-76(%ebp), %eax
	movl	%eax, (%esp)
	call	std__standard__bit_vector_NAND
	leal	-76(%ebp), %eax
	movl	%eax, -40(%ebp)
.L16:
	movl	-40(%ebp), %eax
	movl	4(%eax), %eax
	movl	12(%eax), %eax
	cmpl	$2, %eax
	jne	.L14
	movl	8(%ebp), %eax
	addl	$24, %eax
	movl	%eax, -20(%ebp)
	movl	$0, -16(%ebp)
	jmp	.L15
.L14:
	movl	$_UI00000000, %eax
	movl	$1, 4(%esp)
	movl	%eax, (%esp)
	call	__ghdl_bound_check_failed_l1
	jmp	.L16
.L15:
	cmpl	$2, -16(%ebp)
	jae	.L17
	movl	-16(%ebp), %edx
	movl	-20(%ebp), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -12(%ebp)
	movl	8(%ebp), %eax
	addl	$32, %eax
	addl	-16(%ebp), %eax
	movl	%eax, -4(%ebp)
	movl	-40(%ebp), %eax
	movl	(%eax), %edx
	movl	-16(%ebp), %eax
	movzbl	(%edx,%eax), %edx
	movl	-4(%ebp), %eax
	movb	%dl, (%eax)
	movl	-12(%ebp), %eax
	movzbl	46(%eax), %eax
	testb	%al, %al
	je	.L18
	movb	$1, -5(%ebp)
	jmp	.L19
.L18:
	movl	-12(%ebp), %eax
	movzbl	(%eax), %edx
	movl	-4(%ebp), %eax
	movzbl	(%eax), %eax
	cmpb	%al, %dl
	setne	%al
	movb	%al, -5(%ebp)
.L19:
	movl	-12(%ebp), %eax
	movl	40(%eax), %eax
	testl	%eax, %eax
	sete	%al
	andb	-5(%ebp), %al
	testb	%al, %al
	je	.L20
	movl	-12(%ebp), %eax
	movl	__ghdl_signal_active_chain, %edx
	movl	%edx, 40(%eax)
	movl	-12(%ebp), %eax
	movl	%eax, __ghdl_signal_active_chain
.L20:
	addl	$1, -16(%ebp)
	jmp	.L15
.L17:
	movl	-44(%ebp), %eax
	movl	%eax, (%esp)
	call	__ghdl_stack2_release
	leave
	ret
	.size	work__nand2__ARCH__behav__P0__PROC, .-work__nand2__ARCH__behav__P0__PROC
.globl work__nand2__ARCH__behav__ELAB
	.type	work__nand2__ARCH__behav__ELAB, @function
work__nand2__ARCH__behav__ELAB:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$56, %esp
	movl	8(%ebp), %eax
	movl	%eax, -28(%ebp)
	movl	8(%ebp), %eax
	movl	$work__nand2__ARCH__behav__RTI, (%eax)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	work__nand2__ELAB
	movl	-28(%ebp), %eax
	addl	$32, %eax
	movl	$work__nand2__ARCH__behav__P0__PROC, %edx
	movl	-28(%ebp), %ecx
	movl	%eax, 12(%esp)
	movl	$work__nand2__ARCH__behav__P0__RTI, 8(%esp)
	movl	%edx, 4(%esp)
	movl	%ecx, (%esp)
	call	__ghdl_sensitized_process_register
	movl	-28(%ebp), %eax
	addl	$24, %eax
	movl	%eax, -24(%ebp)
	movl	$0, -20(%ebp)
.L24:
	cmpl	$2, -20(%ebp)
	jae	.L23
	movl	-28(%ebp), %eax
	addl	$32, %eax
	movl	%eax, %ecx
	addl	-20(%ebp), %ecx
	movl	-20(%ebp), %edx
	movl	-24(%ebp), %eax
	movl	(%eax,%edx,4), %eax
	movl	%ecx, 4(%esp)
	movl	%eax, (%esp)
	call	__ghdl_signal_direct_driver
	addl	$1, -20(%ebp)
	jmp	.L24
.L23:
	movl	-28(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -16(%ebp)
	movl	$0, -12(%ebp)
.L26:
	cmpl	$2, -12(%ebp)
	jae	.L25
	movl	-12(%ebp), %edx
	movl	-16(%ebp), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, (%esp)
	call	__ghdl_process_add_sensitivity
	addl	$1, -12(%ebp)
	jmp	.L26
.L25:
	movl	-28(%ebp), %eax
	addl	$16, %eax
	movl	%eax, -8(%ebp)
	movl	$0, -4(%ebp)
.L28:
	cmpl	$2, -4(%ebp)
	jae	.L29
	movl	-4(%ebp), %edx
	movl	-8(%ebp), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, (%esp)
	call	__ghdl_process_add_sensitivity
	addl	$1, -4(%ebp)
	jmp	.L28
.L29:
	leave
	ret
	.size	work__nand2__ARCH__behav__ELAB, .-work__nand2__ARCH__behav__ELAB
	.section	.rodata
	.type	work__run__RTISTR, @object
	.size	work__run__RTISTR, 4
work__run__RTISTR:
	.byte	114
	.byte	117
	.byte	110
	.byte	0
	.align 4
	.type	work__run__RTIARRAY, @object
	.size	work__run__RTIARRAY, 4
work__run__RTIARRAY:
	.zero	4
.globl work__run__RTI
	.align 4
	.type	work__run__RTI, @object
	.size	work__run__RTI, 28
work__run__RTI:
	.byte	4
	.byte	1
	.byte	0
	.byte	0
	.long	work__run__RTISTR
	.long	0
	.long	work__RTI
	.long	8
	.long	0
	.long	work__run__RTIARRAY
	.text
.globl work__run__ELAB
	.type	work__run__ELAB, @function
work__run__ELAB:
	pushl	%ebp
	movl	%esp, %ebp
	popl	%ebp
	ret
	.size	work__run__ELAB, .-work__run__ELAB
.globl work__run__ARCH__simulation__a__OT__STB
	.section	.rodata
	.align 4
	.type	work__run__ARCH__simulation__a__OT__STB, @object
	.size	work__run__ARCH__simulation__a__OT__STB, 16
work__run__ARCH__simulation__a__OT__STB:
	.long	1
	.long	0
	.byte	1
	.zero	3
	.long	2
.globl work__run__ARCH__simulation__b__OT__STB
	.align 4
	.type	work__run__ARCH__simulation__b__OT__STB, @object
	.size	work__run__ARCH__simulation__b__OT__STB, 16
work__run__ARCH__simulation__b__OT__STB:
	.long	1
	.long	0
	.byte	1
	.zero	3
	.long	2
.globl work__run__ARCH__simulation__y__OT__STB
	.align 4
	.type	work__run__ARCH__simulation__y__OT__STB, @object
	.size	work__run__ARCH__simulation__y__OT__STB, 16
work__run__ARCH__simulation__y__OT__STB:
	.long	1
	.long	0
	.byte	1
	.zero	3
	.long	2
.globl work__run__ARCH__simulation__nand2__a__OT__STB
	.align 4
	.type	work__run__ARCH__simulation__nand2__a__OT__STB, @object
	.size	work__run__ARCH__simulation__nand2__a__OT__STB, 16
work__run__ARCH__simulation__nand2__a__OT__STB:
	.long	1
	.long	0
	.byte	1
	.zero	3
	.long	2
.globl work__run__ARCH__simulation__nand2__y__OT__STB
	.align 4
	.type	work__run__ARCH__simulation__nand2__y__OT__STB, @object
	.size	work__run__ARCH__simulation__nand2__y__OT__STB, 16
work__run__ARCH__simulation__nand2__y__OT__STB:
	.long	1
	.long	0
	.byte	1
	.zero	3
	.long	2
.globl work__run__ARCH__simulation__INSTSIZE
	.align 4
	.type	work__run__ARCH__simulation__INSTSIZE, @object
	.size	work__run__ARCH__simulation__INSTSIZE, 4
work__run__ARCH__simulation__INSTSIZE:
	.long	68
.globl work__run__ARCH__simulation__a__OT__RTI
	.align 4
	.type	work__run__ARCH__simulation__a__OT__RTI, @object
	.size	work__run__ARCH__simulation__a__OT__RTI, 24
work__run__ARCH__simulation__a__OT__RTI:
	.byte	35
	.byte	0
	.byte	0
	.byte	0
	.long	0
	.long	std__standard__bit_vector__RTI
	.long	work__run__ARCH__simulation__a__OT__STB
	.long	2
	.long	8
	.type	work__run__ARCH__simulation__a__RTISTR, @object
	.size	work__run__ARCH__simulation__a__RTISTR, 2
work__run__ARCH__simulation__a__RTISTR:
	.byte	97
	.byte	0
.globl work__run__ARCH__simulation__a__RTI
	.align 4
	.type	work__run__ARCH__simulation__a__RTI, @object
	.size	work__run__ARCH__simulation__a__RTI, 16
work__run__ARCH__simulation__a__RTI:
	.byte	14
	.byte	1
	.byte	0
	.byte	0
	.long	work__run__ARCH__simulation__a__RTISTR
	.long	8
	.long	work__run__ARCH__simulation__a__OT__RTI
.globl work__run__ARCH__simulation__b__OT__RTI
	.align 4
	.type	work__run__ARCH__simulation__b__OT__RTI, @object
	.size	work__run__ARCH__simulation__b__OT__RTI, 24
work__run__ARCH__simulation__b__OT__RTI:
	.byte	35
	.byte	0
	.byte	0
	.byte	0
	.long	0
	.long	std__standard__bit_vector__RTI
	.long	work__run__ARCH__simulation__b__OT__STB
	.long	2
	.long	8
	.type	work__run__ARCH__simulation__b__RTISTR, @object
	.size	work__run__ARCH__simulation__b__RTISTR, 2
work__run__ARCH__simulation__b__RTISTR:
	.byte	98
	.byte	0
.globl work__run__ARCH__simulation__b__RTI
	.align 4
	.type	work__run__ARCH__simulation__b__RTI, @object
	.size	work__run__ARCH__simulation__b__RTI, 16
work__run__ARCH__simulation__b__RTI:
	.byte	14
	.byte	1
	.byte	0
	.byte	0
	.long	work__run__ARCH__simulation__b__RTISTR
	.long	16
	.long	work__run__ARCH__simulation__b__OT__RTI
.globl work__run__ARCH__simulation__y__OT__RTI
	.align 4
	.type	work__run__ARCH__simulation__y__OT__RTI, @object
	.size	work__run__ARCH__simulation__y__OT__RTI, 24
work__run__ARCH__simulation__y__OT__RTI:
	.byte	35
	.byte	0
	.byte	0
	.byte	0
	.long	0
	.long	std__standard__bit_vector__RTI
	.long	work__run__ARCH__simulation__y__OT__STB
	.long	2
	.long	8
	.type	work__run__ARCH__simulation__y__RTISTR, @object
	.size	work__run__ARCH__simulation__y__RTISTR, 2
work__run__ARCH__simulation__y__RTISTR:
	.byte	121
	.byte	0
.globl work__run__ARCH__simulation__y__RTI
	.align 4
	.type	work__run__ARCH__simulation__y__RTI, @object
	.size	work__run__ARCH__simulation__y__RTI, 16
work__run__ARCH__simulation__y__RTI:
	.byte	14
	.byte	1
	.byte	0
	.byte	0
	.long	work__run__ARCH__simulation__y__RTISTR
	.long	24
	.long	work__run__ARCH__simulation__y__OT__RTI
.globl work__run__ARCH__simulation__nand2__a__OT__RTI
	.align 4
	.type	work__run__ARCH__simulation__nand2__a__OT__RTI, @object
	.size	work__run__ARCH__simulation__nand2__a__OT__RTI, 24
work__run__ARCH__simulation__nand2__a__OT__RTI:
	.byte	35
	.byte	0
	.byte	0
	.byte	0
	.long	0
	.long	std__standard__bit_vector__RTI
	.long	work__run__ARCH__simulation__nand2__a__OT__STB
	.long	2
	.long	8
	.type	work__run__ARCH__simulation__nand2__a__RTISTR, @object
	.size	work__run__ARCH__simulation__nand2__a__RTISTR, 2
work__run__ARCH__simulation__nand2__a__RTISTR:
	.byte	97
	.byte	0
.globl work__run__ARCH__simulation__nand2__a__RTI
	.align 4
	.type	work__run__ARCH__simulation__nand2__a__RTI, @object
	.size	work__run__ARCH__simulation__nand2__a__RTI, 16
work__run__ARCH__simulation__nand2__a__RTI:
	.byte	16
	.byte	2
	.byte	5
	.byte	0
	.long	work__run__ARCH__simulation__nand2__a__RTISTR
	.long	8
	.long	work__run__ARCH__simulation__nand2__a__OT__RTI
	.type	work__run__ARCH__simulation__nand2__b__RTISTR, @object
	.size	work__run__ARCH__simulation__nand2__b__RTISTR, 2
work__run__ARCH__simulation__nand2__b__RTISTR:
	.byte	98
	.byte	0
.globl work__run__ARCH__simulation__nand2__b__RTI
	.align 4
	.type	work__run__ARCH__simulation__nand2__b__RTI, @object
	.size	work__run__ARCH__simulation__nand2__b__RTI, 16
work__run__ARCH__simulation__nand2__b__RTI:
	.byte	16
	.byte	2
	.byte	5
	.byte	0
	.long	work__run__ARCH__simulation__nand2__b__RTISTR
	.long	16
	.long	work__run__ARCH__simulation__nand2__a__OT__RTI
.globl work__run__ARCH__simulation__nand2__y__OT__RTI
	.align 4
	.type	work__run__ARCH__simulation__nand2__y__OT__RTI, @object
	.size	work__run__ARCH__simulation__nand2__y__OT__RTI, 24
work__run__ARCH__simulation__nand2__y__OT__RTI:
	.byte	35
	.byte	0
	.byte	0
	.byte	0
	.long	0
	.long	std__standard__bit_vector__RTI
	.long	work__run__ARCH__simulation__nand2__y__OT__STB
	.long	2
	.long	8
	.type	work__run__ARCH__simulation__nand2__y__RTISTR, @object
	.size	work__run__ARCH__simulation__nand2__y__RTISTR, 2
work__run__ARCH__simulation__nand2__y__RTISTR:
	.byte	121
	.byte	0
.globl work__run__ARCH__simulation__nand2__y__RTI
	.align 4
	.type	work__run__ARCH__simulation__nand2__y__RTI, @object
	.size	work__run__ARCH__simulation__nand2__y__RTI, 16
work__run__ARCH__simulation__nand2__y__RTI:
	.byte	16
	.byte	2
	.byte	3
	.byte	0
	.long	work__run__ARCH__simulation__nand2__y__RTISTR
	.long	24
	.long	work__run__ARCH__simulation__nand2__y__OT__RTI
	.type	work__run__ARCH__simulation__nand2__RTISTR, @object
	.size	work__run__ARCH__simulation__nand2__RTISTR, 6
work__run__ARCH__simulation__nand2__RTISTR:
	.byte	110
	.byte	97
	.byte	110
	.byte	100
	.byte	50
	.byte	0
	.align 4
	.type	work__run__ARCH__simulation__nand2__RTIARRAY, @object
	.size	work__run__ARCH__simulation__nand2__RTIARRAY, 16
work__run__ARCH__simulation__nand2__RTIARRAY:
	.long	work__run__ARCH__simulation__nand2__a__RTI
	.long	work__run__ARCH__simulation__nand2__b__RTI
	.long	work__run__ARCH__simulation__nand2__y__RTI
	.long	0
.globl work__run__ARCH__simulation__nand2__RTI
	.align 4
	.type	work__run__ARCH__simulation__nand2__RTI, @object
	.size	work__run__ARCH__simulation__nand2__RTI, 16
work__run__ARCH__simulation__nand2__RTI:
	.byte	20
	.byte	2
	.byte	0
	.byte	0
	.long	work__run__ARCH__simulation__nand2__RTISTR
	.long	3
	.long	work__run__ARCH__simulation__nand2__RTIARRAY
	.type	work__run__ARCH__simulation__u_nand2__RTISTR, @object
	.size	work__run__ARCH__simulation__u_nand2__RTISTR, 8
work__run__ARCH__simulation__u_nand2__RTISTR:
	.byte	117
	.byte	95
	.byte	110
	.byte	97
	.byte	110
	.byte	100
	.byte	50
	.byte	0
.globl work__run__ARCH__simulation__u_nand2__RTI
	.align 4
	.type	work__run__ARCH__simulation__u_nand2__RTI, @object
	.size	work__run__ARCH__simulation__u_nand2__RTI, 20
work__run__ARCH__simulation__u_nand2__RTI:
	.byte	10
	.byte	1
	.byte	0
	.byte	0
	.long	work__run__ARCH__simulation__u_nand2__RTISTR
	.long	32
	.long	work__run__ARCH__simulation__RTI
	.long	work__run__ARCH__simulation__nand2__RTI
	.type	work__run__ARCH__simulation__P0__RTISTR, @object
	.size	work__run__ARCH__simulation__P0__RTISTR, 3
work__run__ARCH__simulation__P0__RTISTR:
	.byte	80
	.byte	48
	.byte	0
	.align 4
	.type	work__run__ARCH__simulation__P0__RTIARRAY, @object
	.size	work__run__ARCH__simulation__P0__RTIARRAY, 4
work__run__ARCH__simulation__P0__RTIARRAY:
	.zero	4
.globl work__run__ARCH__simulation__P0__RTI
	.align 4
	.type	work__run__ARCH__simulation__P0__RTI, @object
	.size	work__run__ARCH__simulation__P0__RTI, 28
work__run__ARCH__simulation__P0__RTI:
	.byte	6
	.byte	2
	.byte	0
	.byte	0
	.long	work__run__ARCH__simulation__P0__RTISTR
	.long	64
	.long	work__run__ARCH__simulation__RTI
	.long	4
	.long	0
	.long	work__run__ARCH__simulation__P0__RTIARRAY
	.type	work__run__ARCH__simulation__RTISTR, @object
	.size	work__run__ARCH__simulation__RTISTR, 11
work__run__ARCH__simulation__RTISTR:
	.byte	115
	.byte	105
	.byte	109
	.byte	117
	.byte	108
	.byte	97
	.byte	116
	.byte	105
	.byte	111
	.byte	110
	.byte	0
	.align 4
	.type	work__run__ARCH__simulation__RTIARRAY, @object
	.size	work__run__ARCH__simulation__RTIARRAY, 28
work__run__ARCH__simulation__RTIARRAY:
	.long	work__run__ARCH__simulation__a__RTI
	.long	work__run__ARCH__simulation__b__RTI
	.long	work__run__ARCH__simulation__y__RTI
	.long	work__run__ARCH__simulation__nand2__RTI
	.long	work__run__ARCH__simulation__u_nand2__RTI
	.long	work__run__ARCH__simulation__P0__RTI
	.long	0
.globl work__run__ARCH__simulation__RTI
	.align 4
	.type	work__run__ARCH__simulation__RTI, @object
	.size	work__run__ARCH__simulation__RTI, 28
work__run__ARCH__simulation__RTI:
	.byte	5
	.byte	1
	.byte	0
	.byte	0
	.long	work__run__ARCH__simulation__RTISTR
	.long	0
	.long	work__run__RTI
	.long	68
	.long	6
	.long	work__run__ARCH__simulation__RTIARRAY
	.text
	.type	work__run__ARCH__simulation__u_nand2__ELAB, @function
work__run__ARCH__simulation__u_nand2__ELAB:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$48, %esp
	movl	8(%ebp), %eax
	movl	$work__run__ARCH__simulation__u_nand2__RTI, 36(%eax)
	movl	8(%ebp), %eax
	addl	$40, %eax
	movl	%eax, -36(%ebp)
	movl	8(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -32(%ebp)
	movl	$0, -28(%ebp)
.L34:
	cmpl	$2, -28(%ebp)
	jae	.L33
	movl	-28(%ebp), %ecx
	movl	-28(%ebp), %edx
	movl	-32(%ebp), %eax
	movl	(%eax,%edx,4), %edx
	movl	-36(%ebp), %eax
	movl	%edx, (%eax,%ecx,4)
	addl	$1, -28(%ebp)
	jmp	.L34
.L33:
	movl	8(%ebp), %eax
	addl	$48, %eax
	movl	%eax, -24(%ebp)
	movl	8(%ebp), %eax
	addl	$16, %eax
	movl	%eax, -20(%ebp)
	movl	$0, -16(%ebp)
.L36:
	cmpl	$2, -16(%ebp)
	jae	.L35
	movl	-16(%ebp), %ecx
	movl	-16(%ebp), %edx
	movl	-20(%ebp), %eax
	movl	(%eax,%edx,4), %edx
	movl	-24(%ebp), %eax
	movl	%edx, (%eax,%ecx,4)
	addl	$1, -16(%ebp)
	jmp	.L36
.L35:
	movl	8(%ebp), %eax
	addl	$56, %eax
	movl	%eax, -12(%ebp)
	movl	8(%ebp), %eax
	addl	$24, %eax
	movl	%eax, -8(%ebp)
	movl	$0, -4(%ebp)
.L38:
	cmpl	$2, -4(%ebp)
	jae	.L39
	movl	-4(%ebp), %ecx
	movl	-4(%ebp), %edx
	movl	-8(%ebp), %eax
	movl	(%eax,%edx,4), %edx
	movl	-12(%ebp), %eax
	movl	%edx, (%eax,%ecx,4)
	addl	$1, -4(%ebp)
	jmp	.L38
.L39:
	leave
	ret
	.size	work__run__ARCH__simulation__u_nand2__ELAB, .-work__run__ARCH__simulation__u_nand2__ELAB
	.section	.rodata
	.type	_UI00000001.1269, @object
	.size	_UI00000001.1269, 2
_UI00000001.1269:
	.byte	0
	.byte	1
	.type	_UI00000002.1277, @object
	.size	_UI00000002.1277, 2
_UI00000002.1277:
	.byte	1
	.byte	1
	.type	_UI00000003.1292, @object
	.size	_UI00000003.1292, 2
_UI00000003.1292:
	.byte	1
	.byte	0
	.align 4
	.type	work__run__ARCH__simulation__P0__U2__STB.1293, @object
	.size	work__run__ARCH__simulation__P0__U2__STB.1293, 16
work__run__ARCH__simulation__P0__U2__STB.1293:
	.long	0
	.long	1
	.byte	0
	.zero	3
	.long	2
	.align 4
	.type	_UI00000004.1295, @object
	.size	_UI00000004.1295, 12
_UI00000004.1295:
	.long	_UI00000000
	.long	38
	.long	16
	.type	_UI00000005.1297, @object
	.size	_UI00000005.1297, 13
_UI00000005.1297:
	.byte	69
	.byte	114
	.byte	114
	.byte	111
	.byte	114
	.byte	32
	.byte	99
	.byte	97
	.byte	115
	.byte	101
	.byte	32
	.byte	35
	.byte	48
	.align 4
	.type	work__run__ARCH__simulation__P0__U4__STB.1298, @object
	.size	work__run__ARCH__simulation__P0__U4__STB.1298, 16
work__run__ARCH__simulation__P0__U4__STB.1298:
	.long	1
	.long	13
	.byte	0
	.zero	3
	.long	13
	.text
	.type	work__run__ARCH__simulation__P0__PROC, @function
work__run__ARCH__simulation__P0__PROC:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$104, %esp
	movl	8(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -48(%ebp)
	movl	$0, -44(%ebp)
.L45:
	cmpl	$2, -44(%ebp)
	jae	.L41
	movl	-44(%ebp), %edx
	movl	-48(%ebp), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -40(%ebp)
	movl	8(%ebp), %eax
	addl	$64, %eax
	addl	-44(%ebp), %eax
	movl	%eax, -32(%ebp)
	movl	$_UI00000001.1269, %edx
	movl	-44(%ebp), %eax
	movzbl	(%edx,%eax), %edx
	movl	-32(%ebp), %eax
	movb	%dl, (%eax)
	movl	-40(%ebp), %eax
	movzbl	46(%eax), %eax
	testb	%al, %al
	je	.L42
	movb	$1, -33(%ebp)
	jmp	.L43
.L42:
	movl	-40(%ebp), %eax
	movzbl	(%eax), %edx
	movl	-32(%ebp), %eax
	movzbl	(%eax), %eax
	cmpb	%al, %dl
	setne	%al
	movb	%al, -33(%ebp)
.L43:
	movl	-40(%ebp), %eax
	movl	40(%eax), %eax
	testl	%eax, %eax
	sete	%al
	andb	-33(%ebp), %al
	testb	%al, %al
	je	.L44
	movl	-40(%ebp), %eax
	movl	__ghdl_signal_active_chain, %edx
	movl	%edx, 40(%eax)
	movl	-40(%ebp), %eax
	movl	%eax, __ghdl_signal_active_chain
.L44:
	addl	$1, -44(%ebp)
	jmp	.L45
.L41:
	movl	8(%ebp), %eax
	addl	$16, %eax
	movl	%eax, -28(%ebp)
	movl	$0, -24(%ebp)
.L50:
	cmpl	$2, -24(%ebp)
	jae	.L46
	movl	-24(%ebp), %edx
	movl	-28(%ebp), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -20(%ebp)
	movl	8(%ebp), %eax
	addl	$66, %eax
	addl	-24(%ebp), %eax
	movl	%eax, -12(%ebp)
	movl	$_UI00000002.1277, %edx
	movl	-24(%ebp), %eax
	movzbl	(%edx,%eax), %edx
	movl	-12(%ebp), %eax
	movb	%dl, (%eax)
	movl	-20(%ebp), %eax
	movzbl	46(%eax), %eax
	testb	%al, %al
	je	.L47
	movb	$1, -13(%ebp)
	jmp	.L48
.L47:
	movl	-20(%ebp), %eax
	movzbl	(%eax), %edx
	movl	-12(%ebp), %eax
	movzbl	(%eax), %eax
	cmpb	%al, %dl
	setne	%al
	movb	%al, -13(%ebp)
.L48:
	movl	-20(%ebp), %eax
	movl	40(%eax), %eax
	testl	%eax, %eax
	sete	%al
	andb	-13(%ebp), %al
	testb	%al, %al
	je	.L49
	movl	-20(%ebp), %eax
	movl	__ghdl_signal_active_chain, %edx
	movl	%edx, 40(%eax)
	movl	-20(%ebp), %eax
	movl	%eax, __ghdl_signal_active_chain
.L49:
	addl	$1, -24(%ebp)
	jmp	.L50
.L46:
	movl	$10000000, (%esp)
	movl	$0, 4(%esp)
	call	__ghdl_process_wait_timeout
	movl	8(%ebp), %eax
	addl	$24, %eax
	movl	%eax, -8(%ebp)
	movl	$0, -4(%ebp)
.L52:
	cmpl	$2, -4(%ebp)
	jae	.L51
	movl	-4(%ebp), %ecx
	movl	-4(%ebp), %edx
	movl	-8(%ebp), %eax
	movl	(%eax,%edx,4), %eax
	movzbl	(%eax), %eax
	movb	%al, -50(%ebp,%ecx)
	addl	$1, -4(%ebp)
	jmp	.L52
.L51:
	leal	-50(%ebp), %eax
	movl	%eax, -60(%ebp)
	movl	$work__run__ARCH__simulation__y__OT__STB, -56(%ebp)
	movl	$_UI00000003.1292, %eax
	movl	%eax, -68(%ebp)
	movl	$work__run__ARCH__simulation__P0__U2__STB.1293, -64(%ebp)
	leal	-68(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	-60(%ebp), %eax
	movl	%eax, (%esp)
	call	std__standard__bit_vector_EQ
	xorl	$1, %eax
	testb	%al, %al
	je	.L53
	movl	$_UI00000005.1297, %eax
	movl	%eax, -76(%ebp)
	movl	$work__run__ARCH__simulation__P0__U4__STB.1298, -72(%ebp)
	movl	$0, 12(%esp)
	movl	$_UI00000004.1295, 8(%esp)
	movb	$2, 4(%esp)
	leal	-76(%ebp), %eax
	movl	%eax, (%esp)
	call	__ghdl_assert_failed
.L53:
	call	__ghdl_process_wait_exit
	leave
	ret
	.size	work__run__ARCH__simulation__P0__PROC, .-work__run__ARCH__simulation__P0__PROC
	.section	.rodata
	.type	_UI00000006.1360, @object
	.size	_UI00000006.1360, 2
_UI00000006.1360:
	.zero	2
	.type	_UI00000007.1365, @object
	.size	_UI00000007.1365, 2
_UI00000007.1365:
	.zero	2
	.text
.globl work__run__ARCH__simulation__ELAB
	.type	work__run__ARCH__simulation__ELAB, @function
work__run__ARCH__simulation__ELAB:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$68, %esp
	movl	8(%ebp), %eax
	movl	%eax, -48(%ebp)
	movl	8(%ebp), %eax
	movl	$work__run__ARCH__simulation__RTI, (%eax)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	work__run__ELAB
	movl	-48(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	$work__run__ARCH__simulation__RTI, 4(%esp)
	movl	$work__run__ARCH__simulation__a__RTI, (%esp)
	call	__ghdl_signal_name_rti
	movl	-48(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -44(%ebp)
	movl	$0, -40(%ebp)
.L57:
	cmpl	$2, -40(%ebp)
	jae	.L56
	movl	-40(%ebp), %ebx
	movl	$_UI00000006.1360, %edx
	movl	-40(%ebp), %eax
	movzbl	(%edx,%eax), %eax
	movl	$0, 8(%esp)
	movl	$0, 4(%esp)
	movb	%al, (%esp)
	call	__ghdl_create_signal_b2
	movl	%eax, %edx
	movl	-44(%ebp), %eax
	movl	%edx, (%eax,%ebx,4)
	addl	$1, -40(%ebp)
	jmp	.L57
.L56:
	movl	-48(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	$work__run__ARCH__simulation__RTI, 4(%esp)
	movl	$work__run__ARCH__simulation__b__RTI, (%esp)
	call	__ghdl_signal_name_rti
	movl	-48(%ebp), %eax
	addl	$16, %eax
	movl	%eax, -36(%ebp)
	movl	$0, -32(%ebp)
.L59:
	cmpl	$2, -32(%ebp)
	jae	.L58
	movl	-32(%ebp), %ebx
	movl	$_UI00000007.1365, %edx
	movl	-32(%ebp), %eax
	movzbl	(%edx,%eax), %eax
	movl	$0, 8(%esp)
	movl	$0, 4(%esp)
	movb	%al, (%esp)
	call	__ghdl_create_signal_b2
	movl	%eax, %edx
	movl	-36(%ebp), %eax
	movl	%edx, (%eax,%ebx,4)
	addl	$1, -32(%ebp)
	jmp	.L59
.L58:
	movl	-48(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	$work__run__ARCH__simulation__RTI, 4(%esp)
	movl	$work__run__ARCH__simulation__y__RTI, (%esp)
	call	__ghdl_signal_name_rti
	movl	-48(%ebp), %eax
	addl	$24, %eax
	movl	%eax, -28(%ebp)
	movl	$0, -24(%ebp)
.L61:
	cmpl	$2, -24(%ebp)
	jae	.L60
	movl	-24(%ebp), %ebx
	movl	$0, 8(%esp)
	movl	$0, 4(%esp)
	movb	$0, (%esp)
	call	__ghdl_create_signal_b2
	movl	%eax, %edx
	movl	-28(%ebp), %eax
	movl	%edx, (%eax,%ebx,4)
	addl	$1, -24(%ebp)
	jmp	.L61
.L60:
	movl	-48(%ebp), %eax
	movl	%eax, (%esp)
	call	work__run__ARCH__simulation__u_nand2__ELAB
	movl	-48(%ebp), %eax
	addl	$64, %eax
	movl	$work__run__ARCH__simulation__P0__PROC, %edx
	movl	-48(%ebp), %ecx
	movl	%eax, 12(%esp)
	movl	$work__run__ARCH__simulation__P0__RTI, 8(%esp)
	movl	%edx, 4(%esp)
	movl	%ecx, (%esp)
	call	__ghdl_process_register
	movl	-48(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -20(%ebp)
	movl	$0, -16(%ebp)
.L63:
	cmpl	$2, -16(%ebp)
	jae	.L62
	movl	-48(%ebp), %eax
	addl	$64, %eax
	movl	%eax, %ecx
	addl	-16(%ebp), %ecx
	movl	-16(%ebp), %edx
	movl	-20(%ebp), %eax
	movl	(%eax,%edx,4), %eax
	movl	%ecx, 4(%esp)
	movl	%eax, (%esp)
	call	__ghdl_signal_direct_driver
	addl	$1, -16(%ebp)
	jmp	.L63
.L62:
	movl	-48(%ebp), %eax
	addl	$16, %eax
	movl	%eax, -12(%ebp)
	movl	$0, -8(%ebp)
.L65:
	cmpl	$2, -8(%ebp)
	jae	.L66
	movl	-48(%ebp), %eax
	addl	$66, %eax
	movl	%eax, %ecx
	addl	-8(%ebp), %ecx
	movl	-8(%ebp), %edx
	movl	-12(%ebp), %eax
	movl	(%eax,%edx,4), %eax
	movl	%ecx, 4(%esp)
	movl	%eax, (%esp)
	call	__ghdl_signal_direct_driver
	addl	$1, -8(%ebp)
	jmp	.L65
.L66:
	addl	$68, %esp
	popl	%ebx
	popl	%ebp
	ret
	.size	work__run__ARCH__simulation__ELAB, .-work__run__ARCH__simulation__ELAB
	.ident	"GCC: (GNU) 4.3.4"
	.section	.note.GNU-stack,"",@progbits
