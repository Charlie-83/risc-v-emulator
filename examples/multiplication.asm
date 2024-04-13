# Multiply 123 by 55 and store result in register x12
addi x9, x0, 1
addi x10, x0, 123
addi x11, x0, 55
add x12, x12, x11
sub x10, x10, x9
bne x10, x0, -6
