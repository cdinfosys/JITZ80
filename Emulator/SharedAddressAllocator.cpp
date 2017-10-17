#include "SharedAddressAllocator.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        SharedAddressAllocator::SharedAddressAllocator()
            :   mNextBufferAddress(0u)
        {
        }

        SharedAddressAllocator::SharedAddressAllocator(SharedAddressAllocator&& src)
            :   mNextBufferAddress(src.mNextBufferAddress)
        {
        }

        SharedAddressAllocator& SharedAddressAllocator::operator=(SharedAddressAllocator&& src)
        {
            mNextBufferAddress = src.mNextBufferAddress;
            return *this;
        } 

        SharedAddressAllocator::~SharedAddressAllocator()
        {
        }
    } // namespace Emulator
} // namespace JITZ80Lib
