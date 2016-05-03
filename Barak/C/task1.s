section	.rodata
LC0:
	DB	"%s", 10, 0	; Format string

section .bss
LC1:
	RESB	32

section .data
	count DB 0
	even DB 2 
	pointerToString DD 0

section .text
	align 16
	global my_func
	extern printf
	
my_func:
	push	ebp
	mov	ebp, esp								; Entry code - set up ebp and esp
	mov ecx, dword [ebp+8]						; Get argument (pointer to string)
	pushad										; Save registers
	mov al, 0;
	mov byte [count], al ;
	mov dword [pointerToString], ecx			; save ecx in pointerToString
	
	

		
		push	LC1		; Call printf with 2 arguments: pointer to str
		push	LC0		; and pointer to format string.
		call	printf
		add 	esp, 8		; Clean up stack after call

		popad			; Restore registers
		mov	esp, ebp	; Function exit code
		pop	ebp
		ret
