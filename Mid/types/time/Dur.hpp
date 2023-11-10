#ifndef MID_DUR_HPP
#define MID_DUR_HPP

#include "TimeValue.hpp"

namespace MID
{
	// Forward declaration
	class Pos;

	// Duration within a MIDI sample
	class Dur : public TimeValue
	{
	public:
		// Constructor
		explicit Dur(double time);

		// Conversion
		Pos toPos() const;
		
		// Arithmetic operators
		Dur operator+(Dur const& duration) const;
		Dur operator-(Dur const& duration) const;
		
		// Comparison operators
		bool operator==(Dur const& other) const;
		bool operator!=(Dur const& other) const;
		bool operator<(Dur const& other) const;
		bool operator<=(Dur const& other) const;
		bool operator>(Dur const& other) const;
		bool operator>=(Dur const& other) const;
	};

	// Min and max (free functions)
	Dur min(Dur const& a, Dur const& b);
	Dur max(Dur const& a, Dur const& b);
}

// Free operators
MID::Dur operator*(double mult, MID::Dur const& duration);
MID::Dur operator*(MID::Dur const& duration, double mult);

// This #include provides a way to determine whether a template parameter is compatible with Dur
#include "../traits/isDurOrNumeric.hpp"

#endif
