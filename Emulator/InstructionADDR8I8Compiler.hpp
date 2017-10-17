#ifndef INSTRUCTION_ADDR8I8_1DB0105E312B49309EE4E5BFED5C086E
#define INSTRUCTION_ADDR8I8_1DB0105E312B49309EE4E5BFED5C086E 1

#include "InstructionCompiler.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        /**
         * @brief
         */
        class InstructionADDR8I8Compiler : public InstructionCompiler
        {
            public:
                /**
                 * @brief
                 */
                InstructionADDR8I8Compiler();

                /**
                 * @brief Destructor
                 */
                virtual ~InstructionADDR8I8Compiler();

            public:
                virtual std::unique_ptr<CompiledCode> CompileInstruction
                (
                    const uint8_t* z80MemoryPtr, 
                    uintptr_t instructionAddress, 
                    const CPUStateStruct* cpuStateStructAddress,
                    pfnZ80InstructionDecodedProc decodeCompletedProc
                );
        }; // class InstructionADDR8I8Compiler
    } // namespace Emulator
} // namespace JITZ80Lib

#endif // INSTRUCTION_ADDR8I8_1DB0105E312B49309EE4E5BFED5C086E
