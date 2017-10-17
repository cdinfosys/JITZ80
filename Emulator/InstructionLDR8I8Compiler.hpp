#ifndef INSTRUCTION_LDR8I8_4ACB737B1BA24927AB975567AE329898
#define INSTRUCTION_LDR8I8_4ACB737B1BA24927AB975567AE329898 1

#include "InstructionCompiler.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        /**
         * @brief
         */
        class InstructionLDR8I8Compiler : public InstructionCompiler
        {
            public:
                /** 
                 * @brief
                 */
                InstructionLDR8I8Compiler();

                /**
                 * @brief Destructor
                 */
                virtual ~InstructionLDR8I8Compiler();

            public:
                virtual std::unique_ptr<CompiledCode> CompileInstruction
                (
                    const uint8_t* z80MemoryPtr, 
                    uintptr_t instructionAddress, 
                    const CPUStateStruct* cpuStateStructAddress,
                    pfnZ80InstructionDecodedProc decodeCompletedProc
                );
        }; // class InstructionLDR8I8Compiler
    } // namespace Emulator
} // namespace JITZ80Lib

#endif // INSTRUCTION_LDR8I8_4ACB737B1BA24927AB975567AE329898
