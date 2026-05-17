module;
#include <cstdint>
#include <utility>
#include <tuple>
#include <string>

export module sba.ir.semantics;

export namespace SBA::IR {

	#define OPERATOR_LIST(OP) \
		/* State */ \
		OP(VALUE,		1,	"value") \
		OP(CLOBBERED,	0,	"clobbered") \
		OP(UNDEFINED,	0,	"undefined") \
		/* Conditional Logic */ \
		OP(IF,			3,	"if") \
		/* Arithmetic: Basic */ \
		OP(CMP,			2,	"cmp") \
		OP(ADD,			2,	"add") \
		OP(SUB,			2,	"sub") \
		OP(MUL,			2,	"mul") \
		OP(DIV,			2,	"div") \
		OP(UDIV,		2,	"udiv") \
		OP(MOD,			2,	"mod") \
		OP(UMOD,		2,	"umod") \
		OP(NEG,			1,	"neg") \
		OP(ABS,			1,	"abs") \
		OP(MIN,			2,	"min") \
		OP(MAX,			2,	"max") \
		OP(MADD,		3,	"madd") \
		OP(MSUB,		3,	"msub") \
		OP(ADDC,		3,	"addc") \
		OP(SUBC,		3,	"subc") \
		OP(MULH,		2,	"mulh") \
		OP(UMULH,		2,	"umulh") \
		/* Arithmetic: Floating-Point */ \
		OP(FADD,		2,	"fadd") \
		OP(FSUB,		2,	"fsub") \
		OP(FMUL,		2,	"fmul") \
		OP(FDIV,		2,	"fdiv") \
		OP(FNEG,		1,	"fneg") \
		OP(FMADD,		3,	"fmadd") \
		OP(FMSUB,		3,	"fmsub") \
		/* Bitwise: Basic */ \
		OP(AND,			2,	"and") \
		OP(OR,			2,	"or") \
		OP(XOR,			2,	"xor") \
		OP(NOT,			1,	"not") \
		OP(SHL,			2,	"shl") \
		OP(SHR,			2,	"shr") \
		OP(SAR,			2,	"sar") \
		OP(ROL,			2,	"rol") \
		OP(ROR,			2,	"ror") \
		OP(FSHL,		3,	"fshl") \
		OP(FSHR,		3,	"fshr") \
		/* Bitwise: Advanced */ \
		OP(REV,			1,	"rev") \
		OP(CLZ,			1,	"clz") \
		OP(CTZ,			1,	"ctz") \
		OP(CNT,			1,	"cnt") \
		OP(BFI,			3,	"bfi") \
		OP(BFX,			3,	"bfx") \
		OP(DIST,		2,	"dist") \
		/* Relations */ \
		OP(EQ,			2,	"eq") \
		OP(NE,			2,	"ne") \
		OP(LT,			2,	"lt") \
		OP(LTU,			2,	"ltu") \
		OP(LE,			2,	"le") \
		OP(LEU,			2,	"leu") \
		OP(GT,			2,	"gt") \
		OP(GTU,			2,	"gtu") \
		OP(GE,			2,	"ge") \
		OP(GEU,			2,	"geu") \
		/* Atomic */ \
		OP(CMPXCHG,		3,	"cmpxchg") \
		/* Conversion */ \
		OP(ZEXT,		1,	"zext") \
		OP(SEXT,		1,	"sext") \
		OP(TRUNC,		1,	"trunc")

	enum class Operator : uint16_t {
#define DEF_ENUM(name, arity_val, str) name,
		OPERATOR_LIST(DEF_ENUM)
#undef DEF_ENUM
	};

	constexpr uint8_t arity(Operator op) {
		switch (op) {
#define DEF_ARITY(name, arity_val, str) case Operator::name: return arity_val;
			OPERATOR_LIST(DEF_ARITY)
#undef DEF_ARITY
			default: return 0;
		}
	}

	std::string to_string(Operator op) {
		switch (op) {
#define DEF_STRING(name, arity_val, str) case Operator::name: return str;
			OPERATOR_LIST(DEF_STRING)
#undef DEF_STRING
			default: return "";
		}
	}

