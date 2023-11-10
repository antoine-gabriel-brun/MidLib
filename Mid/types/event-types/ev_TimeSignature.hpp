#ifndef MID_EV_TIME_SIGNATURE_HPP
#define MID_EV_TIME_SIGNATURE_HPP

#include "../event-bases/ev_Meta.hpp"
#include "../sample-options/TimeSignature.hpp"
#include "../tools/Integers.hpp"

namespace MID
{
	// MIDI event that indicates the time signature of a MIDI sample.
	// Different from class TimeSignature, which represents a logical value.
	class Event_TimeSignature : public Event_Meta
	{
	public:
		// Constructors and creator
		Event_TimeSignature(UINT_8 num, UINT_8 denom_4isQuarter_8isEighth_etc, UINT_8 midiTicksPerMetroTick = 96u, Pos const& pos = Pos(0.));
		Event_TimeSignature(TimeSignature const& timeSignature, UINT_8 midiTicksPerMetroTick = 96u, Pos const& pos = Pos(0.));
		static Event_TimeSignature createDefault();
		
		// Accessors
		UINT_8 num() const;
		UINT_8 denom_4isQuarter_8isEighth_etc() const;
		UINT_8 denom_midiEncoding() const;
		UINT_8 midiTicksPerMetroTick() const;

		// Implementations
		virtual std::shared_ptr<Event> clone() const override;
		virtual void writeToFile(FileWriter& fileWriter, UINT_28 deltaTicks) const override;

	private:
		// Fields
		TimeSignature timeSignature_;
		UINT_8 denom_midiEncoding_;
		UINT_8 midiTicksPerMetroTick_;
	};
}

#endif
