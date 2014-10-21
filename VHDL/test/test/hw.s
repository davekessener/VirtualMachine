	.file	"hw.vhdl"
	.section	.rodata
	.type	_UI00000000, @object
	.size	_UI00000000, 8
_UI00000000:
	.byte	104
	.byte	119
	.byte	46
	.byte	118
	.byte	104
	.byte	100
	.byte	108
	.byte	0
	.type	work__hello_world__RTISTR, @object
	.size	work__hello_world__RTISTR, 12
work__hello_world__RTISTR:
	.byte	104
	.byte	101
	.byte	108
	.byte	108
	.byte	111
	.byte	95
	.byte	119
	.byte	111
	.byte	114
	.byte	108
	.byte	100
	.byte	0
	.align 4
	.type	work__hello_world__RTIARRAY, @object
	.size	work__hello_world__RTIARRAY, 4
work__hello_world__RTIARRAY:
	.zero	4
.globl work__hello_world__RTI
	.align 4
	.type	work__hello_world__RTI, @object
	.size	work__hello_world__RTI, 28
work__hello_world__RTI:
	.byte	4
	.byte	1
	.byte	0
	.byte	0
	.long	work__hello_world__RTISTR
	.long	0
	.long	work__RTI
	.long	8
	.long	0
	.long	work__hello_world__RTIARRAY
	.text
.globl work__hello_world__ELAB
	.type	work__hello_world__ELAB, @function
work__hello_world__ELAB:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movzbl	std__textio__ELABORATED, %eax
	xorl	$1, %eax
	testb	%al, %al
	je	.L3
	call	std__textio__ELAB_BODY
.L3:
	leave
	ret
	.size	work__hello_world__ELAB, .-work__hello_world__ELAB
.globl work__hello_world__ARCH__behavior__INSTSIZE
	.section	.rodata
	.align 4
	.type	work__hello_world__ARCH__behavior__INSTSIZE, @object
	.size	work__hello_world__ARCH__behavior__INSTSIZE, 4
work__hello_world__ARCH__behavior__INSTSIZE:
	.long	16
	.type	work__hello_world__ARCH__behavior__P0__l__RTISTR, @object
	.size	work__hello_world__ARCH__behavior__P0__l__RTISTR, 2
work__hello_world__ARCH__behavior__P0__l__RTISTR:
	.byte	108
	.byte	0
.globl work__hello_world__ARCH__behavior__P0__l__RTI
	.align 4
	.type	work__hello_world__ARCH__behavior__P0__l__RTI, @object
	.size	work__hello_world__ARCH__behavior__P0__l__RTI, 16
work__hello_world__ARCH__behavior__P0__l__RTI:
	.byte	13
	.byte	2
	.byte	0
	.byte	0
	.long	work__hello_world__ARCH__behavior__P0__l__RTISTR
	.long	0
	.long	std__textio__line__RTI
	.type	work__hello_world__ARCH__behavior__P0__RTISTR, @object
	.size	work__hello_world__ARCH__behavior__P0__RTISTR, 3
work__hello_world__ARCH__behavior__P0__RTISTR:
	.byte	80
	.byte	48
	.byte	0
	.align 4
	.type	work__hello_world__ARCH__behavior__P0__RTIARRAY, @object
	.size	work__hello_world__ARCH__behavior__P0__RTIARRAY, 8
work__hello_world__ARCH__behavior__P0__RTIARRAY:
	.long	work__hello_world__ARCH__behavior__P0__l__RTI
	.long	0
.globl work__hello_world__ARCH__behavior__P0__RTI
	.align 4
	.type	work__hello_world__ARCH__behavior__P0__RTI, @object
	.size	work__hello_world__ARCH__behavior__P0__RTI, 28
work__hello_world__ARCH__behavior__P0__RTI:
	.byte	6
	.byte	2
	.byte	0
	.byte	0
	.long	work__hello_world__ARCH__behavior__P0__RTISTR
	.long	8
	.long	work__hello_world__ARCH__behavior__RTI
	.long	8
	.long	1
	.long	work__hello_world__ARCH__behavior__P0__RTIARRAY
	.type	work__hello_world__ARCH__behavior__RTISTR, @object
	.size	work__hello_world__ARCH__behavior__RTISTR, 9
work__hello_world__ARCH__behavior__RTISTR:
	.byte	98
	.byte	101
	.byte	104
	.byte	97
	.byte	118
	.byte	105
	.byte	111
	.byte	114
	.byte	0
	.align 4
	.type	work__hello_world__ARCH__behavior__RTIARRAY, @object
	.size	work__hello_world__ARCH__behavior__RTIARRAY, 8
work__hello_world__ARCH__behavior__RTIARRAY:
	.long	work__hello_world__ARCH__behavior__P0__RTI
	.long	0
.globl work__hello_world__ARCH__behavior__RTI
	.align 4
	.type	work__hello_world__ARCH__behavior__RTI, @object
	.size	work__hello_world__ARCH__behavior__RTI, 28
