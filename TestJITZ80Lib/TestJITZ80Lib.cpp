// TestJITZ80Lib.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdexcept>
#include <memory.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstdint>
#include <ios>
#include <fstream>
#include <istream>
#include <iterator>
#include <JITZ80Lib.hpp>

void _cdecl CallTest(uintptr_t val, uintptr_t port, void* a)
{
}

void Z80DecoderCallbackProc(const PJITZ80_DECODED_INSTRUCTION_STRUCT instructionData);
void Z80PortOutProc(uintptr_t value, uintptr_t portNumber, void* additionalDataPtr);

inline void TestHRESULT(HRESULT hRes)
{
    if (FAILED(hRes))
    {
        std::stringstream ss;
        ss << "HRESULT 0x" << std::hex << hRes << " (facility " << HRESULT_FACILITY(hRes) << ",  code " << HRESULT_CODE(hRes) << ")";
        throw std::runtime_error(ss.str());
    }
}

int main(int argc, int argv)
{
    int* pInt = new int(5);
    CallTest(74, 42, (void*)pInt);

    try
    {
        JITZ80_INIT_STRUCT initStruct;
        initStruct.structSizeBytes = sizeof(initStruct);
        initStruct.numberOfBanks = 4u;
        initStruct.decoderProc = Z80DecoderCallbackProc;
        initStruct.portOutProc = Z80PortOutProc;
        initStruct.z80PortOutProcAdditionalData = nullptr;
        TestHRESULT(JITZ80_Initialise(&initStruct));

        std::vector<uint8_t> biosROM;
        std::ifstream biosROMReader("C:\\blueMSX\\blueMSXv282full\\Machines\\MSX - Spectravideo SVI-728\\728bios.rom", std::ios::binary);
        biosROMReader.unsetf(std::ios::skipws);
        biosROM.insert(biosROM.begin(), std::istream_iterator<uint8_t>(biosROMReader), std::istream_iterator<uint8_t>());

        // ROM BIOS and MSX-BASIC
        TestHRESULT(JITZ80_LoadROM(0u, 0u, biosROM.data()));
        TestHRESULT(JITZ80_LoadROM(0u, 1u, biosROM.data() + (16u * 1024u)));

        // Four blocks of RAM in bank 1
        TestHRESULT(JITZ80_CreateRAM(1u, 0u));
        TestHRESULT(JITZ80_CreateRAM(1u, 1u));
        TestHRESULT(JITZ80_CreateRAM(1u, 2u));
        TestHRESULT(JITZ80_CreateRAM(1u, 3u));

        // Setup initial machine configuration
        TestHRESULT(JITZ80_SwitchBankBlock(0, 0));
        TestHRESULT(JITZ80_SwitchBankBlock(0, 1));
        TestHRESULT(JITZ80_SwitchBankBlock(1, 2));
        TestHRESULT(JITZ80_SwitchBankBlock(1, 3));

        TestHRESULT(JITZ80_ResetCPUState());

        TestHRESULT(JITZ80_Step());
        TestHRESULT(JITZ80_Step());
        TestHRESULT(JITZ80_Step());

        TestHRESULT(JITZ80_Dispose());
    }
    catch (std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
    return 0;
}

void Z80DecoderCallbackProc(const PJITZ80_DECODED_INSTRUCTION_STRUCT instructionData)
{
}

void Z80PortOutProc(uintptr_t value, uintptr_t portNumber, void* additionalDataPtr)
{
    std::cout << std::hex << "[PORT 0x" << portNumber << "] <- 0x" << value << std::endl;
}