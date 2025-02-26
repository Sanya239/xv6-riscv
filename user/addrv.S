.data
msg: .asciz "Incorrect format\n"
numbermsg: .asciz "%d\n"
buffer: .space 30

.text
.global main
main:
    la a0, buffer
    call gets
    mv s0, a0

    la a0, buffer
    jal ra, verify

verified:
    mv a0, s0
    jal ra, read_num
    mv s1, a0

    mv a0, s2
    jal ra, read_num
    mv s2, a0


    add s3, s1, s2
    li a0, 1
    la a1, numbermsg
    mv a2, s3
    call fprintf
    jal zero, exit

verify: #check format of a0 and return whitespace position to a0 (or exit with error). return to ra
    lb t0, 0(a0)
    li t1, 0
    li t6, 45
    bne t0, t6, read_before_whitespace
    addi a0, a0, 1
    addi t1, t1, 1
    jal zero, read_before_whitespace

whitespace_found:
    addi a0, a0, 1 #skip whitespace
    addi t1, t1, 1
    add s2, s0, t1 #second number start
    lb t0, 0(a0)
    li t6, 45
    bne t0, t6, read_before_0_or_newline
    addi a0, a0, 1
    addi t1, t1, 1
    jal zero, read_before_0_or_newline


read_before_0_or_newline:
    lb t0, 0(a0)
    li t6, 10 # \n
    beq t0, t6, verified
    li t6, 0 # 0 symbol
    beq t0, t6, verified


    li t6, 58 # '9' +1
    bge t0, t6, format_error
    li t6, 48 #'0
    blt t0, t6, format_error
    addi a0, a0, 1
    jal zero, read_before_0_or_newline

read_before_whitespace:
    lb t0, 0(a0)
    li t6, 32 #whitespace
    beq t0, t6, whitespace_found


    li t6, 58 # '9' +1
    bge t0, t6, format_error

    li t6, 48 #'0'
    blt t0, t6, format_error

    addi a0, a0, 1
    addi t1, t1, 1
    jal zero, read_before_whitespace


format_error:
    li a0, 1
    la a1, msg
    call fprintf
    li a7, 2
    li a0, 1
    ecall

exit: #exit with code 0
    li a7, 2
    li a0, 0
    ecall

read_num: #read positive or negative number and put it to a0. Then return to ra
    lb t0, 0(a0)
    li t1, 45
    li s11, 1
    bne t0, t1, calling_atoi_and_mult_s11
    li s11, -1
    addi a0, a0, 1
    jal zero, calling_atoi_and_mult_s11

calling_atoi_and_mult_s11:
    mv s10, ra
    call atoi
    mul a0, a0, s11
    jalr zero, s10, 0