work__hello_world__ARCH__behavior__RTI:
	.byte	5
	.byte	1
	.byte	0
	.byte	0
	.long	work__hello_world__ARCH__behavior__RTISTR
	.long	0
	.long	work__hello_world__RTI
	.long	16
	.long	1
	.long	work__hello_world__ARCH__behavior__RTIARRAY
	.type	_UI00000001.1255, @object
	.size	_UI00000001.1255, 12
_UI00000001.1255:
	.byte	72
	.byte	101
	.byte	108
	.byte	108
	.byte	111
	.byte	32
	.byte	119
	.byte	111
	.byte	114
	.byte	108
	.byte	100
	.byte	33
	.align 4
	.type	work__hello_world__ARCH__behavior__P0__U1__STB.1256, @object
	.size	work__hello_world__ARCH__behavior__P0__U1__STB.1256, 16
work__hello_world__ARCH__behavior__P0__U1__STB.1256:
	.long	1
	.long	12
	.byte	0
	.zero	3
	.long	12
	.text
	.type	work__hello_world__ARCH__behavior__P0__PROC, @function
work__hello_world__ARCH__behavior__P0__PROC:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$88, %esp
	movl	8(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -40(%ebp)
	leal	-48(%ebp), %eax
	movl	%eax, -36(%ebp)
	movl	-40(%ebp), %eax
	movl	%eax, -32(%ebp)
	movl	-32(%ebp), %eax
	movl	(%eax), %edx
	movl	-36(%ebp), %eax
	movl	%edx, (%eax)
	movl	-32(%ebp), %eax
	movl	4(%eax), %edx
	movl	-36(%ebp), %eax
	movl	%edx, 4(%eax)
	movl	$_UI00000001.1255, %eax
	movl	%eax, -56(%ebp)
	movl	$work__hello_world__ARCH__behavior__P0__U1__STB.1256, -52(%ebp)
	movl	$0, 12(%esp)
	movb	$0, 8(%esp)
	leal	-56(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	-48(%ebp), %eax
	movl	%eax, (%esp)
	call	std__textio__writeO7
	movl	-40(%ebp), %eax
	movl	%eax, -28(%ebp)
	leal	-48(%ebp), %eax
	movl	%eax, -24(%ebp)
	movl	-24(%ebp), %eax
	movl	(%eax), %edx
	movl	-28(%ebp), %eax
	movl	%edx, (%eax)
	movl	-24(%ebp), %eax
	movl	4(%eax), %edx
	movl	-28(%ebp), %eax
	movl	%edx, 4(%eax)
	movl	8(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -20(%ebp)
	leal	-64(%ebp), %eax
	movl	%eax, -16(%ebp)
	movl	-20(%ebp), %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	movl	(%eax), %edx
	movl	-16(%ebp), %eax
	movl	%edx, (%eax)
	movl	-12(%ebp), %eax
	movl	4(%eax), %edx
	movl	-16(%ebp), %eax
	movl	%edx, 4(%eax)
	movl	std__textio__output, %eax
	movl	%eax, 4(%esp)
	leal	-64(%ebp), %eax
	movl	%eax, (%esp)
	call	std__textio__writeline
	movl	-20(%ebp), %eax
	movl	%eax, -8(%ebp)
	leal	-64(%ebp), %eax
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	movl	(%eax), %edx
	movl	-8(%ebp), %eax
	movl	%edx, (%eax)
	movl	-4(%ebp), %eax
	movl	4(%eax), %edx
	movl	-8(%ebp), %eax
	movl	%edx, 4(%eax)
	call	__ghdl_process_wait_exit
	leave
	ret
	.size	work__hello_world__ARCH__behavior__P0__PROC, .-work__hello_world__ARCH__behavior__P0__PROC
.globl work__hello_world__ARCH__behavior__ELAB
	.type	work__hello_world__ARCH__behavior__ELAB, @function
work__hello_world__ARCH__behavior__ELAB:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$40, %esp
	movl	8(%ebp), %eax
	movl	%eax, -8(%ebp)
	movl	8(%ebp), %eax
	movl	$work__hello_world__ARCH__behavior__RTI, (%eax)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	work__hello_world__ELAB
	movl	-8(%ebp), %eax
	addl	$8, %eax
	movl	$work__hello_world__ARCH__behavior__P0__PROC, %edx
	movl	-8(%ebp), %ecx
	movl	%eax, 12(%esp)
	movl	$work__hello_world__ARCH__behavior__P0__RTI, 8(%esp)
	movl	%edx, 4(%esp)
	movl	%ecx, (%esp)
	call	__ghdl_process_register
	movl	-8(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	movl	$0, 4(%eax)
	movl	-4(%ebp), %eax
	movl	$0, (%eax)
	leave
	ret
	.size	work__hello_world__ARCH__behavior__ELAB, .-work__hello_world__ARCH__behavior__ELAB
	.ident	"GCC: (GNU) 4.3.4"
	.section	.note.GNU-stack,"",@progbits
