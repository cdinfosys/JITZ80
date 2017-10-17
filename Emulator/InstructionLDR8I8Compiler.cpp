#include <cstdint>
#include "InstructionLDR8I8Compiler.hpp"
#include "../Support/JITZ80Exception.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        using std::unique_ptr;
        using std::vector;
        using JITZ80Lib::Support::JITZ80Exception;
        using JITZ80Lib::Support::JITZ80ExceptionCode;

        InstructionLDR8I8Compiler::InstructionLDR8I8Compiler()
        {
        }

        InstructionLDR8I8Compiler::~InstructionLDR8I8Compiler()
        {
        }

        unique_ptr<CompiledCode> InstructionLDR8I8Compiler::CompileInstruction
        (
            const uint8_t* z80MemoryPtr, 
            uintptr_t instructionAddress, 
            const CPUStateStruct* /*cpuStateStructAddress*/,
            pfnZ80InstructionDecodedProc decodeCompletedProc
        )
        {
            uint8_t immediateVal = z80MemoryPtr[(instructionAddress + 1) & 0xFFFF];
            uintptr_t instructionOpcode = z80MemoryPtr[instructionAddress];

            if (decodeCompletedProc != nullptr)
            {
                JITZ80_DECODED_INSTRUCTION_STRUCT instructionData;
                memset(&instructionData, 0, sizeof(instructionData));
                instructionData.structSizeBytes.structSize = sizeof(instructionData);
                instructionData.instructionAddress = instructionAddress;
                instructionData.instructionMnemonicID = JITZ80_MNEMONIC_LD;
                instructionData.operationFlags = JITZ80_OPF_TARGET_R8 | JITZ80_OPF_SOURCE_IMMEDIATE;
                instructionData.immediateValue = (uintptr_t)immediateVal;
                switch (instructionOpcode)
                {
                    case 0x06:
                        instructionData.targetRegister = JITZ80_REGF_B;
                        break;

                    case 0x0E:
                        instructionData.targetRegister = JITZ80_REGF_C;
                        break;

                    case 0x16:
                        instructionData.targetRegister = JITZ80_REGF_D;
                        break;

                    case 0x1E:
                        instructionData.targetRegister = JITZ80_REGF_E;
                        break;

                    case 0x26:
                        instructionData.targetRegister = JITZ80_REGF_H;
                        break;

                    case 0x2E:
                        instructionData.targetRegister = JITZ80_REGF_L;
                        break;

                    case 0x3E:
                        instructionData.targetRegister = JITZ80_REGF_A;
                        break;

                    default:
                        instructionData.operationFlags |= JITZ80_OPF_UNKNOWN_INSTR;
                        break;
                }
                decodeCompletedProc(&instructionData);
            }

            #ifdef _M_AMD64
            vector<uint8_t> movInstr = { 0xC6, 0x47, 0x24, immediateVal };
            switch (instructionOpcode)
            {
                case 0x06: // B
                    movInstr[2] = 0x24u;

                case 0x0E: // C
                    movInstr[2] = 0x25u;

                case 0x16: // D
                    movInstr[2] = 0x28u;

                case 0x1E: // E
                    movInstr[2] = 0x29u;

                case 0x26: // H
                    movInstr[2] = 0x2Cu;

                case 0x2E: // L
                    movInstr[2] = 0x2Du;

                case 0x3E:  // A
                    movInstr = { 0xB0, immediateVal }; // mov al, {immediateVal }
                    break;

                default:
                    throw JITZ80Exception(JITZ80ExceptionCode::UnknownInstruction);
            }
            #elif _M_IX86
            #endif // _M_AMD64


            return unique_ptr<CompiledCode>
            (
                new CompiledCode
                (
                    (int)instructionAddress,
                    z80MemoryPtr[instructionAddress], z80MemoryPtr[(instructionAddress + 1u) & 0xFFFF],
                    movInstr
                )
            );
        }
    } // namespace Emulator
} // namespace JITZ80Lib
