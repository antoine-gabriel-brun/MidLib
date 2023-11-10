#ifndef MID_EV_NOTE_ON_OFF_HPP
#define MID_EV_NOTE_ON_OFF_HPP

#include "../event-bases/ev_Midi.hpp"
#include "../tools/Integers.hpp"

namespace MID
{
	// MIDI event representing a note start or a note end within a MIDI track.
	class Event_NoteOnOff : public Event_Midi
	{
	public:
		// Creators
		static Event_NoteOnOff on(Pos const& pos, UINT_7 pitch, UINT_7 velocity, UINT_4 channel = 0);
		static Event_NoteOnOff off(Pos const& pos, UINT_7 pitch, UINT_4 channel = 0);
		static Event_NoteOnOff* on_newRawPtr(Pos const& pos, UINT_7 pitch, UINT_7 velocity, UINT_4 channel = 0);
		static Event_NoteOnOff* off_newRawPtr(Pos const& pos, UINT_7 pitch, UINT_4 channel = 0);
		
		// Accessors
		UINT_7 pitch() const;
		UINT_7 velocity() const;
		void setPitch(UINT_7 pitch);
		void setVelocity(UINT_7 velocity);
		
		// Implementations
		virtual bool isNoteOn() const override;
		virtual bool isNoteOff() const override;
		virtual std::shared_ptr<Event> clone() const override;
		virtual void writeToFile(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_8 lastEventStatusByte) const override;

	private:
		// Private constructor
		Event_NoteOnOff(Pos const& pos, UINT_7 pitch, UINT_7 velocity, UINT_4 channel);

		// Fields
		UINT_7 pitch_;
		UINT_7 velocity_; // if 0, the instance represents a note OFF
	};
}

#endif
