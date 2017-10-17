#include <cstdint>
#include "InstructionADDR8I8Compiler.hpp"
#include "../Support/JITZ80Exception.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        using std::unique_ptr;
        using std::vector;
        using JITZ80Lib::Support::JITZ80Exception;
        using JITZ80Lib::Support::JITZ80ExceptionCode;

        InstructionADDR8I8Compiler::InstructionADDR8I8Compiler()
        {
        }

        InstructionADDR8I8Compiler::~InstructionADDR8I8Compiler()
        {
        }

        unique_ptr<CompiledCode> InstructionADDR8I8Compiler::CompileInstruction
        (
            const uint8_t* z80MemoryPtr, 
            uintptr_t instructionAddress, 
            const CPUStateStruct* cpuStateStructAddress,
            pfnZ80InstructionDecodedProc decodeCompletedProc
        )
        {/*
            #ifdef _M_AMD64
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
            */
            throw JITZ80Exception(JITZ80ExceptionCode::UnknownInstruction);
            return unique_ptr<CompiledCode>
            (
                new CompiledCode
                (
                    (int)instructionAddress,
                    vector<uint8_t>(),
                    vector<uint8_t>()
                )
            );
        }
    } // namespace Emulator
} // namespace JITZ80Lib
