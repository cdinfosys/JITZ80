#ifndef COMMON_STRUCTURES_CCAFDDFC0416494096B11D82A6213F89
#define COMMON_STRUCTURES_CCAFDDFC0416494096B11D82A6213F89 1

namespace JITZ80Lib
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    #pragma pack(push)
    #pragma pack(1)
    union Z80RegisterPair
    {
        struct
        {
            uint8_t LowByte;
            uint8_t HighByte;
        } Byte;
        uint16_t Word;
        uint32_t Filler;
    };
    #pragma pack(pop)

    struct CPUStateStruct
    {
        uint32_t ProgramCounter;
        uint32_t StackPointer;
        Z80RegisterPair InterruptVector;
        Z80RegisterPair RefreshCounter;
        uint32_t IX;
        uint32_t IY;
        uint32_t IFF1;
        uint32_t IFF2;

        Z80RegisterPair AF;
        Z80RegisterPair BC;
        Z80RegisterPair DE;
        Z80RegisterPair HL;

        Z80RegisterPair AF1;
        Z80RegisterPair BC1;
        Z80RegisterPair DE1;
        Z80RegisterPair HL1;
    }; // struct CPUStateStruct

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct CompiledBufferHeader
    {
        CPUStateStruct cpuState; ///< Stores the CPU state
        void* jumpTableBasePtr; ///< Pointer to the jump table
        uint8_t* z80RAMBasePtr; ///< Pointer to the Z80 RAM buffer.
        uint8_t* compiledCodeBasePtr; ///< Pointer to the memory block where the compiled code is stored.
    }; // struct CompiledBufferHeader

    #pragma pack(push)
    #pragma pack(1)
    typedef union qword_address
    {
        uint64_t addr;
        uint8_t byteArray[8];
    } QWORD_ADDRESS;

    typedef union dword_address
    {
        uint32_t addr;
        uint8_t byteArray[4];
    } DWORD_ADDRESS;

    typedef union word_address
    {
        uint16_t addr;
        uint8_t byteArray[2];
    } WORD_ADDRESS;
    #pragma pack(pop)
} // namespace JITZ80Lib

#endif // COMMON_STRUCTURES_CCAFDDFC0416494096B11D82A6213F89
