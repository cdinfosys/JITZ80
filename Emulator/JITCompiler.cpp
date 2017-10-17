#include "JITCompiler.hpp"
#include "InstructionADDR8I8Compiler.hpp"
#include "InstructionDICompiler.hpp"
#include "InstructionJPCompiler.hpp"
#include "InstructionLDR8I8Compiler.hpp"
#include "InstructionOUTCompiler.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        using std::vector;
        using std::unique_ptr;

        JITCompiler::JITCompiler
        (
            pfnZ80InstructionDecodedProc decodeCompletedProc, 
            pfnZ80PortOutProc portOutProc,
            void* portOutAdditionalDataPtr
        )
            :   mDecodeCompletedProc(decodeCompletedProc),
                mDefaultInstructionCompilers(256u)
        {
            mDefaultInstructionCompilers[0x06].reset(new InstructionLDR8I8Compiler);
            mDefaultInstructionCompilers[0x0E].reset(new InstructionLDR8I8Compiler);
            mDefaultInstructionCompilers[0x16].reset(new InstructionLDR8I8Compiler);
            mDefaultInstructionCompilers[0x1E].reset(new InstructionLDR8I8Compiler);
            mDefaultInstructionCompilers[0x26].reset(new InstructionLDR8I8Compiler);
            mDefaultInstructionCompilers[0x2E].reset(new InstructionLDR8I8Compiler);
            mDefaultInstructionCompilers[0x3E].reset(new InstructionLDR8I8Compiler);
            mDefaultInstructionCompilers[0xC3].reset(new InstructionJPCompiler);
            mDefaultInstructionCompilers[0xD3].reset(new InstructionOUTCompiler(portOutProc, portOutAdditionalDataPtr));
            mDefaultInstructionCompilers[0xF3].reset(new InstructionDICompiler);
        }

        JITCompiler::~JITCompiler()
        {
        }

        std::vector<std::unique_ptr<CompiledCode>> JITCompiler::CompileRoutine(uintptr_t z80Address, const CompiledBufferHeader& compiledBufferHeader)
        {
            std::vector<std::unique_ptr<CompiledCode>> result;

            size_t safeAddress = (size_t)(z80Address & 0xFFFFu);
            bool branched(false);
            do
            {
                size_t instructionOpcodeIndex = (size_t)(compiledBufferHeader.z80RAMBasePtr[safeAddress]);
                InstructionCompiler* decoderObj = mDefaultInstructionCompilers[instructionOpcodeIndex].get();
                if (decoderObj != nullptr)
                {
                    result.push_back
                    (
                        decoderObj->CompileInstruction
                        (
                            compiledBufferHeader.z80RAMBasePtr,
                            safeAddress,
                            &(compiledBufferHeader.cpuState),
                            this->mDecodeCompletedProc
                        )
                    );

                    // PC of next instruction.
                    CompiledCode* compiledCodePtr = result.back().get();
                    safeAddress = (safeAddress + compiledCodePtr->GetZ80InstructionLength()) & 0xFFFFu;
                    branched = decoderObj->IsEndOfRoutine();
                }
                else
                {
                    // This will raise an exception to indicate that we cannot decode the opcode.
                    mDummyInstruction.CompileInstruction
                    (
                        compiledBufferHeader.z80RAMBasePtr,
                        safeAddress,
                        &(compiledBufferHeader.cpuState),
                        this->mDecodeCompletedProc
                    );
                }
            } while (!branched);
            return result;
        }
    } // namespace Emulator
} // namespace JITZ80Lib
