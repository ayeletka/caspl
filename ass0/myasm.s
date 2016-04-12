section .data                    	; data section, read-write
        an:    DD 0              	; this is a temporary var

section .text                    	; our code is always in the .text section
        global add_Str_N          	; makes the function appear in global scope
        extern printf            	; tell linker that printf is defined elsewhere 				; (not used in the program)

add_Str_N:                        	; functions are defined as labels
        push    ebp              	; save Base Pointer (bp) original value
        mov     ebp, esp         	; use base pointer to access stack contents
        pushad                   	; push all variables onto stack
        mov ecx, dword [ebp+8]	; get function argument
        mov    dword [an], 0                         ; initialize answer
        label_here:     
                        add byte [ecx], 0x3             ;adds 3 to a char
                        cmp byte [ecx], 0x7A    
                        jg increment                    ; check if the CHAR  > z

                        cmp byte [ecx], 0x61    
                        jge no_need_increment   ; check if the CHAR  <= a

                        cmp byte [ecx], 0x5A    ; check if the CHAR  <=Z
                        jle no_need_increment

                        increment:
                                inc dword [an]
                        no_need_increment:  
                                inc ecx                                         ; increment pointer
                                
                                
                        cmp byte [ecx], 0               ; check if byte pointed to is zero
                        jnz label_here                  ; keep looping until it is null terminated


         popad                    ; restore all previously used registers
         mov     eax,[an]         ; return an (returned values are in eax)
         mov     esp, ebp
         pop     ebp
         ret 
