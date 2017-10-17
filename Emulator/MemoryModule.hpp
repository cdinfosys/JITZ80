#ifndef MEMORY_MODULE_5CDDB644997F4227AFFD3FE6F752100E
#define MEMORY_MODULE_5CDDB644997F4227AFFD3FE6F752100E 1

#include <cstdint>
#include <memory>
#include "CompiledBuffer.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        /**
         * @brief
         */
        class MemoryModule
        {
            private:
                std::unique_ptr<CompiledBuffer> mCompiledBufferPtr; ///< Buffer for compiled code from this module
                uintptr_t mBaseAddress; ///< Base address of the memory module.

            protected:
                /**
                 * @brief Constructor is hidden.
                 */
                MemoryModule(std::shared_ptr<CompiledBuffer>& compiledBufferPtr, uintptr_t baseAddress);

            public:
                /**
                 * @brief Destructor
                 */
                virtual inline ~MemoryModule();

            public:
                void ExecuteAtAddress(uintptr_t z80Address);

            public:
                /**
                 * @brief Called by the emulator when a bank switch is being performed. Copy the state of this object to the emulator.
                 */
                virtual void SwitchBankIn() = 0;

                /**
                 * @brief Called by the emulator when a bank switch is being performed. Copy the emulator state to this object.
                 */
                virtual void SwitchBankOut() = 0;

            protected:
                /**
                 * @brief Get a reference to the compiled buffer object.
                 */
                inline CompiledBuffer& GetCompiledBuffer() const;

                /**
                 * @brief Get the base address of the module.
                 */
                inline uintptr_t GetBaseAddress() const;

            protected:
                /**
                 * @brief Check if the address is within the space of the memory module. The address is relative to the start of the module.
                 */
                virtual void RangeCheckAddress(uintptr_t address) = 0;
        }; // class MemoryModule

        inline MemoryModule::~MemoryModule()
        {
        }

        inline CompiledBuffer& MemoryModule::GetCompiledBuffer() const
        {
            return *(this->mCompiledBufferPtr.get());
        }

        inline uintptr_t MemoryModule::GetBaseAddress() const
        {
            return this->mBaseAddress;
        }
    } // namespace Emulator
} // namespace JITZ80Lib

#endif // MEMORY_MODULE_5CDDB644997F4227AFFD3FE6F752100E
