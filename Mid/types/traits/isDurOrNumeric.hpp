#ifndef MID_IS_DUR_OR_NUMERIC_HPP
#define MID_IS_DUR_OR_NUMERIC_HPP

#include "../time/Dur.hpp"
#include "isNumeric.hpp"

#include <type_traits>

namespace MID
{
	// 'type_is_Dur_or_numeric' checks at compile time if a type is Dur or a valid numeric type.
	template<typename T>
	constexpr bool type_is_Dur_or_numeric =
	    std::is_same<T, Dur>() // Dur: ok
		|| type_is_numeric<T>; // numbers defined by 'type_is_numeric': ok

	// 'enableIfDurOrNumeric<T>' enables a template definition only if T is Dur or a valid numeric type.
	template<typename T>
	using enableIfDurOrNumeric = std::enable_if_t<(
	    type_is_Dur_or_numeric<T>
	)>;
}

#endif
