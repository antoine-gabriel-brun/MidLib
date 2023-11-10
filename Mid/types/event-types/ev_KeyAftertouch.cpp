#include "ev_KeyAftertouch.hpp"

namespace MID
{
	Event_KeyAftertouch::Event_KeyAftertouch(Pos const& pos, UINT_7 pitch, UINT_7 pressure, UINT_4 channel) :
		Event_Midi(pos, 10u, channel),
		pitch_(pitch),
		pressure_(pressure)
	{ }

	Event_KeyAftertouch& Event_KeyAftertouch::channel(UINT_4 channel)
	{
		setChannel(channel);
		return *this;
	}
	Event_KeyAftertouch& Event_KeyAftertouch::pitch(UINT_7 pitch)
	{
		pitch_ = pitch;
		return *this;
	}
	Event_KeyAftertouch& Event_KeyAftertouch::pressureMidi(UINT_7 pressure)
	{
		pressure_ = pressure;
		return *this;
	}

	UINT_7 Event_KeyAftertouch::pitch() const
	{
		return pitch_;
	}
	UINT_7 Event_KeyAftertouch::pressure() const
	{
		return pressure_;
	}

	bool Event_KeyAftertouch::isNoteOn() const
	{
		return false;
	}
	bool Event_KeyAftertouch::isNoteOff() const
	{
		return false;
	}
	std::shared_ptr<Event> Event_KeyAftertouch::clone() const
	{
		return std::make_shared<Event_KeyAftertouch>(*this);
	}
	void Event_KeyAftertouch::writeToFile(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_8 lastEventStatusByte) const
	{
		Event::writeEventToFile_2bytesAfterStatus_canUseRunningStatus(fileWriter, deltaTicks,
			pitch_,
			pressure_,
			lastEventStatusByte
		);
	}

}
