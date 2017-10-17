#ifndef SOURCE_BYTE_A1E94B7A12D14FBBBB1181653378F97F
#define SOURCE_BYTE_A1E94B7A12D14FBBBB1181653378F97F 1

#include <cstdint>

namespace JITZ80Lib
{
    namespace Classes
    {
        /**
         * @brief Information about a byte in the source data buffer.
         */
        class SourceByte
        {
            private:
                uint8_t mAddressVal; ///< Value of the byte at the address.

            public:
                /**
                 * @brief Default constructor.
                 */
                SourceByte();

                /**
                 * @brief Copy constructor.
                 * @param src Object from where the values are copied.
                 */
                SourceByte(const SourceByte& src);

                /**
                 * @brief Move constructor.
                 * @param src Object from where the values are copied.
                 */
                SourceByte(SourceByte&& src);

                /**
                 * @brief Destructor
                 */
                virtual ~SourceByte();

            public:
                /**
                 * @brief Copy assignment operator
                 * @param src Object from where the values are copied.
                 * @returns Returns a reference to this object.
                 */
                SourceByte operator=(const SourceByte& src);

                /**
                 * @brief Move assignment operator
                 * @param src Object from where the values are copied.
                 * @returns Returns a reference to this object.
                 */
                SourceByte operator=(SourceByte&& src);
        }; // class SourceByte
    } // namespace Classes
} // namespace JITZ80Lib

#endif // SOURCE_BYTE_A1E94B7A12D14FBBBB1181653378F97F
