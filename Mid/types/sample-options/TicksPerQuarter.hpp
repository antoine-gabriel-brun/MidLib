#ifndef MID_TICKS_PER_QUARTER_HPP
#define MID_TICKS_PER_QUARTER_HPP

#include "../tools/Integers.hpp"

namespace MID
{
	// Useful values for a sample's ticks per quarter value
	namespace TicksPerQuarter
	{
		// Low accuracy
		static constexpr UINT_16 coarse = 16u;

		// Most commonly used value
		static constexpr UINT_16 defaultValue = 96u;

		// Both high accuracy and divisibility
		static constexpr UINT_16 fine = 1440u;

		// Both very high accuracy and divisibility
		static constexpr UINT_16 veryFine = 20160u;
	}
}

#endif
