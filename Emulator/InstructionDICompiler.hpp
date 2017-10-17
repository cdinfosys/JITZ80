#ifndef INSTRUCTION_DI_1297AAA2C0A8463EA61899AC14439D2E
#define INSTRUCTION_DI_1297AAA2C0A8463EA61899AC14439D2E 1

#include "InstructionCompiler.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        /**
         * @brief
         */
        class InstructionDICompiler : public InstructionCompiler
        {
            public:
                /**
                 * @brief
                 */
                InstructionDICompiler();

                /**
                 * @brief Destructor
                 */
                virtual ~InstructionDICompiler();

            public:
                virtual std::unique_ptr<CompiledCode> CompileInstruction
                (
                    const uint8_t* z80MemoryPtr, 
                    uintptr_t instructionAddress, 
                    const CPUStateStruct* cpuStateStructAddress,
                    pfnZ80InstructionDecodedProc decodeCompletedProc
                );
        }; // class InstructionDICompiler
    } // namespace Emulator
} // namespace JITZ80Lib

#endif // INSTRUCTION_DI_1297AAA2C0A8463EA61899AC14439D2E
