.data
  formatPrintInt: .asciz "%d\n"
  formatPrintString: .asciz "%s\n"
  formatReadInt: .asciz "%d"
  formatReadString: .asciz "%s"

.bss
  buffer: .space 100000

.text
  .globl _printInt
  .globl _printString
  .globl _error
  .globl _readInt
  .globl _readString

_printInt:
  push %rbp
  movq %rsp, %rbp

  pushq %rbx
  movq %rsp, %rax
  subq $8, %rax
  xorq %rdx, %rdx
  movq $16, %rcx
  idivq %rcx
  subq %rdx, %rsp
  pushq %rdx

  lea formatPrintInt(%rip), %rdi
  movq 16(%rbp), %rsi
  xor %rax, %rax
  call _printf

  popq %rdx
  addq %rdx, %rsp
  popq %rbx
  
  movq %rbp, %rsp
  pop %rbp
  ret

_printString:
  push %rbp
  movq %rsp, %rbp

  pushq %rbx
  movq %rsp, %rax
  subq $8, %rax
  xorq %rdx, %rdx
  movq $16, %rcx
  idivq %rcx
  subq %rdx, %rsp
  pushq %rdx

  lea formatPrintString(%rip), %rdi
  movq 16(%rbp), %rsi
  xor %rax, %rax
  call _printf

  #decrease reference count on argument string and free it if it drops to 0
  movq 16(%rbp), %rdi
  decq %rdi
  decb (%rdi)
  cmpb $0, (%rdi)
  jne libSkipFree
  call _free
  libSkipFree:

  popq %rdx
  addq %rdx, %rsp
  popq %rbx
  
  movq %rbp, %rsp
  pop %rbp
  ret

_error:
  movl $0x2000001, %eax
  movl $17, %edi
  syscall

_readInt:
  push %rbp
  movq %rsp, %rbp

  pushq %rbx
  movq %rsp, %rax
  subq $16, %rax
  xorq %rdx, %rdx
  movq $16, %rcx
  idivq %rcx
  subq %rdx, %rsp
  pushq %rdx

  subq  $8, %rsp
  xor   %rax, %rax
  leaq  formatReadInt(%rip), %rdi
  movq  %rsp, %rsi
  call  _scanf
  movq  (%rsp), %rax
  addq  $8, %rsp
  
  popq %rdx
  addq %rdx, %rsp
  popq %rbx

  movq %rbp, %rsp
  pop %rbp
  ret

_readString:
  push %rbp
  movq %rsp, %rbp
  pushq %rbx

  #align stack
  movq %rsp, %rax
  subq $8, %rax
  xorq %rdx, %rdx
  movq $16, %rcx
  idivq %rcx
  subq %rdx, %rsp
  pushq %rdx

  xor %rax, %rax
  leaq formatReadString(%rip), %rdi
  leaq buffer(%rip), %rsi
  call _scanf
  leaq buffer(%rip), %rax

  #count length of read string
  xor %rdi, %rdi
  decq %rax
  count:
    incq %rdi
    incq %rax
    cmpb $0, (%rax)
    jne count
  incq %rdi#make space for reference count
  call _malloc
  movb $0, (%rax)
  incq %rax
  movq %rax, %rbx
  leaq buffer(%rip), %rcx
  #copy string from one address (%rcx) to the other (%rbx)
  write:
    movb (%rcx), %dil
    movb %dil, (%rbx)
    incq %rbx
    incq %rcx
    cmpb $0, (%rcx)
    jne write
  movb $0, (%rbx)

  #align stack
  popq %rdx
  addq %rdx, %rsp

  popq %rbx
  movq %rbp, %rsp
  pop %rbp
  ret
