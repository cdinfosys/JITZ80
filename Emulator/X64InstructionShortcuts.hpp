#ifndef X64_INSTRUCTION_SHORTCUTS_3558094C88464B9FA2331DBDA2AE6DA7
#define X64_INSTRUCTION_SHORTCUTS_3558094C88464B9FA2331DBDA2AE6DA7 1

#include <cstdint>
#include <vector>

namespace JITZ80Lib
{
    namespace Emulator
    {
        /**
         * @brief
         */
        class X64InstructionShortcuts
        {
            private:
                /**
                 * @brief
                 */
                X64InstructionShortcuts();

            public:
                static std::vector<uint8_t> MOV_RDI_IMMEDIATE(uint64_t immediateVal);
        }; // class X64InstructionShortcuts
    } // namespace Emulator
} // namespace JITZ80Lib

#endif // X64_INSTRUCTION_SHORTCUTS_3558094C88464B9FA2331DBDA2AE6DA7
