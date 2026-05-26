module;
#include <cstdint>

export module sba.ir.operation;

export namespace SBA::IR {

	enum class OperationType : uint8_t {
		STORE,
		SYSCALL,
		FENCE,
		TRAP,
		HALT,
		NOP
	};

	enum class OperandType : uint8_t {
		REGISTER,
		IMMEDIATE,
		MEMORY,
		MEMORY_SIB
	};

	struct Operand {
		union {
			uint32_t raw;

			struct {
				uint32_t type      : 2;
				uint32_t lg_length : 4;
				uint32_t id        : 16;
				uint32_t offset    : 8;
				uint32_t           : 2;
			} reg;

			struct {
				uint32_t type      : 2;
				uint32_t lg_length : 4;
				uint32_t index     : 26;
			} ref;
		};
	};

	struct Memory {
		Operand base;
		int32_t displacement;

		bool operator==(const Memory& obj) const {
			return base.raw == obj.base.raw &&
				   displacement == obj.displacement;
		}
	};

	struct MemorySIB {
		uint16_t length;
		uint8_t  segment;
		uint8_t  scale;
		int32_t  displacement;
		Operand  index;
		Operand  base;

		bool operator==(const MemorySIB& obj) const {
			return length == obj.length &&
				   segment == obj.segment &&
				   scale == obj.scale &&
				   displacement == obj.displacement &&
				   index.raw == obj.index.raw &&
				   base.raw == obj.base.raw;
		}
	};

	struct Operation {
		uint64_t type   : 8;
		uint64_t length : 8;
		uint64_t index  : 40;
		uint64_t count  : 8;
	};

}
