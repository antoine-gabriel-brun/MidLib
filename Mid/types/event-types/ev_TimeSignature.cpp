#include "ev_TimeSignature.hpp"

#include "../tools/Exceptions.hpp"

#include <vector>

namespace MID
{
	namespace
	{
		UINT_8 denomToMidiEncoding(UINT_8 denom_4isQuarter_8isEighth_etc)
		{
			switch (denom_4isQuarter_8isEighth_etc)
			{
			case 1: return 0;
			case 2: return 1;
			case 4: return 2;
			case 8: return 3;
			case 16: return 4;
			case 32: return 5;
			case 64: return 6;
			case 128: return 7;
			default: return 0; // This won't happen because an exception was already thrown when the TimeSignature object was created.
			}
		}
		UINT_8 checkMidiTicksPerMetroTick(UINT_8 midiTicksPerMetroTick)
		{
			if (midiTicksPerMetroTick > 0) {
				return midiTicksPerMetroTick;
			} else {
				throw Exceptions::WrongMidiTicksPerMetroTick();
			}
		}
	}

	Event_TimeSignature::Event_TimeSignature(UINT_8 num, UINT_8 denom_4isQuarter_8isEighth_etc, UINT_8 midiTicksPerMetroTick, Pos const& pos) :
		Event_Meta(pos, 0x58),
		timeSignature_(num, denom_4isQuarter_8isEighth_etc),
		denom_midiEncoding_(denomToMidiEncoding(denom_4isQuarter_8isEighth_etc)),
		midiTicksPerMetroTick_(checkMidiTicksPerMetroTick(midiTicksPerMetroTick))
	{ }
	Event_TimeSignature::Event_TimeSignature(TimeSignature const& timeSignature, UINT_8 midiTicksPerMetroTick, Pos const& pos) :
		Event_Meta(pos, 0x58),
		timeSignature_(timeSignature),
		denom_midiEncoding_(denomToMidiEncoding(timeSignature.denom_4isQuarter_8isEighth_etc())),
		midiTicksPerMetroTick_(checkMidiTicksPerMetroTick(midiTicksPerMetroTick))
	{ }
	Event_TimeSignature Event_TimeSignature::createDefault()
	{
		return Event_TimeSignature(TimeSignature::createDefault(), 96u);
	}

	UINT_8 Event_TimeSignature::num() const
	{
		return timeSignature_.num();
	}
	UINT_8 Event_TimeSignature::denom_4isQuarter_8isEighth_etc() const
	{
		return timeSignature_.denom_4isQuarter_8isEighth_etc();
	}
	UINT_8 Event_TimeSignature::denom_midiEncoding() const
	{
		return denom_midiEncoding_;
	}
	UINT_8 Event_TimeSignature::midiTicksPerMetroTick() const
	{
		return midiTicksPerMetroTick_;
	}

	std::shared_ptr<Event> Event_TimeSignature::clone() const
	{
		return std::make_shared<Event_TimeSignature>(*this);
	}
	void Event_TimeSignature::writeToFile(FileWriter& fileWriter, UINT_28 deltaTicks) const
	{
		Event_Meta::writeMetaEventToFile(fileWriter, deltaTicks, std::vector<UINT_8>{
			num(),
			denom_midiEncoding_,
			midiTicksPerMetroTick_,
			8u
		});
	}
}
