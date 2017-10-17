#ifndef JIT_COMPILER_4E0AB806A46440D48DDF1A8E4A4D2F87
#define JIT_COMPILER_4E0AB806A46440D48DDF1A8E4A4D2F87 1

#include <cstdint>
#include <memory>
#include <vector>
#include "CompiledBuffer.hpp"
#include "InstructionCompiler.hpp"
#include "DummyInstructionCompiler.hpp"
#include "../Z80InstructionDecodedProc.hpp"
#include "../Z80PortOutProc.hpp"
#include "../Support/CommonStructures.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        /**
         * @brief
         */
        class JITCompiler
        {
            private:
                pfnZ80InstructionDecodedProc mDecodeCompletedProc;
                std::vector<std::unique_ptr<InstructionCompiler>> mDefaultInstructionCompilers; ///< Instruction compilers for opcodes that do not have overrides.
                DummyInstructionCompiler mDummyInstruction; ///< Used to report instructions for which no compilers exist.
                std::unique_ptr<CompiledCode> mPrologCode; ///< Routine prolog code
                std::unique_ptr<CompiledCode> mEpilogCode; ///< Routine epilog code

            public:
                /**
                 * @brief
                 */
                JITCompiler
                (
                    pfnZ80InstructionDecodedProc decodeCompletedProc,
                    pfnZ80PortOutProc portOutProc,
                    void* portOutAdditionalDataPtr
                );

                /**
                 * @brief Destructor
                 */
                virtual ~JITCompiler();

            public:
                std::vector<std::unique_ptr<CompiledCode>> CompileRoutine(uintptr_t z80Address, const CompiledBufferHeader& compiledBufferHeader);
        }; // class JITCompiler
    } // namespace Emulator
} // namespace JITZ80Lib

#endif // JIT_COMPILER_4E0AB806A46440D48DDF1A8E4A4D2F87
