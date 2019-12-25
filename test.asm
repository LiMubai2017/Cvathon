.data
_prompt: .asciiz "Enter an integer"
_ret: .asciiz "\n"
.globl main
.text
read:
  li $v0, 4
  la $a0, _prompt 
  syscall
  li $v0, 5
  syscall
  jr $ra
write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _ret
  syscall
  move $v0, $0
  jr $ra
test: 
lw $t1, 0($sp)
sw $t1, 116($sp)
addi $sp, $sp, 4
li $t3, 0
sw $t3, 92($sp)
lw $t1, 92($sp)
move $t3, $t1
sw $t3, 104($sp)
li $t3, 1
sw $t3, 76($sp)
lw $t1, 76($sp)
move $t3, $t1
sw $t3, 108($sp)
label1: 
lw $t1, 108($sp)
move $t3, $t1
sw $t3, 72($sp)
lw $t1, 112($sp)
move $t3, $t1
sw $t3, 68($sp)
lw $t1, 72($sp)
lw $t2, 68($sp)
ble $t1, $t2, label2
j label3
label2: 
lw $t1, 104($sp)
move $t3, $t1
sw $t3, 84($sp)
lw $t1, 108($sp)
move $t3, $t1
sw $t3, 80($sp)
lw $t1, 84($sp)
lw $t2, 80($sp)
add $t3, $t1, $t2
sw $t3, 88($sp)
lw $t1, 88($sp)
move $t3, $t1
sw $t3, 104($sp)
label4: 
li $t3, 1
sw $t3, 64($sp)
lw $t1, 108($sp)
lw $t2, 64($sp)
add $t3, $t1, $t2
sw $t3, 60($sp)
lw $t1, 60($sp)
move $t3, $t1
sw $t3, 108($sp)
j label1
label3: 
lw $t1, 104($sp)
move $t3, $t1
sw $t3, 56($sp)
lw $t1, 56($sp)
move $v0, $t1
jr $ra
main: 
addi $sp, $sp, -116
li $t3, 5
sw $t3, 48($sp)
lw $t1, 48($sp)
move $t3, $t1
sw $t3, 96($sp)
li $t3, 10
sw $t3, 44($sp)
lw $t1, 44($sp)
move $t3, $t1
sw $t3, 92($sp)
lw $t1, 92($sp)
move $t3, $t1
sw $t3, 36($sp)
addi $sp, $sp, -4
sw $ra, 0($sp)
lw $t1, 40($sp)
addi $sp, $sp, -4
sw $t1, 0($sp)
jal test
lw $ra, 0($sp)
addi $sp, $sp, 4
sw $v0, 40($sp)
lw $t1, 40($sp)
move $t3, $t1
sw $t3, 96($sp)
lw $t1, 96($sp)
move $t3, $t1
sw $t3, 32($sp)
lw $t2, 32($sp)
move $a0, $t2
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
lw $t1, 92($sp)
move $t3, $t1
sw $t3, 28($sp)
li $t3, 1
sw $t3, 24($sp)
lw $t1, 92($sp)
lw $t2, 24($sp)
add $t3, $t1, $t2
sw $t3, 20($sp)
lw $t1, 20($sp)
move $t3, $t1
sw $t3, 92($sp)
lw $t2, 28($sp)
move $a0, $t2
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
lw $t1, 92($sp)
move $t3, $t1
sw $t3, 16($sp)
lw $t2, 16($sp)
move $a0, $t2
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
li $t3, 1
sw $t3, 8($sp)
lw $t1, 92($sp)
lw $t2, 8($sp)
add $t3, $t1, $t2
sw $t3, 4($sp)
lw $t1, 4($sp)
move $t3, $t1
sw $t3, 92($sp)
lw $t1, 92($sp)
move $t3, $t1
sw $t3, 12($sp)
lw $t2, 12($sp)
move $a0, $t2
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
li $t3, 0
sw $t3, 0($sp)
lw $t1, 0($sp)
move $v0, $t1
jr $ra
