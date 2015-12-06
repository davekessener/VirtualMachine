	.file	"main.c"
	.comm	states,24,16
	.comm	time,4,4
	.comm	update,4,4
	.comm	state,4,4
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	call	initialize
.L3:
	call	checktimer
	movl	time(%rip), %edx
	addl	%edx, %eax
	movl	%eax, time(%rip)
	movl	update(%rip), %eax
	testl	%eax, %eax
	je	.L2
	movl	time(%rip), %eax
	movl	%eax, %edi
	call	displaytime
.L2:
	movl	state(%rip), %eax
	cltq
	movq	states(,%rax,8), %rax
	call	*%rax
	movl	%eax, state(%rip)
	jmp	.L3
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.globl	initialize
	.type	initialize, @function
initialize:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	call	TI_Init_Board
	call	initHW
	movq	$state_init, states(%rip)
	movq	$state_running, states+8(%rip)
	movq	$state_hold, states+24(%rip)
	movl	$0, %esi
	movl	$64, %edi
	call	setled
	movl	$0, %esi
	movl	$128, %edi
	call	setled
	movl	$0, time(%rip)
	movl	$0, update(%rip)
	call	checktimer
	movl	$0, %edi
	call	displaytime
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	initialize, .-initialize
	.globl	state_init
	.type	state_init, @function
state_init:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$128, %edi
	call	checkkey
	testl	%eax, %eax
	je	.L6
	movl	$1, %esi
	movl	$128, %edi
	call	setled
	movl	$1, update(%rip)
	movl	$0, time(%rip)
	movl	$1, %eax
	jmp	.L7
.L6:
	movl	$0, %eax
.L7:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	state_init, .-state_init
	.globl	state_running
	.type	state_running, @function
state_running:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$32, %edi
	call	checkkey
	testl	%eax, %eax
	je	.L9
	movl	$0, %esi
	movl	$128, %edi
	call	setled
	movl	$0, update(%rip)
	movl	$0, %eax
	jmp	.L10
.L9:
	movl	$64, %edi
	call	checkkey
	testl	%eax, %eax
	je	.L11
	movl	$1, %esi
	movl	$64, %edi
	call	setled
	movl	$0, update(%rip)
	movl	$3, %eax
	jmp	.L10
.L11:
	movl	$1, %eax
.L10:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	state_running, .-state_running
	.globl	state_hold
	.type	state_hold, @function
state_hold:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$32, %edi
	call	checkkey
	testl	%eax, %eax
	je	.L13
	movl	$0, %esi
	movl	$64, %edi
	call	setled
	movl	$0, %esi
	movl	$128, %edi
	call	setled
	movl	$0, %eax
	jmp	.L14
.L13:
	movl	$128, %edi
	call	checkkey
	testl	%eax, %eax
	je	.L15
	movl	$0, %esi
	movl	$64, %edi
	call	setled
	movl	$1, update(%rip)
	movl	$1, %eax
	jmp	.L14
.L15:
	movl	$3, %eax
.L14:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	state_hold, .-state_hold
	.globl	checkkey
	.type	checkkey, @function
