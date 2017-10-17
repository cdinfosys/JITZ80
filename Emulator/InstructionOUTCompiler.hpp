#ifndef INSTRUCTION_OUT_D87A22FAFEB440239427E73ACEBD1866
#define INSTRUCTION_OUT_D87A22FAFEB440239427E73ACEBD1866 1

#include "InstructionCompiler.hpp"
#include "../Z80PortOutProc.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        /**
         * @brief
         */
        class InstructionOUTCompiler : public InstructionCompiler
        {
            private:
                pfnZ80PortOutProc mOutCallbackProc;
                void* mAdditionalDataPtr;

            public:
                /**
                 * @brief
                 */
                InstructionOUTCompiler(pfnZ80PortOutProc outCallbackProc, void* additionalDataPtr);

                /**
                 * @brief Destructor
                 */
                virtual ~InstructionOUTCompiler();

            public:
                virtual std::unique_ptr<CompiledCode> CompileInstruction
                (
                    const uint8_t* z80MemoryPtr, 
                    uintptr_t instructionAddress, 
                    const CPUStateStruct* cpuStateStructAddress,
                    pfnZ80InstructionDecodedProc decodeCompletedProc
                );

                virtual inline bool IsEndOfRoutine() const;
        }; // class InstructionOUTCompiler

        inline bool InstructionOUTCompiler::IsEndOfRoutine() const
        {
            return true;
        }
    } // namespace Emulator
} // namespace JITZ80Lib

#endif // INSTRUCTION_OUT_D87A22FAFEB440239427E73ACEBD1866
