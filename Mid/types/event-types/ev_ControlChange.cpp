#include "ev_ControlChange.hpp"

namespace MID
{
	Event_ControlChange::Event_ControlChange(Pos const& pos, UINT_7 controller, UINT_7 value, UINT_4 channel) :
		Event_Midi(pos, 0xBu, channel),
		controller_(controller),
		value_(value)
	{ }
	Event_ControlChange::Event_ControlChange(double pos, UINT_7 controller, UINT_7 value, UINT_4 channel) :
		Event_Midi(Pos(pos), 0xBu, channel),
		controller_(controller),
		value_(value)
	{ }
	
	Event_ControlChange& Event_ControlChange::channel(UINT_4 channel)
	{
		setChannel(channel);
		return *this;
	}
	Event_ControlChange& Event_ControlChange::controller(UINT_7 controller)
	{
		controller_ = controller;
		return *this;
	}
	Event_ControlChange& Event_ControlChange::value(UINT_7 value)
	{
		value_ = value;
		return *this;
	}

	UINT_7 Event_ControlChange::controller() const
	{
		return controller_;
	}
	UINT_7 Event_ControlChange::value() const
	{
		return value_;
	}
	
	bool Event_ControlChange::isNoteOn() const
	{
		return false;
	}
	bool Event_ControlChange::isNoteOff() const
	{
		return false;
	}
	std::shared_ptr<Event> Event_ControlChange::clone() const
	{
		return std::make_shared<Event_ControlChange>(*this);
	}
	void Event_ControlChange::writeToFile(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_8 lastEventStatusByte) const
	{
		Event::writeEventToFile_2bytesAfterStatus_canUseRunningStatus(fileWriter, deltaTicks,
			controller_,
			value_,
			lastEventStatusByte
		);
	}
}

