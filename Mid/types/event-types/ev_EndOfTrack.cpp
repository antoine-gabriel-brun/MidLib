#include "ev_EndOfTrack.hpp"

namespace MID
{
	Event_EndOfTrack::Event_EndOfTrack(Pos const& pos) :
		Event_Meta(pos, 0x2F)
	{ }
	
	std::shared_ptr<Event> Event_EndOfTrack::clone() const
	{
		return std::make_shared<Event_EndOfTrack>(*this);
	}
	void Event_EndOfTrack::writeToFile(FileWriter& fileWriter, UINT_28 deltaTicks) const
	{
		Event_Meta::writeMetaEventToFile(fileWriter, deltaTicks, "");
	}
}
