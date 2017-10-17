#include<memory.h>
#include <algorithm>
#include "../Support/OperatingSystemException.hpp"
#include "../Support/JITZ80Exception.hpp"
#include "InstructionBinCode.hpp"
#include "CompiledBuffer.hpp"

namespace JITZ80Lib
{
    namespace Emulator
    {
        using std::shared_ptr;
        using std::unique_ptr;

        using JITZ80Lib::Support::OperatingSystemException;
        using JITZ80Lib::Support::JITZ80Exception;
        using JITZ80Lib::Support::JITZ80ExceptionCode;

        static LONG AllocationExceptionFilter(PEXCEPTION_POINTERS exceptionPointers);

        CompiledBuffer::CompiledBuffer(std::shared_ptr<JITZ80Lib::Emulator::JITCompiler>& jitCompiler)
            :   mVirtualMemoryBlockPtr(NULL),
                mZ80AddressSpaceMapping((size_t)(64u * 1024u)),
                mAddressAllocatorPtr(new SharedAddressAllocator),
                mBufferHeaderPtr(nullptr),
                mJITCompiler(jitCompiler),
                mOwnsVirtualMemory(true)
        {
            PVOID virtualMemPtr = ::VirtualAlloc(NULL, (SIZE_T)VIRTUAL_MEMORY_SIZE_BYTES, MEM_RESERVE, PAGE_EXECUTE_READWRITE);
            if (virtualMemPtr == NULL)
            {
                throw OperatingSystemException(::GetLastError(), __FUNCSIG__);
            }
            mVirtualMemoryBlockPtr = virtualMemPtr;

            mBufferHeaderPtr = AllocateBuffer(sizeof(CompiledBufferHeader), true);
            mCompiledBufferHeaderPtr = (CompiledBufferHeader*)mBufferHeaderPtr;

            mJumpTableBaseAddress = (PVOID*)AllocateBuffer(64u * 1024u * sizeof(PVOID), true);
            mCompiledBufferHeaderPtr->jumpTableBasePtr = mJumpTableBaseAddress;

            mZ80RAMBaseAddress = (uint8_t*)AllocateBuffer((64u * 1024u), true);
            mCompiledBufferHeaderPtr->z80RAMBasePtr = mZ80RAMBaseAddress;
        }

        CompiledBuffer::CompiledBuffer
            (
                PVOID virtualMemoryBlockPtr, 
                std::shared_ptr<SharedAddressAllocator> addressAllocatorPtr,
                std::shared_ptr<JITZ80Lib::Emulator::JITCompiler> jitCompilerPtr
            )
            :   mVirtualMemoryBlockPtr(virtualMemoryBlockPtr),
                mZ80AddressSpaceMapping((size_t)(64u * 1024u)),
                mAddressAllocatorPtr(addressAllocatorPtr),
                mBufferHeaderPtr(reinterpret_cast<CompiledBufferHeader*>(virtualMemoryBlockPtr)),
                mCompiledBufferHeaderPtr(reinterpret_cast<CompiledBufferHeader*>(virtualMemoryBlockPtr)),
                mJITCompiler(jitCompilerPtr),
                mZ80RAMBaseAddress(reinterpret_cast<CompiledBufferHeader*>(virtualMemoryBlockPtr)->z80RAMBasePtr),
                mOwnsVirtualMemory(false)
        {
        }

        CompiledBuffer::CompiledBuffer(CompiledBuffer&& src)
            :   mVirtualMemoryBlockPtr(src.mVirtualMemoryBlockPtr),
                mZ80AddressSpaceMapping(std::move(mZ80AddressSpaceMapping))
        {
            src.mVirtualMemoryBlockPtr = NULL;
        }
        CompiledBuffer& CompiledBuffer::operator=(CompiledBuffer&& src)
        {
            this->mVirtualMemoryBlockPtr = src.mVirtualMemoryBlockPtr;
            src.mVirtualMemoryBlockPtr = NULL;
            mZ80AddressSpaceMapping = std::move(mZ80AddressSpaceMapping);
            return *this;
        } 

