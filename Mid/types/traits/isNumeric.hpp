#ifndef MID_IS_NUMERIC_HPP
#define MID_IS_NUMERIC_HPP

#include <type_traits>

namespace MID
{
	// 'type_is_numeric' checks at compile time if a type is a valid numeric type.
	template<typename T>
	constexpr bool type_is_numeric =
		std::is_arithmetic<T>()         // float, double, char, unsigned int...: ok
		&& !std::is_same<T, bool>()     // bool: not ok
		&& !std::is_same<T, wchar_t>(); // wchar_t: not ok
	
	// 'enableIfNumeric<T>' enables a template definition only if T is a valid numeric type.
	template<typename T>
	using enableIfNumeric = std::enable_if_t<(
	    type_is_numeric<T>
	)>;
}

#endif
