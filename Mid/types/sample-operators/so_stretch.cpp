#include "SampleOperators.hpp"

namespace MID
{
	namespace SampleOperators
	{
		namespace
		{
			class ElementTransformator_Stretch : public ElementTransformatorBase
			{
			public:
				// Constructor
				ElementTransformator_Stretch(double timeRatio, Selector const& what) :
					timeRatio_(timeRatio),
					what_(what)
				{ }

				// Implementations
				virtual std::unique_ptr<Note> transformNote(Note const& note) const override
				{
					if (what_.selectNotes()) {
						double newStart = timeRatio_ * note.start().time();
						double newEnd = timeRatio_ * note.end().time();

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
						double newPos = timeRatio_ * unsafeEvent->pos().time();
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
					double newPos = timeRatio_ * ev.pos().time();

					auto clone = copy(ev);
					clone->setPos(newPos);
					return clone;
				}

				// Fields
				double timeRatio_;
				Selector what_;
			};
		}

		Sample stretch(Sample const& sample, double timeRatio, Selector const& what)
		{
			// Check the time ratio
			if (timeRatio < 0.0001 || timeRatio > 10000) {
				throw Exceptions::WrongSampleOperatorArgument("stretch", "timeRatio", "the time ratio should be between 0.0001 and 10000.");
			}
			
			// Copy with transformation
			Sample ans = sample.copyTransform(
				ElementTransformator_Stretch(timeRatio, what)
			);

			// Adjust the duration, if specified
			if (ans.dur().isSpecified()) {
				ans.dur(timeRatio * ans.dur().dur());
			}

			// Return
			return ans;
		}
	}
}
