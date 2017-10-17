#ifndef INSTRUCTION_JP_82275019B0D34D33B4E01C3DAAE50A4D
#define INSTRUCTION_JP_82275019B0D34D33B4E01C3DAAE50A4D 1

#include "InstructionCompiler.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        /**
         * @brief
         */
        class InstructionJPCompiler : public InstructionCompiler
        {
            public:
                /**
                 * @brief
                 */
                InstructionJPCompiler();

                /**
                 * @brief Destructor
                 */
                virtual ~InstructionJPCompiler();

            public:
                virtual std::unique_ptr<CompiledCode> CompileInstruction
                (
                    const uint8_t* z80MemoryPtr, 
                    uintptr_t instructionAddress, 
                    const CPUStateStruct* cpuStateStructAddress,
                    pfnZ80InstructionDecodedProc decodeCompletedProc
                );

                virtual inline bool IsEndOfRoutine() const;
        }; // class InstructionJPCompiler

        inline bool InstructionJPCompiler::IsEndOfRoutine() const
        {
            return true;
        }
    } // namespace Emulator
} // namespace JITZ80Lib

#endif // INSTRUCTION_JP_82275019B0D34D33B4E01C3DAAE50A4D
