module;
#include <array>
#include <cstdint>

export module sba.arch.x86_64;

import sba.arch;

export namespace SBA::Arch {

	struct X86_64 {
		enum class SegmentReg : uint8_t {
			CS, DS, SS, ES, FS, GS
		};

		#define REGISTER_LIST(REG) \
			REG(RAX,    8) \
			REG(RBX,    8) \
			REG(RCX,    8) \
			REG(RDX,    8) \
			REG(RSI,    8) \
			REG(RDI,    8) \
			REG(RBP,    8) \
			REG(RSP,    8) \
			REG(R8,     8) \
			REG(R9,     8) \
			REG(R10,    8) \
			REG(R11,    8) \
			REG(R12,    8) \
			REG(R13,    8) \
			REG(R14,    8) \
			REG(R15,    8) \
			REG(RIP,    8) \
			REG(RFLAGS, 8) \
			REG(CR0,    8) \
			REG(CR2,    8) \
			REG(CR3,    8) \
			REG(CR4,    8) \
			REG(CR8,    8) \
			REG(DR0,    8) \
			REG(DR1,    8) \
			REG(DR2,    8) \
			REG(DR3,    8) \
			REG(DR4,    8) \
			REG(DR5,    8) \
			REG(DR6,    8) \
			REG(DR7,    8) \
			REG(GDTR,   10) \
			REG(IDTR,   10) \
			REG(LDTR,   2) \
			REG(TR,     2) \
			REG(MSW,    2) \
			REG(ST0,    10) \
			REG(ST1,    10) \
			REG(ST2,    10) \
			REG(ST3,    10) \
			REG(ST4,    10) \
			REG(ST5,    10) \
			REG(ST6,    10) \
			REG(ST7,    10) \
			REG(ZMM0,   64) \
			REG(ZMM1,   64) \
			REG(ZMM2,   64) \
			REG(ZMM3,   64) \
			REG(ZMM4,   64) \
			REG(ZMM5,   64) \
			REG(ZMM6,   64) \
			REG(ZMM7,   64) \
			REG(ZMM8,   64) \
			REG(ZMM9,   64) \
			REG(ZMM10,  64) \
			REG(ZMM11,  64) \
			REG(ZMM12,  64) \
			REG(ZMM13,  64) \
			REG(ZMM14,  64) \
			REG(ZMM15,  64) \
			REG(ZMM16,  64) \
			REG(ZMM17,  64) \
			REG(ZMM18,  64) \
			REG(ZMM19,  64) \
			REG(ZMM20,  64) \
			REG(ZMM21,  64) \
			REG(ZMM22,  64) \
			REG(ZMM23,  64) \
			REG(ZMM24,  64) \
			REG(ZMM25,  64) \
			REG(ZMM26,  64) \
			REG(ZMM27,  64) \
			REG(ZMM28,  64) \
			REG(ZMM29,  64) \
			REG(ZMM30,  64) \
			REG(ZMM31,  64) \
			REG(K0,     8) \
			REG(K1,     8) \
			REG(K2,     8) \
			REG(K3,     8) \
			REG(K4,     8) \
			REG(K5,     8) \
			REG(K6,     8) \
			REG(K7,     8)

		enum class Reg : uint16_t {
			#define DEF_REG_ENUM(name, size) name,
			REGISTER_LIST(DEF_REG_ENUM)
			#undef DEF_REG_ENUM
		};

		static constexpr uint16_t length(Reg r) noexcept {
			switch (r) {
				#define DEF_REG_LEN(name, size) case Reg::name: return size;
				REGISTER_LIST(DEF_REG_LEN)
				#undef DEF_REG_LEN
				default: return 0;
			}
		}

		static constexpr Reg program_counter = Reg::RIP;
		static constexpr Reg stack_pointer   = Reg::RSP;
		static constexpr Reg frame_pointer   = Reg::RBP;
		static constexpr Reg status_flags    = Reg::RFLAGS;

		struct ABI {
			struct SystemV {
				static constexpr std::array arg_registers = {
					Reg::RDI, Reg::RSI, Reg::RDX, Reg::RCX,
					Reg::R8, Reg::R9
				};

				static constexpr std::array callee_saved_registers = {
					Reg::RBX, Reg::RSP, Reg::RBP,
					Reg::R12, Reg::R13, Reg::R14, Reg::R15
				};

				static constexpr std::array return_registers = {
					Reg::RAX, Reg::RDX
				};
			};
		};
	};

}
