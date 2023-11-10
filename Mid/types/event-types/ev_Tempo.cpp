#include "ev_Tempo.hpp"

#include "../tools/Exceptions.hpp"
#include "../tools/Convert.hpp"

#include <vector>

namespace MID
{
	Event_Tempo::Event_Tempo(Tempo const& tempo, Pos const& pos) :
		Event_Meta(pos, 0x51),
		tempo_(tempo)
	{ }
	
	Event_Tempo Event_Tempo::createDefault(Pos const& pos)
	{
		// This is 60 BPM at t=pos (or t=0 with default argument value):
		return Event_Tempo(Tempo::createDefault(), pos);
	}
	Event_Tempo Event_Tempo::fromMidi(UINT_24 microsecondsPerQuarter, Pos const& pos)
	{
		return Event_Tempo(Tempo::fromMidi(microsecondsPerQuarter), pos);
	}
	Event_Tempo Event_Tempo::fromBpm(double bpm, Pos const& pos)
	{
		return Event_Tempo(Tempo::fromBpm(bpm), pos);
	}
	
	double Event_Tempo::bpm() const
	{
		return tempo_.bpm();
	}
	UINT_24 Event_Tempo::microsecondsPerQuarter() const
	{
		return tempo_.microsecondsPerQuarter();
	}

	std::shared_ptr<Event> Event_Tempo::clone() const
	{
		return std::make_shared<Event_Tempo>(*this);
	}
	void Event_Tempo::writeToFile(FileWriter& fileWriter, UINT_28 deltaTicks) const
	{
		UINT_24 mic = microsecondsPerQuarter();
		
		UINT_8 MSB = mic >> 16;
		UINT_8 CEN = (mic >> 8) & 0xFFu;
		UINT_8 LSB = mic & 0xFFu;

		Event_Meta::writeMetaEventToFile(fileWriter, deltaTicks, std::vector<UINT_8>{
			MSB,
			CEN,
			LSB
		});
	}
}
