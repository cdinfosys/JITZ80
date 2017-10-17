// JITZ80Lib.cpp : Defines the exported functions for the DLL application.
//

#include <algorithm>
#include <cstdint>
#include <vector>
#include <memory>

#include "stdafx.h"
#include "JITZ80Lib.hpp"
#include "Emulator/CompiledBuffer.hpp"
#include "Emulator/JITCompiler.hpp"
#include "Emulator/RAMModule.hpp"
#include "Emulator/ROMModule.hpp"
#include "Support/JITZ80Exception.hpp"
#include "DLLBody.hpp"

using std::min;
using std::vector;
using std::shared_ptr;
using std::unique_ptr;

// ----
using JITZ80Lib::Emulator::CompiledBuffer;
using JITZ80Lib::Emulator::JITCompiler;
using JITZ80Lib::Emulator::MemoryModule;
using JITZ80Lib::Emulator::RAMModule;
using JITZ80Lib::Emulator::ROMModule;
using JITZ80Lib::Support::JITZ80Exception;

const size_t Z80_ADDRESS_MAX = 0xFFFFu;
const size_t Z80_MAX_BYTES = 64u * 1024u;

bool gIsInitialised(false); ///< Flag to indicate that the module is initialised.
size_t gBankBlockSizeBytes(1u); ///< Size of each block in a memory bank.
size_t gNumberOfBanks(1u); ///< Number of banks.
vector<unique_ptr<MemoryModule>> gMemoryModules(64u); ///< Up to 64 memory modules can be active.
vector<MemoryModule*> gActiveMemoryModules(8u); ///< Points to entries in gMemoryModules that are active.
shared_ptr<CompiledBuffer> gCompiledBufferPtr; ///< Compiled code buffer and emulator state.
shared_ptr<JITCompiler> gZ80JITCompiler; ///< JIT compiler object.
pfnZ80InstructionDecodedProc gDecoderCallback(nullptr); ///< Pointer to a function that is called when the decoder is decoding an instruction.

// This is an example of an exported function.
/*
JITZ80LIB_API HRESULT fnJITZ80Lib(void)
{
    JITZ80Lib::Emulator::CompiledCode compiledCode
    (
        0, 
        std::vector<uint8_t> { 0x00 }, 
        std::vector<uint8_t> { 0x90 }
    );
    compiledBuffer.EmitRoutine(std::vector<JITZ80Lib::Emulator::CompiledCode> { compiledCode });
    auto p = compiledBuffer.GetCompiledCodeCallback(0);
    p();
    /*
    for (int i = 0; i < 10000; ++i)
    {
    }
    */
    /*
    return S_OK;
}
*/

JITZ80LIB_API HRESULT JITZ80_Initialise(PJITZ80_INIT_STRUCT initDataPtr)
{
    if (gIsInitialised) return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, E_JITZ80_ALREADY_INITIALISED);
    if (initDataPtr->structSizeBytes != sizeof(jitz80_init_struct)) return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, E_JITZ80_STRUCT_SIZE);

    switch (initDataPtr->numberOfBanks)
    {
        case 1:
            gBankBlockSizeBytes = (64u * 1024u);
            gNumberOfBanks = 1u;
            break;

        case 2:
            gBankBlockSizeBytes = (32u * 1024u);
            gNumberOfBanks = 2u;
            break;

        case 4:
            gBankBlockSizeBytes = (16u * 1024u);
            gNumberOfBanks = 4u;
            break;

        case 8:
            gBankBlockSizeBytes = (8u * 1024u);
            gNumberOfBanks = 8u;
            break;

        default:
            return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, E_JITZ80_NUMBER_OF_BANKS);
    }

    gZ80JITCompiler.reset(new JITCompiler(initDataPtr->decoderProc, initDataPtr->portOutProc, initDataPtr->z80PortOutProcAdditionalData));
    gCompiledBufferPtr.reset(new CompiledBuffer(gZ80JITCompiler));
    gDecoderCallback = initDataPtr->decoderProc;
    gCompiledBufferPtr->ResetCPUState();

    gIsInitialised = true;
    return S_OK;
}

JITZ80LIB_API HRESULT JITZ80_Dispose()
{
    gCompiledBufferPtr.reset();
    return S_OK;
}

