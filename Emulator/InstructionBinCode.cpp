#include "InstructionBinCode.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        CompiledFunctionPrologue::CompiledFunctionPrologue(uintptr_t z80MemoryAddress, uintptr_t cpuStateStructAddress)
            :   InstructionBinCode(GetBinaryCode(z80MemoryAddress, cpuStateStructAddress))
        {
        }

        #ifdef _M_AMD64
        std::vector<uint8_t> CompiledFunctionPrologue::GetBinaryCode(uintptr_t z80MemoryAddress, uintptr_t cpuStateStructAddress)
        {
            #pragma pack(push)
            #pragma pack(1)
            union 
            {
                uint64_t address;
                uint8_t addressBytes[8];
            } z80MemoryAddressStruct;

            union 
            {
                uint64_t address;
                uint8_t addressBytes[8];
            } cpuStateAddressStruct;
            #pragma pack(pop)
            z80MemoryAddressStruct.address = z80MemoryAddress;
            cpuStateAddressStruct.address = cpuStateStructAddress;

            std::vector<uint8_t> result
            {
                0x40, 0x55,                                 // push        rbp  
                0x57,                                       // push        rdi  
                0x48, 0x81, 0xEC, 0xC8, 0x00, 0x00, 0x00,   // sub         rsp,0C8h  
                0x48, 0x8B, 0xEC,                           // mov         rbp,rsp  
                0x48, 0x8B, 0xFC,                           // mov         rdi,rsp  
                0xB9, 0x32, 0x00, 0x00, 0x00,               // mov         ecx,32h  
                0xB8, 0xCC, 0xCC, 0xCC, 0xCC,               // mov         eax,0CCCCCCCCh  
                0xF3, 0xAB,                                 // rep stos    dword ptr [rdi]  

                0x57,                                       // push rdi
                0x56,                                       // push rsi
                0x48, 0xBF,                                 // mov rdi, mCPUStateStructAddress
                    cpuStateAddressStruct.addressBytes[0],
                    cpuStateAddressStruct.addressBytes[1],
                    cpuStateAddressStruct.addressBytes[2],
                    cpuStateAddressStruct.addressBytes[3],
                    cpuStateAddressStruct.addressBytes[4],
                    cpuStateAddressStruct.addressBytes[5],
                    cpuStateAddressStruct.addressBytes[6],
                    cpuStateAddressStruct.addressBytes[7],
                0x48, 0xBE,                                 // mov rsi, mZ80MemoryAddress
                    z80MemoryAddressStruct.addressBytes[0],
                    z80MemoryAddressStruct.addressBytes[1],
                    z80MemoryAddressStruct.addressBytes[2],
                    z80MemoryAddressStruct.addressBytes[3],
                    z80MemoryAddressStruct.addressBytes[4],
                    z80MemoryAddressStruct.addressBytes[5],
                    z80MemoryAddressStruct.addressBytes[6],
                    z80MemoryAddressStruct.addressBytes[7],
                0x8A, 0x47, 0x20                            // mov al, BYTE[rdi + cpu_state_struct.AF]
            };
            return result;
        }
        #elif _M_IX86
        std::vector<uint8_t> CompiledFunctionPrologue::GetBinaryCode()
        {
            std::vector<uint8_t> result
            {
                0x55,                     // push ebp
                0x8B, 0xEC                // mov  ebp,esp 
            };

            return result;
        }
        #endif // _M_AMD64

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        CompiledFunctionEpilogue::CompiledFunctionEpilogue()
            :   InstructionBinCode(GetBinaryCode())
        {
        }

        #ifdef _M_AMD64
        std::vector<uint8_t> CompiledFunctionEpilogue::GetBinaryCode()
        {
            std::vector<uint8_t> result
            {
                
                0x88, 0x47, 0x20,                           // mov BYTE[rdi + cpu_state_struct.AF], al
                0x5E,                                       // pop rsi
                0x5F,                                       // pop rdi

                0x48, 0x8D, 0xA5, 0xC8, 0x00, 0x00, 0x00,   //lea         rsp,[rbp+0C8h]  
                0x5F,                                       //pop         rdi  
                0x5D,                                       //pop         rbp  
                0xC3,                                       //ret 
            };

            return result;
        }
        #elif _M_IX86
        std::vector<uint8_t> CompiledFunctionEpilogue::GetBinaryCode()
        {
            std::vector<uint8_t> result
            {
                0x8B, 0xE5,             // mov esp,ebp  
                0x5D,                   // pop ebp  
                0xC3,                   // ret  
            };

            return result;
        }
        #endif // _M_AMD64
    } // namespace Emulator
} // namespace JITZ80Lib
