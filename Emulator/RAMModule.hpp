#ifndef RAM_MODULE_93CB39188073476BBBA76130DF31E5AA
#define RAM_MODULE_93CB39188073476BBBA76130DF31E5AA 1

#include <cstdint>
#include <vector>
#include <memory>

#include "MemoryModule.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        /**
         * @brief
         */
        class RAMModule : public MemoryModule
        {
            private:
                std::vector<uint8_t> mRAM; ///< Contents of RAM.

            public:
                /**
                 * @brief
                 */
                RAMModule(std::shared_ptr<CompiledBuffer>& compiledBufferPtr, uintptr_t baseAddress, size_t blockSizeBytes);

                /**
                 * @brief Destructor
                 */
                virtual ~RAMModule();

            public:
                /**
                 * @brief Called by the emulator when a bank switch is being performed. Copy the state of this object to the emulator.
                 */
                virtual void SwitchBankIn();

                /**
                 * @brief Called by the emulator when a bank switch is being performed. Copy the emulator state to this object.
                 */
                virtual void SwitchBankOut();

            protected:
                /**
                 * @brief Check if the address is within the space of the memory module. The address is relative to the start of the module.
                 */
                virtual void RangeCheckAddress(uintptr_t address);
        }; // class RAMModule
    } // namespace Emulator
} // namespace JITZ80Lib

#endif // RAM_MODULE_93CB39188073476BBBA76130DF31E5AA
