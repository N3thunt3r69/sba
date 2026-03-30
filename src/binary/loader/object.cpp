module;
#include <algorithm>
#include <cstdint>
#include <expected>
#include <optional>
#include <llvm/Support/Endian.h>

module sba.binary.object;

namespace SBA::Binary {

	static thread_local const Segment* tls_last_segment = nullptr;

	static const Segment* find_segment(const Object* obj, uint64_t addr) {
		if (tls_last_segment && tls_last_segment->contains(addr))
			return tls_last_segment;

		const auto& segs = obj->segments();
		auto it = std::lower_bound(segs.begin(), segs.end(), addr,
			[](const Segment& s, uint64_t val) {
				return s.address + s.size <= val;
			});

		if (it != segs.end() && it->contains(addr)) {
			tls_last_segment = &(*it);
			return tls_last_segment;
		}

		return nullptr;
	}

	template <typename T>
	static uint64_t read_scalar(const uint8_t* ptr, Endian e) {
		return (e == Endian::LITTLE) ?
			llvm::support::endian::read<T, llvm::endianness::little,
										llvm::support::unaligned>(ptr) :
			llvm::support::endian::read<T, llvm::endianness::big,
										llvm::support::unaligned>(ptr);
	}

	Object::~Object() {
		for (auto& seg : segments_)
			if (seg.bytes)
				delete[] seg.bytes;
	}

	std::expected<void, Error> Object::load(const std::string& path) {
		return {};
	}

	Endian Object::endian() const {
		return endian_;
	}

	std::optional<uint64_t> Object::program_entry() const {
		return program_entry_;
	}

	const std::vector<Segment>& Object::segments() const {
		return segments_;
	}

	const std::vector<Symbol>& Object::symbols() const {
		return symbols_;
	}

	const std::vector<Export>& Object::exports() const {
		return exports_;
	}

	const std::vector<Import>& Object::imports() const {
		return imports_;
	}

	const std::vector<Relocation>& Object::relocations() const {
		return relocations_;
	}

	std::optional<uint64_t> Object::read(uint64_t addr, uint8_t width) const {
		const Segment* seg = find_segment(this, addr);

		if (!seg) [[unlikely]]
			return std::nullopt;

		uint64_t offset = addr - seg->address;

		if (offset + width > seg->size) [[unlikely]]
			return std::nullopt;

		if (offset >= seg->file_size) [[unlikely]]
			return 0;

		if (!seg->bytes || offset + width > seg->file_size) [[unlikely]]
			return std::nullopt;

		const uint8_t* ptr = seg->bytes + offset;

		switch (width) {
			case 1: return *ptr;
			case 2: return read_scalar<uint16_t>(ptr, endian_);
			case 4: return read_scalar<uint32_t>(ptr, endian_);
			case 8: return read_scalar<uint64_t>(ptr, endian_);
			default: return std::nullopt;
		}
	}

	std::expected<void, Error> Object::validate() {
		auto& segs = segments_;

		std::sort(segs.begin(), segs.end(), [](const auto& a, const auto& b) {
			return a.address < b.address;
		});

		for (size_t i = 0; i + 1 < segs.size(); ++i)
			if (segs[i].address + segs[i].size > segs[i + 1].address) [[unlikely]]
				return std::unexpected(Error::OVERLAPPING_SEGMENTS);

		return {};
	}

}
