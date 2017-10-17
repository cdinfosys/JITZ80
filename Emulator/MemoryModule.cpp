#include "MemoryModule.hpp"
#include "../Support/JITZ80Exception.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        using std::shared_ptr;
        using JITZ80Lib::Emulator::FnCallCompiledInstructions;
        using JITZ80Lib::Support::JITZ80Exception;
        using JITZ80Lib::Support::JITZ80ExceptionCode;

        MemoryModule::MemoryModule(shared_ptr<CompiledBuffer>& compiledBufferPtr, uintptr_t baseAddress)
            :   mCompiledBufferPtr(compiledBufferPtr->CreateClone()),
                mBaseAddress(baseAddress)
        {
        }

        void MemoryModule::ExecuteAtAddress(uintptr_t z80Address)
        {
            if (z80Address < this->mBaseAddress) throw JITZ80Exception(JITZ80ExceptionCode::AddressOutOfRange);
            RangeCheckAddress(z80Address - this->mBaseAddress);

            FnCallCompiledInstructions compiledRoutine = this->mCompiledBufferPtr->GetCompiledCodeCallback(z80Address);
            compiledRoutine();
        }

    } // namespace Emulator
} // namespace JITZ80Lib