        CompiledBuffer::~CompiledBuffer() noexcept
        {
            if (this->mOwnsVirtualMemory && (this->mVirtualMemoryBlockPtr != NULL))
            {
                if (::VirtualFree(this->mVirtualMemoryBlockPtr, (SIZE_T)0, MEM_DECOMMIT))
                {
                    ::VirtualFree(this->mVirtualMemoryBlockPtr, (SIZE_T)0, MEM_RELEASE);
                }

                this->mVirtualMemoryBlockPtr = NULL;
            }
        }

        unique_ptr<CompiledBuffer> CompiledBuffer::CreateClone() const
        {
            return unique_ptr<CompiledBuffer>
            (
                new CompiledBuffer(this->mVirtualMemoryBlockPtr, this->mAddressAllocatorPtr, this->mJITCompiler)
            );
        }

        PVOID CompiledBuffer::AllocateBuffer(size_t bufferSizeBytes, bool padBuffer)
        {
            if (bufferSizeBytes < 1u)
            {
                return NULL;
            }

            uint8_t* bufferStartPtr = reinterpret_cast<uint8_t*>(mVirtualMemoryBlockPtr);
            uint8_t* resultPtr = bufferStartPtr + mAddressAllocatorPtr->GetNextBufferAddress();

            // Check if each byte in the block is addressable
            __try
            {
                for (size_t blockByteIndex = 0u; blockByteIndex < bufferSizeBytes; ++blockByteIndex)
                {
                    resultPtr[blockByteIndex] = 0xcc; // Write garbage into the byte
                }
            }
            __except(AllocationExceptionFilter(GetExceptionInformation()))
            {
            }

            if (padBuffer)
            {
                size_t roundedAllocationBlocks = ((bufferSizeBytes + 15) / 16);
                mAddressAllocatorPtr->AdjustNextBufferAddress(16u * roundedAllocationBlocks);
            }
            else
            {
                mAddressAllocatorPtr->AdjustNextBufferAddress(bufferSizeBytes);
            }

            return reinterpret_cast<PVOID>(resultPtr);
        }

        PVOID CompiledBuffer::EmitFunctionPrologue()
        {
            CompiledFunctionPrologue prologue((uintptr_t)this->mCompiledBufferHeaderPtr->z80RAMBasePtr, (uintptr_t)&this->mCompiledBufferHeaderPtr->cpuState);
            PVOID compiledCodeBuffer = AllocateBuffer(prologue.GetOpcodeBuffer().size());
            memcpy(compiledCodeBuffer, prologue.GetOpcodeBuffer().data(), prologue.GetOpcodeBuffer().size());
            return compiledCodeBuffer;
        }

        void CompiledBuffer::EmitFunctionEpilogue()
        {
            CompiledFunctionEpilogue epilogue;
            PVOID compiledCodeBuffer = AllocateBuffer(epilogue.GetOpcodeBuffer().size(), true);
            memcpy(compiledCodeBuffer, epilogue.GetOpcodeBuffer().data(), epilogue.GetOpcodeBuffer().size());
        }

        void CompiledBuffer::Emit(const CompiledCode& compiledCode)
        {
            // First check if the new code does not conflict with code already in the buffer.
            for (size_t instructionByteNumber = 0; instructionByteNumber < compiledCode.GetZ80InstructionLength(); ++instructionByteNumber)
            {
                int instructionByteAddress = compiledCode.GetZ80InstructionAddress() + (int)instructionByteNumber;
                const CompiledBufferInstructionAddress& instructionData = (this->mZ80AddressSpaceMapping[instructionByteAddress]);
                if (CompiledBufferFlags::Allocated == (instructionData.GetFlags() & CompiledBufferFlags::Allocated))
                {
                    throw JITZ80Exception(JITZ80ExceptionCode::AddressInUse, compiledCode.GetZ80InstructionAddress());
                }
            }

            // Copy the compiled code into the buffer.
            PVOID compiledCodeBuffer = AllocateBuffer(compiledCode.GetCompiledCodeSize());
            memcpy(compiledCodeBuffer, compiledCode.GetCompiledCode().data(), compiledCode.GetCompiledCodeSize());

            // Store the Z80 instruction opcode bytes
            uint8_t* z80RAMPtr = this->mZ80RAMBaseAddress + compiledCode.GetZ80InstructionAddress();
            int addressIndex = compiledCode.GetZ80InstructionAddress();
            bool isLeadByte(true);
            for (uint8_t opcodeByte : compiledCode.GetZ80InstructionOpcodes())
            {
                *(z80RAMPtr++) = opcodeByte;

                CompiledBufferInstructionAddress& mapping = this->mZ80AddressSpaceMapping[addressIndex];
                if (isLeadByte)
                {
                    mapping.SetFlags(CompiledBufferFlags::Allocated | CompiledBufferFlags::InstructionLeadByte);
                    mapping.SetBufferAddress(compiledCodeBuffer);
                    mapping.SetZ80InstructionLengthBytes(compiledCode.GetZ80InstructionLength());
                    isLeadByte = false;
                }
                else
                {
                    mapping.SetFlags(CompiledBufferFlags::Allocated);
                }
                addressIndex = (addressIndex + 1u) & 0xFFFFu;
            }
        }

