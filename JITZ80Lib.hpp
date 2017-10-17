// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the JITZ80LIB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// JITZ80LIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef JITZ80LIB_EXPORTS
#define JITZ80LIB_API extern "C" __declspec(dllexport)
#else
#define JITZ80LIB_API extern "C" __declspec(dllimport)
#endif

#include <cstdint>
#include <Windows.h>
#include "Z80InstructionDecodedProc.hpp"
#include "Z80PortOutProc.hpp"

// Error codes
const int E_JITZ80_ADDRESS_RANGE                    = 0xA001; ///< An address is out of range
const int E_JITZ80_STRUCT_SIZE                      = 0xA002; ///< Invalid structure size
const int E_JITZ80_NUMBER_OF_BANKS                  = 0xA003; ///< Invalid number of banks. Must be 1, 2, 4, or 8.
const int E_JITZ80_ALREADY_INITIALISED              = 0xA004; ///< Initialisation was already performed.
const int E_JITZ80_NOT_INITIALISED                  = 0xA005; ///< Initialisation has not been performed.
const int E_JITZ80_BANK_NUMBER                      = 0xA006; ///< Invalid bank number
const int E_JITZ80_BANK_BLOCK_NUMBER                = 0xA007; ///< Invalid bank sub-block number
const int E_JITZ80_BANK_BLOCK_ALREADY_CREATED       = 0xA008; ///< Attempting to create a RAM or ROM block in an area that has already been initialised.
const int E_JITZ80_NO_MEMORY_MODULE                 = 0xA009; ///< There is no memory module at the requested bank/block
const int E_JITZ80_EMULATOR_ERROR                   = 0xA010; ///< The emulator encountered an error.

#pragma pack(push)
#pragma pack(1)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Callback function prototypes
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Parameter for the \c JITZ80_Initialise function.
 */
typedef struct jitz80_init_struct
{
    uint32_t structSizeBytes;   ///< Size of this structure
    uint32_t numberOfBanks;     ///< Number of banks (1, 2, 4, 8)
    pfnZ80InstructionDecodedProc decoderProc; ///< Function to call when an instruction is decoded. (May be null)
    pfnZ80PortOutProc portOutProc; ///< Routine called when an OUT instruction is encountered.
    void* z80PortOutProcAdditionalData; ///< Additional data that is sent to \c portOutProc
} JITZ80_INIT_STRUCT, *PJITZ80_INIT_STRUCT;
#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Exported functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Initialise the module.
 * @param initDataPtr Pointer to a JITZ80_INIT_STRUCT structure.
 */
JITZ80LIB_API HRESULT JITZ80_Initialise(PJITZ80_INIT_STRUCT initDataPtr);

/**
 * @brief Perform housekeeping and release resources held by the module.
 */
JITZ80LIB_API HRESULT JITZ80_Dispose();

/**
 * @brief Call after initialization to create a block of RAM in a bank
 * @param bankNumber Bank number (0..7, depening on setup)
 * @param blockNumber Block number (0..7, depening on setup)
 */
JITZ80LIB_API HRESULT JITZ80_CreateRAM(uintptr_t bankNumber, uintptr_t blockNumber);

/**
 * @brief Load Z80 machine code into the emulator. Do not call this method after the emulator has started executing.
 * @param bankNumber Bank number (0..7, depening on setup)
 * @param blockNumber Block number (0..7, depening on setup)
 * @param bufferPtr bufferPtr Pointer to the buffer that holds the Z80 code to move into the buffer.
 */
JITZ80LIB_API HRESULT JITZ80_LoadROM(uintptr_t bankNumber, uintptr_t blockNumber, const void* bufferPtr);

/**
 * @brief Activate a block in a memory bank (bank switch).
 */
JITZ80LIB_API HRESULT JITZ80_SwitchBankBlock(uintptr_t bankNumber, uintptr_t blockNumber);

/**
 * @brief Initialise the CPU to its initial state.
 */
JITZ80LIB_API HRESULT JITZ80_ResetCPUState();

/**
 * @brief Single step.
 */
JITZ80LIB_API HRESULT JITZ80_Step();