checkkey:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movq	portin(%rip), %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	-4(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	checkkey, .-checkkey
	.globl	setled
	.type	setled, @function
setled:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	cmpl	$0, -8(%rbp)
	je	.L19
	movq	portset(%rip), %rax
	movl	-4(%rbp), %edx
	movw	%dx, (%rax)
	jmp	.L18
.L19:
	movq	portclr(%rip), %rax
	movl	-4(%rbp), %edx
	movw	%dx, (%rax)
.L18:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	setled, .-setled
	.globl	write_2d
	.type	write_2d, @function
write_2d:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movq	-8(%rbp), %rsi
	leaq	1(%rsi), %rax
	movq	%rax, -8(%rbp)
	movl	-12(%rbp), %ecx
	movl	$1717986919, %edx
	movl	%ecx, %eax
	imull	%edx
	sarl	$2, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	$1717986919, %edx
	movl	%ecx, %eax
	imull	%edx
	sarl	$2, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	subl	%eax, %ecx
	movl	%ecx, %edx
	movl	%edx, %eax
	addl	$48, %eax
	movb	%al, (%rsi)
	movl	-12(%rbp), %ecx
	movl	$1717986919, %edx
	movl	%ecx, %eax
	imull	%edx
	sarl	$2, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	subl	%eax, %ecx
	movl	%ecx, %edx
	movl	%edx, %eax
	addl	$48, %eax
	movl	%eax, %edx
	movq	-8(%rbp), %rax
	movb	%dl, (%rax)
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	write_2d, .-write_2d
	.globl	puts
	.type	puts, @function
puts:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movq	%rdx, -16(%rbp)
	jmp	.L23
.L24:
	movq	-16(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -16(%rbp)
	movzbl	(%rax), %eax
	movsbl	%al, %edx
	movl	-4(%rbp), %eax
	leal	1(%rax), %ecx
	movl	%ecx, -4(%rbp)
	movl	-8(%rbp), %ecx
	movl	%ecx, %esi
	movl	%eax, %edi
	call	tftPut
.L23:
	movq	-16(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L24
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	puts, .-puts
	.section	.rodata
.LC0:
	.string	"HAW Timer"
	.text
	.globl	displaytime
	.type	displaytime, @function
displaytime:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movl	-20(%rbp), %ecx
	movl	$1374389535, %edx
	movl	%ecx, %eax
	mull	%edx
	movl	%edx, %eax
	shrl	$5, %eax
	imull	$100, %eax, %eax
	subl	%eax, %ecx
	movl	%ecx, %eax
	movl	%eax, -16(%rbp)
	movl	-20(%rbp), %eax
	movl	$1374389535, %edx
	mull	%edx
	movl	%edx, %eax
	shrl	$5, %eax
	movl	%eax, -20(%rbp)
	movl	-20(%rbp), %ecx
	movl	$-2004318071, %edx
	movl	%ecx, %eax
	mull	%edx
	movl	%edx, %eax
	shrl	$5, %eax
	sall	$2, %eax
	movl	%eax, %edx
	sall	$4, %edx
	subl	%eax, %edx
	movl	%ecx, %eax
	subl	%edx, %eax
	movl	%eax, -12(%rbp)
	movl	-20(%rbp), %eax
	movl	$-2004318071, %edx
	mull	%edx
	movl	%edx, %eax
	shrl	$5, %eax
	movl	%eax, -20(%rbp)
	movl	-20(%rbp), %ecx
	movl	$-2004318071, %edx
	movl	%ecx, %eax
	mull	%edx
	movl	%edx, %eax
	shrl	$5, %eax
	sall	$2, %eax
	movl	%eax, %edx
	sall	$4, %edx
	subl	%eax, %edx
	movl	%ecx, %eax
	subl	%edx, %eax
	movl	%eax, -8(%rbp)
	movl	-20(%rbp), %eax
	movl	$-2004318071, %edx
	mull	%edx
	movl	%edx, %eax
	shrl	$5, %eax
	movl	%eax, -20(%rbp)
	movl	-20(%rbp), %ecx
	movl	$-1431655765, %edx
	movl	%ecx, %eax
	mull	%edx
	shrl	$4, %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	sall	$3, %eax
	subl	%eax, %ecx
	movl	%ecx, %edx
	movl	%edx, -4(%rbp)
	movl	-20(%rbp), %eax
	movl	$-1431655765, %edx
	mull	%edx
	movl	%edx, %eax
	shrl	$4, %eax
	movl	%eax, -20(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	movl	$disp.1799, %edi
	call	write_2d
	movl	-8(%rbp), %eax
	movl	%eax, %esi
	movl	$disp.1799+3, %edi
	call	write_2d
	movl	-12(%rbp), %eax
	movl	%eax, %esi
	movl	$disp.1799+6, %edi
	call	write_2d
	movl	-16(%rbp), %eax
	movl	%eax, %esi
	movl	$disp.1799+9, %edi
	call	write_2d
	movl	$.LC0, %edx
	movl	$1, %esi
	movl	$4, %edi
	call	puts
	movl	$disp.1799, %edx
	movl	$3, %esi
	movl	$3, %edi
	call	puts
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	displaytime, .-displaytime
	.globl	checktimer
	.type	checktimer, @function
checktimer:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	timer(%rip), %rax
	movzwl	(%rax), %eax
	movw	%ax, -6(%rbp)
	movzwl	-6(%rbp), %edx
	movzwl	time.1807(%rip), %eax
	movzwl	%ax, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, -4(%rbp)
	movzwl	-6(%rbp), %eax
	movw	%ax, time.1807(%rip)
	movl	-4(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	checktimer, .-checktimer
	.data
	.type	disp.1799, @object
	.size	disp.1799, 12
disp.1799:
	.string	"  :  :  .  "
	.local	time.1807
	.comm	time.1807,2,2
	.ident	"GCC: (Ubuntu 4.8.1-2ubuntu1~12.04) 4.8.1"
	.section	.note.GNU-stack,"",@progbits
