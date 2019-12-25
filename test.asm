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
main: 
addi $sp, $sp, -40
li $t3, 10
sw $t3, 24($sp)
lw $t1, 24($sp)
move $t3, $t1
sw $t3, 32($sp)
label1: 
lw $t1, 32($sp)
move $t3, $t1
sw $t3, 8($sp)
li $t3, 0
sw $t3, 4($sp)
lw $t1, 8($sp)
lw $t2, 4($sp)
bgt $t1, $t2, label2
j label3
label2: 
lw $t1, 32($sp)
move $t3, $t1
sw $t3, 20($sp)
li $t3, 1
sw $t3, 16($sp)
lw $t1, 32($sp)
lw $t2, 16($sp)
sub $t3, $t1, $t2
sw $t3, 12($sp)
lw $t1, 12($sp)
move $t3, $t1
sw $t3, 32($sp)
lw $t2, 20($sp)
move $a0, $t2
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
j label1
label3: 
li $t3, 0
sw $t3, 0($sp)
lw $t1, 0($sp)
move $v0, $t1
jr $ra
