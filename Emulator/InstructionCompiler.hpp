#ifndef INSTRUCTION_EXECUTOR_1DCB780B333F40D6B15E0FD945C7BF12
#define INSTRUCTION_EXECUTOR_1DCB780B333F40D6B15E0FD945C7BF12 1

#include <cstdint>
#include <memory>
#include <vector>
#include "../Z80InstructionDecodedProc.hpp"
#include "../Support/CommonStructures.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @brief Helper to emit compiled code into the \c CompiledBuffer
         */
        class CompiledCode
        {
            private:
                int mZ80InstructionAddress;
                std::vector<uint8_t> mZ80InstructionOpcodes;
                std::vector<uint8_t> mCompiledCode;

            public:
                inline CompiledCode
                (
                    int z80InstructionAddress,
                    const std::vector<uint8_t>& z80InstructionOpcodes,
                    const std::vector<uint8_t>& compiledCode
                );

                inline CompiledCode
                (
                    int z80InstructionAddress,
                    uint8_t z80InstructionOpcode,
                    const std::vector<uint8_t>& compiledCode
                );

                inline CompiledCode
                (
                    int z80InstructionAddress,
                    uint8_t z80InstructionOpcode1,
                    uint8_t z80InstructionOpcode2,
                    const std::vector<uint8_t>& compiledCode
                );

                inline CompiledCode
                (
                    int z80InstructionAddress,
                    uint8_t z80InstructionOpcode1,
                    uint8_t z80InstructionOpcode2,
                    uint8_t z80InstructionOpcode3,
                    const std::vector<uint8_t>& compiledCode
                );

                inline CompiledCode
                (
                    int z80InstructionAddress,
                    uint8_t z80InstructionOpcode1,
                    uint8_t z80InstructionOpcode2,
                    uint8_t z80InstructionOpcode3,
                    uint8_t z80InstructionOpcode4,
                    const std::vector<uint8_t>& compiledCode
                );

            public:
                inline int GetZ80InstructionAddress() const;
                inline size_t GetZ80InstructionLength() const;
                inline size_t GetCompiledCodeSize() const;
                inline const std::vector<uint8_t>& GetCompiledCode() const;
                inline const std::vector<uint8_t>& GetZ80InstructionOpcodes() const;
        }; // class CompiledCode

        inline CompiledCode::CompiledCode
        (
            int z80InstructionAddress,
            const std::vector<uint8_t>& z80InstructionOpcodes,
            const std::vector<uint8_t>& compiledCode
        )
            :   mZ80InstructionAddress(z80InstructionAddress),
                mZ80InstructionOpcodes(z80InstructionOpcodes),
                mCompiledCode(compiledCode)
        {
        }

        inline CompiledCode::CompiledCode
        (
            int z80InstructionAddress,
            uint8_t z80InstructionOpcode,
            const std::vector<uint8_t>& compiledCode
        )
            :   mZ80InstructionAddress(z80InstructionAddress),
                mZ80InstructionOpcodes { z80InstructionOpcode },
                mCompiledCode(compiledCode)
        {
        }

        inline CompiledCode::CompiledCode
        (
            int z80InstructionAddress,
            uint8_t z80InstructionOpcode1,
            uint8_t z80InstructionOpcode2,
            const std::vector<uint8_t>& compiledCode
        )
            :   mZ80InstructionAddress(z80InstructionAddress),
                mZ80InstructionOpcodes { z80InstructionOpcode1, z80InstructionOpcode2 },
                mCompiledCode(compiledCode)
        {
        }

        inline CompiledCode::CompiledCode
        (
            int z80InstructionAddress,
            uint8_t z80InstructionOpcode1,
            uint8_t z80InstructionOpcode2,
            uint8_t z80InstructionOpcode3,
            const std::vector<uint8_t>& compiledCode
        )
            :   mZ80InstructionAddress(z80InstructionAddress),
                mZ80InstructionOpcodes { z80InstructionOpcode1, z80InstructionOpcode2, z80InstructionOpcode3 },
                mCompiledCode(compiledCode)
        {
        }

        inline CompiledCode::CompiledCode
        (
            int z80InstructionAddress,
            uint8_t z80InstructionOpcode1,
            uint8_t z80InstructionOpcode2,
            uint8_t z80InstructionOpcode3,
            uint8_t z80InstructionOpcode4,
            const std::vector<uint8_t>& compiledCode
        )
            :   mZ80InstructionAddress(z80InstructionAddress),
                mZ80InstructionOpcodes { z80InstructionOpcode1, z80InstructionOpcode2, z80InstructionOpcode3, z80InstructionOpcode4 },
                mCompiledCode(compiledCode)
        {
        }

        inline int CompiledCode::GetZ80InstructionAddress() const { return this->mZ80InstructionAddress; }
        inline size_t CompiledCode::GetZ80InstructionLength() const { return this->mZ80InstructionOpcodes.size(); }
        inline size_t CompiledCode::GetCompiledCodeSize() const { return this->mCompiledCode.size(); } 
        inline const std::vector<uint8_t>& CompiledCode::GetCompiledCode() const { return this->mCompiledCode; }
        inline const std::vector<uint8_t>& CompiledCode::GetZ80InstructionOpcodes() const { return this->mZ80InstructionOpcodes; }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @brief
         */
        class InstructionCompiler
        {
            public:
                /**
                 * @brief
                 */
                InstructionCompiler();

                /**
                 * @brief Destructor
                 */
                virtual ~InstructionCompiler();

            public:
                virtual std::unique_ptr<CompiledCode> CompileInstruction
                (
                    const uint8_t* z80MemoryPtr, 
                    uintptr_t instructionAddress, 
                    const CPUStateStruct* cpuStateStructAddress,
                    pfnZ80InstructionDecodedProc decodeCompletedProc
                ) = 0;

                virtual inline bool IsEndOfRoutine() const;
        }; // class InstructionCompiler

        inline bool InstructionCompiler::IsEndOfRoutine() const
        {
            return false;
        }
    } // namespace Emulator
} // namespace JITZ80Lib

#endif // INSTRUCTION_EXECUTOR_1DCB780B333F40D6B15E0FD945C7BF12
