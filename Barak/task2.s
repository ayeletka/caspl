section .bss
	numOfRound RESB 32		; value of
	x	   RESB 64		; pointer to ...


section .text
	global calc_func

calc_func:       			; 2 arguments , first 64 bit (8 byte) sec 32 bit
	push ebp
	mov ebp, esp
	mov edx , [ebp+8]		; x address
	mov [x], edx
	mov edx , [ebp+24]		; numOfRound VALUE!!!!
	mov [numOfRound] , edx
	pushad







end_of_program:
	popad
	mov	esp, ebp	; Function exit code
	pop	ebp
	ret

