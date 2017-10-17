#ifndef Z80_INSTR_DECODEDPROC_14369BC8F8624F8BAFC16E8A9BEF1E4A
#define Z80_INSTR_DECODEDPROC_14369BC8F8624F8BAFC16E8A9BEF1E4A 1

#pragma pack(push)
#pragma pack(1)
/**
 * @brief Parameter for the \c pfnZ80InstructionDecodedProc callback function.
 */
typedef struct jitz80_decoded_instruction_struct
{
    union
    {
        uint32_t structSize;   ///< Size of this structure
        uintptr_t filler;
    } structSizeBytes;

    uintptr_t instructionAddress; ///< Address of the first byte of the instruction
    uintptr_t instructionMnemonicID; ///< Identifier for the instruction mnemonic.
    uintptr_t operationFlags; ///< Identifies the type of operation to be performed.

    uintptr_t sourceAddress; ///< Source address of the data
    uintptr_t targetAddress; ///< Target address of the data
    uintptr_t sourceRegister; ///< Source register
    uintptr_t targetRegister; ///< Source register
    uintptr_t branchAddress; ///< Branch address
    uintptr_t immediateValue; ///< Immediate value
} JITZ80_DECODED_INSTRUCTION_STRUCT, *PJITZ80_DECODED_INSTRUCTION_STRUCT;
#pragma pack(pop)

const uintptr_t JITZ80_OPF_SOURCE_R8        = 0x0001; ///< Source is an 8-bit register.
const uintptr_t JITZ80_OPF_TARGET_R8        = 0x0002; ///< Target is an 8-bit register.
const uintptr_t JITZ80_OPF_SOURCE_R16       = 0x0004; ///< Source is a 16-bit register.
const uintptr_t JITZ80_OPF_TARGET_R16       = 0x0008; ///< Target is a 16-bit register.
const uintptr_t JITZ80_OPF_BRANCH           = 0x0010; ///< Branch operation
const uintptr_t JITZ80_OPF_SOURCE_IMMEDIATE = 0x0020; ///< Source is an 8-bit immediate value
const uintptr_t JITZ80_OPF_TARGET_IMMEDIATE = 0x0030; ///< Target is an 8-bit immediate value
const uintptr_t JITZ80_OPF_UNKNOWN_INSTR    = 0x8000; ///< Instruction could not be decoded.

const uintptr_t JITZ80_REGF_A               = 0x0001; ///< Register A
const uintptr_t JITZ80_REGF_F               = 0x0002; ///< Flags register
const uintptr_t JITZ80_REGF_B               = 0x0004; ///< Register B
const uintptr_t JITZ80_REGF_C               = 0x0008; ///< Register C
const uintptr_t JITZ80_REGF_D               = 0x0010; ///< Register D
const uintptr_t JITZ80_REGF_E               = 0x0020; ///< Register E
const uintptr_t JITZ80_REGF_H               = 0x0040; ///< Register H
const uintptr_t JITZ80_REGF_L               = 0x0080; ///< Register L

const uintptr_t JITZ80_MNEMONIC_JP      = 0xC3;
const uintptr_t JITZ80_MNEMONIC_DI      = 0xF3;
const uintptr_t JITZ80_MNEMONIC_LD      = 0x3E;
const uintptr_t JITZ80_MNEMONIC_OUT     = 0xD3;
const uintptr_t JITZ80_MNEMONIC_UNKNOWN = (uintptr_t)-1;

/**
 * @brief Callback function prototype for functions called by the library when an instruction is decoded.
 */
typedef void (*pfnZ80InstructionDecodedProc)(const PJITZ80_DECODED_INSTRUCTION_STRUCT instructionData);

#endif //Z80_INSTR_DECODEDPROC_14369BC8F8624F8BAFC16E8A9BEF1E4A
