#ifndef MID_EV_END_OF_TRACK_HPP
#define MID_EV_END_OF_TRACK_HPP

#include "../event-bases/ev_Meta.hpp"

namespace MID
{
	// MIDI event that indicates the time position of the end of a MIDI track.
	class Event_EndOfTrack : public Event_Meta
	{
	public:
		// Constructor
		explicit Event_EndOfTrack(Pos const& pos);

		// Implementations
		virtual std::shared_ptr<Event> clone() const override;
		virtual void writeToFile(FileWriter& fileWriter, UINT_28 deltaTicks) const override;
	};
}

#endif
