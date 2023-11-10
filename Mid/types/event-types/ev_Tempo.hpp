#ifndef MID_EV_TEMPO_HPP
#define MID_EV_TEMPO_HPP

#include "../event-bases/ev_Meta.hpp"
#include "../sample-options/Tempo.hpp"
#include "../tools/Integers.hpp"

namespace MID
{
	// MIDI event that indicates the tempo of a MIDI sample.
	// Different from class Tempo, which represents a logical value.
	class Event_Tempo : public Event_Meta
	{
	public:
		// Constructor
		Event_Tempo(Tempo const& tempo, Pos const& pos = Pos(0.));
		
		// Creators
		static Event_Tempo createDefault(Pos const& pos = Pos(0.));
		static Event_Tempo fromBpm(double bpm, Pos const& pos = Pos(0.));
		static Event_Tempo fromMidi(UINT_24 microsecondsPerQuarter, Pos const& pos = Pos(0.));

		// Accessors
		double bpm() const;
		UINT_24 microsecondsPerQuarter() const;

		// Implementations
		virtual std::shared_ptr<Event> clone() const override;
		virtual void writeToFile(FileWriter& fileWriter, UINT_28 deltaTicks) const override;

	private:
		// Fields
		Tempo tempo_;
	};
}

#endif
