#include "SampleOperators.hpp"

#include <array>

namespace MID
{
	namespace SampleOperators
	{
		namespace
		{
			class ElementTransformator_TrimTime : public ElementTransformatorBase
			{
			public:
				// Constructors
				ElementTransformator_TrimTime(double rangeBegin, double rangeEnd, PreserveCurves preserveCurves) :
					rangeBegin_(rangeBegin),
					rangeEnd_(rangeEnd),
					ignoreEnd_(false),
					preserveCurves_(preserveCurves)
				{ }
				ElementTransformator_TrimTime(double rangeBegin, PreserveCurves preserveCurves) :
					rangeBegin_(rangeBegin),
					rangeEnd_(0.),
					ignoreEnd_(true),
					preserveCurves_(preserveCurves)
				{ }

				// Implementations
				virtual void before(Track const& track_in, Track& track_out) const override
				{
					// Handle events whose automation curve should be preserved
					if (preserveCurves_ == PreserveCurves::preserveCurves) {
						addInitialPitchBendsIfNeeded_preserveCurves(track_in.pitchBends(), track_out.pitchBends());
						addInitialControlChangesIfNeeded_preserveCurves(track_in.controlChanges(), track_out.controlChanges());
						addInitialProgramChangesIfNeeded_preserveCurves(track_in.programChanges(), track_out.programChanges());
						addInitialChannelAftertouchIfNeeded_preserveCurves(track_in.channelAftertouches(), track_out.channelAftertouches());
						addInitialKeyAftertouchIfNeeded_preserveCurves(track_in.keyAftertouches(), track_out.keyAftertouches());
					}
				}
				virtual std::unique_ptr<Note> transformNote(Note const& note) const override
				{
					double posStart = note.start().time();
					double posEnd = note.end().time();
					double newPosStart = posStart - rangeBegin_;
					double newPosEnd = posEnd - rangeBegin_;
					
					if (posEnd <= rangeBegin_ || (!ignoreEnd_ && posStart >= rangeEnd_)) {
						// The entire note occurs before the start of the range or after its end
						return discard<Note>();
					} else if (posStart >= rangeBegin_ && (ignoreEnd_ || posEnd <= rangeEnd_)) {
						// The entire note is within the range
						auto clone = copy(note);
						clone->startEnd(newPosStart, newPosEnd);
						return clone;
					} else if (ignoreEnd_ || posEnd <= rangeEnd_) {
						// The note starts before the start of the range, but ends within the range
						auto clone = copy(note);
						clone->startEnd(0, newPosEnd);
						return clone;
					} else if (posStart >= rangeBegin_) {
						// The note starts within the range, but ends after its end
						auto clone = copy(note);
						clone->startEnd(newPosStart, rangeEnd_ - rangeBegin_);
						return clone;
					} else {
						// The note starts before the start of the range and ends after its end
						auto clone = copy(note);
						clone->startEnd(0, rangeEnd_ - rangeBegin_);
						return clone;
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
					double pos = unsafeEvent->pos().time();
					
					if (pos >= rangeBegin_ && (ignoreEnd_ || pos <= rangeEnd_)) {
						double newPos = pos - rangeBegin_;
						auto clone = copy(unsafeEvent);
						clone->setPos(newPos);
						return clone;
					} else {
						return discard<Event>();
					}
				}

			private:
				// Helper functions
				template<typename T_Event>
				std::unique_ptr<T_Event> transform(T_Event const& ev) const
				{
					double pos = ev.pos().time();
					
					if (pos >= rangeBegin_ && (ignoreEnd_ || pos <= rangeEnd_)) {
						double newPos = pos - rangeBegin_;
						auto clone = copy(ev);
						clone->setPos(newPos);
						return clone;
					} else {
						return discard<T_Event>();
					}
				}
				
				void addInitialPitchBendsIfNeeded_preserveCurves(std::vector<Event_PitchBend> const& events_in, std::vector<Event_PitchBend>& events_out) const
				{
					// On each channel, get the last pitch bend added before the range begins
					std::array<Event_PitchBend const*, 16> lastValues = { nullptr };
					for (Event_PitchBend const& pitchBend : events_in) {
						int channel = pitchBend.getChannel();
						double pos = pitchBend.pos().time();
						Event_PitchBend const*& last = lastValues[channel];
						if (pos <= rangeBegin_ && (last == nullptr || pos >= last->pos().time())) {
							last = &pitchBend;
						}
					}

					// For each channel that has a pitch bend value before the range begins, add it at t=0
					for (int channel = 0; channel <= 15; channel++) {
						Event_PitchBend const* last = lastValues[channel];
						if (last != nullptr && last->pos().time() < rangeBegin_) {
							Event_PitchBend pitchBend(
								Pos(0),
								last->semitones_MSB(),
								last->semitones_LSB(),
								last->getChannel()
							);
							events_out.push_back(pitchBend);
						}
					}
				}
				void addInitialControlChangesIfNeeded_preserveCurves(std::vector<Event_ControlChange> const& events_in, std::vector<Event_ControlChange>& events_out) const
				{
					// On each channel and for each controller, get the last control change added before the range begins
					std::vector<std::array<Event_ControlChange const*, 128>> lastValues(16);
					for (int i = 0; i < 16; i++) lastValues[i] = { nullptr };
					for (Event_ControlChange const& controlChange: events_in) {
						int channel = controlChange.getChannel();
						int controller = controlChange.controller();
						double pos = controlChange.pos().time();
						Event_ControlChange const*& last = lastValues[channel][controller];
						if (pos <= rangeBegin_ && (last == nullptr || pos >= last->pos().time())) {
							last = &controlChange;
						}
					}

					// For each channel that has a control change before the range begins, add it at t=0
					for (int channel = 0; channel <= 15; channel++) {
						for (int controller = 0; controller <= 127; controller++) {
							Event_ControlChange const* last = lastValues[channel][controller];
							if (last != nullptr && last->pos().time() < rangeBegin_) {
								Event_ControlChange controlChange(
									0,
									last->controller(),
									last->value(),
									last->getChannel()
								);
								events_out.push_back(controlChange);
							}
						}
					}
				}
				void addInitialProgramChangesIfNeeded_preserveCurves(std::vector<Event_ProgramChange> const& events_in, std::vector<Event_ProgramChange>& events_out) const
				{
					// On each channel, get the last program change added before the range begins
					std::array<Event_ProgramChange const*, 16> lastValues = { nullptr };
					for (Event_ProgramChange const& programChange: events_in) {
						int channel = programChange.getChannel();
						double pos = programChange.pos().time();
						Event_ProgramChange const*& last = lastValues[channel];
						if (pos <= rangeBegin_ && (last == nullptr || pos >= last->pos().time())) {
							last = &programChange;
						}
					}

					// For each channel that has a program change before the range begins, add it at t=0
					for (int channel = 0; channel <= 15; channel++) {
						Event_ProgramChange const* last = lastValues[channel];
						if (last != nullptr && last->pos().time() < rangeBegin_) {
							Event_ProgramChange programChange(
								Pos(0),
								last->instrumentIndex(),
								last->getChannel(),
								last->specialChannel9()
							);
							events_out.push_back(programChange);
						}
					}
				}
				void addInitialChannelAftertouchIfNeeded_preserveCurves(std::vector<Event_ChannelAftertouch> const& events_in, std::vector<Event_ChannelAftertouch>& events_out) const
				{
					// On each channel, get the last channel aftertouch added before the range begins
					std::array<Event_ChannelAftertouch const*, 16> lastValues = { nullptr };
					for (Event_ChannelAftertouch const& channelAftertouch : events_in) {
						int channel = channelAftertouch.getChannel();
						double pos = channelAftertouch.pos().time();
						Event_ChannelAftertouch const*& last = lastValues[channel];
						if (pos <= rangeBegin_ && (last == nullptr || pos >= last->pos().time())) {
							last = &channelAftertouch;
						}
					}

					// For each channel that has a channel aftertouch value before the range begins, add it at t=0
					for (int channel = 0; channel <= 15; channel++) {
						Event_ChannelAftertouch const* last = lastValues[channel];
						if (last != nullptr && last->pos().time() < rangeBegin_) {
							Event_ChannelAftertouch channelAftertouch(
								Pos(0),
								last->pressure(),
								last->getChannel()
							);
							events_out.push_back(channelAftertouch);
						}
					}
				}
				void addInitialKeyAftertouchIfNeeded_preserveCurves(std::vector<Event_KeyAftertouch> const& events_in, std::vector<Event_KeyAftertouch>& events_out) const
				{
					// On each channel and for each pitch, get the last key aftertouch added before the range begins
					std::vector<std::array<Event_KeyAftertouch const*, 128>> lastValues(16);
					for (int i = 0; i < 16; i++) lastValues[i] = { nullptr };
					for (Event_KeyAftertouch const& keyAftertouch: events_in) {
						int channel = keyAftertouch.getChannel();
						int controller = keyAftertouch.pitch();
						double pos = keyAftertouch.pos().time();
						Event_KeyAftertouch const*& last = lastValues[channel][controller];
						if (pos <= rangeBegin_ && (last == nullptr || pos >= last->pos().time())) {
							last = &keyAftertouch;
						}
					}

					// For each channel that has a key aftertouch value before the range begins, add it at t=0
					for (int channel = 0; channel <= 15; channel++) {
						for (int pitch = 0; pitch <= 127; pitch++) {
							Event_KeyAftertouch const* last = lastValues[channel][pitch];
							if (last != nullptr && last->pos().time() < rangeBegin_) {
								Event_KeyAftertouch keyAftertouch(
									Pos(0),
									last->pitch(),
									last->pressure(),
									last->getChannel()
								);
								events_out.push_back(keyAftertouch);
							}
						}
					}
				}

				// Fields
				double rangeBegin_;
				double rangeEnd_;
				bool ignoreEnd_;
				PreserveCurves preserveCurves_;
			};
		}
		
