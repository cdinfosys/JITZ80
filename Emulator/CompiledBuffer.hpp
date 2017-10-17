#ifndef COMPILED_BUFFER_7AA7F9D73BD64406B9453602C4D623A8
#define COMPILED_BUFFER_7AA7F9D73BD64406B9453602C4D623A8 1

#include <cstdint>
#include <memory>
#include <vector>
#include <Windows.h>

#include "JITCompiler.hpp"
#include "SharedAddressAllocator.hpp"
#include "../Support/CommonStructures.hpp"


namespace JITZ80Lib
{
    namespace Emulator
    {
        class CompiledBuffer;
        class CompiledCode;
        class JITCompiler;

        /**
         * @brief Callback function to execute code in the compiled buffer.
         */
        typedef void (_cdecl *FnCallCompiledInstructions)();

        const int VIRTUAL_MEMORY_SIZE_BYTES = (16 * 1024 * 1024); ///< Size of the virtual memory block

        enum class CompiledBufferFlags
        {
            None                    = 0x0000,
            Allocated               = 0x0001,
            InstructionLeadByte     = 0x0002, // This is the first byte of an instruction.
        };

        inline CompiledBufferFlags operator & (const CompiledBufferFlags leftVal, const CompiledBufferFlags& rightVal)
        {
            unsigned int v1 = (unsigned int)leftVal;
            unsigned int v2 = (unsigned int)rightVal;
            return (CompiledBufferFlags)(v1 & v2);
        }

