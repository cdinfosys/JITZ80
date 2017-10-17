#ifndef OPERATING_SYSTEM_EXCEPTION_H_F9880DE5_E1FF_44DD_8F74_E029B0DCF07
#define OPERATING_SYSTEM_EXCEPTION_H_F9880DE5_E1FF_44DD_8F74_E029B0DCF07 1

#include <stdexcept>
#include <errno.h>
#include <Windows.h>

namespace JITZ80Lib
{
    namespace Support
    {
        /**
         * Class for reporting operating system errors.
         *
         * \brief Operating system error.
         */
        class OperatingSystemException : public std::runtime_error
        {
            private:
                DWORD _errorCode;

            public:
                /**
                    * \brief Constructor overload.
                    * \param errorCode Operating system error code value.
                    * \param what_arg Additional exception information.
                    */
                OperatingSystemException(DWORD errorCode, const std::string& what_arg);

                /**
                 * \brief Destructor.
                 */
                virtual ~OperatingSystemException(void) noexcept;

                /**
                 * \brief Copy constructor.
                 * \param other Referenct to another instance of the OperatingSystemException class from where this instance's member values are copied.
                 */
                OperatingSystemException(const OperatingSystemException& other);

                /**
                 * \brief Move constructor.
                 * \param other Referenct to another instance of the OperatingSystemException class from where this instance's member values are copied.
                 */
                OperatingSystemException(OperatingSystemException&& other);

            public:
                /**
                 * \brief Assignment operator.
                 * \param other Reference to another instance of the OperatingSystemException class from where the values for this object are copied.
                 */
                OperatingSystemException& operator=(const OperatingSystemException& other);

                /**
                 * \brief Move assignment operator.
                 * \param other Reference to another instance of the OperatingSystemException class from where the values for this object are copied.
                 */
                OperatingSystemException& operator=(OperatingSystemException&& other);

                /**
                 * \brief Get the O/S error code value.
                 * \return Returns the O/S error code.
                 */
                DWORD GetErrorCode(void) const { return _errorCode; }
        };
    } // namespace Support
} // namespace JITZ80Lib

#endif // OPERATING_SYSTEM_EXCEPTION_H_F9880DE5_E1FF_44DD_8F74_E029B0DCF07
