#ifndef SHARED_ADDRESS_ALLOCATOR_3F3E7EF2C8C94FFFAA5D7661FF4BD22B
#define SHARED_ADDRESS_ALLOCATOR_3F3E7EF2C8C94FFFAA5D7661FF4BD22B 1

#include <cstdint>

namespace JITZ80Lib
{
    namespace Emulator
    {
        /**
         * @brief
         */
        class SharedAddressAllocator
        {
            private:
                uintptr_t mNextBufferAddress;

            public:
                /**
                 * @brief
                 */
                SharedAddressAllocator();

                /**
                 * @brief Copy constructor.
                 * @param src Object from where the values are copied.
                 */
                SharedAddressAllocator(const SharedAddressAllocator& src) = delete;

                /**
                 * @brief Move constructor.
                 * @param src Object from where the values are copied.
                 */
                SharedAddressAllocator(SharedAddressAllocator&& src);

                /**
                 * @brief Destructor
                 */
                virtual ~SharedAddressAllocator();

            public:
                /**
                 * @brief Copy assignment operator
                 * @param src Object from where the values are copied.
                 * @returns Returns a reference to this object.
                 */
                SharedAddressAllocator& operator=(const SharedAddressAllocator& src) = delete;

                /**
                 * @brief Move assignment operator
                 * @param src Object from where the values are copied.
                 * @returns Returns a reference to this object.
                 */
                SharedAddressAllocator& operator=(SharedAddressAllocator&& src);

            public:
                inline uintptr_t GetNextBufferAddress() const;
                inline void AdjustNextBufferAddress(uintptr_t allocatedBufferBlockSize);
        }; // class SharedAddressAllocator

        inline uintptr_t SharedAddressAllocator::GetNextBufferAddress() const
        {
            return this->mNextBufferAddress;
        }

        inline void SharedAddressAllocator::AdjustNextBufferAddress(uintptr_t allocatedBufferBlockSize)
        {
            this->mNextBufferAddress += allocatedBufferBlockSize;
        }
    } // namespace Emulator
} // namespace JITZ80Lib

#endif // SHARED_ADDRESS_ALLOCATOR_3F3E7EF2C8C94FFFAA5D7661FF4BD22B
