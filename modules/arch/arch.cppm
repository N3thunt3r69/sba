module;
#include <concepts>
#include <cstdint>

export module sba.arch;

export namespace SBA::Arch {

	template <typename T>
	concept Trait = requires {
		typename T::Reg;
		typename T::ABI;

		{ T::program_counter } -> std::convertible_to<uint16_t>;
		{ T::stack_pointer }   -> std::convertible_to<uint16_t>;
		{ T::status_flags }    -> std::convertible_to<uint16_t>;
		{ T::frame_pointer }   -> std::convertible_to<uint16_t>;
	};

}
