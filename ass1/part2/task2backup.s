

section .bss
y_num:
	RESB	64
x_num:
	RESB	64

counter:    DD 0 

section .text
	align 16
	global culc_func
	extern printf
	extern compare
	extern main

_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes
    add     eax,esi     ; add the size to the address of argv 
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc

    call    main        ; int main( int argc, char *argv[], char *envp[] )

    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop



culc_func:
	push	ebp
	mov	ebp, esp	; Entry code - set up ebp and esp
	pushad			; Save registers

	mov     eax, [ebp+8]    ; Copy function args to registers: leftmost (pointer to unsigned long long x_num)      
    mov     ebx, [ebp+12]   ; Next argument (int numOfRounds)

	mov edx, y_num 	; pointer to y_num
	mov ecx,0		; starting the counterLoop
	mov y_num,0		;initialize y_num to zero

	

	loop1:

				inc ecx						; inc the counterLoop1

				loop2:	
						inc counterLoop		; inc the counterLoop2




						cmp dword[counter],16					; if counterLoop is equal to 16 stop
						jnz loop1






				cmp ecx,ebx					; if counterLoop is equal to numOfRounds stop
				jnz loop1









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

	
	push	LC1		; Call printf with 2 arguments: pointer to str
	push	LC0		; and pointer to format string.
	call	printf
	add 	esp, 8		; Clean up stack after call

	popad			; Restore registers
	mov	esp, ebp	; Function exit code
	pop	ebp
	ret