JITZ80LIB_API HRESULT JITZ80_CreateRAM(uintptr_t bankNumber, uintptr_t blockNumber)
{
    if (!gIsInitialised) return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, E_JITZ80_NOT_INITIALISED);
    if (bankNumber >= gNumberOfBanks) return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, E_JITZ80_BANK_NUMBER);
    if (blockNumber >= gNumberOfBanks) return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, E_JITZ80_BANK_BLOCK_NUMBER);

    size_t blockIndex = (size_t)((bankNumber * gNumberOfBanks) + blockNumber);
    size_t blockBaseAddress = (size_t)(blockNumber * gBankBlockSizeBytes);

    if (gMemoryModules[blockIndex] != nullptr) return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, E_JITZ80_BANK_BLOCK_ALREADY_CREATED);

    gMemoryModules[blockIndex].reset(new RAMModule(gCompiledBufferPtr, blockBaseAddress, gBankBlockSizeBytes));
    gActiveMemoryModules[blockNumber] = gMemoryModules[blockIndex].get();
    gActiveMemoryModules[blockNumber]->SwitchBankIn();
    return S_OK;
}

JITZ80LIB_API HRESULT JITZ80_LoadROM(uintptr_t bankNumber, uintptr_t blockNumber, const void* bufferPtr)
{
    if (!gIsInitialised) return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, E_JITZ80_NOT_INITIALISED);

    if (bankNumber >= gNumberOfBanks) return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, E_JITZ80_BANK_NUMBER);
    if (blockNumber >= gNumberOfBanks) return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, E_JITZ80_BANK_BLOCK_NUMBER);

    size_t blockIndex = (size_t)((bankNumber * gNumberOfBanks) + blockNumber);
    size_t blockBaseAddress = (size_t)(blockNumber * gBankBlockSizeBytes);

    if (gMemoryModules[blockIndex] != nullptr) return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, E_JITZ80_BANK_BLOCK_ALREADY_CREATED);

    gMemoryModules[blockIndex].reset(new ROMModule(gCompiledBufferPtr, blockBaseAddress, gBankBlockSizeBytes, bufferPtr));
    gActiveMemoryModules[blockNumber] = gMemoryModules[blockIndex].get();
    gActiveMemoryModules[blockNumber]->SwitchBankIn();

    return S_OK;
}

JITZ80LIB_API HRESULT JITZ80_SwitchBankBlock(uintptr_t bankNumber, uintptr_t blockNumber)
{
    if (!gIsInitialised) return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, E_JITZ80_NOT_INITIALISED);

    if (bankNumber >= gNumberOfBanks) return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, E_JITZ80_BANK_NUMBER);
    if (blockNumber >= gNumberOfBanks) return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, E_JITZ80_BANK_BLOCK_NUMBER);

    size_t blockIndex = (size_t)((bankNumber * gNumberOfBanks) + blockNumber);

    MemoryModule* newActiveMemoryModule = gMemoryModules[blockIndex].get();
    if (newActiveMemoryModule != gActiveMemoryModules[blockNumber])
    {
        if (gActiveMemoryModules[blockNumber] != nullptr)
        {
            gActiveMemoryModules[blockNumber]->SwitchBankOut();
        }
        gActiveMemoryModules[blockNumber] = newActiveMemoryModule;
        if (gActiveMemoryModules[blockNumber] != nullptr)
        {
            gActiveMemoryModules[blockNumber]->SwitchBankIn();
        }
    }

    return S_OK;
}

JITZ80LIB_API HRESULT JITZ80_ResetCPUState()
{
    if (!gIsInitialised) return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, E_JITZ80_NOT_INITIALISED);
    gCompiledBufferPtr->ResetCPUState();

    return S_OK;
}

JITZ80LIB_API HRESULT JITZ80_Step()
{
    if (!gIsInitialised) return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, E_JITZ80_NOT_INITIALISED);

    uintptr_t currentPC = gCompiledBufferPtr->GetProgramCounter();
    uintptr_t blockNumber = (size_t)currentPC / gBankBlockSizeBytes;
    if (gActiveMemoryModules[blockNumber] == nullptr) return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, E_JITZ80_NO_MEMORY_MODULE);

    try
    {
        gActiveMemoryModules[blockNumber]->ExecuteAtAddress(currentPC);
    }
    catch (JITZ80Exception&)
    {
        return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, E_JITZ80_EMULATOR_ERROR);
    }

    return S_OK;
}
