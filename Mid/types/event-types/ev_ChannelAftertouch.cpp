#include "ev_ChannelAftertouch.hpp"

namespace MID
{
	Event_ChannelAftertouch::Event_ChannelAftertouch(Pos const& pos, UINT_7 pressure, UINT_4 channel) :
		Event_Midi(pos, 13u, channel),
		pressure_(pressure)
	{ }

	Event_ChannelAftertouch& Event_ChannelAftertouch::channel(UINT_4 channel)
	{
		setChannel(channel);
		return *this;
	}
	Event_ChannelAftertouch& Event_ChannelAftertouch::pressureMidi(UINT_7 pressure)
	{
		pressure_ = pressure;
		return *this;
	}

	UINT_7 Event_ChannelAftertouch::pressure() const
	{
		return pressure_;
	}

	bool Event_ChannelAftertouch::isNoteOn() const
	{
		return false;
	}
	bool Event_ChannelAftertouch::isNoteOff() const
	{
		return false;
	}
	std::shared_ptr<Event> Event_ChannelAftertouch::clone() const
	{
		return std::make_shared<Event_ChannelAftertouch>(*this);
	}
	void Event_ChannelAftertouch::writeToFile(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_8 lastEventStatusByte) const
	{
		Event::writeEventToFile_1byteAfterStatus_canUseRunningStatus(fileWriter, deltaTicks,
			pressure_,
			lastEventStatusByte
		);
	}

}
