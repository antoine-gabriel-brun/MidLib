#include "SampleOperators.hpp"

namespace MID
{
	namespace SampleOperators
	{
		namespace
		{
			class ElementTransformator_PitchMap : public ElementTransformatorBase
			{
			public:
				// Constructor
				ElementTransformator_PitchMap(std::array<int, 128> const& map) :
					map_(map)
				{ }

				// Implementations
				virtual std::unique_ptr<Note> transformNote(Note const& note) const override
				{
					// Get target pitch, discard note if -1
					int newPitch = map_[note.pitch()];
					if (newPitch == -1) {
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
					// Get target pitch, discard note if -1
					Event_NoteOnOff* eventNoteOnOff = dynamic_cast<Event_NoteOnOff*>(unsafeEvent.get());
					if (eventNoteOnOff == nullptr) {
						// Not an event with pitch: leave unchanged
						return copy(unsafeEvent);
					}
					int newPitch = map_[eventNoteOnOff->pitch()];
					if (newPitch == -1) {
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
				// Fields
				std::array<int, 128> map_;
			};
		}

		Sample pitchMap(Sample const& sample, std::array<int, 128> const& map)
		{
			// Check target values (0-127, or -1)
			for (int i = 0; i < 128; i++) {
				if (map[i] < -1 || map[i] > 127) {
					throw Exceptions::WrongSampleOperatorArgument("pitchMap", "map", "some target values in the map are out of bounds. Correct target pitches are integers between 0 and 127; the extra value -1 is also allowed.");
				}
			}
			
			// Copy with transformation
			return sample.copyTransform(
				ElementTransformator_PitchMap(map)
			);
		}
		Sample pitchMap(Sample const& sample, std::vector<int> const& map, UnspecifiedMapValue unspecifiedMapValue)
		{
			// Convert std::vector to std::array
			std::array<int, 128> map_array;
			{
				int mapSize = (int)map.size();
				if (mapSize > 128) {
					throw Exceptions::WrongSampleOperatorArgument("pitchMap", "map", "the map size should not exceed 128.");
				}
				for (int i = 0; i < mapSize; i++) {
					map_array[i] = map[i];
				}
				if (mapSize < 128) {
					if (unspecifiedMapValue == UnspecifiedMapValue::discard) {
						for (int i = mapSize; i < 128; i++) {
							map_array[i] = -1;
						}
					} else if (unspecifiedMapValue == UnspecifiedMapValue::leaveUnchanged) {
						for (int i = mapSize; i < 128; i++) {
							map_array[i] = i;
						}
					} else if (unspecifiedMapValue == UnspecifiedMapValue::error) {
						throw Exceptions::WrongSampleOperatorArgument("pitchMap", "map", "some target pitches are not specified by the map.");
					}
				}
			}

			// Perform operation
			return pitchMap(sample, map_array);
		}
		Sample pitchMap(Sample const& sample, std::map<int, int> const& map, UnspecifiedMapValue unspecifiedMapValue)
		{
			// Convert std::map to std::array
			std::array<int, 128> map_array;
			{
				for (std::pair<int, int> const& pair : map) {
					int source = pair.first;
					int target = pair.second;
					if (source < 0 || source > 127) {
						throw Exceptions::WrongSampleOperatorArgument("pitchMap", "map", "the map specifies a target value for an invalid source pitch value. Valid source pitch values are integers between 0 and 127.");
					}
					map_array[source] = target;
				}
				if (unspecifiedMapValue == UnspecifiedMapValue::discard) {
					for (int i = 0; i < 128; i++) {
						if (map.count(i) == 0) {
							map_array[i] = -1;
						}
					}
				} else if (unspecifiedMapValue == UnspecifiedMapValue::leaveUnchanged) {
					for (int i = 0; i < 128; i++) {
						if (map.count(i) == 0) {
							map_array[i] = i;
						}
					}
				} else if (unspecifiedMapValue == UnspecifiedMapValue::error) {
					for (int i = 0; i < 128; i++) {
						if (map.count(i) == 0) {
							throw Exceptions::WrongSampleOperatorArgument("pitchMap", "map", "some target pitches are not specified by the map.");
						}
					}
				}
			}

			// Perform operation
			return pitchMap(sample, map_array);
		}
	}
}
