section .data
	stack_cap: EQU 5
	stack_items_count: db 0
	stack_overflow: "Error: Operand Stack Overflow",10,0
	stack_overflow_len: EQU $-stack_overflow
	stack_underflow: "Error: Insufficient Number of Arguments on Stack",10,0
	stack_underflow_len: EQU $-stack_underflow
section .bss
	my_stack: RESB stack_cap
section .text 
     align 16 
     global main 
     extern printf 
     extern malloc 
     extern gets 
main: 

	mov ebx,0
    mov eax,1
    int 0x80

my_pop:
	push ebp
	mov ebp,esp

	cmp [stack_items_count],0
	JE stack_empty
	dec stack_items_count
	mov eax,[my_stack+stack_items_count]

	mov esp,ebp
	pop ebp
	ret

my_push: ;
	push ebp
	mov ebp,esp

	cmp stack_items_count,stack_cap
	JE stack_full
	mov [my_stack+stack_items_count],[ebp+8] ;put into stack
	inc stack_items_count

	mov esp,ebp
	pop ebp
	ret

stack_empty:
	mov eax,4
	mov ebx,1
	mov ecx,stack_underflow
	mov edx,stack_underflow_len
	int 0x80

	mov esp,ebp
	pop ebp
	ret

stack_full:
	mov eax,4
	mov ebx,1
	mov ecx,stack_overflow
	mov edx,stack_overflow_len
	int 0x80

	mov esp,ebp
	pop ebp
	ret