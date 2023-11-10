#ifndef MID_SO_DIFFERENT_TICKS_PER_QUARTER_HPP
#define MID_SO_DIFFERENT_TICKS_PER_QUARTER_HPP

#include "../tools/Integers.hpp"

#include <string>

namespace MID
{
	namespace SampleOperators
	{
		// Specifies what happens when an operation is performed between samples that don't have the same number of ticks per quarter.
		enum class DifferentTicksPerQuarter {
			// The precision of the first sample is retained.
			keepFirst,

			// The precision of the second sample is retained.
			keepSecond,

			// The precision of the sample with the lowest number of ticks per quarter is retained.
			keepMinimum,

			// The precision of the sample with the highest number of ticks per quarter is retained.
			keepMaximum,

			// The default precision 'TicksPerQuarter::defaultValue' is used.
			setTicksPerQuarterDefault,
			
			// The precision 'TicksPerQuarter::fine' is used.
			setTicksPerQuarterFine,

			// The precision 'TicksPerQuarter::veryFine' is used.
			setTicksPerQuarterVeryFine,

			// An exception is thrown if the two samples don't have the same number of ticks per quarter.
			error,

			// Same as 'keepMaximum'.
			defaultBehavior = keepMaximum
		};

		// Merge the ticks per quarter of two samples according to the option set by DifferentTicksPerQuarter.
		UINT_16 mergeTicksPerQuarter(UINT_16 ticksPerQuarter1, UINT_16 ticksPerQuarter2, DifferentTicksPerQuarter differentTicksPerQuarter, std::string const& callingOperatorName);
	};
}

#endif