		Sample trim(Sample const& sample, double rangeBegin, double rangeEnd, PreserveCurves preserveCurves)
		{
			// Check for trim positions
			if (rangeBegin < 0.) {
				throw Exceptions::WrongSampleOperatorArgument("trimTime", "rangeBegin", "the position should note be negative.");
			} else if (sample.dur().isSpecified() && rangeBegin > sample.dur().time()) {
				throw Exceptions::WrongSampleOperatorArgument("trimTime", "rangeBegin", "if the sample duration is specified, the position should not exceed it.");
			} else if (rangeEnd < 0.) {
				throw Exceptions::WrongSampleOperatorArgument("trimTime", "rangeEnd", "the position should note be negative.");
			} else if (sample.dur().isSpecified() && rangeEnd > sample.dur().time()) {
				throw Exceptions::WrongSampleOperatorArgument("trimTime", "rangeEnd", "if the sample duration is specified, the position should not exceed it.");
			}

			// Get and check duration
			double rangeDur = rangeEnd - rangeBegin;
			if (rangeDur < 0) {
				throw Exceptions::WrongSampleOperatorArgument("trimTime", "rangeEnd", "the end of the range should come after the beginning of the range.");
			}
			
			// Copy with transformation
			Sample ans = sample.copyTransform(
				ElementTransformator_TrimTime(rangeBegin, rangeEnd, preserveCurves)
			);

			// Adjust the duration, if specified
			if (ans.dur().isSpecified()) {
				ans.dur(rangeDur);
			}

			// Return
			return ans;
		}
		Sample trim(Sample const& sample, double rangeBegin, PreserveCurves preserveCurves)
		{
			// Check for trim positions
			if (rangeBegin < 0.) {
				throw Exceptions::WrongSampleOperatorArgument("trimTime", "rangeBegin", "the position should note be negative.");
			} else if (sample.dur().isSpecified() && rangeBegin > sample.dur().time()) {
				throw Exceptions::WrongSampleOperatorArgument("trimTime", "rangeBegin", "if the sample duration is specified, the position should not exceed it.");
			}
			
			// Copy with transformation
			Sample ans = sample.copyTransform(
				ElementTransformator_TrimTime(rangeBegin, preserveCurves)
			);

			// Adjust the duration, if specified
			if (ans.dur().isSpecified()) {
				ans.dur(ans.dur().time() - rangeBegin);
			}

			// Return
			return ans;
		}
	}
}
