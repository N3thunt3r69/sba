module;
#include <cstdint>
#include <vector>
#include <unordered_map>

export module sba.lift.dedup;

import sba.ir.operation;
import sba.ir.stream;
import sba.ir.semantics;

export namespace SBA::Lift {

	struct Hasher {
		const std::vector<uint8_t>* raw = nullptr;

		static std::size_t hash(const SBA::IR::Memory& m) noexcept;
		static std::size_t hash(const SBA::IR::MemorySIB& m) noexcept;
		std::size_t operator()(const SBA::IR::Memory& m) const noexcept;
		std::size_t operator()(const SBA::IR::MemorySIB& m) const noexcept;

		std::size_t operator()(const SBA::IR::Operation& op) const noexcept;
		bool operator()(const SBA::IR::Operation& lhs,
						const SBA::IR::Operation& rhs) const noexcept;
	};

	struct IRCache {
		SBA::IR::IRStream& stream;

		std::unordered_map<uint32_t, uint32_t> imm32;
		std::unordered_map<uint64_t, uint32_t> imm64;
		std::unordered_map<SBA::IR::Memory, uint32_t, Hasher> mem;
		std::unordered_map<SBA::IR::MemorySIB, uint32_t, Hasher> memsib;
		std::unordered_map<SBA::IR::Operation, uint32_t, Hasher, Hasher> op;

		IRCache(SBA::IR::IRStream& stream)
			: stream(stream),
			  op(0, Hasher{&stream.raw},
					Hasher{&stream.raw}) {}
	};

}
