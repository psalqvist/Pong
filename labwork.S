  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #

.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #
	.data
	.align 2
mytime:	.word 0x0000
timstr:	.ascii "hello\0"

	.text
	.align 2
	.global hexasc
	.global delay
	.global time2string
  .global enable_interrupt

hexasc:
	li	$t0, 9		# loads 9 into register t0

	andi	$a0, $a0, 0xF	# masks everything but the 4 least significant bits

	bgt	$a0, $t0, large	# branches of to label large if the argument is bigger than 9
	nop

	addi	$v0, $a0, 0x30	# if we don't fo to large, add 0x30 to the argument register. Gives correct ASCII 0-9

	j	hexend		# jump to hexend

	nop

large:
	addi	$v0, $a0, 0x37	# adds 0x37 to argument register. Gives correct ASCII 10-15

hexend:
	jr	$ra		# jump to return adress
	nop

delay:
	PUSH	$s0
	PUSH	$ra

	move	$s0, $a0	# setting $s0 to the argument of delay
	li	$t0, 0		# $t0 = i
	li	$t1, 3700000	# $t1 = upper bound of i in loop

while:
	ble  	$s0, $0, end_while
	nop
	addi	$s0, $s0, -1	# subtracting 1 from $s0 holding argument

loop:
	bge	$t0, $t1, end_loop
	nop
	addi	$t0, $t0, 1	# adding 1 to i
	j	loop
	nop


end_loop:
	j	while
	nop

end_while:

	POP	$ra
	POP	$s0

 	jr $ra
 	nop



time2string:
	PUSH	$s0
	PUSH	$s1
	PUSH	$ra		# jal will overwrite return address, so must be stored on stack

	# we now have time value in s1 and the adress that a0 points to in s0
	move	$s0, $a0
	move	$s1, $a1


	andi	$t0, $s1, 0xF000	# clean time value to only include 4 msb
	srl	$a0, $t0, 12		# shift right 12 bits to place bits in lsb, hexasc works on the 4 least significant
	jal	hexasc
	nop
	sb	$v0, 0($s0)

	andi	$t1, $s1, 0x0F00	# clean time value to only include the 4 digits to the right of msb
	srl	$a0, $t1, 8		# shift right 8 bits to place bits in lsb, hexasc works on the 4 least significant
	jal	hexasc
	nop
	sb	$v0, 1($s0)

	li	$t2, 0x3A		# colon according to ASCII
	sb	$t2, 2($s0)

	andi	$t3, $s1, 0x00F0	# clean time value to only include the 4 digits to the right of msb
	srl	$a0, $t3, 4		# shift right 8 bits to place bits in lsb, hexasc works on the 4 least significant
	jal	hexasc
	nop
	sb	$v0, 3($s0)

	andi	$a0, $s1, 0x000F	# clean time value to only include the 4 digits to the right of msb
	jal	hexasc
	nop
	sb	$v0, 4($s0)

	li	$t5, 0x00		# finnish with a null byte
	sb	$t5, 5($s0)

	POP	$ra			# restore registers from stack
	POP	$s1
	POP	$s0

	jr 	$ra
	nop

enable_interrupt:
  PUSH  $ra
  ei
  POP $ra

  jr  $ra
  nop
