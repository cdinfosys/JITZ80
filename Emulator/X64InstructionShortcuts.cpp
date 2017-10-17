#include "X64InstructionShortcuts.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        X64InstructionShortcuts::X64InstructionShortcuts()
        {
        }

        
        std::vector<uint8_t> X64InstructionShortcuts::MOV_RDI_IMMEDIATE(uint64_t immediateVal)
        {
            std::vector<uint8_t> result { 0x48, 0xBF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
            *((uint64_t*)(result.data() + 2u)) = immediateVal;
            return result;
        }

    } // namespace Emulator
} // namespace JITZ80Lib
