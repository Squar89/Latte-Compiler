.data
  formatPrintInt: .asciz "%d\n"
  formatPrintString: .asciz "%s\n"
  formatReadInt: .asciz "%d"
  formatReadString: .asciz "%s"
  errorMessage: .asciz "runtime error\n"

.bss
  buffer: .space 100000

.text
  .globl printInt
  .globl printString
  .globl error
  .globl readInt
  .globl readString

printInt:
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
  call printf

  popq %rdx
  addq %rdx, %rsp
  popq %rbx
  
  movq %rbp, %rsp
  pop %rbp
  ret

printString:
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
  call printf

  #decrease reference count on argument string and free it if it drops to 0
  movq 16(%rbp), %rdi
  decq %rdi
  decb (%rdi)
  cmpb $0, (%rdi)
  jne libSkipFree
  call free
  libSkipFree:

  popq %rdx
  addq %rdx, %rsp
  popq %rbx
  
  movq %rbp, %rsp
  pop %rbp
  ret

error:
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

  lea errorMessage(%rip), %rdi
  call printf

  popq %rdx
  addq %rdx, %rsp
  popq %rbx

  movq $60, %rax
  movq $17, %rdi
  syscall

  movq %rbp, %rsp
  pop %rbp
  ret

readInt:
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
  call  scanf
  movq  (%rsp), %rax
  addq  $8, %rsp
  
  popq %rdx
  addq %rdx, %rsp
  popq %rbx

  movq %rbp, %rsp
  pop %rbp
  ret

readString:
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
  call scanf
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
  call malloc
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
