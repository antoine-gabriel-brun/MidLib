#ifndef MID_DEAL_WITH_OVERLAPPING_NOTES_HPP
#define MID_DEAL_WITH_OVERLAPPING_NOTES_HPP

#include "../types/event-bases/Event.hpp"
#include "../types/sample-options/OverlappingNotes.hpp"

#include <vector>

namespace MID
{
	// Subroutine used in the writeMidiFile() function
	std::vector<Event*> dealWithOverlappingNotes(
		std::vector<Event*>& trackEvents,
		OverlappingNotes mode,
		std::vector<Event*>& toBeDeleted
	);
}

#endif
