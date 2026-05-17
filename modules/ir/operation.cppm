module;
#include <array>
#include <cstdint>

export module sba.ir.operation;

export namespace SBA::IR {

	enum class OperationType : uint8_t {
		DATAFLOW,
		SYSCALL,
		FENCE,
		TRAP,
		HALT,
		NOP
	};

	enum class OperandType : uint8_t {
		REGISTER,
		IMMEDIATE,
		MEMORY_B,
		MEMORY_SIB
	};

	struct Operand {
		union {
			uint32_t raw;

			struct {
				uint32_t type   : 2;
				uint32_t length : 4;
				uint32_t id     : 16;
				uint32_t offset : 8;
				uint32_t        : 2;
			} reg;

			struct {
				uint32_t type   : 2;
				uint32_t length : 4;
				uint32_t index  : 26;
			} ref;
		};
	};

	struct Expression {
		uint16_t op;
		uint16_t num_operands;
	};

	struct MemoryB {
		Operand base;
		int32_t displacement;

		bool operator==(const MemoryB& obj) const {
			return base.raw == obj.base.raw &&
				   displacement == obj.displacement;
		}
	};

	struct MemorySIB {
		uint16_t length;
		uint8_t  segment;
		uint8_t  scale;
		Operand  index;
		Operand  base;
		int32_t  displacement;

		bool operator==(const MemorySIB& obj) const {
			return length == obj.length &&
				   segment == obj.segment &&
				   scale == obj.scale &&
				   index.raw == obj.index.raw &&
				   base.raw == obj.base.raw &&
				   displacement == obj.displacement;
		}
	};

	struct Operation {
		uint64_t type       : 8;
		uint64_t unit_index : 40;
		uint64_t unit_count : 8;
		uint64_t length     : 8;
	};

}
