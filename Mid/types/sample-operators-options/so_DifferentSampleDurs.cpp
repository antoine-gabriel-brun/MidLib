#include "so_DifferentSampleDurs.hpp"

#include "../tools/Exceptions.hpp"

namespace MID
{
	namespace SampleOperators
	{
		namespace
		{
			SampleDur mergeSampleDurs_bothSpecified(SampleDur const& sampleDur1, SampleDur const& sampleDur2, DifferentSampleDurs differentSampleDurs, std::string const& callingOperatorName, UINT_16 ticksPerQuarter)
			{
				// Get durations as numbers of MIDI ticks
				UINT_32 totalSpecifiedNbTicks1 = (UINT_32)round(sampleDur1.time() * ticksPerQuarter);
				UINT_32 totalSpecifiedNbTicks2 = (UINT_32)round(sampleDur2.time() * ticksPerQuarter);

				// Check for equality
				if (totalSpecifiedNbTicks1 == totalSpecifiedNbTicks2) {
					// Durations are equal (after conversion to MIDI ticks). Return the duration.
					return sampleDur1;
				} else {
					// Durations are different.
					if (differentSampleDurs == DifferentSampleDurs::setUnspecifiedDuration) {
						return SampleDur::unspecified();
					} else if (differentSampleDurs == DifferentSampleDurs::keepFirst) {
						return sampleDur1;
					} else if (differentSampleDurs == DifferentSampleDurs::keepSecond) {
						return sampleDur2;
					} else if (differentSampleDurs == DifferentSampleDurs::keepMinimum) {
						return (sampleDur1 <= sampleDur2) ? sampleDur1 : sampleDur2;
					} else if (differentSampleDurs == DifferentSampleDurs::keepMaximum) {
						return (sampleDur1 >= sampleDur2) ? sampleDur1 : sampleDur2;
					} else if (differentSampleDurs == DifferentSampleDurs::error) {
						throw Exceptions::WrongSampleOperatorArgument(callingOperatorName, "sample2", "the durations of the two samples are specified and different.");
					} else {
						throw 0; // won't happen
					}
				}
			}
		}

		SampleDur mergeSampleDurs(SampleDur const& sampleDur1, SampleDur const& sampleDur2, DifferentSampleDurs differentSampleDurs, std::string const& callingOperatorName, UINT_16 ticksPerQuarter)
		{
			// Are the durations specified?
			bool durSpecified1 = sampleDur1.isSpecified();
			bool durSpecified2 = sampleDur2.isSpecified();
			
			if (durSpecified1 && durSpecified2) {
				// Both durations are specified. Merge them according to 'differentSampleDurs'.
				return mergeSampleDurs_bothSpecified(sampleDur1, sampleDur2, differentSampleDurs, callingOperatorName, ticksPerQuarter);
			} else if (durSpecified1) {
				// Only the first duration is specified. Return it.
				return sampleDur1;
			} else if (durSpecified2) {
				// Only the second duration is specified. Return it.
				return sampleDur2;
			} else {
				// No duration is specified. Return an unspecified duration.
				return SampleDur::unspecified();
			}
		}
	};
}
