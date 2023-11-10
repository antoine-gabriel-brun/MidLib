#include "SampleOperators.hpp"

namespace MID
{
	namespace SampleOperators
	{
		namespace
		{
			class ElementTransformator_TrimChannels : public ElementTransformatorBase
			{
			public:
				// Constructor
				ElementTransformator_TrimChannels(UINT_8 min, UINT_8 max, bool keepNoChannelEvents) :
					min_(min),
					max_(max),
					keepNoChannelEvents_(keepNoChannelEvents)
				{ }

				// Implementations
				virtual std::unique_ptr<Note> transformNote(Note const& note) const override
				{
					// In the correct range?
					if (note.channel() >= min_ && note.channel() <= max_) {
						// Keep
						return copy(note);
					} else {
						// Discard
						return discard<Note>();
					}
				}
				virtual std::unique_ptr<Event_PitchBend> transformPitchBend(Event_PitchBend const& pitchBend) const override
				{
					return transform(pitchBend);
				}
				virtual std::unique_ptr<Event_ControlChange> transformControlChange(Event_ControlChange const& controlChange) const override
				{
					return transform(controlChange);
				}
				virtual std::unique_ptr<Event_ProgramChange> transformProgramChange(Event_ProgramChange const& programChange) const override
				{
					return transform(programChange);
				}
				virtual std::unique_ptr<Event_ChannelAftertouch> transformChannelAftertouch(Event_ChannelAftertouch const& channelAftertouch) const override
				{
					return transform(channelAftertouch);
				}
				virtual std::unique_ptr<Event_KeyAftertouch> transformKeyAftertouch(Event_KeyAftertouch const& keyAftertouch) const override
				{
					return transform(keyAftertouch);
				}
				virtual std::shared_ptr<Event> transformUnsafeEvent(std::shared_ptr<Event> const unsafeEvent) const override
				{
					// Convert to event with channel
					Event_Midi* eventMidi = dynamic_cast<Event_Midi*>(unsafeEvent.get());
					if (eventMidi == nullptr) {
						// Not an event with channel
						if (keepNoChannelEvents_) {
							// Keep
							return copy(unsafeEvent);
						} else {
							// Discard
							return discard<Event>();
						}
					}

					// Event with channel. In the correct range?
					if (eventMidi->getChannel() >= min_ && eventMidi->getChannel() <= max_) {
						// Keep
						return copy(unsafeEvent);
					} else {
						// Discard
						return discard<Event>();
					}
				}

			private:
				// Helper functions
				template<typename T_Event>
				std::unique_ptr<T_Event> transform(T_Event const& eventToTransform) const
				{
					// Get channel
					int channel = ((Event_Midi*)&eventToTransform)->getChannel();
					
					// In the correct range?
					if (channel >= min_ && channel <= max_) {
						// Keep
						return copy(eventToTransform);
					} else {
						// Discard
						return discard<T_Event>();
					}
				}

				// Fields
				UINT_8 min_;
				UINT_8 max_;
				bool keepNoChannelEvents_;
			};
		}

		Sample trimChannels(Sample const& sample, UINT_4 min, UINT_4 max, bool keepNoChannelEvents)
		{
			// Check min and max
			UINT_8 min_int = min;
			UINT_8 max_int = max;
			if (max_int < min_int - 1) {
				throw Exceptions::WrongSampleOperatorArgument("trimChannels", "max", "the maximum channel should not be less than the minimum channel minus one.");
			}
			
			// Copy with transformation
			return sample.copyTransform(
				ElementTransformator_TrimChannels(min_int, max_int, keepNoChannelEvents)
			);
		}
	}
}