        inline CompiledBufferFlags operator | (const CompiledBufferFlags leftVal, const CompiledBufferFlags& rightVal)
        {
            unsigned int v1 = (unsigned int)leftVal;
            unsigned int v2 = (unsigned int)rightVal;
            return (CompiledBufferFlags)(v1 | v2);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        class CompiledBufferInstructionAddress
        {
            friend class CompiledBuffer;

            private:
                size_t mZ80InstructionLengthBytes; ///< Length of the Z80 instruction in bytes
                PVOID mBufferAddress; ///< Address of the instruction in the compiled buffer
                CompiledBufferFlags mFlags; ///< State flags

            public:
                /**
                 * @brief Construct a new item that links a Z80 instruction to a region in the buffer.
                 */
                inline CompiledBufferInstructionAddress();

            public:
                inline size_t GetZ80InstructionLengthBytes() const;
                inline PVOID GetBufferAddress() const;
                inline CompiledBufferFlags GetFlags() const;

            private:
                inline void SetFlags(CompiledBufferFlags flags);
                inline void SetBufferAddress(PVOID address);
                inline void SetZ80InstructionLengthBytes(size_t length);
        }; // class CompiledBufferInstructionAddress

        inline CompiledBufferInstructionAddress::CompiledBufferInstructionAddress()
            :   mZ80InstructionLengthBytes(0),
                mBufferAddress(nullptr),
                mFlags(CompiledBufferFlags::None)
        {

        }

        inline size_t CompiledBufferInstructionAddress::GetZ80InstructionLengthBytes() const
        {
            return this->mZ80InstructionLengthBytes;
        }

        inline PVOID CompiledBufferInstructionAddress::GetBufferAddress() const
        {
            return this->mBufferAddress;
        }

        inline CompiledBufferFlags CompiledBufferInstructionAddress::GetFlags() const
        {
            return this->mFlags;
        }

        inline void CompiledBufferInstructionAddress::SetFlags(CompiledBufferFlags flags)
        {
            this->mFlags = flags;
        }

        void CompiledBufferInstructionAddress::SetBufferAddress(PVOID address)
        {
            this->mBufferAddress = address;
        }

        void CompiledBufferInstructionAddress::SetZ80InstructionLengthBytes(size_t length)
        {
            this->mZ80InstructionLengthBytes = length;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @brief
         */
        class CompiledBuffer
        {
            private:
                std::vector<CompiledBufferInstructionAddress> mZ80AddressSpaceMapping; ///< Links each Z80 address 
                PVOID mVirtualMemoryBlockPtr; ///< Pointer to the block of virtual memory where the compiled code is stored.
                std::shared_ptr<SharedAddressAllocator> mAddressAllocatorPtr; ///< Object that allocates addresses in the virtual memory block for new routines.
                PVOID mBufferHeaderPtr; ///< Pointer to the area that holds the header of the buffer.
                CompiledBufferHeader* mCompiledBufferHeaderPtr; ///< Pointer to the header structure of the buffer.
                PVOID* mJumpTableBaseAddress; ///< Address of the jump table
                uint8_t* mZ80RAMBaseAddress; ///< Address of the Z80 RAM block
                std::shared_ptr<JITCompiler> mJITCompiler; ///< JIT compiler object;
                bool mOwnsVirtualMemory; ///< Flag to indicate that this object is the owner of the virtual memory block.

            private:
                /**
                 * @Constructor called by \c CreateClone()
                 */
                CompiledBuffer
                (
                    PVOID virtualMemoryBlockPtr, 
                    std::shared_ptr<SharedAddressAllocator> addressAllocatorPtr, 
                    std::shared_ptr<JITZ80Lib::Emulator::JITCompiler> jitCompilerPtr
                );

            public:
                /**
                 * @brief Construct a compiled buffer object.
                 */
                CompiledBuffer(std::shared_ptr<JITZ80Lib::Emulator::JITCompiler>& jitCompiler);

                /**
                 * @brief Copy constructor.
                 * @param src Object from where the values are copied.
                 */
                CompiledBuffer(const CompiledBuffer& src) = delete;

                /**
                 * @brief Move constructor.
                 * @param src Object from where the values are copied.
                 */
                CompiledBuffer(CompiledBuffer&& src);

                /**
                 * @brief Destructor
                 */
                virtual ~CompiledBuffer() noexcept;

            public:
                /**
                 * @brief Create a clone of the object that points to the same Z80 memory buffer and state object.
                 */
                std::unique_ptr<CompiledBuffer> CreateClone() const;

            public:
                /**
                 * @brief Copy assignment operator
                 * @param src Object from where the values are copied.
                 * @returns Returns a reference to this object.
                 */
                CompiledBuffer operator=(const CompiledBuffer& src) = delete;

                /**
                 * @brief Move assignment operator
                 * @param src Object from where the values are copied.
                 * @returns Returns a reference to this object.
                 */
                CompiledBuffer& operator=(CompiledBuffer&& src);

            public:
                /**
                 * @brief Get a pointer to the area in the virtual memory block where the machine memory is stored.
                 */
                inline PVOID GetMachineMemoryBlockPtr() const;

                /**
                 * @brief Get the current value of the program counter.
                 */
                inline uintptr_t GetProgramCounter() const;

                /**
                 * @brief Initialize the CPU to its initial state. This does not affect the decomiled buffer.
                 */
                inline void ResetCPUState();

                /**
                 * Emit the instructions of a routine into the buffer. The method encloses the routine with function prologue and eplioge code.
                 */
                void EmitRoutine(const std::vector<std::unique_ptr<CompiledCode>>& instructions);

                FnCallCompiledInstructions GetCompiledCodeCallback(uintptr_t z80InstructionAddress);

            protected:
                PVOID EmitFunctionPrologue();

                void EmitFunctionEpilogue();

                /**
                 * @brief Emit code into the buffer.
                 */
                void Emit(const CompiledCode& compiledCode);

            private:
                /**
                 * @brief Allocate space in the buffer for a compiled instruction or infrastructure block.
                 */
                PVOID AllocateBuffer(size_t bufferSizeBytes, bool padBuffer = false);

                /**
                 * @brief Helper for \c GetCompiledCodeCallback() to compile a routine before returning the pointer to the routine
                 */
                FnCallCompiledInstructions CompileCode(uintptr_t z80InstructionAddress);
        }; // class CompiledBuffer

        inline PVOID CompiledBuffer::GetMachineMemoryBlockPtr() const
        {
            return (PVOID)this->mCompiledBufferHeaderPtr->z80RAMBasePtr;
        }

        inline void CompiledBuffer::ResetCPUState()
        {
            memset(&(this->mCompiledBufferHeaderPtr->cpuState), 0, sizeof(this->mCompiledBufferHeaderPtr->cpuState));
            this->mCompiledBufferHeaderPtr->cpuState.StackPointer = 0xFFFF;
            this->mCompiledBufferHeaderPtr->cpuState.AF.Word = 0xFFFF;
            this->mCompiledBufferHeaderPtr->cpuState.AF1.Word = 0xFFFF;
        }

        inline uintptr_t CompiledBuffer::GetProgramCounter() const
        {
            return (this->mCompiledBufferHeaderPtr->cpuState.ProgramCounter & 0xFFFFu);
        }

    } // namespace Emulator
} // namespace JITZ80Lib

#endif // COMPILED_BUFFER_7AA7F9D73BD64406B9453602C4D623A8
