#include <cstdint>
#include "InstructionDICompiler.hpp"

#ifdef _M_AMD64
#include "X64InstructionShortcuts.hpp"
#endif // _M_AMD64

namespace JITZ80Lib
{
    namespace Emulator
    {
        using std::unique_ptr;
        using std::vector;

        InstructionDICompiler::InstructionDICompiler()
        {
        }

        InstructionDICompiler::~InstructionDICompiler()
        {
        }

        unique_ptr<CompiledCode> InstructionDICompiler::CompileInstruction
        (
            const uint8_t* z80MemoryPtr, 
            uintptr_t instructionAddress, 
            const CPUStateStruct* cpuStateStructAddress,
            pfnZ80InstructionDecodedProc decodeCompletedProc
        )
        {
            #ifdef _M_AMD64
            vector<uint8_t> clearIFF1 { 0xC7, 0x47, 0x18, 0x00, 0x00, 0x00, 0x00 };
            #elif _M_IX86
            #endif // _M_AMD64

            if (decodeCompletedProc != nullptr)
            {
                JITZ80_DECODED_INSTRUCTION_STRUCT instructionData;
                memset(&instructionData, 0, sizeof(instructionData));
                instructionData.structSizeBytes.structSize = sizeof(instructionData);
                instructionData.instructionAddress = instructionAddress;
                instructionData.instructionMnemonicID = JITZ80_MNEMONIC_DI;
                decodeCompletedProc(&instructionData);
            }

            return unique_ptr<CompiledCode>
            (
                new CompiledCode
                (
                    (int)instructionAddress,
                    z80MemoryPtr[instructionAddress],
                    clearIFF1
                )
            );
        }
    } // namespace Emulator
} // namespace JITZ80Lib