        FnCallCompiledInstructions CompiledBuffer::GetCompiledCodeCallback(uintptr_t z80InstructionAddress)
        {
            int safeAddress = z80InstructionAddress & 0xFFFF;
            const CompiledBufferInstructionAddress& mapping = mZ80AddressSpaceMapping[safeAddress];

            if ((mapping.GetFlags() & CompiledBufferFlags::Allocated) == CompiledBufferFlags::Allocated)
            {
                if ((mapping.GetFlags() & CompiledBufferFlags::InstructionLeadByte) != CompiledBufferFlags::InstructionLeadByte)
                {
                    throw JITZ80Exception(JITZ80ExceptionCode::NotInstructionLeadByte, safeAddress);
                }

                return (FnCallCompiledInstructions)mapping.GetBufferAddress();
            }

            return CompileCode(safeAddress);
        }

        FnCallCompiledInstructions CompiledBuffer::CompileCode(uintptr_t z80InstructionAddress)
        {
            EmitRoutine
            (
                this->mJITCompiler->CompileRoutine(z80InstructionAddress, *(this->mCompiledBufferHeaderPtr))
            );

            const CompiledBufferInstructionAddress& mapping = mZ80AddressSpaceMapping[z80InstructionAddress];

            if ((mapping.GetFlags() & CompiledBufferFlags::Allocated) != CompiledBufferFlags::Allocated)
            {
                throw JITZ80Exception(JITZ80ExceptionCode::NoAllocation, (int)z80InstructionAddress);
            }

            if ((mapping.GetFlags() & CompiledBufferFlags::InstructionLeadByte) != CompiledBufferFlags::InstructionLeadByte)
            {
                throw JITZ80Exception(JITZ80ExceptionCode::NotInstructionLeadByte, (int)z80InstructionAddress);
            }

            return (FnCallCompiledInstructions)mapping.GetBufferAddress();
        }

        void CompiledBuffer::EmitRoutine(const std::vector<std::unique_ptr<CompiledCode>>& instructions)
        {
            if (instructions.size() < 1) return;

            PVOID prologueAddress = EmitFunctionPrologue();

            for (const std::unique_ptr<CompiledCode>& compiledInstruction : instructions)
            {
                Emit(*compiledInstruction);
            }
            EmitFunctionEpilogue();

            // Point the first byte of the routine to the function prologue code.
            CompiledBufferInstructionAddress& mapping = this->mZ80AddressSpaceMapping[instructions[0]->GetZ80InstructionAddress()];
            mapping.SetBufferAddress(prologueAddress);
        }

        static LONG AllocationExceptionFilter(PEXCEPTION_POINTERS exceptionPointers)
        {
            if (exceptionPointers->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
            {
                PVOID allocationAddress = (PVOID)exceptionPointers->ExceptionRecord->ExceptionInformation[1];
                if (::VirtualAlloc(allocationAddress, 1u, MEM_COMMIT, PAGE_EXECUTE_READWRITE))
                {
                    return EXCEPTION_CONTINUE_EXECUTION;
                }
            }

            return EXCEPTION_CONTINUE_SEARCH;
        }

    } // namespace Emulator
} // namespace JITZ80Lib
