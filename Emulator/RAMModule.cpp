#include "RAMModule.hpp"
#include "../Support/JITZ80Exception.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        using std::vector;
        using JITZ80Lib::Support::JITZ80Exception;
        using JITZ80Lib::Support::JITZ80ExceptionCode;

        RAMModule::RAMModule(std::shared_ptr<CompiledBuffer>& compiledBufferPtr, uintptr_t baseAddress, size_t blockSizeBytes)
            :   MemoryModule(compiledBufferPtr, baseAddress),
                mRAM(blockSizeBytes, 0u)
        {
        }

        RAMModule::~RAMModule()
        {
        }

        void RAMModule::SwitchBankIn()
        {
            uint8_t* destPtr = reinterpret_cast<uint8_t*>(this->GetCompiledBuffer().GetMachineMemoryBlockPtr());
            memcpy(destPtr + GetBaseAddress(), this->mRAM.data(), this->mRAM.size());
        }

        void RAMModule::SwitchBankOut()
        {
            uint8_t* srcPtr = reinterpret_cast<uint8_t*>(this->GetCompiledBuffer().GetMachineMemoryBlockPtr());
            memcpy(this->mRAM.data(), srcPtr + GetBaseAddress(), this->mRAM.size());
        }

        void RAMModule::RangeCheckAddress(uintptr_t address)
        {
            if (address >= (uintptr_t)mRAM.size()) throw JITZ80Exception(JITZ80ExceptionCode::AddressOutOfRange);
        }
    } // namespace Emulator
} // namespace JITZ80Lib
