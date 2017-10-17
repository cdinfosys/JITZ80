#ifndef DUMMY_INSTRUCTION_1BCDFC99E35B44A48C00D2EF9D41DE36
#define DUMMY_INSTRUCTION_1BCDFC99E35B44A48C00D2EF9D41DE36 1

#include "InstructionCompiler.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        /**
         * @brief
         */
        class DummyInstructionCompiler : public InstructionCompiler

        {
            public:
                /**
                 * @brief
                 */
                DummyInstructionCompiler();

                /**
                 * @brief Destructor
                 */
                virtual ~DummyInstructionCompiler();

            public:
                virtual std::unique_ptr<CompiledCode> CompileInstruction
                (
                    const uint8_t* z80MemoryPtr, 
                    uintptr_t instructionAddress, 
                    const CPUStateStruct* cpuStateStructAddress,
                    pfnZ80InstructionDecodedProc decodeCompletedProc
                );
        }; // class DummyInstructionCompiler
    } // namespace Emulator
} // namespace JITZ80Lib

#endif // DUMMY_INSTRUCTION_1BCDFC99E35B44A48C00D2EF9D41DE36
