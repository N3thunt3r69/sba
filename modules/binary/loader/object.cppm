module;
#include <cstdint>
#include <expected>
#include <optional>
#include <string>
#include <vector>

export module sba.binary.object;

export import sba.binary.types;
export import sba.framework;

export namespace SBA::Binary {

	class Object {
	public:
		Object() = default;
		~Object() = default;

		std::expected<void, Error> load(const std::string& path);

		Endian endian() const;
		std::optional<uint64_t> program_entry() const;
		const std::vector<Segment>& segments() const;
		const std::vector<Symbol>& symbols() const;
		const std::vector<Export>& exports() const;
		const std::vector<Import>& imports() const;
		const std::vector<Relocation>& relocations() const;

		std::optional<uint64_t> read(uint64_t addr, uint8_t width) const;

	private:
		Endian endian_;
		std::optional<uint64_t> program_entry_;
		std::vector<Segment> segments_;
		std::vector<Symbol> symbols_;
		std::vector<Export> exports_;
		std::vector<Import> imports_;
		std::vector<Relocation> relocations_;

		std::expected<void, Error> validate();
	};

}
