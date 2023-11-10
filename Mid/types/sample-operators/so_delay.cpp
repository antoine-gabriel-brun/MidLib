#include "SampleOperators.hpp"

namespace MID
{
	namespace SampleOperators
	{
		namespace
		{
			class ElementTransformator_Delay : public ElementTransformatorBase
			{
			public:
				// Constructor
				ElementTransformator_Delay(Dur const& timeOffset, Selector const& what) :
					timeOffset_(timeOffset),
					what_(what)
				{ }

				// Implementations
				virtual std::unique_ptr<Note> transformNote(Note const& note) const override
				{
					if (what_.selectNotes()) {
						Pos newStart = note.start() + timeOffset_;
						Pos newEnd = note.end() + timeOffset_;

						auto clone = copy(note);
						clone->startEnd(newStart, newEnd);
						return clone;
					} else {
						return copy(note);
					}
				}
				virtual std::unique_ptr<Event_PitchBend> transformPitchBend(Event_PitchBend const& pitchBend) const override
				{
					if (what_.selectPitchBends()) {
						return transform(pitchBend);
					} else {
						return copy(pitchBend);
					}
				}
				virtual std::unique_ptr<Event_ControlChange> transformControlChange(Event_ControlChange const& controlChange) const override
				{
					if (what_.selectControlChanges()) {
						return transform(controlChange);
					} else {
						return copy(controlChange);
					}
				}
				virtual std::unique_ptr<Event_ProgramChange> transformProgramChange(Event_ProgramChange const& programChange) const override
				{
					if (what_.selectProgramChanges()) {
						return transform(programChange);
					} else {
						return copy(programChange);
					}
				}
				virtual std::unique_ptr<Event_ChannelAftertouch> transformChannelAftertouch(Event_ChannelAftertouch const& channelAftertouch) const override
				{
					if (what_.selectAftertouches()) {
						return transform(channelAftertouch);
					} else {
						return copy(channelAftertouch);
					}
				}
				virtual std::unique_ptr<Event_KeyAftertouch> transformKeyAftertouch(Event_KeyAftertouch const& keyAftertouch) const override
				{
					if (what_.selectAftertouches()) {
						return transform(keyAftertouch);
					} else {
						return copy(keyAftertouch);
					}
				}
				virtual std::shared_ptr<Event> transformUnsafeEvent(std::shared_ptr<Event> const unsafeEvent) const override
				{
					if (what_.selectUnsafeEvents()) {
						Pos newPos = unsafeEvent->pos() + timeOffset_;
						auto clone = copy(unsafeEvent);
						clone->setPos(newPos);
						return clone;
					} else {
						return copy(unsafeEvent);
					}
				}

			private:
				// Helper functions
				template<typename T_Event>
				std::unique_ptr<T_Event> transform(T_Event const& ev) const
				{
					Pos newPos = ev.pos() + timeOffset_;

					auto clone = copy(ev);
					clone->setPos(newPos);
					return clone;
				}

				// Fields
				Dur timeOffset_;
				Selector what_;
			};
		}

		Sample delay(Sample const& sample, double timeOffset, Selector const& what)
		{
			// Get the time offset as a duration
			Dur timeOffset_dur(timeOffset);
			
			// Copy with transformation
			Sample ans = sample.copyTransform(
				ElementTransformator_Delay(timeOffset_dur, what)
			);

			// Adjust the duration, if specified
			if (ans.dur().isSpecified()) {
				ans.dur(timeOffset_dur + ans.dur().dur());
			}

			// Return
			return ans;
		}
	}
}
