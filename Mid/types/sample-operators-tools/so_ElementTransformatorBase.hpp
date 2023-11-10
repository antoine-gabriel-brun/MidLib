#ifndef MID_SO_ELEMENT_TRANSFORMATOR_BASE_HPP
#define MID_SO_ELEMENT_TRANSFORMATOR_BASE_HPP

#include "../event-bases/Event.hpp"
#include "../event-creators/ev_NoteOnOffEventsCreator.hpp"
#include "../event-types/ev_PitchBend.hpp"
#include "../event-types/ev_ControlChange.hpp"
#include "../event-types/ev_ProgramChange.hpp"
#include "../event-types/ev_ChannelAftertouch.hpp"
#include "../event-types/ev_KeyAftertouch.hpp"

#include <memory>

namespace MID
{
	// Forward declarations
	class Track;
	class Sample;

	// Abstract base class representing a transformation that can be applied to track elements.
	class ElementTransformatorBase
	{
	public:
		// To implement: what to do [inside the track / inside the sample] before each element is copied
		virtual void before(Track const& track_in, Track& track_out) const;
		virtual void before(Sample const& sample_in, Sample& sample_out) const;

		// To implement: transformator for each element (if not overridden in base classes, elements remain unchanged)
		virtual std::unique_ptr<Note                   > transformNote             (Note                    const& note             ) const;
		virtual std::unique_ptr<Event_PitchBend        > transformPitchBend        (Event_PitchBend         const& pitchBend        ) const;
		virtual std::unique_ptr<Event_ControlChange    > transformControlChange    (Event_ControlChange     const& controlChange    ) const;
		virtual std::unique_ptr<Event_ProgramChange    > transformProgramChange    (Event_ProgramChange     const& programChange    ) const;
		virtual std::unique_ptr<Event_ChannelAftertouch> transformChannelAftertouch(Event_ChannelAftertouch const& channelAftertouch) const;
		virtual std::unique_ptr<Event_KeyAftertouch    > transformKeyAftertouch    (Event_KeyAftertouch     const& keyAftertouch    ) const;
		virtual std::shared_ptr<Event                  > transformUnsafeEvent      (std::shared_ptr<Event>  const  unsafeEvent      ) const;

		// To implement: what to do [inside the track / inside the sample] after each element has been copied
		virtual void after(Track const& track_in, Track& track_out) const;
		virtual void after(Sample const& sample_in, Sample& sample_out) const;

	protected:
		// Helpers functions for children classes
		static std::unique_ptr<Note                   > copy(Note                    const& note             );
		static std::unique_ptr<Event_PitchBend        > copy(Event_PitchBend         const& pitchBend        );
		static std::unique_ptr<Event_ControlChange    > copy(Event_ControlChange     const& controlChange    );
		static std::unique_ptr<Event_ProgramChange    > copy(Event_ProgramChange     const& programChange    );
		static std::unique_ptr<Event_ChannelAftertouch> copy(Event_ChannelAftertouch const& channelAftertouch);
		static std::unique_ptr<Event_KeyAftertouch    > copy(Event_KeyAftertouch     const& keyAftertouch    );
		static std::shared_ptr<Event                  > copy(std::shared_ptr<Event>  const  unsafeEvent      );
		template<typename T>
		static std::unique_ptr<T> discard()
		{
			return nullptr;
		}
	};
}

#endif
