#include <cstdint>
#include "InstructionOUTCompiler.hpp"
#include "../Support/CommonStructures.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        using std::unique_ptr;
        using std::vector;

        InstructionOUTCompiler::InstructionOUTCompiler(pfnZ80PortOutProc outCallbackProc, void* additionalDataPtr)
            :   mOutCallbackProc(outCallbackProc),
                mAdditionalDataPtr(additionalDataPtr)
        {
        }

        InstructionOUTCompiler::~InstructionOUTCompiler()
        {
        }

        unique_ptr<CompiledCode> InstructionOUTCompiler::CompileInstruction
        (
            const uint8_t* z80MemoryPtr, 
            uintptr_t instructionAddress, 
            const CPUStateStruct* /*cpuStateStructAddress*/,
            pfnZ80InstructionDecodedProc decodeCompletedProc
        )
        {
            uint8_t immediateVal = z80MemoryPtr[(instructionAddress + 1) & 0xFFFF];

            if (decodeCompletedProc != nullptr)
            {
                JITZ80_DECODED_INSTRUCTION_STRUCT instructionData;
                memset(&instructionData, 0, sizeof(instructionData));
                instructionData.structSizeBytes.structSize = sizeof(instructionData);
                instructionData.instructionAddress = instructionAddress;
                instructionData.instructionMnemonicID = JITZ80_MNEMONIC_OUT;
                instructionData.immediateValue = (uintptr_t)immediateVal;
                instructionData.sourceRegister = JITZ80_REGF_A;
                instructionData.operationFlags = JITZ80_OPF_TARGET_IMMEDIATE | JITZ80_OPF_SOURCE_R8;
                decodeCompletedProc(&instructionData);
            }

            QWORD_ADDRESS additionalData;
            additionalData.addr = (uint64_t)(this->mAdditionalDataPtr);

            QWORD_ADDRESS routineAddress;
            routineAddress.addr = (uint64_t)(this->mOutCallbackProc);

            WORD_ADDRESS portNumber;
            portNumber.addr = (uint16_t)immediateVal;

            WORD_ADDRESS resumeAddress;
            resumeAddress.addr = ((instructionAddress + 2u) & 0xFFFFu);

            return unique_ptr<CompiledCode>
            (
                new CompiledCode
                (
                    (int)instructionAddress,
                    z80MemoryPtr[instructionAddress], z80MemoryPtr[(instructionAddress + 1u) & 0xFFFF],
                    vector<uint8_t> 
                    {
                        0x49, 0xB8,                             // mov r8, {additionalData}
                            additionalData.byteArray[0], 
                            additionalData.byteArray[1], 
                            additionalData.byteArray[2], 
                            additionalData.byteArray[3],
                            additionalData.byteArray[4], 
                            additionalData.byteArray[5], 
                            additionalData.byteArray[6], 
                            additionalData.byteArray[7],

                        0xBA, 
                            portNumber.byteArray[0],            // mov edx, { portNumber }
                            portNumber.byteArray[1],
                            0x00,
                            0x00,

                        0x31, 0xC9,                             // xor ecx, ecx
                        0x88, 0xC1,                             // mov cl, al

                        0x49, 0xBA,                             // mov r10, {routineAddress}
                            routineAddress.byteArray[0], 
                            routineAddress.byteArray[1], 
                            routineAddress.byteArray[2], 
                            routineAddress.byteArray[3],
                            routineAddress.byteArray[4], 
                            routineAddress.byteArray[5], 
                            routineAddress.byteArray[6], 
                            routineAddress.byteArray[7],

                        0x41, 0xFF, 0xD2,                        // call r10

                        // Resume execution at the instruction after the OUT instruction
                        0xC7, 0x07,                             // mov DWORD [rdi + cpu_state_struct.ProgramCounter], {address} 
                            resumeAddress.byteArray[0], 
                            resumeAddress.byteArray[1], 
                            0x00, 
                            0x00,
                    }
                )
            );
        }
    } // namespace Emulator
} // namespace JITZ80Lib
