#include "SampleOperators.hpp"

#include <algorithm>
#include <array>
#include <vector>
#include <set>
#include <utility>

namespace MID
{
	namespace SampleOperators
	{
		namespace
		{
			class ElementTransformator_Reverse : public ElementTransformatorBase
			{
			public:
				// Constructor
				ElementTransformator_Reverse(double end, Selector const& what, PreserveCurves preserveCurves) :
					end_(end),
					what_(what),
					preserveCurves_(preserveCurves)
				{ }

				// Implementations
				virtual std::unique_ptr<Note> transformNote(Note const& note) const override
				{
					if (what_.selectNotes()) {
						double newStart = end_ - note.end().time();
						double newEnd = end_ - note.start().time();

						if (newStart >= 0) {
							// The entire note is in the correct time range
							auto clone = copy(note);
							clone->startEnd(newStart, newEnd);
							return clone;
						} else if (newEnd >= 0) {
							// The note starts before the start of the sample, but ends in the correct time range
							auto clone = copy(note);
							clone->startEnd(0, newEnd);
							return clone;
						} else {
							// The entire note occurs before the start of the sample
							return discard<Note>();
						}
					} else {
						return copy(note);
					}
				}
				virtual std::unique_ptr<Event_PitchBend> transformPitchBend(Event_PitchBend const& pitchBend) const override
				{
					if (what_.selectPitchBends()) {
						if (preserveCurves_ == PreserveCurves::preserveCurves) {
							return discard<Event_PitchBend>(); // will be handled by the after() function
						} else {
							return transform(pitchBend);
						}
					} else {
						return copy(pitchBend);
					}
				}
				virtual std::unique_ptr<Event_ControlChange> transformControlChange(Event_ControlChange const& controlChange) const override
				{
					if (what_.selectControlChanges()) {
						if (preserveCurves_ == PreserveCurves::preserveCurves) {
							return discard<Event_ControlChange>(); // will be handled by the after() function
						} else {
							return transform(controlChange);
						}
					} else {
						return copy(controlChange);
					}
				}
				virtual std::unique_ptr<Event_ProgramChange> transformProgramChange(Event_ProgramChange const& programChange) const override
				{
					if (what_.selectProgramChanges()) {
						if (preserveCurves_ == PreserveCurves::preserveCurves) {
							return discard<Event_ProgramChange>(); // will be handled by the after() function
						} else {
							return transform(programChange);
						}
					} else {
						return copy(programChange);
					}
				}
				virtual std::unique_ptr<Event_ChannelAftertouch> transformChannelAftertouch(Event_ChannelAftertouch const& channelAftertouch) const override
				{
					if (what_.selectAftertouches()) {
						if (preserveCurves_ == PreserveCurves::preserveCurves) {
							return discard<Event_ChannelAftertouch>(); // will be handled by the after() function
						} else {
							return transform(channelAftertouch);
						}
					} else {
						return copy(channelAftertouch);
					}
				}
				virtual std::unique_ptr<Event_KeyAftertouch> transformKeyAftertouch(Event_KeyAftertouch const& keyAftertouch) const override
				{
					if (what_.selectAftertouches()) {
						if (preserveCurves_ == PreserveCurves::preserveCurves) {
							return discard<Event_KeyAftertouch>(); // will be handled by the after() function
						} else {
							return transform(keyAftertouch);
						}
					} else {
						return copy(keyAftertouch);
					}
				}
				virtual std::shared_ptr<Event> transformUnsafeEvent(std::shared_ptr<Event> const unsafeEvent) const override
				{
					if (what_.selectUnsafeEvents()) {
						double newPos = end_ - unsafeEvent->pos().time();

						if (newPos >= 0) {
							auto clone = copy(unsafeEvent);
							clone->setPos(newPos);
							return clone;
						} else {
							return discard<Event>();
						}
					} else {
						return copy(unsafeEvent);
					}
				}
				virtual void after(Track const& track_in, Track& track_out) const override
				{
					// Handle events whose automation curve should be preserved
					if (preserveCurves_ == PreserveCurves::preserveCurves) {
						handlePitchBends(track_in, track_out);
						handleControlChanges(track_in, track_out);
						handleProgramChanges(track_in, track_out);
						handleChannelAftertouches(track_in, track_out);
						handleKeyAftertouches(track_in, track_out);
					}
				}

