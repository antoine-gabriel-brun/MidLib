#ifndef MID_TEMPO_HPP
#define MID_TEMPO_HPP

#include "../tools/Integers.hpp"

namespace MID
{
	// Logical tempo, expressed as the speed of quarter notes.
	// Different from class Event_Tempo, which represents a MIDI event.
	class Tempo
	{
	public:
		// Creators
		static Tempo createDefault();
		static Tempo fromBpm(double bpm);
		static Tempo fromMidi(UINT_24 microsecondsPerQuarter);

		// Subsequent modification
		void bpm(double bpm);
		void microsecondsPerQuarter(UINT_24 microsecondsPerQuarter);

		// Accessors
		double bpm() const;
		UINT_24 microsecondsPerQuarter() const;

		// Comparison operators
		bool operator==(Tempo const& other) const; // all of the comparison operators only
		bool operator!=(Tempo const& other) const; // compare 'microsecondsPerQuarter' because
		bool operator<(Tempo const& other) const;  // this is the value used when writing to
		bool operator<=(Tempo const& other) const; // the file. Due to calculation inaccuracies,
		bool operator>(Tempo const& other) const;  // an equal number of microseconds per quarter
		bool operator>=(Tempo const& other) const; // doesn't mean that bpms are exactly equal.

	private:
		// Private constructor (doesn't check anything)
		Tempo(double bpm, UINT_24 microsecondsPerQuarter);
		
		// Fields
		double bpm_;
		UINT_24 microsecondsPerQuarter_;
	};

	// Min and max (free functions)
	Tempo min(Tempo const& a, Tempo const& b);
	Tempo max(Tempo const& a, Tempo const& b);
}

#endif
