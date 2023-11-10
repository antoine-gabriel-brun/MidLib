#include "ev_NoteOnOffEventsCreator.hpp"

namespace MID
{
	NoteOnOffEventsCreator& NoteOnOffEventsCreator::pitch(UINT_7 pitch)
	{
		pitch_ = pitch;
		return *this;
	}
	NoteOnOffEventsCreator& NoteOnOffEventsCreator::velocityMidi(UINT_7 velocity)
	{
		velocity_ = velocity;
		return *this;
	}
	NoteOnOffEventsCreator& NoteOnOffEventsCreator::channel(UINT_4 channel)
	{
		channel_ = channel;
		return *this;
	}

	Pos const& NoteOnOffEventsCreator::start() const
	{
		return start_;
	}
	Pos const& NoteOnOffEventsCreator::end() const
	{
		return end_;
	}
	UINT_7 NoteOnOffEventsCreator::pitch() const
	{
		return pitch_;
	}
	UINT_7 NoteOnOffEventsCreator::velocity() const
	{
		return velocity_;
	}
	UINT_4 NoteOnOffEventsCreator::channel() const
	{
		return channel_;
	}
	
	Dur NoteOnOffEventsCreator::calcDur() const
	{
		return end_ - start_;
	}
	Event_NoteOnOff* NoteOnOffEventsCreator::calcNoteOn_newRawPtr() const
	{
		return Event_NoteOnOff::on_newRawPtr(start_, pitch_, velocity_, channel_);
	}
	Event_NoteOnOff* NoteOnOffEventsCreator::calcNoteOff_newRawPtr() const
	{
		return Event_NoteOnOff::off_newRawPtr(end_, pitch_, channel_);
	}

	bool NoteOnOffEventsCreator::operator==(NoteOnOffEventsCreator const& other) const
	{
		return
			(start_    == other.start_   ) &&
			(end_      == other.end_     ) &&
			(pitch_    == other.pitch_   ) &&
			(velocity_ == other.velocity_) &&
			(channel_  == other.channel_ );
	}
	bool NoteOnOffEventsCreator::operator!=(NoteOnOffEventsCreator const& other) const
	{
		return
			(start_    != other.start_   ) ||
			(end_      != other.end_     ) ||
			(pitch_    != other.pitch_   ) ||
			(velocity_ != other.velocity_) ||
			(channel_  != other.channel_ );
	}
}
