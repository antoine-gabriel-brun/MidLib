#ifndef MID_EV_PITCH_BEND_EVENTS_CREATOR_HPP
#define MID_EV_PITCH_BEND_EVENTS_CREATOR_HPP

#include "../event-types/ev_PitchBend.hpp"
#include "../time/Pos.hpp"
#include "../tools/Integers.hpp"
#include "../traits/isNumeric.hpp"

namespace MID
{
	// PitchBend: Class for pitch bend creation.
	// Different from class Event_PitchBend, which represents a MIDI event.
	// Alias name: PitchBend = PitchBendEventsCreator.
	class PitchBendEventsCreator
	{
	public:
		// Constructor
		template<typename PosOrNumeric, typename dummy = enableIfPosOrNumeric<PosOrNumeric>>
		PitchBendEventsCreator(PosOrNumeric const& pos, UINT_4 channel = 0) :
			pos_(Pos(pos)),
			channel_(channel)
		{ }
		
		// Get event
		Event_PitchBend fromMidi(UINT_14 value)
		{
			return Event_PitchBend(pos_, value, channel_);
		}
		template<typename Numeric, typename dummy = enableIfNumeric<Numeric>>
		Event_PitchBend fromFloat(Numeric semitones)
		{
			return Event_PitchBend(pos_, (float)semitones, channel_);
		}
		
		// Default getters
		template<typename Numeric, typename dummy = enableIfNumeric<Numeric>>
		Event_PitchBend operator()(Numeric semitones)
		{
			return fromFloat((float)semitones);
		}
		Event_PitchBend operator()()
		{
			return fromMidi(0b1000000'0000000);
		}

	protected:
		// Fields
		Pos pos_;
		UINT_4 channel_;
	};

	typedef PitchBendEventsCreator PitchBend;
}

#endif
