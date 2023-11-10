#ifndef MID_EV_CONTROL_CHANGE_HPP
#define MID_EV_CONTROL_CHANGE_HPP

#include "../event-bases/ev_Midi.hpp"
#include "../time/Pos.hpp"
#include "../tools/Integers.hpp"
#include "../tools/Convert.hpp"

#include <vector>

namespace MID
{
	// MIDI event that performs a control change, such as Pan or Volume track.
	class Event_ControlChange : public Event_Midi
	{
	public:
		// Constructors
		Event_ControlChange(Pos const& pos, UINT_7 controller, UINT_7 value, UINT_4 channel = 0);
		Event_ControlChange(double pos, UINT_7 controller, UINT_7 value, UINT_4 channel = 0);

		// Named parameter idiom and subsequent modification
		Event_ControlChange& channel(UINT_4 channel);
		Event_ControlChange& controller(UINT_7 value);
		Event_ControlChange& value(UINT_7 value);
		
		// Accessors
		UINT_7 controller() const;
		UINT_7 value() const;
		
		// Implementations
		virtual bool isNoteOn() const override;
		virtual bool isNoteOff() const override;
		virtual std::shared_ptr<Event> clone() const override;
		virtual void writeToFile(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_8 lastEventStatusByte) const override;
	
	private:
		// Fields
		UINT_7 controller_;
		UINT_7 value_;
	};
}

#endif
