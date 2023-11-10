#ifndef MID_EV_KEY_AFTERTOUCH_HPP
#define MID_EV_KEY_AFTERTOUCH_HPP

#include "../event-bases/ev_Midi.hpp"
#include "../tools/Integers.hpp"
#include "../tools/Convert.hpp"

namespace MID
{
	// MIDI event representing a pressure value that affects a single depressed key.
	class Event_KeyAftertouch : public Event_Midi
	{
	public:
		// Constructor
		Event_KeyAftertouch(Pos const& pos, UINT_7 pitch, UINT_7 pressure, UINT_4 channel = 0);

		// Named parameter idiom and subsequent modification
		Event_KeyAftertouch& channel(UINT_4 channel);
		Event_KeyAftertouch& pitch(UINT_7 pitch);
		template<typename Numeric, typename dummy = enableIfNumeric<Numeric>>
		Event_KeyAftertouch& pressure(Numeric pressure)
		{
			pressure_ = Convert::Aftertouch::float_to_midi((float)pressure);
			return *this;
		}
		Event_KeyAftertouch& pressureMidi(UINT_7 pressure);

		// Accessors
		UINT_7 pitch() const;
		UINT_7 pressure() const;
		
		// Implementations
		virtual bool isNoteOn() const override;
		virtual bool isNoteOff() const override;
		virtual std::shared_ptr<Event> clone() const override;
		virtual void writeToFile(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_8 lastEventStatusByte) const override;

	private:
		// Fields
		UINT_7 pitch_;
		UINT_7 pressure_;
	};
}

#endif
