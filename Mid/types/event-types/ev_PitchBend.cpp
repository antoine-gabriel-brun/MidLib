#include "ev_PitchBend.hpp"

#include "../tools/Exceptions.hpp"
#include "../tools/Convert.hpp"

namespace MID
{
	Event_PitchBend::Event_PitchBend(Pos const& pos, float semitones, UINT_4 channel) :
		Event_Midi(Pos(pos), 0xEu, channel),
		semitones_(semitones)
	{
		UINT_16 value_14bits = Convert::PitchBend::floatSemitones_to_midi(semitones);
		semitones_MSB_ = value_14bits >> 7;
		semitones_LSB_ = value_14bits & 0b1111111u;
	}
	Event_PitchBend::Event_PitchBend(Pos const& pos, UINT_7 semitones_MSB, UINT_7 semitones_LSB, UINT_4 channel) :
		Event_Midi(Pos(pos), 0xEu, channel),
		semitones_MSB_(semitones_MSB),
		semitones_LSB_(semitones_LSB)
	{
		UINT_16 value_14bits = ((UINT_8)semitones_MSB << 7) | (UINT_8)semitones_LSB;
		semitones_ = Convert::PitchBend::midi_to_floatSemitones(value_14bits);
	}
	Event_PitchBend::Event_PitchBend(Pos const& pos, UINT_14 semitones_midi, UINT_4 channel) :
		Event_Midi(Pos(pos), 0xEu, channel),
		semitones_MSB_((UINT_16)semitones_midi >> 7),
		semitones_LSB_((UINT_16)semitones_midi & 0b1111111u)
	{
		semitones_ = Convert::PitchBend::midi_to_floatSemitones(semitones_midi);
	}

	Event_PitchBend& Event_PitchBend::channel(UINT_4 channel)
	{
		setChannel(channel);
		return *this;
	}

	float Event_PitchBend::semitones() const
	{
		return semitones_;
	}
	UINT_8 Event_PitchBend::semitones_MSB() const
	{
		return semitones_MSB_;
	}
	UINT_8 Event_PitchBend::semitones_LSB() const
	{
		return semitones_LSB_;
	}

	bool Event_PitchBend::isNoteOn() const
	{
		return false;
	}
	bool Event_PitchBend::isNoteOff() const
	{
		return false;
	}
	std::shared_ptr<Event> Event_PitchBend::clone() const
	{
		return std::make_shared<Event_PitchBend>(*this);
	}
	void Event_PitchBend::writeToFile(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_8 lastEventStatusByte) const
	{
		Event::writeEventToFile_2bytesAfterStatus_canUseRunningStatus(fileWriter, deltaTicks,
			semitones_LSB_,
			semitones_MSB_,
			lastEventStatusByte
		);
	}
}

