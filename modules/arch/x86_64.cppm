module;
#include <array>
#include <cstdint>

export module sba.arch.x86_64;

import sba.arch;

export namespace SBA::Arch {

	struct X86_64 {
		enum class Reg : uint16_t {
			NONE,
			RAX, RBX, RCX, RDX, RSI, RDI, RBP, RSP,
			R8,  R9,  R10, R11, R12, R13, R14, R15,
			RIP,
			RFLAGS,
			CS, DS, SS, ES, FS, GS,
			CR0, CR2, CR3, CR4, CR8,
			DR0, DR1, DR2, DR3, DR4, DR5, DR6, DR7,
			GDTR, IDTR, LDTR, TR, MSW,
			ST0, ST1, ST2, ST3, ST4, ST5, ST6, ST7,
			ZMM0,  ZMM1,  ZMM2,  ZMM3,  ZMM4,  ZMM5,  ZMM6,  ZMM7,
			ZMM8,  ZMM9,  ZMM10, ZMM11, ZMM12, ZMM13, ZMM14, ZMM15,
			ZMM16, ZMM17, ZMM18, ZMM19, ZMM20, ZMM21, ZMM22, ZMM23,
			ZMM24, ZMM25, ZMM26, ZMM27, ZMM28, ZMM29, ZMM30, ZMM31,
			K0, K1, K2, K3, K4, K5, K6, K7
		};

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
