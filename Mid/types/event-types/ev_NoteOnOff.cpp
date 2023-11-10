#include "ev_NoteOnOff.hpp"

#include "../tools/Convert.hpp"

namespace MID
{
	Event_NoteOnOff Event_NoteOnOff::on(Pos const& pos, UINT_7 pitch, UINT_7 velocity, UINT_4 channel)
	{
		return Event_NoteOnOff(
			pos,
			pitch,
			velocity,
			channel
		);
	}
	Event_NoteOnOff Event_NoteOnOff::off(Pos const& pos, UINT_7 pitch, UINT_4 channel)
	{
		return Event_NoteOnOff(
			pos,
			pitch,
			0u,
			channel
		);
	}
	Event_NoteOnOff* Event_NoteOnOff::on_newRawPtr(Pos const& pos, UINT_7 pitch, UINT_7 velocity, UINT_4 channel)
	{
		return new Event_NoteOnOff(
			pos,
			pitch,
			velocity,
			channel
		);
	}
	Event_NoteOnOff* Event_NoteOnOff::off_newRawPtr(Pos const& pos, UINT_7 pitch, UINT_4 channel)
	{
		return new Event_NoteOnOff(
			pos,
			pitch,
			0u,
			channel
		);
	}

	UINT_7 Event_NoteOnOff::pitch() const
	{
		return pitch_;
	}
	UINT_7 Event_NoteOnOff::velocity() const
	{
		return velocity_;
	}
	void Event_NoteOnOff::setPitch(UINT_7 pitch)
	{
		pitch_ = pitch;
	}
	void Event_NoteOnOff::setVelocity(UINT_7 velocity)
	{
		velocity_ = velocity;
	}
	
	bool Event_NoteOnOff::isNoteOn() const
	{
		return velocity_ != 0;
	}
	bool Event_NoteOnOff::isNoteOff() const
	{
		return velocity_ == 0;
	}
	std::shared_ptr<Event> Event_NoteOnOff::clone() const
	{
		return std::make_shared<Event_NoteOnOff>(*this);
	}
	void Event_NoteOnOff::writeToFile(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_8 lastEventStatusByte) const
	{
		Event::writeEventToFile_2bytesAfterStatus_canUseRunningStatus(fileWriter, deltaTicks,
			pitch_,
			velocity_,
			lastEventStatusByte
		);
	}
	
	Event_NoteOnOff::Event_NoteOnOff(Pos const& pos, UINT_7 pitch, UINT_7 velocity, UINT_4 channel) :
		Event_Midi(pos, 9u, channel),
		pitch_(pitch),
		velocity_(velocity)
	{ }
}
