#include "compute-EOT.hpp"

#include <memory>

namespace MID
{
	// Helper functions
	namespace
	{
		Pos getMaxFromSorted(std::vector<std::vector<Event*>> const& events)
		{
			Pos maxPos = Pos(0.);

			// Run through the tracks
			for (std::vector<Event*> const& eventsInTrack : events) {
				// Contains something?
				if (!eventsInTrack.empty()) {
					// The last element in the sorted track is the one with the biggest pos
					Event& lastInTrack = *eventsInTrack.back();
					// Maximize
					maxPos = max(maxPos, lastInTrack.pos());
				}
			}

			// Return maximum position
			return maxPos;
		}
	}

	// Main function
	Event_EndOfTrack computeEndOfTrack(
		std::vector<std::vector<Event*>> const& events,
		SampleDur const& sampleDuration)
	{
		if (sampleDuration.isSpecified()) {
			// Specified duration
			return Event_EndOfTrack(sampleDuration.dur().toPos());
		} else {
			// Computed duration: max position of an event in all tracks
			return Event_EndOfTrack(getMaxFromSorted(events));
		}
	}
}
