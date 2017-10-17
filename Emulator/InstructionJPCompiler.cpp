#include "InstructionJPCompiler.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        using std::unique_ptr;
        using std::vector;

        InstructionJPCompiler::InstructionJPCompiler()
        {
        }

        InstructionJPCompiler::~InstructionJPCompiler()
        {
        }

        unique_ptr<CompiledCode> InstructionJPCompiler::CompileInstruction
        (
            const uint8_t* z80MemoryPtr, 
            uintptr_t instructionAddress, 
            const CPUStateStruct* /*cpuStateStructAddress*/,
            pfnZ80InstructionDecodedProc decodeCompletedProc
        )
        {
            uintptr_t branchAddress = (uintptr_t)(z80MemoryPtr[(instructionAddress  + 2u) & 0xFFFFu] << 8u) | (z80MemoryPtr[(instructionAddress  + 1u) & 0xFFFFu]);
            #pragma pack(push)
            #pragma pack(1)
            union 
            {
                uintptr_t addressVal;
                uint8_t addressBytes[2];
            } address;
            address.addressVal = branchAddress;
            #pragma pack (pop)

            #ifdef _M_AMD64
            vector<uint8_t> storeProgramCounter { 0xC7, 0x07, address.addressBytes[0], address.addressBytes[1], 0x00, 0x00 }; // mov DWORD [rdi + cpu_state_struct.ProgramCounter], {address}
            #elif _M_IX86
            #endif // _M_AMD64

            if (decodeCompletedProc != nullptr)
            {
                JITZ80_DECODED_INSTRUCTION_STRUCT instructionData;
                memset(&instructionData, 0, sizeof(instructionData));
                instructionData.structSizeBytes.structSize = sizeof(instructionData);
                instructionData.instructionAddress = instructionAddress;
                instructionData.instructionMnemonicID = JITZ80_MNEMONIC_JP;
                instructionData.branchAddress = (uintptr_t)branchAddress;
                instructionData.operationFlags = JITZ80_OPF_BRANCH;
                decodeCompletedProc(&instructionData);
            }

            return unique_ptr<CompiledCode>
            (
                new CompiledCode
                (
                    (int)instructionAddress,
                    z80MemoryPtr[instructionAddress], z80MemoryPtr[instructionAddress + 1], z80MemoryPtr[instructionAddress + 2],
                    storeProgramCounter
                )
            );
        }
    } // namespace Emulator
} // namespace JITZ80Lib
