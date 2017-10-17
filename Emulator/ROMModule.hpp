#ifndef ROM_MODULE_5F18CF3127F7425EBD128309DBBEA218
#define ROM_MODULE_5F18CF3127F7425EBD128309DBBEA218 1

#include <memory>
#include <vector>
#include "MemoryModule.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        /**
         * @brief
         */
        class ROMModule : public MemoryModule
        {
            private:
                std::vector<uint8_t> mROM; ///< Contents of ROM.

            public:
                /**
                 * @brief Construct the ROM module
                 */
                ROMModule(std::shared_ptr<CompiledBuffer>& compiledBufferPtr, uintptr_t baseAddress, size_t blockSizeBytes, const void* sourceBuffer);

                /**
                 * @brief Destructor
                 */
                virtual ~ROMModule();

            public:
                /**
                 * @brief Called by the emulator when a bank switch is being performed. Copy the state of this object to the emulator.
                 */
                virtual void SwitchBankIn();

                /**
                 * @brief Called by the emulator when a bank switch is being performed. Copy the emulator state to this object.
                 */
                virtual inline void SwitchBankOut();

            protected:
                /**
                 * @brief Check if the address is within the space of the memory module. The address is relative to the start of the module.
                 */
                virtual void RangeCheckAddress(uintptr_t address);
        }; // class ROMModule

        inline void ROMModule::SwitchBankOut()
        {
        }
    } // namespace Emulator
} // namespace JITZ80Lib

#endif // ROM_MODULE_5F18CF3127F7425EBD128309DBBEA218
