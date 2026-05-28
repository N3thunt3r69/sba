module;
#include <cstdint>
#include <vector>
#include <functional>

module sba.lift.dedup;

namespace SBA::Lift {

	inline void hash_combine(std::size_t& seed, uint64_t val) noexcept
	{
		if constexpr(sizeof(std::size_t) == 8) {
			seed ^= std::hash<uint64_t>{}(val) + 0x9e3779b97f4a7c15ULL +
					(seed << 6) + (seed >> 2);
		} else {
			seed ^= std::hash<uint64_t>{}(val) + 0x9e3779b9U +
					(seed << 6) + (seed >> 2);
		}
	}

	std::size_t Hasher::hash(const SBA::IR::Memory& m) noexcept
	{
		std::size_t seed = 0;
		hash_combine(seed, (static_cast<uint64_t>(m.base.raw) << 32) |
						   static_cast<uint64_t>(m.displacement));
		return seed;
	}

	std::size_t Hasher::hash(const SBA::IR::MemorySIB& m) noexcept
	{
		std::size_t seed = 0;
		hash_combine(seed, (static_cast<uint64_t>(m.length) << 48)   |
						   (static_cast<uint64_t>(m.segment) << 40)  |
						   (static_cast<uint64_t>(m.scale) << 32)    |
						   (static_cast<uint64_t>(m.displacement)));
		hash_combine(seed, (static_cast<uint64_t>(m.base.raw) << 32) |
						   (static_cast<uint64_t>(m.index.raw)));
		return seed;
	}

	std::size_t Hasher::operator()(const SBA::IR::Memory& m) const noexcept
	{
		return hash(m);
	}

	std::size_t Hasher::operator()(const SBA::IR::MemorySIB& m) const noexcept
	{
		return hash(m);
	}

	std::size_t Hasher::operator()(const SBA::IR::Operation& op) const noexcept
	{
		std::size_t seed = 0;
		hash_combine(seed, op.type);
		hash_combine(seed, op.length);

		uint32_t index = op.index;
		for (uint32_t i = 0; i < op.count; ++i) {
			auto opcode = static_cast<SBA::IR::Operator>((*raw)[index]);
			auto count = sizeof(SBA::IR::Operand) *
						 (1 + SBA::IR::arity(opcode)) + 1;

			for (uint32_t j = 0; j < count; ++j)
				hash_combine(seed, (*raw)[index + j]);

			index += count;
		}
		return seed;
	}

	bool Hasher::operator()(
		const SBA::IR::Operation& lhs,
		const SBA::IR::Operation& rhs) const noexcept
	{
		if (lhs.type != rhs.type || lhs.length != rhs.length ||
			lhs.count != rhs.count)
				return false;

		uint32_t l_index = lhs.index;
		uint32_t r_index = rhs.index;

		for (uint32_t i = 0; i < lhs.count; ++i) {
			auto l_opcode = static_cast<SBA::IR::Operator>((*raw)[l_index]);
			auto r_opcode = static_cast<SBA::IR::Operator>((*raw)[r_index]);
			auto count = sizeof(SBA::IR::Operand) *
						 (1 + SBA::IR::arity(l_opcode)) + 1;
			if (l_opcode != r_opcode)
				return false;

			for (uint32_t j = 0; j < count; ++j) {
				if ((*raw)[l_index + j] != (*raw)[r_index + j])
					return false;
			}

			l_index += count;
			r_index += count;
		}
		return true;
	}

}