	template <typename T, typename... Args>
	T evaluate(Operator op, Args&&... args) {
		auto&& tuple = std::forward_as_tuple(std::forward<Args>(args)...);

		if constexpr (sizeof...(Args) == 0) {
			switch (op) {
				case Operator::CLOBBERED:	return T::clobbered();
				case Operator::UNDEFINED:	return T::undefined();
				default:					return T::undefined();
			}
		}
		else if constexpr (sizeof...(Args) == 1) {
			auto&& a = std::get<0>(tuple);
			switch (op) {
				case Operator::VALUE:		return a;
				case Operator::NEG:			return -a;
				case Operator::NOT:			return ~a;
				case Operator::ABS:			return T::abs(a);
				case Operator::REV:			return T::rev(a);
				case Operator::CLZ:			return T::clz(a);
				case Operator::CTZ:			return T::ctz(a);
				case Operator::CNT:			return T::cnt(a);
				case Operator::ZEXT:		return T::zext(a);
				case Operator::SEXT:		return T::sext(a);
				case Operator::TRUNC:		return T::trunc(a);
				case Operator::FNEG:		return T::fneg(a);
				default:					return T::undefined();
			}
		}
		else if constexpr (sizeof...(Args) == 2) {
			auto&& a = std::get<0>(tuple);
			auto&& b = std::get<1>(tuple);
			switch (op) {
				case Operator::ADD:			return a + b;
				case Operator::SUB:			return a - b;
				case Operator::MUL:			return a * b;
				case Operator::DIV:			return a / b;
				case Operator::MOD:			return a % b;
				case Operator::AND:			return a & b;
				case Operator::OR:			return a | b;
				case Operator::XOR:			return a ^ b;
				case Operator::EQ:			return a == b;
				case Operator::NE:			return a != b;
				case Operator::LT:			return a < b;
				case Operator::GT:			return a > b;
				case Operator::LE:			return a <= b;
				case Operator::GE:			return a >= b;
				case Operator::SHL:			return T::shl(a, b);
				case Operator::SHR:			return T::shr(a, b);
				case Operator::SAR:			return T::sar(a, b);
				case Operator::UDIV:		return T::udiv(a, b);
				case Operator::UMOD:		return T::umod(a, b);
				case Operator::MIN:			return T::min(a, b);
				case Operator::MAX:			return T::max(a, b);
				case Operator::MULH:		return T::mulh(a, b);
				case Operator::UMULH:		return T::umulh(a, b);
				case Operator::ROL:			return T::rol(a, b);
				case Operator::ROR:			return T::ror(a, b);
				case Operator::LTU:			return T::ltu(a, b);
				case Operator::LEU:			return T::leu(a, b);
				case Operator::GTU:			return T::gtu(a, b);
				case Operator::GEU:			return T::geu(a, b);
				case Operator::DIST:		return T::dist(a, b);
				case Operator::CMP:			return T::cmp(a, b);
				case Operator::FADD:		return T::fadd(a, b);
				case Operator::FSUB:		return T::fsub(a, b);
				case Operator::FMUL:		return T::fmul(a, b);
				case Operator::FDIV:		return T::fdiv(a, b);
				default:					return T::undefined();
			}
		}
		else if constexpr (sizeof...(Args) == 3) {
			auto&& a = std::get<0>(tuple);
			auto&& b = std::get<1>(tuple);
			auto&& c = std::get<2>(tuple);
			switch (op) {
				case Operator::IF:			return T::cond_if(a, b, c);
				case Operator::MADD:		return T::madd(a, b, c);
				case Operator::MSUB:		return T::msub(a, b, c);
				case Operator::ADDC:		return T::addc(a, b, c);
				case Operator::SUBC:		return T::subc(a, b, c);
				case Operator::FSHL:		return T::fshl(a, b, c);
				case Operator::FSHR:		return T::fshr(a, b, c);
				case Operator::BFI:			return T::bfi(a, b, c);
				case Operator::BFX:			return T::bfx(a, b, c);
				case Operator::CMPXCHG:		return T::cmpxchg(a, b, c);
				case Operator::FMADD:		return T::fmadd(a, b, c);
				case Operator::FMSUB:		return T::fmsub(a, b, c);
				default:					return T::undefined();
			}
		}
	}
}
