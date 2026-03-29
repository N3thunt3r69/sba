module;
#include <cstdint>

export module sba.framework.error;

export namespace SBA {

	enum class Error : uint8_t {
		LOAD_ERROR,
		INVALID_FORMAT,
	};

}
