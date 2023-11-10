#include "Tempo.hpp"

#include "../tools/Convert.hpp"

namespace MID
{
	Tempo Tempo::createDefault()
	{
		return Tempo(
			60.0,      // This is 60 BPM:
			1'000'000u // each quarter note lasts one million microseconds
		);
	}
	Tempo Tempo::fromBpm(double bpm)
	{
		return Tempo(
			bpm,
			Convert::Tempo::bpm_to_microsecondsPerQuarter(bpm) // this line also checks the range
		);
	}
	Tempo Tempo::fromMidi(UINT_24 microsecondsPerQuarter)
	{
		return Tempo(
			Convert::Tempo::microsecondsPerQuarter_to_bpm(microsecondsPerQuarter), // this line also checks the range
			microsecondsPerQuarter
		);
	}

	void Tempo::bpm(double bpm)
	{
		bpm_ = bpm;
		microsecondsPerQuarter_ = Convert::Tempo::bpm_to_microsecondsPerQuarter(bpm); // this line also checks the range
	}
	void Tempo::microsecondsPerQuarter(UINT_24 microsecondsPerQuarter)
	{
		bpm_ = Convert::Tempo::microsecondsPerQuarter_to_bpm(microsecondsPerQuarter); // this line also checks the range
		microsecondsPerQuarter_ = microsecondsPerQuarter;
	}

	double Tempo::bpm() const
	{
		return bpm_;
	}
	UINT_24 Tempo::microsecondsPerQuarter() const
	{
		return microsecondsPerQuarter_;
	}

	bool Tempo::operator==(Tempo const& other) const
	{
		return microsecondsPerQuarter_ == other.microsecondsPerQuarter_;
	}
	bool Tempo::operator!=(Tempo const& other) const
	{
		return microsecondsPerQuarter_ != other.microsecondsPerQuarter_;
	}
	bool Tempo::operator<(Tempo const& other) const
	{
		return microsecondsPerQuarter_ > other.microsecondsPerQuarter_; // intentionally reversed: microseconds per quarter and bpm grow in opposite directions
	}
	bool Tempo::operator<=(Tempo const& other) const
	{
		return microsecondsPerQuarter_ >= other.microsecondsPerQuarter_; // intentionally reversed: microseconds per quarter and bpm grow in opposite directions
	}
	bool Tempo::operator>(Tempo const& other) const
	{
		return microsecondsPerQuarter_ < other.microsecondsPerQuarter_; // intentionally reversed: microseconds per quarter and bpm grow in opposite directions
	}
	bool Tempo::operator>=(Tempo const& other) const
	{
		return microsecondsPerQuarter_ <= other.microsecondsPerQuarter_; // intentionally reversed: microseconds per quarter and bpm grow in opposite directions
	}

	Tempo::Tempo(double bpm, UINT_24 microsecondsPerQuarter) :
		bpm_(bpm),
		microsecondsPerQuarter_(microsecondsPerQuarter)
	{ }

	Tempo min(Tempo const& a, Tempo const& b)
	{
		return (a <= b) ? a : b;
	}
	Tempo max(Tempo const& a, Tempo const& b)
	{
		return (a >= b) ? a : b;
	}
}
