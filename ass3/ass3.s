

;---------------------------------------------------------------------------------
; macros
;---------------------------------------------------------------------------------

%macro print 2
        section .rodata
        %%string:
                DB %1, 10, 0

        section .text
        pushad
        push    %2
        push    %%string
        call    printf
        add             esp, 8
        popad
%endmacro

%macro print_label 1
        pusha
        push    %1
        call    printf
        add     esp, 4
        popa    
%endmacro

;---------------------------------------------------------------------------------
; declerations
;---------------------------------------------------------------------------------

global main, array_element_address, k, t, timesec
global state, WorldLength, WorldWidth        ; TODO - remove this
extern init_co, init_cell, start_co, resume
extern scheduler, printer, cell

extern printf, atoi

;; /usr/include/asm/unistd_32.h

SYS_EXIT        equ     1
ARG0            equ     0       ; program name
ARG1            equ     4       ; init filename
ARG2            equ     8       ; length
ARG3            equ     12      ; width
ARG4            equ     16      ; t
ARG5            equ     20      ; k
SPECIAL         equ     24      ; special sleep indicator


section .rodata

_msgArgs:
        DB "Error: should run with args <filename> <length> <width> <t> <k>" ,10, 0

_msgFile:
        DB "Error: init file could not be opened", 10, 0

_msgZero:
        DB "Error: zero argument is not valid", 10, 0


section .data

    state:         times   60*60 db ' '   ;max 60 on 60 with max 60 newline symbols
    WorldLength:    dd      0
    WorldWidth:     dd      0    
    t:              dd      0
    k:              dd      0

    timesec:        dd      0              ; struct for sys_nanosleep
                    dd      0


section .text

;---------------------------------------------------------------------------------
main:
;---------------------------------------------------------------------------------
    enter 0, 0


    ; verify that call for program made with all parameters
    ; -----------------------------------------------------

    cmp     dword [ebp+8], 6        ;ebp+8 location of argc (gcc style)
    jb     .args_error

    ; extracting args value
    ; ---------------------

    mov     ebx, [ebp+12]           ; pointer to argv array

    push    dword [ebx+ARG2]        ; push string for length
    call    atoi
    mov     [WorldLength], eax      ; saving length value into memory
    add     esp, 4

    push    dword [ebx+ARG3]        ; push string for width
    call    atoi
    mov     [WorldWidth], eax       ; saving width value into memory
    add     esp, 4

    push    dword [ebx+ARG4]        ; push string for t
    call    atoi
    mov     [t], eax                ; saving t value into memory
    add     esp, 4

    push    dword [ebx+ARG5]        ; push string for k
    call    atoi
    mov     [k], eax                ; saving k value into memory
    add     esp, 4

    ; check and extract special sleep indicator
    ; -----------------------------------------
    cmp     dword [ebp+8], 7        ; check if special arg (sleep?) was sent
    jne     .skip_special
    push    dword [ebx+SPECIAL]
    call    atoi
    mov     [timesec+4], eax
    add     esp, 4
    .skip_special:


    ; verify right size for length and width
    ; --------------------------------------

    cmp     dword [WorldWidth], 0
    je      .zero_error

    cmp     dword [WorldLength], 0
    je      .zero_error

    cmp     dword [k], 0
    je      .zero_error

    cmp     dword [t], 0
    je      .zero_error

    ; open file and initialize table !
    ; --------------------------------

    mov     eax, 5                  ; open system call
    mov     ebx, [ebx+ARG1]         ; pointer to file name from args
    mov     ecx, 0                  ; access bit - read only
    mov     edx, 0644               ; premission
    int     0x80

    cmp     eax, 0
    jng      .init_file_error

    push    eax                     ; push file discriptor
    call    init_matrix_from_file
    add     esp, 4


    ; co-routines initialization  
    ; --------------------------

    xor ebx, ebx            ; scheduler is co-routine 0
    mov edx, scheduler
    mov esi, [t]
    mov edi, [k]
    call init_co            ; initialize scheduler state

    inc ebx                 ; printer is co-routine 1
    mov edx, printer
    call init_co            ; initialize printer state

    mov esi, 0              ; rows counter
    mov edi, 0              ; columns counter

    ; i can assume matrix is at least 1x1
    .init_cells_loop:

        ; calling init_cell while: 
        ; ebx is the serial number for current cell co-routines
        ; esi is the row (y)
        ; edi is the column (x)
        inc ebx     
        mov edx, cell
        call init_co

        inc     edi             ; advancing in same line
        cmp     edi, [WorldWidth]
        jne     .init_cells_loop     

        mov     edi, 0          ; reseting for columns

        inc     esi             ; advancing to next line
        cmp     esi, [WorldLength]
        jne      .init_cells_loop       


    ;print "index of last co-routine: %d", ebx


    .start_routines:

        xor ebx, ebx            ; starting co-routine = scheduler
        call start_co           ; start co-routines

    jmp .finilize


    .init_file_error:
        print_label _msgFile
        jmp .finilize

    .args_error:
        print_label _msgArgs
        jmp .finilize

    .zero_error:
        print_label _msgZero
        jmp .finilize

    .finilize:
    ;; exit
    mov eax, SYS_EXIT
    xor ebx, ebx
    int 80h







;---------------------------------------------------------------------------------
init_matrix_from_file:
;---------------------------------------------------------------------------------

;function start 
;--------------
push    ebp 
mov     ebp, esp                ; Entry code - set up ebp and esp
sub     esp, 1                  ; local variable (char)
pusha                           ; Save registers

%define     tmp     byte [ebp-1]
%define     filed   dword [ebp+8]

mov     esi, 0                      ; offset of current value in matrix

.read_next_byte:

    mov     eax, 3
    mov     ebx, filed              ; value of file discriptor
    mov     ecx, ebp                ; address for tmp variable is ebp-1
    dec     ecx                     
    mov     edx, 1
    int     0x80

    cmp     eax, 0                  ; if no bytes has read then its means EOF
    je      .done

    cmp     tmp, 10             
    je      .read_next_byte         ; if newline char - skip

    cmp     tmp, 13
    je      .read_next_byte         ; if Carriage return - skip

    cmp     tmp, '1'                ; if not "1", advance to next cell in matrix
    jne     .advance

    mov     byte [state+esi], '1'  ; if is "1", update matrix and then advance

.advance:
    inc     esi
    jmp     .read_next_byte

.done:

;function end
;------------
popa                                ; Restore registers
mov     esp, ebp                    ; Function exit code
pop     ebp
ret

;---------------------------------------------------------------------------------
array_element_address: ;(Array_start, Element_size, Array_width, x, y) 
;---------------------------------------------------------------------------------
; computes and returns the address of a[x,y] in the array "a"

;function start 
;--------------
push    ebp 
mov     ebp, esp                    ; Entry code - set up ebp and esp
sub     esp, 4
pusha                               ; Save registers

%define array dword [ebp+8]         ; array_start variable
%define element dword [ebp+12]      ; element size variable
%define line_width dword [ebp+16]   ; array width variable
%define x dword [ebp+20]
%define y dword [ebp+24]

mov     eax, line_width
mul     y
mul     element
; now eax have the correct offset for the line

add     eax, x
; now eax have the correct offset for the request value

add     eax, array 
; now eax have the correct address of the request value in the array

mov     [ebp-4], eax

;function end
;------------
popa                            ; Restore registers
mov     esp, ebp                ; Function exit code
mov     eax, [ebp-4]            ; saving return value to eax
pop     ebp
ret



