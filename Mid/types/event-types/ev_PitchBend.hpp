#ifndef MID_EV_PITCH_BEND_HPP
#define MID_EV_PITCH_BEND_HPP

#include "../event-bases/ev_Midi.hpp"
#include "../tools/Integers.hpp"
#include "../tools/Convert.hpp"

namespace MID
{
	// MIDI event representing a pitch bend (tuning change).
	// Different from class PitchBend, which allows to create Event_PitchBend events.
	class Event_PitchBend : public Event_Midi
	{
	public:
		// Constructors
		Event_PitchBend(Pos const& pos, float semitones, UINT_4 channel = 0);
		Event_PitchBend(Pos const& pos, UINT_7 semitones_MSB, UINT_7 semitones_LSB, UINT_4 channel = 0);
		Event_PitchBend(Pos const& pos, UINT_14 semitones_midi, UINT_4 channel = 0);

		// Named parameter idiom and subsequent modification
		Event_PitchBend& channel(UINT_4 channel);
		template<typename Numeric, typename dummy = enableIfNumeric<Numeric>>
		Event_PitchBend& semitones(Numeric semitones)
		{
			semitones_ = (float)semitones;
			UINT_16 value_14bits = Convert::PitchBend::floatSemitones_to_midi(semitones_);
			semitones_MSB_ = value_14bits >> 7;
			semitones_LSB_ = value_14bits & 0b1111111u;
			return *this;
		}
		
		// Accessors
		float semitones() const;
		UINT_8 semitones_MSB() const;
		UINT_8 semitones_LSB() const;
		
		// Implementations
		virtual bool isNoteOn() const override;
		virtual bool isNoteOff() const override;
		virtual std::shared_ptr<Event> clone() const override;
		virtual void writeToFile(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_8 lastEventStatusByte) const override;
	
	private:
		// Fields
		float semitones_;
		UINT_8 semitones_MSB_;
		UINT_8 semitones_LSB_;
	};
}

#endif
