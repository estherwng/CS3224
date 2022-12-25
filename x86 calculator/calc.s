.code16         # tell assembler this is 16 bit code
.global start   # make start public for the linker

start:
    xor %ax, %ax # zeroing reg
    xor %bx, %bx # zeroing reg
    xor %cx, %cx # zeroing reg
    xor %dx, %dx # zeroing reg

    movw $op1_msg, %si  # load msg into si
    call print_string   # call print on si
    call handle_input   # store input into reg dl
    movb %dl, %bl       # mov input into bl
    subb $0x30, %bl     # atoi

    movw $op2_msg, %si  # load msg into si
    call print_string   # call print on si
    call handle_input   # store input into reg dl
    movb %dl, %cl       # move input into cl
    subb $0x30, %cl     # atoi

    movw $operator_msg, %si # load msg into si
    call print_string       # call print on si
    call handle_input       # store input into reg dl

    call calc # perform calculation on bl and cl based on operator in dl

    jmp start # looping calculator 


handle_input:
    xor %ah, %ah        # clears ah

    movb $0x00, %ah     # set up BIOS interrupt
    int $0x16           # call BIOS interrupt to get input

    movb $0x0E, %ah     # BIOS interrupt print output set up
    int $0x10           # BIOS interrupt print 

    cmp $0x71, %al      # compare for 'q'
    je done             # if equal to 'q' infinite loop

    xor %dh, %dh       # clear dh
    movb %al, %dl       # move al to dl 
   
    ret                 # return 

print_string:
    lodsb               # loads byte into al and increment si
    testb %al, %al      # checks if byte == 0
    jz print_done       # if 0 jump 
    movb $0x0E, %ah     # print single character
    int $0x10           # calls BIOS
    jmp print_string    # loop

print_done:
    ret       # return

# performs calculation on the operand according to the operator 
calc:
    movb $0x2b, %al # move hex value of '+' into %al
    cmp %al, %dl    # compare the operator to see if it is add
    je addition     # jump if operator is add

    movb $0x2d, %al # move hex value of '-' into %al
    cmp %al, %dl    # compare the operator to see if it is subtract
    je subtract     # jump if operator is subtract

    movb $0x2a, %al # move hex value of '*' into %al
    cmp %al, %dl    # compare the operator to see if it is multiply
    je multiply     # jump if operator is multiply

    movb $0x2f, %al # move hex value of '/' into %al
    cmp %al, %dl    # compare the operator to see if it is divide
    je divide       # jump if operator is divide


# performs the addition operation
addition: 
    addb %cl, %bl
    call print_result_msg


# performs the subtraction operation
subtract: 
    subb %cl, %bl
    call print_result_msg


# performs the multiplication operation
multiply:
    imul %cx, %bx
    call print_result_msg


# perfroms the division operation
divide:
    xor %ax, %ax
    movw %ax, %bx
    idiv %cl
    xor %bx, %bx
    movb %al, %bl
    call print_result_msg

print_result_msg:
    movw $result_msg, %si
    call print_string

print_result:
    movb $0x09, %al
    cmp %al, %bl
    jg print_overflow

    movb %bl, %al
    add $0x30, %al # itoa
    movb $0x0E, %ah
    int $0x10
    
    jmp print_done

print_overflow:
    mov %bx, %ax
    movb $10, %cl
    idiv %cl

    movb %ah, %dl
    add $0x30, %al
    movb $0x0E, %ah
    int $0x10

    movb %dl, %al
    add $0x30, %al
    int $0x10
    
    jmp print_done


# infinite loop
done:
    jmp done

# needed strings to prompt input
op1_msg:
    .string "\r\nFirst: "

op1_msg_len = . - op1_msg

op2_msg:
    .string "\r\nSecond: "

op2_msg_len = . - op2_msg

operator_msg:
    .string "\r\nOp: "

operator_msg_len = . - operator_msg

result_msg:
    .string "\r\nResult: "

result_msg_len = . - result_msg
