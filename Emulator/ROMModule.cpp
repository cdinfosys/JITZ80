#include "ROMModule.hpp"
#include "../Support/JITZ80Exception.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        using std::vector;
        using std::shared_ptr;
        using JITZ80Lib::Support::JITZ80Exception;
        using JITZ80Lib::Support::JITZ80ExceptionCode;

        ROMModule::ROMModule(shared_ptr<CompiledBuffer>& compiledBufferPtr, uintptr_t baseAddress, size_t blockSizeBytes, const void* sourceBuffer)
            :   MemoryModule(compiledBufferPtr, baseAddress),
                mROM(blockSizeBytes)
        {
            memcpy(mROM.data(), sourceBuffer, blockSizeBytes);
        }

        ROMModule::~ROMModule()
        {
        }

        void ROMModule::SwitchBankIn()
        {
            uint8_t* destPtr = reinterpret_cast<uint8_t*>(this->GetCompiledBuffer().GetMachineMemoryBlockPtr());
            memcpy(destPtr + GetBaseAddress(), this->mROM.data(), this->mROM.size());
        }

        void ROMModule::RangeCheckAddress(uintptr_t address)
        {
            if (address >= (uintptr_t)mROM.size()) throw JITZ80Exception(JITZ80ExceptionCode::AddressOutOfRange);
        }
    } // namespace Emulator
} // namespace JITZ80Lib
