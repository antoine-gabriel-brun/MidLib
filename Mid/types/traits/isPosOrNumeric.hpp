#ifndef MID_IS_POS_OR_NUMERIC_HPP
#define MID_IS_POS_OR_NUMERIC_HPP

#include "../time/Pos.hpp"
#include "isNumeric.hpp"

#include <type_traits>

namespace MID
{
	// 'type_is_Pos_or_numeric' checks at compile time if a type is Pos or a valid numeric type.
	template<typename T>
	constexpr bool type_is_Pos_or_numeric =
	    std::is_same<T, Pos>() // Pos: ok
		|| type_is_numeric<T>; // numbers defined by 'type_is_numeric': ok

	// 'enableIfPosOrNumeric<T>' enables a template definition only if T is Pos or a valid numeric type.
	template<typename T>
	using enableIfPosOrNumeric = std::enable_if_t<(
	    type_is_Pos_or_numeric<T>
	)>;
}

#endif
