#ifndef MID_SAMPLE_DUR_HPP
#define MID_SAMPLE_DUR_HPP

#include "../time/Dur.hpp"

namespace MID
{
	// Duration of a MIDI sample (can be specified as duration, or unspecified)
	class SampleDur
	{
	public:
		// Creators
		static SampleDur unspecified();
		static SampleDur specified(Dur const& dur);
		static SampleDur specified(double dur);

		// Accessors
		bool isSpecified() const;
		Dur const& dur() const;
		double time() const;
		
		// Comparison operators
		bool operator==(SampleDur const& other) const;
		bool operator!=(SampleDur const& other) const;
		bool operator<(SampleDur const& other) const;
		bool operator<=(SampleDur const& other) const;
		bool operator>(SampleDur const& other) const;
		bool operator>=(SampleDur const& other) const;

	private:
		// Private costructors
		SampleDur();                    // create unspecified sample duration
		SampleDur(Dur const& duration); // create specified sample duration

		// Fields
		bool isSpecified_;
		Dur dur_;
	};

	// Min and max (free functions)
	SampleDur min(SampleDur const& a, SampleDur const& b);
	SampleDur max(SampleDur const& a, SampleDur const& b);
}

#endif
