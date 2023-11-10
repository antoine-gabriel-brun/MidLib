#ifndef MID_EV_CHANNEL_AFTERTOUCH_HPP
#define MID_EV_CHANNEL_AFTERTOUCH_HPP

#include "../event-bases/ev_Midi.hpp"
#include "../tools/Integers.hpp"
#include "../tools/Convert.hpp"

namespace MID
{
	// MIDI event representing a pressure value that affects
	// all currently depressed keys inside one channel.
	class Event_ChannelAftertouch : public Event_Midi
	{
	public:
		// Constructor
		Event_ChannelAftertouch(Pos const& pos, UINT_7 pressure, UINT_4 channel = 0);

		// Named parameter idiom and subsequent modification
		Event_ChannelAftertouch& channel(UINT_4 channel);
		template<typename Numeric, typename dummy = enableIfNumeric<Numeric>>
		Event_ChannelAftertouch& pressure(Numeric pressure)
		{
			pressure_ = Convert::Aftertouch::float_to_midi((float)pressure);
			return *this;
		}
		Event_ChannelAftertouch& pressureMidi(UINT_7 pressure);

		// Accessors
		UINT_7 pressure() const;
		
		// Implementations
		virtual bool isNoteOn() const override;
		virtual bool isNoteOff() const override;
		virtual std::shared_ptr<Event> clone() const override;
		virtual void writeToFile(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_8 lastEventStatusByte) const override;

	private:
		// Fields
		UINT_7 pressure_;
	};
}

#endif
