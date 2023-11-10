#include "SampleOperators.hpp"

namespace MID
{
	namespace SampleOperators
	{
		namespace
		{
			class ElementTransformator_ChannelMap : public ElementTransformatorBase
			{
			public:
				// Constructor
				ElementTransformator_ChannelMap(std::array<int, 16> const& map, Selector const& what) :
					map_(map),
					what_(what)
				{ }

				// Implementations
				virtual std::unique_ptr<Note> transformNote(Note const& note) const override
				{
					if (what_.selectNotes()) {
						// Get target channel, discard note if -1
						int newChannel = map_[note.channel()];
						if (newChannel == -1) {
							return discard<Note>();
						}

						// Copy note
						std::unique_ptr<Note> ans = copy(note);

						// Change channel
						ans->channel(newChannel);

						// Return
						return ans;
					} else {
						// Copy without transformation
						return copy(note);
					}
				}
				virtual std::unique_ptr<Event_PitchBend> transformPitchBend(Event_PitchBend const& pitchBend) const override
				{
					if (what_.selectPitchBends()) {
						// Transform
						return transformEvent(pitchBend);
					} else {
						// Copy without transformation
						return copy(pitchBend);
					}
				}
				virtual std::unique_ptr<Event_ControlChange> transformControlChange(Event_ControlChange const& controlChange) const override
				{
					if (what_.selectControlChanges()) {
						// Transform
						return transformEvent(controlChange);
					} else {
						// Copy without transformation
						return copy(controlChange);
					}
				}
				virtual std::unique_ptr<Event_ProgramChange> transformProgramChange(Event_ProgramChange const& programChange) const override
				{
					if (what_.selectProgramChanges()) {
						// Transform
						return transformEvent(programChange);
					} else {
						// Copy without transformation
						return copy(programChange);
					}
				}
				virtual std::unique_ptr<Event_ChannelAftertouch> transformChannelAftertouch(Event_ChannelAftertouch const& channelAftertouch) const override
				{
					if (what_.selectAftertouches()) {
						// Transform
						return transformEvent(channelAftertouch);
					} else {
						// Copy without transformation
						return copy(channelAftertouch);
					}
				}
				virtual std::unique_ptr<Event_KeyAftertouch> transformKeyAftertouch(Event_KeyAftertouch const& keyAftertouch) const override
				{
					if (what_.selectAftertouches()) {
						// Transform
						return transformEvent(keyAftertouch);
					} else {
						// Copy without transformation
						return copy(keyAftertouch);
					}
				}
				virtual std::shared_ptr<Event> transformUnsafeEvent(std::shared_ptr<Event> const unsafeEvent) const override
				{
					if (what_.selectUnsafeEvents()) {
						// Get target channel, discard event if -1
						Event_Midi* eventMidi = dynamic_cast<Event_Midi*>(unsafeEvent.get());
						if (eventMidi == nullptr) {
							// Not an event with channel: leave unchanged
							return copy(unsafeEvent);
						}
						int oldChannel = eventMidi->getChannel();
						int newChannel = map_[oldChannel];
						if (newChannel == -1) {
							return discard<Event>();
						}

						// Copy event
						std::shared_ptr<Event> ans = copy(unsafeEvent);

						// Change channel
						((Event_Midi*)ans.get())->setChannel(newChannel);

						// Return
						return ans;
					} else {
						// Copy without transformation
						return copy(unsafeEvent);
					}
				}

			private:
				// Helper functions
				template<typename T_Event>
				std::unique_ptr<T_Event> transformEvent(T_Event const& eventToTransform) const
				{
					// Get target channel, discard event if -1
					int oldChannel = ((Event_Midi*)&eventToTransform)->getChannel();
					int newChannel = map_[oldChannel];
					if (newChannel == -1) {
						return discard<T_Event>();
					}

					// Copy event
					std::unique_ptr<T_Event> ans = copy(eventToTransform);

					// Change channel
					ans->channel(newChannel);

					// Return
					return ans;
				}

				// Fields
				std::array<int, 16> map_;
				Selector what_;
			};
		}

		Sample channelMap(Sample const& sample, std::array<int, 16> const& map, Selector const& what)
		{
			// Check target values (0-15, or -1)
			for (int i = 0; i < 16; i++) {
				if (map[i] < -1 || map[i] > 15) {
					throw Exceptions::WrongSampleOperatorArgument("channelMap", "map", "some target values in the map are out of bounds. Correct target channels are integers between 0 and 15; the extra value -1 is also allowed.");
				}
			}
			
			// Copy with transformation
			return sample.copyTransform(
				ElementTransformator_ChannelMap(map, what)
			);
		}
		Sample channelMap(Sample const& sample, std::vector<int> const& map, Selector const& what, UnspecifiedMapValue unspecifiedMapValue)
		{
			// Convert std::vector to std::array
			std::array<int, 16> map_array;
			{
				int mapSize = (int)map.size();
				if (mapSize > 16) {
					throw Exceptions::WrongSampleOperatorArgument("channelMap", "map", "the map size should not exceed 16.");
				}
				for (int i = 0; i < mapSize; i++) {
					map_array[i] = map[i];
				}
				if (mapSize < 16) {
					if (unspecifiedMapValue == UnspecifiedMapValue::discard) {
						for (int i = mapSize; i < 16; i++) {
							map_array[i] = -1;
						}
					} else if (unspecifiedMapValue == UnspecifiedMapValue::leaveUnchanged) {
						for (int i = mapSize; i < 16; i++) {
							map_array[i] = i;
						}
					} else if (unspecifiedMapValue == UnspecifiedMapValue::error) {
						throw Exceptions::WrongSampleOperatorArgument("channelMap", "map", "some target channels are not specified by the map.");
					}
				}
			}

			// Perform operation
			return channelMap(sample, map_array, what);
		}
		Sample channelMap(Sample const& sample, std::map<int, int> const& map, Selector const& what, UnspecifiedMapValue unspecifiedMapValue)
		{
			// Convert std::map to std::array
			std::array<int, 16> map_array;
			{
				for (std::pair<int, int> const& pair : map) {
					int source = pair.first;
					int target = pair.second;
					if (source < 0 || source > 15) {
						throw Exceptions::WrongSampleOperatorArgument("channelMap", "map", "the map specifies a target value for an invalid source channel value. Valid source channel values are integers between 0 and 15.");
					}
					map_array[source] = target;
				}
				if (unspecifiedMapValue == UnspecifiedMapValue::discard) {
					for (int i = 0; i < 16; i++) {
						if (map.count(i) == 0) {
							map_array[i] = -1;
						}
					}
				} else if (unspecifiedMapValue == UnspecifiedMapValue::leaveUnchanged) {
					for (int i = 0; i < 16; i++) {
						if (map.count(i) == 0) {
							map_array[i] = i;
						}
					}
				} else if (unspecifiedMapValue == UnspecifiedMapValue::error) {
					for (int i = 0; i < 16; i++) {
						if (map.count(i) == 0) {
							throw Exceptions::WrongSampleOperatorArgument("channelMap", "map", "some target channels are not specified by the map.");
						}
					}
				}
			}

			// Perform operation
			return channelMap(sample, map_array, what);
		}
		Sample channelMap(Sample const& sample, std::vector<int> const& map, UnspecifiedMapValue unspecifiedMapValue)
		{
			return channelMap(sample, map, Selector::all(), unspecifiedMapValue);
		}
		Sample channelMap(Sample const& sample, std::map<int, int> const& map, UnspecifiedMapValue unspecifiedMapValue)
		{
			return channelMap(sample, map, Selector::all(), unspecifiedMapValue);
		}
	}
}
