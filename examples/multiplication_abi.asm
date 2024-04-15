# Multiply 123 by 55 and store result in register s3
addi t0, zero, 1
addi t1, zero, 123
addi t2, zero, 55
add t3, t3, t2
sub t1, t1, t0
bne t1, zero, -6
