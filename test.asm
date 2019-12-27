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
sw $t1, 220($sp)
addi $sp, $sp, 4
li $t3, 0
sw $t3, 188($sp)
lw $t3, 188($sp)
sw $t3, 208($sp)
li $t3, 1
sw $t3, 172($sp)
lw $t3, 172($sp)
sw $t3, 212($sp)
label1: 
lw $t3, 212($sp)
sw $t3, 168($sp)
lw $t3, 216($sp)
sw $t3, 164($sp)
lw $t1, 168($sp)
lw $t2, 164($sp)
ble $t1, $t2, label2
j label3
label2: 
lw $t3, 208($sp)
sw $t3, 180($sp)
lw $t3, 212($sp)
sw $t3, 176($sp)
lw $t1, 180($sp)
lw $t2, 176($sp)
add $t3, $t1, $t2
sw $t3, 184($sp)
lw $t3, 184($sp)
sw $t3, 208($sp)
label4: 
li $t3, 1
sw $t3, 160($sp)
lw $t1, 212($sp)
lw $t2, 160($sp)
add $t3, $t1, $t2
sw $t3, 156($sp)
lw $t3, 156($sp)
sw $t3, 212($sp)
j label1
label3: 
lw $t3, 208($sp)
sw $t3, 152($sp)
lw $t1, 152($sp)
move $v0, $t1
jr $ra
main: 
addi $sp, $sp, -220
li $t3, 0
sw $t3, 120($sp)
lw $t3, 120($sp)
sw $t3, 196($sp)
label5: 
lw $t3, 196($sp)
sw $t3, 116($sp)
li $t3, 10
sw $t3, 112($sp)
lw $t1, 116($sp)
lw $t2, 112($sp)
blt $t1, $t2, label6
j label7
label6: 
li $t3, 4
sw $t3, 132($sp)
lw $t3, 196($sp)
sw $t3, 140($sp)
lw $t1, 140($sp)
lw $t2, 132($sp)
mul $t3, $t1, $t2
sw $t3, 136($sp)
li $t2, 200
add $t3, $sp, $t2
sw $t3, 128($sp)
lw $t1, 128($sp)
lw $t2, 136($sp)
add $t3, $t1, $t2
sw $t3, 144($sp)
lw $t3, 196($sp)
sw $t3, 124($sp)
lw $t3, 124($sp)
lw $t1, 144($sp)
sw $t3, ($t1)
label8: 
li $t3, 1
sw $t3, 108($sp)
lw $t1, 196($sp)
lw $t2, 108($sp)
add $t3, $t1, $t2
sw $t3, 104($sp)
lw $t3, 104($sp)
sw $t3, 196($sp)
j label5
label7: 
li $t3, 4
sw $t3, 88($sp)
li $t3, 5
sw $t3, 96($sp)
lw $t1, 96($sp)
lw $t2, 88($sp)
mul $t3, $t1, $t2
sw $t3, 92($sp)
li $t2, 200
add $t3, $sp, $t2
sw $t3, 84($sp)
lw $t1, 84($sp)
lw $t2, 92($sp)
add $t3, $t1, $t2
sw $t3, 100($sp)
lw $t1, 100($sp)
lw $t3, ($t1)
sw $t3, 192($sp)
li $t3, 0
sw $t3, 36($sp)
lw $t3, 36($sp)
sw $t3, 196($sp)
label11: 
lw $t3, 196($sp)
sw $t3, 32($sp)
li $t3, 10
sw $t3, 28($sp)
lw $t1, 32($sp)
lw $t2, 28($sp)
blt $t1, $t2, label12
j label13
label12: 
li $t3, 4
sw $t3, 44($sp)
lw $t3, 196($sp)
sw $t3, 52($sp)
lw $t1, 52($sp)
lw $t2, 44($sp)
mul $t3, $t1, $t2
sw $t3, 48($sp)
li $t2, 200
add $t3, $sp, $t2
sw $t3, 40($sp)
lw $t1, 40($sp)
lw $t2, 48($sp)
add $t3, $t1, $t2
sw $t3, 60($sp)
lw $t3, 192($sp)
sw $t3, 56($sp)
lw $t1, 60($sp)
lw $t2, 56($sp)
bgt $t1, $t2, label9
j label10
label9: 
li $t3, 4
sw $t3, 68($sp)
lw $t3, 196($sp)
sw $t3, 76($sp)
lw $t1, 76($sp)
lw $t2, 68($sp)
mul $t3, $t1, $t2
sw $t3, 72($sp)
li $t2, 200
add $t3, $sp, $t2
sw $t3, 64($sp)
lw $t1, 64($sp)
lw $t2, 72($sp)
add $t3, $t1, $t2
sw $t3, 80($sp)
lw $t1, 80($sp)
lw $t3, ($t1)
sw $t3, 192($sp)
label10: 
label14: 
li $t3, 1
sw $t3, 24($sp)
lw $t1, 196($sp)
lw $t2, 24($sp)
add $t3, $t1, $t2
sw $t3, 20($sp)
lw $t3, 20($sp)
sw $t3, 196($sp)
j label11
label13: 
lw $t3, 192($sp)
sw $t3, 16($sp)
lw $t2, 16($sp)
move $a0, $t2
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
lw $t3, 192($sp)
sw $t3, 8($sp)
addi $sp, $sp, -4
sw $ra, 0($sp)
lw $t1, 12($sp)
addi $sp, $sp, -4
sw $t1, 0($sp)
jal test
lw $ra, 0($sp)
addi $sp, $sp, 4
sw $v0, 12($sp)
lw $t3, 12($sp)
sw $t3, 188($sp)
lw $t3, 188($sp)
sw $t3, 4($sp)
lw $t2, 4($sp)
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
