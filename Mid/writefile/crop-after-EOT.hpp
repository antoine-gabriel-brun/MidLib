#ifndef MID_CROP_AFTER_EOT_HPP
#define MID_CROP_AFTER_EOT_HPP

#include "../types/event-types/ev_EndOfTrack.hpp"

#include <vector>

namespace MID
{
	// Subroutine used in the writeMidiFile() function.
	// Events should be sorted chronologically and cleaned.
	void cropAfterEOT(
		std::vector<std::vector<Event*>>& events,
		Event_EndOfTrack const& endOfTrack,
		std::vector<Event*>& toBeDeleted
	);
}

#endif
