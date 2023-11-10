#include "so_ElementTransformatorBase.hpp"

#include "../containers/Sample.hpp"

namespace MID
{
	void ElementTransformatorBase::before(Track const& track_in, Track& track_out) const
	{
		// do nothing (to be overridden)
	}
	void ElementTransformatorBase::before(Sample const& sample_in, Sample& sample_out) const
	{
		// do nothing (to be overridden)
	}

	std::unique_ptr<Note> ElementTransformatorBase::transformNote(Note const& note) const
	{
		return copy(note);
	}
	std::unique_ptr<Event_PitchBend> ElementTransformatorBase::transformPitchBend(Event_PitchBend const& pitchBend) const
	{
		return copy(pitchBend);
	}
	std::unique_ptr<Event_ControlChange> ElementTransformatorBase::transformControlChange(Event_ControlChange const& controlChange) const
	{
		return copy(controlChange);
	}
	std::unique_ptr<Event_ProgramChange> ElementTransformatorBase::transformProgramChange(Event_ProgramChange const& programChange) const
	{
		return copy(programChange);
	}
	std::unique_ptr<Event_ChannelAftertouch> ElementTransformatorBase::transformChannelAftertouch(Event_ChannelAftertouch const& channelAftertouch) const
	{
		return copy(channelAftertouch);
	}
	std::unique_ptr<Event_KeyAftertouch> ElementTransformatorBase::transformKeyAftertouch(Event_KeyAftertouch const& keyAftertouch) const
	{
		return copy(keyAftertouch);
	}
	std::shared_ptr<Event> ElementTransformatorBase::transformUnsafeEvent(std::shared_ptr<Event> const unsafeEvent) const
	{
		return copy(unsafeEvent);
	}
	
	void ElementTransformatorBase::after(Track const& track_in, Track& track_out) const
	{
		// do nothing (to be overridden)
	}
	void ElementTransformatorBase::after(Sample const& sample_in, Sample& sample_out) const
	{
		// do nothing (to be overridden)
	}

	std::unique_ptr<Note                   > ElementTransformatorBase::copy(Note                    const& note             )
	{
		return std::make_unique<Note>(note);
	}
	std::unique_ptr<Event_PitchBend        > ElementTransformatorBase::copy(Event_PitchBend         const& pitchBend        )
	{
		return std::make_unique<Event_PitchBend>(pitchBend);
	}
	std::unique_ptr<Event_ControlChange    > ElementTransformatorBase::copy(Event_ControlChange     const& controlChange    )
	{
		return std::make_unique<Event_ControlChange>(controlChange);
	}
	std::unique_ptr<Event_ProgramChange    > ElementTransformatorBase::copy(Event_ProgramChange     const& programChange    )
	{
		return std::make_unique<Event_ProgramChange>(programChange);
	}
	std::unique_ptr<Event_ChannelAftertouch> ElementTransformatorBase::copy(Event_ChannelAftertouch const& channelAftertouch)
	{
		return std::make_unique<Event_ChannelAftertouch>(channelAftertouch);
	}
	std::unique_ptr<Event_KeyAftertouch    > ElementTransformatorBase::copy(Event_KeyAftertouch     const& keyAftertouch    )
	{
		return std::make_unique<Event_KeyAftertouch>(keyAftertouch);
	}
	std::shared_ptr<Event                  > ElementTransformatorBase::copy(std::shared_ptr<Event>  const  unsafeEvent      )
	{
		return unsafeEvent->clone();
	}
}
