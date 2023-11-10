#ifndef MID_SO_DIFFERENT_SAMPLE_DURS_HPP
#define MID_SO_DIFFERENT_SAMPLE_DURS_HPP

#include "../sample-options/SampleDur.hpp"

#include <string>

namespace MID
{
	namespace SampleOperators
	{
		// Specifies what happens when an operation is performed between samples that don't have the same specified duration.
		enum class DifferentSampleDurs {
			// The resulting sample is set to an unspecified duration.
			setUnspecifiedDuration,

			// The duration of the first sample is retained.
			keepFirst,

			// The duration of the second sample is retained.
			keepSecond,

			// The duration of the shortest sample is retained.
			keepMinimum,

			// The duration of the longest sample is retained.
			keepMaximum,

			// An exception is thrown if the two samples don't have the same specified duration.
			error,

			// Same as 'keepMaximum'.
			defaultBehavior = keepMaximum
		};

		// Merge the durations of two samples according to the option set by DifferentSampleDurs.
		SampleDur mergeSampleDurs(SampleDur const& sampleDur1, SampleDur const& sampleDur2, DifferentSampleDurs differentSampleDurs, std::string const& callingOperatorName, UINT_16 ticksPerQuarter);
	};
}

#endif
