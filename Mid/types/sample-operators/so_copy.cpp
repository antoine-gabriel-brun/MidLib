#include "SampleOperators.hpp"

namespace MID
{
	namespace SampleOperators
	{
		namespace
		{
			class ElementTransformator_Copy : public ElementTransformatorBase
			{
			public:
				// Constructor
				ElementTransformator_Copy(Selector const& what) :
					what_(what)
				{ }

				// Implementations
				virtual std::unique_ptr<Note> transformNote(Note const& note) const override
				{
					if (what_.selectNotes()) {
						return copy(note);
					} else {
						return discard<Note>();
					}
				}
				virtual std::unique_ptr<Event_PitchBend> transformPitchBend(Event_PitchBend const& pitchBend) const override
				{
					if (what_.selectPitchBends()) {
						return copy(pitchBend);
					} else {
						return discard<Event_PitchBend>();
					}
				}
				virtual std::unique_ptr<Event_ControlChange> transformControlChange(Event_ControlChange const& controlChange) const override
				{
					if (what_.selectControlChanges()) {
						return copy(controlChange);
					} else {
						return discard<Event_ControlChange>();
					}
				}
				virtual std::unique_ptr<Event_ProgramChange> transformProgramChange(Event_ProgramChange const& programChange) const override
				{
					if (what_.selectProgramChanges()) {
						return copy(programChange);
					} else {
						return discard<Event_ProgramChange>();
					}
				}
				virtual std::unique_ptr<Event_ChannelAftertouch> transformChannelAftertouch(Event_ChannelAftertouch const& channelAftertouch) const override
				{
					if (what_.selectAftertouches()) {
						return copy(channelAftertouch);
					} else {
						return discard<Event_ChannelAftertouch>();
					}
				}
				virtual std::unique_ptr<Event_KeyAftertouch> transformKeyAftertouch(Event_KeyAftertouch const& keyAftertouch) const override
				{
					if (what_.selectAftertouches()) {
						return copy(keyAftertouch);
					} else {
						return discard<Event_KeyAftertouch>();
					}
				}
				virtual std::shared_ptr<Event> transformUnsafeEvent(std::shared_ptr<Event> const unsafeEvent) const override
				{
					if (what_.selectUnsafeEvents()) {
						return copy(unsafeEvent);
					} else {
						return discard<Event>();
					}
				}

			private:
				// Fields
				Selector what_;
			};
		}

		Sample copy(Sample const& sample, Selector const& what)
		{
			// Copy with transformation
			return sample.copyTransform(
				ElementTransformator_Copy(what)
			);
		}
	}
}
