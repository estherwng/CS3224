.code16       # Tell the assembler that this is 16-bit code
.globl start  # Make our "start" label public for the linker

start:
  mov $0x00, %ah    # Set Video Mode to:
  mov $0x03, %al    # Text, 80x25 characters
  int $0x10
  # Your code can start here

done:
  jmp done    # When we're done, loop infinitely
