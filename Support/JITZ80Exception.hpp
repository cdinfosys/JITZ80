#ifndef JITZ80_EXCEPTION_B32EB858AA164A92AA980291BA9A6BCB
#define JITZ80_EXCEPTION_B32EB858AA164A92AA980291BA9A6BCB 1

#include <stdexcept>

namespace JITZ80Lib
{
    namespace Support
    {
        enum class JITZ80ExceptionCode
        {
            None = 0,
            AddressInUse,   // Attempting to place an instruction's compiled code at an address where compiled code is already stored.
            NoAllocation,   // There is no compiled code for the address being called.
            NotInstructionLeadByte, // The address being called is not the first byte of an instruction opcode
            AddressOutOfRange, // The address is out of range
            UnknownInstruction, // Unable to determine the type of instruction
        }; // class JITZ80ExceptionCode

        /**
         * @brief
         */
        class JITZ80Exception : public std::runtime_error
        {
            private:
                JITZ80ExceptionCode mErrorCode; ///< Error code
                int mZ80InstructionAddress; ///< Z80 instruction that caused the exception.

            public:
                /**
                 * @brief
                 */
                JITZ80Exception(JITZ80ExceptionCode mErrorCode);

                JITZ80Exception(JITZ80ExceptionCode mErrorCode, const std::string& what);

                JITZ80Exception(JITZ80ExceptionCode mErrorCode, int z80InstructionAddress);

                JITZ80Exception(JITZ80ExceptionCode mErrorCode, const std::string& what, int z80InstructionAddress);

                /**
                 * @brief Destructor
                 */
                virtual ~JITZ80Exception();

            public:
                inline int GetZ80InstructionAddress() const;
        }; // class JITZ80Exception

        inline int JITZ80Exception::GetZ80InstructionAddress() const
        {
            return this->mZ80InstructionAddress;
        }
    } // namespace Support
} // namespace JITZ80Lib

#endif // JITZ80_EXCEPTION_B32EB858AA164A92AA980291BA9A6BCB
