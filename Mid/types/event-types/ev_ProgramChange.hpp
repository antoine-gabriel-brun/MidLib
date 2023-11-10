#ifndef MID_EV_PROGRAM_CHANGE_HPP
#define MID_EV_PROGRAM_CHANGE_HPP

#include "../event-bases/ev_Midi.hpp"
#include "../tools/Integers.hpp"

namespace MID
{
	// MIDI event representing a program change (this basically
	// defines which instrument should play a given channel).
	class Event_ProgramChange : public Event_Midi
	{
	public:
		// Constructor
		Event_ProgramChange(Pos const& pos, UINT_7 instrumentIndex, UINT_4 channel = 0, char specialChannel9 = 'u');

		// Named parameter idiom and subsequent modification
		Event_ProgramChange& channel(UINT_4 channel);
		Event_ProgramChange& instrumentIndex(UINT_7 instrumentIndex);
		
		// Accessors
		UINT_7 instrumentIndex() const;
		char specialChannel9() const;
		bool isMeantForSpecialChannel9() const;
		bool isMeantForNormalChannel() const;
		bool isMeantForAnyChannel() const;
		
		// Implementations
		virtual bool isNoteOn() const override;
		virtual bool isNoteOff() const override;
		virtual std::shared_ptr<Event> clone() const override;
		virtual void writeToFile(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_8 lastEventStatusByte) const override;

	private:
		// Fields
		UINT_7 instrumentIndex_;
		char specialChannel9_; // 's' for special drum channel, 'n' for normal channel, 'u' for unspecified channel type
	};
}

#endif
