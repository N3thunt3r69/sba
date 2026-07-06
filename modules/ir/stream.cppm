module;
#include <cstdint>

export module sba.ir.stream;

import sba.ir.operation;
import sba.util.container;

export namespace SBA::IR {

	struct IRStream {
		SBA::Util::PVector<uint8_t>   raw;
		SBA::Util::PVector<uint32_t>  imm32;
		SBA::Util::PVector<uint64_t>  imm64;
		SBA::Util::PVector<Memory>    mem;
		SBA::Util::PVector<MemorySIB> memsib;
	};

}
