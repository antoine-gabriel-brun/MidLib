#include "ev_Midi.hpp"

#include "../tools/Exceptions.hpp"

namespace MID
{
	namespace
	{
		UINT_8 makeStatusByte_canThrow(UINT_4 type, UINT_4 channel)
		{
			if (type >= 8u && type <= 14u) {
				return ((UINT_8)type << 4) | (UINT_8)channel;
			} else {
				throw Exceptions::WrongMidiEventTypeValue();
			}
		}
	}

	Event_Midi::Event_Midi(Pos const& pos, UINT_4 type, UINT_4 channel) :
		Event(pos, makeStatusByte_canThrow(type, channel)),
		channel_(channel)
	{ }
	
	UINT_4 Event_Midi::getChannel() const {
		return channel_;
	}
	void Event_Midi::setChannel(UINT_4 channel)
	{
		channel_ = channel;
	}
}