			private:
				// Helper functions
				double transformPos(Pos const& pos) const
				{
					return end_ - pos.time();
				}
				template<typename T_Event>
				std::unique_ptr<T_Event> transform(T_Event const& ev) const
				{
					double newPos = transformPos(ev.pos());

					if (newPos >= 0) {
						auto clone = copy(ev);
						clone->setPos(newPos);
						return clone;
					} else {
						return discard<T_Event>();
					}
				}

				template<typename T_Event>
				static bool comparer(T_Event const& a, T_Event const& b)
				{
					return a.pos() < b.pos();
				}

				void handlePitchBends(Track const& track_in, Track& track_out) const
				{
					if (what_.selectPitchBends()) {
						// Find channels used for pitch bends
						std::array<bool, 16> used = { false };
						for (Event_PitchBend const& pitchBend : track_in.pitchBends()) {
							used[pitchBend.getChannel()] = true;
						}
						// Handle pitch bends
						for (int channel = 0; channel <= 15; channel++) {
							if (used[channel]) {
								handlePitchBends_preserveCurves(track_in.pitchBends(), track_out.pitchBends(), channel);
							}
						}
					}
				}
				void handleControlChanges(Track const& track_in, Track& track_out) const
				{
					if (what_.selectControlChanges()) {
						// Find channels and controllers used for control changes
						std::set<std::pair<char, char>> used;
						for (Event_ControlChange const& controlChange : track_in.controlChanges()) {
							used.insert({controlChange.getChannel(), controlChange.controller()});
						}
						// Handle program changes
						for (std::pair<char, char> const& channelAndController : used) {
							handleControlChanges_preserveCurves(track_in.controlChanges(), track_out.controlChanges(), channelAndController.first, channelAndController.second);
						}
					}
				}
				void handleProgramChanges(Track const& track_in, Track& track_out) const
				{
					if (what_.selectProgramChanges()) {
						// Find channels used for program changes
						std::array<bool, 16> used = { false };
						for (Event_ProgramChange const& programChange : track_in.programChanges()) {
							used[programChange.getChannel()] = true;
						}
						// Handle program changes
						for (int channel = 0; channel <= 15; channel++) {
							if (used[channel]) {
								handleProgramChanges_preserveCurves(track_in.programChanges(), track_out.programChanges(), channel);
							}
						}
					}
				}
				void handleChannelAftertouches(Track const& track_in, Track& track_out) const
				{
					if (what_.selectAftertouches()) {
						// Find channels used for channel aftertouches
						std::array<bool, 16> used = { false };
						for (Event_ChannelAftertouch const& channelAftertouch : track_in.channelAftertouches()) {
							used[channelAftertouch.getChannel()] = true;
						}
						// Handle channel aftertouches
						for (int channel = 0; channel <= 15; channel++) {
							if (used[channel]) {
								handleChannelAftertouches_preserveCurves(track_in.channelAftertouches(), track_out.channelAftertouches(), channel);
							}
						}
					}
				}
				void handleKeyAftertouches(Track const& track_in, Track& track_out) const
				{
					if (what_.selectAftertouches()) {
						// Find channels and pitches used for key aftertouches
						std::set<std::pair<char, char>> used;
						for (Event_KeyAftertouch const& keyAftertouch : track_in.keyAftertouches()) {
							used.insert({keyAftertouch.getChannel(), keyAftertouch.pitch()});
						}
						// Handle key aftertouches
						for (std::pair<char, char> const& channelAndPitch : used) {
							handleKeyAftertouches_preserveCurves(track_in.keyAftertouches(), track_out.keyAftertouches(), channelAndPitch.first, channelAndPitch.second);
						}
					}
				}

