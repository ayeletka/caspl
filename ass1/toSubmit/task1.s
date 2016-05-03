section	.rodata
LC0:
	DB	"%s", 10, 0	; Format string

section .bss
LC1:
	RESB	32

section .text
	align 16
	global my_func
	extern printf

my_func:
	push	ebp
	mov	ebp, esp	; Entry code - set up ebp and esp
	mov ecx, dword [ebp+8]	; Get argument (pointer to string)
	pushad			; Save registers


	mov edx, LC1 	; pointer to the ans
	mov eax,0		; starting the counter

	checkIfOdd: 
				inc eax
				inc ecx
				cmp byte [ecx], 0xa       ; check if byte pointed to is zero
				jnz checkIfOdd
	sub ecx, eax
	and eax, 1 ;Check if the number is even	
	cmp eax, 0
	jz loop   ; if the length is Even continue to loop
	mov bl, [ecx]
	mov [edx], bl
	inc edx
	inc ecx

	cmp byte [ecx], 0xa       ; check if byte pointed to is zero
	jz printing          ; keep looping until it is null terminated

	loop: 
			mov  AL, byte [ecx]
			sub AL, 48
			shl AL, 2              ; mul by 4
			sub byte [ecx+1], 48
			add AL, byte [ecx+1]
			add AL, 48
			cmp AL, 0x39
			jle continue
			add AL, 7
			continue: 
						mov byte [edx], AL
						add ecx , 2
						inc edx
						cmp byte [ecx], 0xa       ; check if byte pointed to is zero
						jnz loop          ; keep looping until it is null terminated

	printing:
	push	LC1		; Call printf with 2 arguments: pointer to str
	push	LC0		; and pointer to format string.
	call	printf
	add 	esp, 8		; Clean up stack after call


	done:
	popad			; Restore registers
	mov	esp, ebp	; Function exit code
	pop	ebp
	ret

