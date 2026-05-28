module;
#include <cstdint>
#include <vector>

export module sba.ir.stream;

import sba.ir.operation;

export namespace SBA::IR {

	struct IRStream {
		std::vector<uint8_t>   raw;
		std::vector<uint32_t>  imm32;
		std::vector<uint64_t>  imm64;
		std::vector<Memory>    mem;
		std::vector<MemorySIB> memsib;
	};

}
