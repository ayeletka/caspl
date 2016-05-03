global my_func

section	.rodata
LC0:
	DB	"%s", 10, 0	; Format string

LC1:
	DB	"true" , 10
	

section .bss

y: 
	RESB 	8
	
x:
	RESB 	32
rounds:
	RESB	4
	
section .text
	align 16
	global my_func
	extern printf

my_func:
	push	ebp
	mov	ebp, esp	; Entry code - set up ebp and esp
	mov ebx, dword [ebp+8]	; Get argument (pointer to x)
	mov edx, dword [ebp+12]
	mov [rounds], edx
	pushad			; Save registers


;       Your code should be here...
big:
      
      mov ecx, 8
      mov [x], ebx
forloop:
	
	mov ah, 0xf0
	and ah, [ebx]
	shr ah, 4
	mov edx, y
	add edx, ah
	inc [edx]
	mov ah, 0x0f
	and ah, [ebx]
	inc [y + ah]
	
	add ebx, 1
	
	loop forloop, ecx
      mov ecx, 8
      mov ebx, [x]
      cmp [ebx], [y]
      je end
      inc [rounds]
      cmp [ronds] , 0
      jnz big
      jmp fatal
      
;		end code
end:
	push	LC1		; Call printf with 2 arguments: pointer to str
	push	LC0		; and pointer to format string.
	call	printf
	add 	esp, 8		; Clean up stack after call
fatal:
	popad			; Restore registers
	mov	esp, ebp	; Function exit code
	pop	ebp
	ret

