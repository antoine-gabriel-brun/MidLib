#include "ev_ProgramChange.hpp"

#include "../tools/Exceptions.hpp"

namespace MID
{
	Event_ProgramChange::Event_ProgramChange(Pos const& pos, UINT_7 instrumentIndex, UINT_4 channel, char specialChannel9) :
		Event_Midi(pos, 12u, channel),
		instrumentIndex_(instrumentIndex),
		specialChannel9_(specialChannel9)
	{
		if (specialChannel9_ != 's' && specialChannel9_ != 'n' && specialChannel9_ != 'u') {
			throw Exceptions::WrongSpecialChannel9Specifier();
		}
	}

	Event_ProgramChange& Event_ProgramChange::channel(UINT_4 channel)
	{
		setChannel(channel);
		return *this;
	}
	Event_ProgramChange& Event_ProgramChange::instrumentIndex(UINT_7 instrumentIndex)
	{
		instrumentIndex_ = instrumentIndex;
		return *this;
	}

	UINT_7 Event_ProgramChange::instrumentIndex() const
	{
		return instrumentIndex_;
	}
	char Event_ProgramChange::specialChannel9() const
	{
		return specialChannel9_;
	}
	bool Event_ProgramChange::isMeantForSpecialChannel9() const
	{
		return specialChannel9_ == 's';
	}
	bool Event_ProgramChange::isMeantForNormalChannel() const
	{
		return specialChannel9_ == 'n';
	}
	bool Event_ProgramChange::isMeantForAnyChannel() const
	{
		return specialChannel9_ == 'u';
	}

	bool Event_ProgramChange::isNoteOn() const
	{
		return false;
	}
	bool Event_ProgramChange::isNoteOff() const
	{
		return false;
	}
	std::shared_ptr<Event> Event_ProgramChange::clone() const
	{
		return std::make_shared<Event_ProgramChange>(*this);
	}
	void Event_ProgramChange::writeToFile(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_8 lastEventStatusByte) const
	{
		Event::writeEventToFile_1byteAfterStatus_canUseRunningStatus(fileWriter, deltaTicks,
			instrumentIndex_,
			lastEventStatusByte
		);
	}
}
