#ifndef MID_TIME_SIGNATURE_HPP
#define MID_TIME_SIGNATURE_HPP

#include "../tools/Integers.hpp"

namespace MID
{
	// Logical time signature such as 4/4.
	// Different from class Event_TimeSignature, which represents a MIDI event.
	class TimeSignature
	{
	public:
		// Constructor and creator
		TimeSignature(UINT_8 num, UINT_8 denom_4isQuarter_8isEighth_etc);
		static TimeSignature createDefault();
		
		// Accessors
		UINT_8 num() const;
		UINT_8 denom_4isQuarter_8isEighth_etc() const;

		// Comparison operators
		bool operator==(TimeSignature const& other) const;
		bool operator!=(TimeSignature const& other) const;

	private:
		// Fields
		UINT_8 num_;
		UINT_8 denom_4isQuarter_8isEighth_etc_;
	};
}

#endif
