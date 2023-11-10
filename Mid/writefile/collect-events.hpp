#ifndef MID_COLLECT_EVENTS_HPP
#define MID_COLLECT_EVENTS_HPP

#include "../types/event-bases/Event.hpp"
#include "../types/containers/Sample.hpp"

#include <vector>

namespace MID
{
	// Subroutine used in the writeMidiFile() function
	std::vector<std::vector<Event*>> collectEventsInTracks_sorted_cleaned(
		Sample& sample,
		std::vector<Event*>& toBeDeleted
	);
}

#endif
