.code16       # Tell the assembler that this is 16-bit code
.globl start  # Make our "start" label public for the linker

start:
  mov $0x00, %ah    # Set Video Mode to:
  mov $0x03, %al    # Text, 80x25 characters
  int $0x10
  mov $message, %si
  # Your code can start here

print_chars:
    lodsb
    testb %al, %al
    jz done
    mov $0x0E, %ah 
    int $0x10
    jmp print_chars
  # You may want to write a subroutine

done:
  jmp done    # When we're done, loop infinitely

message:                   # "message" is the address at the start of the
  .ascii "Hello World\r\n" # character buffer

message_len = . - message  # "message_len" is a constant representing the length
                           # of the character buffer
