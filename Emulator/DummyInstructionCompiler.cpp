#include "DummyInstructionCompiler.hpp"
#include "../Support/JITZ80Exception.hpp"
#include <memory>
#include <vector>

namespace JITZ80Lib
{
    namespace Emulator
    {
        using std::unique_ptr;
        using std::vector;
        using JITZ80Lib::Support::JITZ80Exception;
        using JITZ80Lib::Support::JITZ80ExceptionCode;

        DummyInstructionCompiler::DummyInstructionCompiler()
        {
        }

        DummyInstructionCompiler::~DummyInstructionCompiler()
        {
        }

        std::unique_ptr<CompiledCode> DummyInstructionCompiler::CompileInstruction
        (
            const uint8_t* z80MemoryPtr, 
            uintptr_t instructionAddress, 
            const CPUStateStruct* /*cpuStateStructAddress*/,
            pfnZ80InstructionDecodedProc decodeCompletedProc
        )
        {
            if (decodeCompletedProc != nullptr)
            {
                JITZ80_DECODED_INSTRUCTION_STRUCT instructionData;
                memset(&instructionData, 0, sizeof(instructionData));
                instructionData.structSizeBytes.structSize = sizeof(instructionData);
                instructionData.instructionAddress = instructionAddress;
                instructionData.instructionMnemonicID = (uintptr_t)(*z80MemoryPtr);
                instructionData.operationFlags = JITZ80_OPF_UNKNOWN_INSTR;
                decodeCompletedProc(&instructionData);
            }

            throw JITZ80Exception(JITZ80ExceptionCode::UnknownInstruction);
            /*
            return unique_ptr<CompiledCode>
            (
                new CompiledCode
                (
                    (int)instructionAddress,
                    vector<uint8_t>(),
                    vector<uint8_t>()
                )
            );
            */
        }
    } // namespace Emulator
} // namespace JITZ80Lib
