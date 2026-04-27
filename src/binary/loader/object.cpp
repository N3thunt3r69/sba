module;
#include <algorithm>
#include <cstdint>
#include <expected>
#include <optional>
#include <llvm/Object/Binary.h>
#include <llvm/Object/ObjectFile.h>
#include <llvm/Support/Error.h>

module sba.binary.object;

import sba.binary.elf;

namespace SBA::Binary {

	static thread_local const Segment* tls_last_segment = nullptr;

	static const Segment* find_segment(const Object* obj, uint64_t addr)
	{
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

	std::expected<void, Error> Object::load(const std::string& path)
	{
		auto bin = llvm::object::createBinary(path);
		if (!bin) {
			llvm::consumeError(bin.takeError());
			return std::unexpected(Error::INVALID_FORMAT);
		}

		if (auto* obj = llvm::dyn_cast<llvm::object::ObjectFile>(bin.get().getBinary())) {
			if (obj->isELF()) {
				if (auto err = parse_elf(obj, endian_, entry_, segments_,
										 symbols_, exports_, imports_, relocs_);
				!err)
					return err;
				return validate();
			}
		}

		return std::unexpected(Error::INVALID_FORMAT);
	}

	std::optional<uint64_t> Object::read(uint64_t addr, uint8_t width) const
	{
		const Segment* seg = find_segment(this, addr);

		if (!seg) [[unlikely]]
			return std::nullopt;

		return seg->read(addr, width, endian_);
	}

	std::expected<void, Error> Object::validate() {
		auto& segs = segments_;

		std::sort(segs.begin(), segs.end(), [](const auto& a, const auto& b) {
			return a.address < b.address;
		});

		for (size_t i = 0; i + 1 < segs.size(); ++i)
			if (segs[i].address + segs[i].size > segs[i + 1].address) [[unlikely]]
				return std::unexpected(Error::INVALID_SEGMENT);

		return {};
	}

}
