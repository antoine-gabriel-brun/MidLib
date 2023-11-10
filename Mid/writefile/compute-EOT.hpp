#ifndef MID_COMPUTE_EOT_HPP
#define MID_COMPUTE_EOT_HPP

#include "../types/event-types/ev_EndOfTrack.hpp"
#include "../types/sample-options/SampleDur.hpp"

#include <vector>

namespace MID
{
	// Subroutine used in the writeMidiFile() function.
	// Events should be sorted chronologically.
	Event_EndOfTrack computeEndOfTrack(
		std::vector<std::vector<Event*>> const& events,
		SampleDur const& sampleDuration
	);
}

#endif
