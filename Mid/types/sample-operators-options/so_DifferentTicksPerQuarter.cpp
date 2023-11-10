#include "so_DifferentTicksPerQuarter.hpp"

#include "../sample-options/TicksPerQuarter.hpp"
#include "../tools/Exceptions.hpp"

namespace MID
{
	namespace SampleOperators
	{
		UINT_16 mergeTicksPerQuarter(UINT_16 ticksPerQuarter1, UINT_16 ticksPerQuarter2, DifferentTicksPerQuarter differentTicksPerQuarter, std::string const& callingOperatorName)
		{
			if (ticksPerQuarter1 == ticksPerQuarter2) {
				return ticksPerQuarter1;
			} else {
				if (differentTicksPerQuarter == DifferentTicksPerQuarter::keepFirst) {
					return ticksPerQuarter1;
				} else if (differentTicksPerQuarter == DifferentTicksPerQuarter::keepSecond) {
					return ticksPerQuarter2;
				} else if (differentTicksPerQuarter == DifferentTicksPerQuarter::keepMinimum) {
					return (ticksPerQuarter1 <= ticksPerQuarter2) ? ticksPerQuarter1 : ticksPerQuarter2;
				} else if (differentTicksPerQuarter == DifferentTicksPerQuarter::keepMaximum) {
					return (ticksPerQuarter1 >= ticksPerQuarter2) ? ticksPerQuarter1 : ticksPerQuarter2;
				} else if (differentTicksPerQuarter == DifferentTicksPerQuarter::setTicksPerQuarterDefault) {
					return TicksPerQuarter::defaultValue;
				} else if (differentTicksPerQuarter == DifferentTicksPerQuarter::setTicksPerQuarterFine) {
					return TicksPerQuarter::fine;
				} else if (differentTicksPerQuarter == DifferentTicksPerQuarter::setTicksPerQuarterVeryFine) {
					return TicksPerQuarter::veryFine;
				} else if (differentTicksPerQuarter == DifferentTicksPerQuarter::error) {
					throw Exceptions::WrongSampleOperatorArgument(callingOperatorName, "sample2", "the number of ticks per quarter of the two samples are different.");
				} else {
					throw 0; // won't happen
				}
			}
		}
	};
}