				void handlePitchBends_preserveCurves(std::vector<Event_PitchBend> const& events_in, std::vector<Event_PitchBend>& events_out, int channel) const
				{
					// Get pitch bends with correct channel
					std::vector<Event_PitchBend> ev;
					for (Event_PitchBend const& pitchBend : events_in) {
						if (pitchBend.getChannel() == channel) {
							ev.push_back(pitchBend);
						}
					}

					// Count them
					int nb = (int)ev.size();
					if (nb == 0) return;

					// Sort them chronologically
					std::stable_sort(ev.begin(), ev.end(), comparer<Event_PitchBend>);

					// Add initial pitch bend from the final value
					Event_PitchBend copy = ev.back();
					copy.setPos(0);
					events_out.push_back(copy);

					// Add pitch bends. Create an offset between values and positions to preserve curves.
					for (int i = nb - 1; i > 0; i--) {
						Event_PitchBend originalVal = ev[i - 1];
						Pos originalPos = ev[i].pos();
						Event_PitchBend pitchBend(
							Pos(end_ - originalPos.time()),
							originalVal.semitones_MSB(),
							originalVal.semitones_LSB(),
							originalVal.getChannel()
						);
						events_out.push_back(pitchBend);
					}

					// Add final pitch bend from the initial default value
					if (!ev[0].pos().isZero()) {
						Event_PitchBend pitchBend = Event_PitchBend(
							Pos(end_ - ev[0].pos().time()),
							(UINT_14)8192,
							ev[0].getChannel()
						);
						events_out.push_back(pitchBend);
					}
				}
				void handleControlChanges_preserveCurves(std::vector<Event_ControlChange> const& events_in, std::vector<Event_ControlChange>& events_out, int channel, int controller) const
				{
					// Get control changes with correct channel and controller index
					std::vector<Event_ControlChange> ev;
					for (Event_ControlChange const& controlChange : events_in) {
						if (controlChange.getChannel() == channel && controlChange.controller() == controller) {
							ev.push_back(controlChange);
						}
					}

					// Count them
					int nb = (int)ev.size();
					if (nb == 0) return;

					// Sort them chronologically
					std::stable_sort(ev.begin(), ev.end(), comparer<Event_ControlChange>);

					// Add initial control change from the final value
					Event_ControlChange copy = ev.back();
					copy.setPos(0);
					events_out.push_back(copy);

					// Add control changes. Create an offset between values and positions to preserve curves.
					for (int i = nb - 1; i > 0; i--) {
						Event_ControlChange originalVal = ev[i - 1];
						Pos originalPos = ev[i].pos();
						Event_ControlChange controlChange(
							end_ - originalPos.time(),
							originalVal.controller(),
							originalVal.value(),
							originalVal.getChannel()
						);
						events_out.push_back(controlChange);
					}

					// Add final control change from the initial default value
					if (!ev[0].pos().isZero()) {
						Event_ControlChange controlChange(
							end_ - ev[0].pos().time(),
							ev[0].controller(),
							64,
							ev[0].getChannel()
						);
						events_out.push_back(controlChange);
					}
				}
				void handleProgramChanges_preserveCurves(std::vector<Event_ProgramChange> const& events_in, std::vector<Event_ProgramChange>& events_out, int channel) const
				{
					// Get program changes with correct channel
					std::vector<Event_ProgramChange> ev;
					for (Event_ProgramChange const& programChange : events_in) {
						if (programChange.getChannel() == channel) {
							ev.push_back(programChange);
						}
					}

					// Count them
					int nb = (int)ev.size();
					if (nb == 0) return;

					// Sort them chronologically
					std::stable_sort(ev.begin(), ev.end(), comparer<Event_ProgramChange>);

					// Add initial program change from the final value
					Event_ProgramChange copy = ev.back();
					copy.setPos(0);
					events_out.push_back(copy);

					// Add program changes. Create an offset between values and positions to preserve curves.
					for (int i = nb - 1; i > 0; i--) {
						Event_ProgramChange originalVal = ev[i - 1];
						Pos originalPos = ev[i].pos();
						Event_ProgramChange programChange(
							Pos(end_ - originalPos.time()),
							originalVal.instrumentIndex(),
							originalVal.getChannel(),
							originalVal.specialChannel9()
						);
						events_out.push_back(programChange);
					}

					// Add final program change from the initial default value
					if (!ev[0].pos().isZero()) {
						Event_ProgramChange programChange(
							Pos(end_ - ev[0].pos().time()),
							0,
							ev[0].getChannel(),
							ev[0].specialChannel9()
						);
						events_out.push_back(programChange);
					}
				}
				void handleChannelAftertouches_preserveCurves(std::vector<Event_ChannelAftertouch> const& events_in, std::vector<Event_ChannelAftertouch>& events_out, int channel) const
				{
					// Get channel aftertouches with correct channel
					std::vector<Event_ChannelAftertouch> ev;
					for (Event_ChannelAftertouch const& channelAftertouch : events_in) {
						if (channelAftertouch.getChannel() == channel) {
							ev.push_back(channelAftertouch);
						}
					}

					// Count them
					int nb = (int)ev.size();
					if (nb == 0) return;

					// Sort them chronologically
					std::stable_sort(ev.begin(), ev.end(), comparer<Event_ChannelAftertouch>);

					// Add initial channel aftertouches from the final value
					Event_ChannelAftertouch copy = ev.back();
					copy.setPos(0);
					events_out.push_back(copy);

					// Add channel aftertouches. Create an offset between values and positions to preserve curves.
					for (int i = nb - 1; i > 0; i--) {
						Event_ChannelAftertouch originalVal = ev[i - 1];
						Pos originalPos = ev[i].pos();
						Event_ChannelAftertouch channelAftertouch(
							Pos(end_ - originalPos.time()),
							originalVal.pressure(),
							originalVal.getChannel()
						);
						events_out.push_back(channelAftertouch);
					}

					// Add final channel aftertouch from the initial default value
					if (!ev[0].pos().isZero()) {
						Event_ChannelAftertouch channelAftertouch(
							Pos(end_ - ev[0].pos().time()),
							64,
							ev[0].getChannel()
						);
						events_out.push_back(channelAftertouch);
					}
				}
				void handleKeyAftertouches_preserveCurves(std::vector<Event_KeyAftertouch> const& events_in, std::vector<Event_KeyAftertouch>& events_out, int channel, int pitch) const
				{
					// Get key aftertouches with correct channel and pitch
					std::vector<Event_KeyAftertouch> ev;
					for (Event_KeyAftertouch const& keyAftertouch : events_in) {
						if (keyAftertouch.getChannel() == channel && keyAftertouch.pitch() == pitch) {
							ev.push_back(keyAftertouch);
						}
					}

					// Count them
					int nb = (int)ev.size();
					if (nb == 0) return;

					// Sort them chronologically
					std::stable_sort(ev.begin(), ev.end(), comparer<Event_KeyAftertouch>);

					// Add initial key aftertouch from the final value
					Event_KeyAftertouch copy = ev.back();
					copy.setPos(0);
					events_out.push_back(copy);

					// Add key aftertouches. Create an offset between values and positions to preserve curves.
					for (int i = nb - 1; i > 0; i--) {
						Event_KeyAftertouch originalVal = ev[i - 1];
						Pos originalPos = ev[i].pos();
						Event_KeyAftertouch keyAftertouch(
							Pos(end_ - originalPos.time()),
							originalVal.pitch(),
							originalVal.pressure(),
							originalVal.getChannel()
						);
						events_out.push_back(keyAftertouch);
					}

					// Add final key aftertouch from the initial default value
					if (!ev[0].pos().isZero()) {
						Event_KeyAftertouch keyAftertouch(
							Pos(end_ - ev[0].pos().time()),
							ev[0].pitch(),
							64,
							ev[0].getChannel()
						);
						events_out.push_back(keyAftertouch);
					}
				}

				// Fields
				double end_;
				Selector what_;
				PreserveCurves preserveCurves_;
			};
		}

		Sample reverse(Sample const& sample, Selector const& what, PreserveCurves preserveCurves)
		{
			// Find the last time position
			Pos end = sample.calcEndPos();

			// Copy with transformation
			return sample.copyTransform(
				ElementTransformator_Reverse(end.time(), what, preserveCurves)
			);
		}
		Sample reverse(Sample const& sample, PreserveCurves preserveCurves)
		{
			return reverse(sample, Selector::all(), preserveCurves);
		}
	}
}
