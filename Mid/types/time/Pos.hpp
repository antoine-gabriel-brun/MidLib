#ifndef MID_POS_HPP
#define MID_POS_HPP

#include "TimeValue.hpp"

namespace MID
{
	// Forward declaration
	class Dur;

	// Position within a MIDI sample
	class Pos : public TimeValue
	{
	public:
		// Constructor
		explicit Pos(double time);

		// Conversion
		Dur toDuration() const;
		
		// Arithmetic operators
		Pos operator+(Dur const& dur) const;
		Pos operator-(Dur const& dur) const;
		Dur operator-(Pos const& pos) const;
		
		// Comparison operators
		bool operator==(Pos const& other) const;
		bool operator!=(Pos const& other) const;
		bool operator<(Pos const& other) const;
		bool operator<=(Pos const& other) const;
		bool operator>(Pos const& other) const;
		bool operator>=(Pos const& other) const;
	};

	// Min and max (free functions)
	Pos min(Pos const& a, Pos const& b);
	Pos max(Pos const& a, Pos const& b);
}

// This #include provides a way to determine whether a template parameter is compatible with Pos
#include "../traits/isPosOrNumeric.hpp"

#endif
