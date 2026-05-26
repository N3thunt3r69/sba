module;
#include <cstdint>
#include <vector>

export module sba.ir.context;

import sba.ir.operation;

export namespace SBA::IR {

	struct Context {
		std::vector<uint8_t>   ir_stream;
		std::vector<uint32_t>  imm32;
		std::vector<uint64_t>  imm64;
		std::vector<Memory>    mem;
		std::vector<MemorySIB> memsib;
	};

}
