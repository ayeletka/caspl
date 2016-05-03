

section .rodata
LC0:
	DB	"%08x", 10, 0	; Format 
LC02:
	DB	"%08X", 0	; Format 

section .bss
y_num:
	RESB	8
y2:
	RESB	8
x_num:
	RESB	8
numOfRounds:
	RESB	4
Pointer: 	
	RESB 4


section .text


	align 16
	global calc_func
	extern printf
	extern compare
	extern main



calc_func:
;Start function:
	push	ebp
	mov	ebp, esp	; Entry code - set up ebp and esp
	pushad			; Save registers

;Set arguments in registers
	mov     eax, [ebp+8]    ; Copy function args to registers: leftmost (pointer to unsigned long long x_num)      
    mov     ebx, [ebp+12]   ; Next argument (int numOfRounds)	
	mov 	[numOfRounds], ebx
	cmp 	[numOfRounds],byte 0
	jz 		done  			;is numOfRounds is 0, finish


	mainLoop:

	;initializing 
	mov 	esi,  0		;counterLoop(16 digits)
	mov 	[y_num], dword 0
	mov 	[y_num+4], dword 0
	mov 	ecx, y_num
	mov 	edx, 0
	mov 	ebx, 0
	mov 	[Pointer], dword 0


	;Copy X from it's pointer to x_num:
	loop2:
	mov 	ecx, y_num
	mov 	bl, byte [Pointer]
	mov 	dl, byte [eax]
	mov 	byte[x_num+ebx],dl
	add 	[Pointer], byte 1 

	;taking the left digit
    mov 	edx, 0
	mov 	dl, 0xf0
	and 	dl, [eax]
	shr		dl, 4 


	;check if the left digit is even
	mov		dh,dl
	and		dh,1
	cmp 	dh,0
	jz 		isEven

	isOdd:
			mov 	dh,0
			dec 	dl
			shr		dl,1
			add 	ecx,7 	
			sub 	ecx,edx
			add 	byte [ecx],00000001b
			jnz		nextDigit

	isEven:
			mov 	dh,0
			shr		dl,1
			add 	ecx,7
			sub 	ecx,edx
			add 	byte[ecx],0x10


    nextDigit:

    		;taking the right digit
			mov 	ecx, y_num
			mov 	edx, 0
    		mov 	dl, 0x0f
			and 	dl, [eax]
		
			;check if the right digit is even
			mov		dh,dl
			and		dh,1
			cmp 	dh,0
			jz 		isEven2

			isOdd2:
					mov 	dh,0
					dec 	dl
					shr		dl,1
					add 	ecx,7
					sub 	ecx,edx
					add 	byte [ecx],00000001b
					jnz		continue
			isEven2:
					mov 	dh,0
					shr		dl,1
					add 	ecx,7
					sub 	ecx,edx
					add 	byte[ecx],0x10

	continue:	
	inc eax 
	inc esi
	inc esi
	cmp esi, 16
	jnz loop2 	;continue to the next byte in x_num



 	;saving y_num in y2 
	mov 	ebx, 0
	mov 	edx, 0
	mov 	[Pointer], dword 0
	coping_y_to_y2:
	mov 	bl, byte [Pointer]
	mov 	dl, byte [y_num+ebx]
	mov 	byte[y2+ebx],dl
	cmp 	[Pointer], byte 7
	jz 		compareX_Y
	add 	[Pointer], byte 1
	jmp 	coping_y_to_y2


 	;call compare c function
	compareX_Y:
	push	y_num	
	push	x_num		
	call	compare
	add 	esp, 8		; Clean up stack after call
	cmp eax,0
	jz rounds

 	;in case x_num and y_num are equal,print x_num
	mov 	ecx, [x_num+4]
	push	ecx		; Call printf with 2 arguments: pointer to str
	push	LC02		; and pointer to format string.
	call	printf
	add 	esp, 8		; Clean up stack after call

	mov 	ecx, [x_num]
	push	ecx		; Call printf with 2 arguments: pointer to str
	push	LC0		; and pointer to format string.
	call	printf
	add 	esp, 8		; Clean up stack after call

	jmp 	done 	;no need to continue



 	;in case x_num and y_num are different
	rounds:
	sub	 	[numOfRounds],byte 1
	mov 	eax , y2 	;now eax is pointer to the new y_num
	cmp 	[numOfRounds],byte 0
	jnz 	mainLoop 	;more rounds to go


	;print y if it was not printed before
	mov 	ecx, [y_num+4]
	push	ecx		; Call printf with 2 arguments: pointer to str
	push	LC02		; and pointer to format string.
	call	printf
	add 	esp, 8		; Clean up stack after call

	mov 	ecx, [y_num]
	push	ecx		; Call printf with 2 arguments: pointer to str
	push	LC0		; and pointer to format string.
	call	printf
	add 	esp, 8		; Clean up stack after call


	done:
	popad			; Restore registers
	mov	esp, ebp	; Function exit code
	pop	ebp
	ret

