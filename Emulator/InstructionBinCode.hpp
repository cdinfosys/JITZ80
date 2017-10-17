#ifndef INSTRUCTION_BINCODE_C54B60CD81024AC29FFE509D269BF5A7
#define INSTRUCTION_BINCODE_C54B60CD81024AC29FFE509D269BF5A7 1

#include <cstdint>
#include <memory>
#include <vector>

namespace JITZ80Lib
{
    namespace Emulator
    {
        /**
         * @brief Base class for instruction opcode containers.
         */
        class InstructionBinCode
        {
            private:
                std::vector<uint8_t> mOpcodeBuffer;

            protected:
                /**
                 * @brief Constructor is hidden.
                 */
                inline InstructionBinCode(const std::vector<uint8_t>& opcodeBytes);

            public:
                inline const std::vector<uint8_t>& GetOpcodeBuffer() const;
        }; // class InstructionBinCode

        inline InstructionBinCode::InstructionBinCode(const std::vector<uint8_t>& opcodeBytes)
            :   mOpcodeBuffer(opcodeBytes)
        {
        }

        inline const std::vector<uint8_t>& InstructionBinCode::GetOpcodeBuffer() const { return this->mOpcodeBuffer; }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        class CompiledFunctionPrologue : public InstructionBinCode
        {
            private:
                uintptr_t mZ80MemoryAddress;
                uintptr_t mCPUStateStructAddress;

            public:
                CompiledFunctionPrologue(uintptr_t z80MemoryAddress, uintptr_t cpuStateStructAddress);

            private:
                std::vector<uint8_t> GetBinaryCode(uintptr_t z80MemoryAddress, uintptr_t cpuStateStructAddress);
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        class CompiledFunctionEpilogue : public InstructionBinCode
        {
            public:
                CompiledFunctionEpilogue();

            private:
                std::vector<uint8_t> GetBinaryCode();
        };
    } // namespace Emulator
} // namespace JITZ80Lib

#endif // INSTRUCTION_BINCODE_C54B60CD81024AC29FFE509D269BF5A7
