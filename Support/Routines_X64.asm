BITS 64

;-- RDI points to the CPU state structure (loaded on routine entry)
;-- RSI points to the Z80 memory block (loaded on routine entry)
;-- AL contains the value of the accumulator (written back after the routine completes)

struc cpu_state_struct
    .ProgramCounter         resd 1
    .StackPointer           resd 1
    .InterruptVector        resd 1
    .RefreshCounter         resd 1
    .IX                     resd 1
    .IY                     resd 1
    .IFF1                   resd 1
    .IFF2                   resd 1
    .AF                     resd 1
    .BC                     resd 1
    .DE                     resd 1
    .HL                     resd 1
    .AF1                    resd 1
    .BC1                    resd 1
    .DE1                    resd 1
    .HL1                    resd 1
endstruc ; cpu_state_struct

PROLOG_CODE:
    push rdi
    push rsi
    mov rdi, 0x12345678AABBCCDD
    mov rsi, 0x12345678AABBCCDD
    mov al, BYTE[rdi + cpu_state_struct.AF]

EPILOGUE_CODE:
    mov BYTE[rdi + cpu_state_struct.AF], al
    pop rsi
    pop rdi

INSTR_DI:
    mov DWORD [rdi + cpu_state_struct.IFF1], 0

INSTR_EI:
    mov DWORD [rdi + cpu_state_struct.IFF1], 1

INSTR_JP:
    mov DWORD [rdi + cpu_state_struct.ProgramCounter], 0x00001234

INSTR_OUT:
    mov r8, 0x2211221122112211
    mov edx, 0x33443344
    xor ecx, ecx
    mov cl, al
    mov r10, 0x123456789ABCDEFF
    call r10

;-----------
; LD R8, nn
;------------
INSTR_LD_A_I8:
    mov al, 0xCC

INSTR_LD_B_I8:
    mov BYTE [rdi + cpu_state_struct.BC], 0xCC

INSTR_LD_C_I8:
    mov BYTE [rdi + cpu_state_struct.BC + 1], 0xCC

INSTR_LD_D_I8:
    mov BYTE [rdi + cpu_state_struct.DE], 0xCC

INSTR_LD_E_I8:
    mov BYTE [rdi + cpu_state_struct.DE + 1], 0xCC

INSTR_LD_H_I8:
    mov BYTE [rdi + cpu_state_struct.HL], 0xCC

INSTR_LD_L_I8:
    mov BYTE [rdi + cpu_state_struct.HL + 1], 0xCC
