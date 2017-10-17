#include "JITZ80Exception.hpp"

namespace JITZ80Lib
{
    namespace Support
    {
        using std::runtime_error;

        JITZ80Exception::JITZ80Exception(JITZ80ExceptionCode mErrorCode)
            :   runtime_error(nullptr),
                mErrorCode(mErrorCode),
                mZ80InstructionAddress(-1)
        {
        }

        JITZ80Exception::JITZ80Exception(JITZ80ExceptionCode mErrorCode, const std::string& what)
            :   runtime_error(what),
                mErrorCode(mErrorCode),
                mZ80InstructionAddress(-1)
        {
        }

        JITZ80Exception::JITZ80Exception(JITZ80ExceptionCode mErrorCode, int z80InstructionAddress)
            :   runtime_error(nullptr),
                mErrorCode(mErrorCode),
                mZ80InstructionAddress(z80InstructionAddress)
        {
        }

        JITZ80Exception::JITZ80Exception(JITZ80ExceptionCode mErrorCode, const std::string& what, int z80InstructionAddress)
            :   runtime_error(what),
                mErrorCode(mErrorCode),
                mZ80InstructionAddress(z80InstructionAddress)
        {
        }

        JITZ80Exception::~JITZ80Exception()
        {
        }

    } // namespace Support
} // namespace JITZ80Lib
