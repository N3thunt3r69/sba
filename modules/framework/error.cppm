module;
#include <cstdint>

export module sba.framework.error;

export namespace SBA {

	enum class Error : uint8_t {
		INVALID_FORMAT,
		INVALID_SEGMENT,
		INVALID_SYMBOL,
		INVALID_RELOCATION,
		INVALID_ENTRY
	};

}
