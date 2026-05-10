module;
#include <cstdint>
#include <vector>
#include <functional>
#include <unordered_map>

export module sba.ir.context;

import sba.ir.operation;

export namespace SBA::IR {

	struct Hasher {
		std::size_t operator()(const MemoryB& m) const noexcept {
			uint64_t combined = (static_cast<uint64_t>(m.base.raw) << 32) |
								 static_cast<uint32_t>(m.displacement);
			return std::hash<uint64_t>{}(combined);
		}

		std::size_t operator()(const MemorySIB& m) const noexcept {
			auto combine = [](std::size_t& seed, uint64_t val) {
				std::hash<uint64_t> hasher;
				seed ^= hasher(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			};

			uint32_t meta = (static_cast<uint32_t>(m.length) << 16) |
							(static_cast<uint32_t>(m.segment) << 8) |
							 m.scale;

			uint64_t h1 = (static_cast<uint64_t>(meta) << 32) | m.index.raw;
			uint64_t h2 = (static_cast<uint64_t>(m.base.raw) << 32) |
						   static_cast<uint32_t>(m.displacement);

			std::size_t seed = 0;
			combine(seed, h1);
			combine(seed, h2);
			return seed;
		}
	};

	struct Context {
		std::vector<MicroOp>   uops;
		std::vector<uint32_t>  imm32;
		std::vector<uint64_t>  imm64;
		std::vector<MemoryB>   memb;
		std::vector<MemorySIB> memsib;

		std::unordered_map<uint32_t, uint32_t>          ref_imm32;
		std::unordered_map<uint64_t, uint32_t>          ref_imm64;
		std::unordered_map<MemoryB, uint32_t, Hasher>   ref_memb;
		std::unordered_map<MemorySIB, uint32_t, Hasher>	ref_memsib;
	};

}
