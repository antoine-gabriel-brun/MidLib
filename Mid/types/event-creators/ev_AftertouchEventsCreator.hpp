#ifndef MID_EV_AFTERTOUCH_EVENTS_CREATOR_HPP
#define MID_EV_AFTERTOUCH_EVENTS_CREATOR_HPP

#include "../event-types/ev_ChannelAftertouch.hpp"
#include "../event-types/ev_KeyAftertouch.hpp"
#include "../time/Pos.hpp"
#include "../tools/Integers.hpp"
#include "../tools/Convert.hpp"

namespace MID
{
	// Aftertouch: Class for aftertouch events creation.
	// This class can create either ChannelAftertouch or KeyAftertouch events.
	// Different from classes Event_ChannelAftertouch and Event_KeyAftertouch, which represent MIDI events.
	// Alias name: Aftertouch = AftertouchEventsCreator.
	class AftertouchEventsCreator
	{
	public:
		// Constructor
		template<typename PosOrNumeric, typename dummy = enableIfPosOrNumeric<PosOrNumeric>>
		AftertouchEventsCreator(PosOrNumeric const& pos, UINT_4 channel = 0) :
			pos_(Pos(pos)),
			channel_(channel)
		{ }
		
		// Get event
		Event_ChannelAftertouch fromMidi(UINT_7 pressure)
		{
			return Event_ChannelAftertouch(pos_, pressure, channel_);
		}
		Event_KeyAftertouch fromMidi(UINT_7 pitch, UINT_7 pressure)
		{
			return Event_KeyAftertouch(pos_, pitch, pressure, channel_);
		}
		template<typename Numeric, typename dummy = enableIfNumeric<Numeric>>
		Event_ChannelAftertouch fromFloat(Numeric pressure01)
		{
			return fromMidi(Convert::Aftertouch::float_to_midi((float)pressure01));
		}
		template<typename Numeric, typename dummy = enableIfNumeric<Numeric>>
		Event_KeyAftertouch fromFloat(UINT_7 pitch, Numeric pressure01)
		{
			return fromMidi(pitch, Convert::Aftertouch::float_to_midi((float)pressure01));
		}
		
		// Default getters
		template<typename Numeric, typename dummy = enableIfNumeric<Numeric>>
		Event_ChannelAftertouch operator()(Numeric pressure01)
		{
			return fromFloat(pressure01);
		}
		template<typename Numeric, typename dummy = enableIfNumeric<Numeric>>
		Event_KeyAftertouch operator()(UINT_7 pitch, Numeric pressure01)
		{
			return fromFloat(pitch, pressure01);
		}
	
	protected:
		// Fields
		Pos pos_;
		UINT_4 channel_;
	};

	typedef AftertouchEventsCreator Aftertouch;
}

#endif
