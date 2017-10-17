#ifndef DLL_BODY_AF991561F5944C8D8FE7F0C2603A2BA0
#define DLL_BODY_AF991561F5944C8D8FE7F0C2603A2BA0 1

#include <cstdint>
#include <vector>
#include "Emulator/CompiledBuffer.hpp"

extern JITZ80Lib::Emulator::CompiledBuffer gCompiledBuffer;
extern std::vector<uint8_t> gZ80CodeBuffer;

extern bool StartInstance();
extern void EndInstance();


#endif // DLL_BODY_AF991561F5944C8D8FE7F0C2603A2BA0
