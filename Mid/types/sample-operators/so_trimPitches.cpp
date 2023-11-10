#include "SampleOperators.hpp"

namespace MID
{
	namespace SampleOperators
	{
		namespace
		{
			class ElementTransformator_TrimPitches : public ElementTransformatorBase
			{
			public:
				// Constructor
				ElementTransformator_TrimPitches(UINT_8 min, UINT_8 max, bool keepNoPitchEvents) :
					min_(min),
					max_(max),
					keepNoPitchEvents_(keepNoPitchEvents)
				{ }

				// Implementations
				virtual std::unique_ptr<Note> transformNote(Note const& note) const override
				{
					// In the correct range?
					if (note.pitch() >= min_ && note.pitch() <= max_) {
						// Keep
						return copy(note);
					} else {
						// Discard
						return discard<Note>();
					}
				}
				virtual std::unique_ptr<Event_PitchBend> transformPitchBend(Event_PitchBend const& pitchBend) const override
				{
					if (keepNoPitchEvents_) {
						return copy(pitchBend);
					} else {
						return discard<Event_PitchBend>();
					}
				}
				virtual std::unique_ptr<Event_ControlChange> transformControlChange(Event_ControlChange const& controlChange) const override
				{
					if (keepNoPitchEvents_) {
						return copy(controlChange);
					} else {
						return discard<Event_ControlChange>();
					}
				}
				virtual std::unique_ptr<Event_ProgramChange> transformProgramChange(Event_ProgramChange const& programChange) const override
				{
					if (keepNoPitchEvents_) {
						return copy(programChange);
					} else {
						return discard<Event_ProgramChange>();
					}
				}
				virtual std::unique_ptr<Event_ChannelAftertouch> transformChannelAftertouch(Event_ChannelAftertouch const& channelAftertouch) const override
				{
					if (keepNoPitchEvents_) {
						return copy(channelAftertouch);
					} else {
						return discard<Event_ChannelAftertouch>();
					}
				}
				virtual std::unique_ptr<Event_KeyAftertouch> transformKeyAftertouch(Event_KeyAftertouch const& keyAftertouch) const override
				{
					if (keepNoPitchEvents_) {
						return copy(keyAftertouch);
					} else {
						return discard<Event_KeyAftertouch>();
					}
				}
				virtual std::shared_ptr<Event> transformUnsafeEvent(std::shared_ptr<Event> const unsafeEvent) const override
				{
					// Event with pitch?
					Event_NoteOnOff* eventNoteOnOff = dynamic_cast<Event_NoteOnOff*>(unsafeEvent.get());
					if (eventNoteOnOff == nullptr) {
						// Not an event with pitch
						if (keepNoPitchEvents_) {
							return copy(unsafeEvent);
						} else {
							return discard<Event>();
						}
					} else {
						// Event with pitch. In the correct range?
						if (eventNoteOnOff->pitch() >= min_ && eventNoteOnOff->pitch() <= max_) {
							// Keep
							return copy(unsafeEvent);
						} else {
							// Discard
							return discard<Event>();
						}
					}
				}

			private:
				// Fields
				UINT_8 min_;
				UINT_8 max_;
				bool keepNoPitchEvents_;
			};
		}

		Sample trimPitches(Sample const& sample, UINT_7 min, UINT_7 max, bool keepNoPitchEvents)
		{
			// Check min and max
			UINT_8 min_int = min;
			UINT_8 max_int = max;
			if (max_int < min_int - 1) {
				throw Exceptions::WrongSampleOperatorArgument("trimPitches", "max", "the maximum pitch should not be less than the minimum pitch minus one.");
			}
			
			// Copy with transformation
			return sample.copyTransform(
				ElementTransformator_TrimPitches(min_int, max_int, keepNoPitchEvents)
			);
		}
	}
}
