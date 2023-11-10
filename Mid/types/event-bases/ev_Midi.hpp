#ifndef MID_EV_MIDI_HPP
#define MID_EV_MIDI_HPP

#include "Event.hpp"
#include "../tools/Integers.hpp"

namespace MID
{
	// Base class for non-Meta MIDI events (these include note ON, pitch bend, control change...)
	class Event_Midi : public Event
	{
	public:
		// Constructor
		Event_Midi(Pos const& pos, UINT_4 type, UINT_4 channel);
		
		// Accessors
		UINT_4 getChannel() const;
		void setChannel(UINT_4 channel);

	protected:
		// Field
		UINT_4 channel_;
	};
}

#endif
