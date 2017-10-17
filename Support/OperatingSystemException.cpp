#include "OperatingSystemException.hpp"

namespace JITZ80Lib
{
    namespace Support
    {
        OperatingSystemException::OperatingSystemException(DWORD errorCode, const std::string& what_arg)
            :   _errorCode(errorCode),
                std::runtime_error(what_arg)
        {
        }

        OperatingSystemException::~OperatingSystemException(void) noexcept
        {
        }

        OperatingSystemException::OperatingSystemException(const OperatingSystemException& other)
            :   std::runtime_error(other),
                _errorCode(other._errorCode)
        {
        }

        OperatingSystemException::OperatingSystemException(OperatingSystemException&& other)
            :   std::runtime_error(std::move(other)),
                _errorCode(other._errorCode)
        {
        }

        OperatingSystemException& OperatingSystemException::operator=(const OperatingSystemException& other)
        {
            if (this != &other)
            {
                this->_errorCode = other._errorCode;
                runtime_error::operator=(other);
            }

            return *this;
        }

        OperatingSystemException& OperatingSystemException::operator=(OperatingSystemException&& other)
        {
            if (this != &other)
            {
                this->_errorCode = other._errorCode;
                runtime_error::operator=(std::move(other));
            }

            return *this;
        }
    } // namespace Support
} // namespace JITZ80Lib
