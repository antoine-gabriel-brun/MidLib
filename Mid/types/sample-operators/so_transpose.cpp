#include "SampleOperators.hpp"

namespace MID
{
	namespace SampleOperators
	{
		namespace
		{
			class ElementTransformator_Transpose : public ElementTransformatorBase
			{
			public:
				// Constructor
				ElementTransformator_Transpose(int semitonesOffset, TransposedPitchOutOfRange transposedPitchOutOfRange) :
					semitonesOffset_(semitonesOffset),
					transposedPitchOutOfRange_(transposedPitchOutOfRange)
				{ }

				// Implementations
				virtual std::unique_ptr<Note> transformNote(Note const& note) const override
				{
					// Get new pitch and test for errors
					int newPitch = note.pitch() + semitonesOffset_;
					if (checkPitch_shouldDiscard(newPitch)) {
						return discard<Note>();
					}

					// Copy note
					std::unique_ptr<Note> ans = copy(note);

					// Change pitch
					ans->pitch(newPitch);

					// Return
					return ans;
				}
				virtual std::shared_ptr<Event> transformUnsafeEvent(std::shared_ptr<Event> const unsafeEvent) const override
				{
					// Get new pitch and test for errors
					Event_NoteOnOff* eventNoteOnOff = dynamic_cast<Event_NoteOnOff*>(unsafeEvent.get());
					if (eventNoteOnOff == nullptr) {
						// Not an event with pitch: leave unchanged
						return copy(unsafeEvent);
					}
					int newPitch = eventNoteOnOff->pitch() + semitonesOffset_;
					if (checkPitch_shouldDiscard(newPitch)) {
						return discard<Event>();
					}

					// Copy event
					std::shared_ptr<Event> ans = copy(unsafeEvent);

					// Change pitch
					((Event_NoteOnOff*)ans.get())->setPitch(newPitch);

					// Return
					return ans;
				}

			private:
				// Helper functions
				bool checkPitch_shouldDiscard(int pitch) const
				{
					if (pitch >= 0 && pitch <= 127) {
						// In the correct range: keep
						return false;
					} else {
						if (transposedPitchOutOfRange_ == TransposedPitchOutOfRange::error) {
							// Out of range: throw
							throw Exceptions::SampleOperatorFailed("transpose", "transposition results in out-of-range pitch.");
						} else if (transposedPitchOutOfRange_ == TransposedPitchOutOfRange::ignore) {
							// Out of range: discard
							return true;
						} else {
							throw 0; // won't happen
						}
					}
				}

				// Fields
				int semitonesOffset_;
				TransposedPitchOutOfRange transposedPitchOutOfRange_;
			};
		}

		Sample transpose(Sample const& sample, int semitonesOffset, TransposedPitchOutOfRange transposedPitchOutOfRange)
		{
			// Copy with transformation
			return sample.copyTransform(
				ElementTransformator_Transpose(semitonesOffset, transposedPitchOutOfRange)
			);
		}
	}
}
