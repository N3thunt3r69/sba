module;
#include <cstdint>
#include <vector>
#include <unordered_map>

export module sba.lift.dedup;

import sba.ir.operation;
import sba.ir.stream;
import sba.ir.semantics;
import sba.util.container;

export namespace SBA::Lift {

	struct Hasher {
		const SBA::Util::PVector<uint8_t>* raw = nullptr;

		std::size_t operator()(const SBA::IR::Memory& m) const noexcept;
		std::size_t operator()(const SBA::IR::MemorySIB& m) const noexcept;

		std::size_t operator()(const SBA::IR::Operation& op) const noexcept;
		bool operator()(const SBA::IR::Operation& lhs,
						const SBA::IR::Operation& rhs) const noexcept;
	};

	struct IRCache {
		SBA::IR::IRStream& stream;

		SBA::Util::PMap<uint32_t, uint32_t> imm32;
		SBA::Util::PMap<uint64_t, uint32_t> imm64;
		SBA::Util::PMap<SBA::IR::Memory, uint32_t, Hasher> mem;
		SBA::Util::PMap<SBA::IR::MemorySIB, uint32_t, Hasher> memsib;
		SBA::Util::PMap<SBA::IR::Operation, uint32_t, Hasher, Hasher> op;

		IRCache(SBA::IR::IRStream& stream)
			: stream(stream),
			  op(Hasher{&stream.raw},
				 Hasher{&stream.raw}) {}
	};

}
