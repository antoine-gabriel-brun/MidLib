#include "Track.hpp"

namespace MID
{
	void Track::add(Note const& note)
	{
		notes_.push_back(note);
	}
	void Track::add(Event_PitchBend const& pitchBend)
	{
		pitchBends_.push_back(pitchBend);
	}
	void Track::add(Event_ControlChange const& controlChange)
	{
		controlChanges_.push_back(controlChange);
	}
	void Track::add(Event_ProgramChange const& programChange)
	{
		programChanges_.push_back(programChange);
	}
	void Track::add(Event_ChannelAftertouch const& channelAftertouch)
	{
		channelAftertouches_.push_back(channelAftertouch);
	}
	void Track::add(Event_KeyAftertouch const& keyAftertouch)
	{
		keyAftertouches_.push_back(keyAftertouch);
	}

	std::vector<Note>& Track::notes()
	{
		return notes_;
	}
	std::vector<Event_PitchBend>& Track::pitchBends()
	{
		return pitchBends_;
	}
	std::vector<Event_ControlChange>& Track::controlChanges()
	{
		return controlChanges_;
	}
	std::vector<Event_ProgramChange>& Track::programChanges()
	{
		return programChanges_;
	}
	std::vector<Event_ChannelAftertouch>& Track::channelAftertouches()
	{
		return channelAftertouches_;
	}
	std::vector<Event_KeyAftertouch>& Track::keyAftertouches()
	{
		return keyAftertouches_;
	}
	std::vector<std::shared_ptr<Event>>& Track::unsafeEvents()
	{
		return unsafeEvents_;
	}

	std::vector<Note> const& Track::notes() const
	{
		return notes_;
	}
	std::vector<Event_PitchBend> const& Track::pitchBends() const
	{
		return pitchBends_;
	}
	std::vector<Event_ControlChange> const& Track::controlChanges() const
	{
		return controlChanges_;
	}
	std::vector<Event_ProgramChange> const& Track::programChanges() const
	{
		return programChanges_;
	}
	std::vector<Event_ChannelAftertouch> const& Track::channelAftertouches() const
	{
		return channelAftertouches_;
	}
	std::vector<Event_KeyAftertouch> const& Track::keyAftertouches() const
	{
		return keyAftertouches_;
	}
	std::vector<std::shared_ptr<Event>> const& Track::unsafeEvents() const
	{
		return unsafeEvents_;
	}

	size_t Track::nbNotes() const
	{
		return notes_.size();
	}
	size_t Track::nbPitchBends() const
	{
		return pitchBends_.size();
	}
	size_t Track::nbControlChanges() const
	{
		return controlChanges_.size();
	}
	size_t Track::nbProgramChanges() const
	{
		return programChanges_.size();
	}
	size_t Track::nbChannelAftertouches() const
	{
		return channelAftertouches_.size();
	}
	size_t Track::nbKeyAftertouches() const
	{
		return keyAftertouches_.size();
	}
	size_t Track::nbUnsafeEvents() const
	{
		return unsafeEvents_.size();
	}
	
	Note& Track::note(unsigned int index)
	{
		return notes_[index];
	}
	Event_PitchBend& Track::pitchBend(unsigned int index)
	{
		return pitchBends_[index];
	}
	Event_ControlChange& Track::controlChange(unsigned int index)
	{
		return controlChanges_[index];
	}
	Event_ProgramChange& Track::programChange(unsigned int index)
	{
		return programChanges_[index];
	}
	Event_ChannelAftertouch& Track::channelAftertouch(unsigned int index)
	{
		return channelAftertouches_[index];
	}
	Event_KeyAftertouch& Track::keyAftertouch(unsigned int index)
	{
		return keyAftertouches_[index];
	}
	std::shared_ptr<Event>& Track::unsafeEvent(unsigned int index)
	{
		return unsafeEvents_[index];
	}

	Note const& Track::note(unsigned int index) const
	{
		return notes_[index];
	}
	Event_PitchBend const& Track::pitchBend(unsigned int index) const
	{
		return pitchBends_[index];
	}
	Event_ControlChange const& Track::controlChange(unsigned int index) const
	{
		return controlChanges_[index];
	}
	Event_ProgramChange const& Track::programChange(unsigned int index) const
	{
		return programChanges_[index];
	}
	Event_ChannelAftertouch const& Track::channelAftertouch(unsigned int index) const
	{
		return channelAftertouches_[index];
	}
	Event_KeyAftertouch const& Track::keyAftertouch(unsigned int index) const
	{
		return keyAftertouches_[index];
	}
	std::shared_ptr<Event> const& Track::unsafeEvent(unsigned int index) const
	{
		return unsafeEvents_[index];
	}

	Note& Track::operator[](unsigned int index)
	{
		return note(index);
	}
	Note const& Track::operator[](unsigned int index) const
	{
		return note(index);
	}

	Pos Track::calcEndPos() const
	{
		Pos maxPos = Pos(0.);
		
		for (Note                    const& note              : notes_              ) maxPos = max(maxPos, note             .end());
		for (Event_PitchBend         const& pitchBend         : pitchBends_         ) maxPos = max(maxPos, pitchBend        .pos());
		for (Event_ControlChange     const& controlChange     : controlChanges_     ) maxPos = max(maxPos, controlChange    .pos());
		for (Event_ProgramChange     const& programChange     : programChanges_     ) maxPos = max(maxPos, programChange    .pos());
		for (Event_ChannelAftertouch const& channelAftertouch : channelAftertouches_) maxPos = max(maxPos, channelAftertouch.pos());
		for (Event_KeyAftertouch     const& keyAftertouch     : keyAftertouches_    ) maxPos = max(maxPos, keyAftertouch    .pos());
		for (std::shared_ptr<Event>  const  unsafeEvent       : unsafeEvents_       ) maxPos = max(maxPos, unsafeEvent     ->pos());
		
		return maxPos;
	}

	namespace
	{
		template<typename T>
		void insertElements(std::vector<T>& destination, std::vector<T> const& source)
		{
			destination.insert(destination.end(), source.begin(), source.end());
		}
		void insertElementClones(std::vector<std::shared_ptr<Event>>& destination, std::vector<std::shared_ptr<Event>> const& source)
		{
			for (std::shared_ptr<Event> const& unsafeEvent : source) {
				destination.push_back(unsafeEvent->clone());
			}
		}
	}
	Track Track::merge(Track const& track1, Track const& track2)
	{
		// Copy the first track
		Track ans = track1;

		// Add the elements of the second track
		insertElements(ans.notes_              , track2.notes_              );
		insertElements(ans.pitchBends_         , track2.pitchBends_         );
		insertElements(ans.controlChanges_     , track2.controlChanges_     );
		insertElements(ans.programChanges_     , track2.programChanges_     );
		insertElements(ans.channelAftertouches_, track2.channelAftertouches_);
		insertElements(ans.keyAftertouches_    , track2.keyAftertouches_    );
		insertElementClones(ans.unsafeEvents_  , track2.unsafeEvents_       );

		// Return
		return ans;
	}
	namespace
	{
		void concatNotes(std::vector<Note>& destination, std::vector<Note> const& source1, std::vector<Note> const& source2, double timeOffset)
		{
			for (Note const& note : source1) {
				double start = note.start().time();
				double end = note.end().time();
				
				if (end <= timeOffset) {
					destination.push_back(note);
				} else if (start <= timeOffset) {
					Note copy = note;
					copy.startEnd(start, timeOffset);
					destination.push_back(copy);
				}
			}
			for (Note const& note : source2) {
				double start = note.start().time();
				double end = note.end().time();

				Note copy = note;
				copy.startEnd(timeOffset + start, timeOffset + end);
				destination.push_back(copy);
			}
		}
		template<typename T>
		void concatElements(std::vector<T>& destination, std::vector<T> const& source1, std::vector<T> const& source2, double timeOffset)
		{
			for (T const& ev : source1) {
				if (ev.pos().time() <= timeOffset) {
					destination.push_back(ev);
				}
			}
			for (T const& ev : source2) {
				T copy = ev;
				copy.setPos(timeOffset + copy.pos().time());
				destination.push_back(copy);
			}
		}
		void concatElementClones(std::vector<std::shared_ptr<Event>>& destination, std::vector<std::shared_ptr<Event>> const& source1, std::vector<std::shared_ptr<Event>> const& source2, double timeOffset)
		{
			for (std::shared_ptr<Event> const ev : source1) {
				if (ev->pos().time() <= timeOffset) {
					destination.push_back(ev->clone());
				}
			}
			for (std::shared_ptr<Event> const ev : source2) {
				std::shared_ptr<Event> copy = ev->clone();
				copy->setPos(timeOffset + copy->pos().time());
				destination.push_back(copy);
			}
		}
	}
	Track Track::concat(Track const& track1, Track const& track2, double timeOffset)
	{
		// Create an empty track
		Track ans;

		// Add the elements of the two tracks
		concatNotes   (ans.notes_              , track1.notes_              , track2.notes_              , timeOffset);
		concatElements(ans.pitchBends_         , track1.pitchBends_         , track2.pitchBends_         , timeOffset);
		concatElements(ans.controlChanges_     , track1.controlChanges_     , track2.controlChanges_     , timeOffset);
		concatElements(ans.programChanges_     , track1.programChanges_     , track2.programChanges_     , timeOffset);
		concatElements(ans.channelAftertouches_, track1.channelAftertouches_, track2.channelAftertouches_, timeOffset);
		concatElements(ans.keyAftertouches_    , track1.keyAftertouches_    , track2.keyAftertouches_    , timeOffset);
		concatElementClones(ans.unsafeEvents_  , track1.unsafeEvents_       , track2.unsafeEvents_       , timeOffset);

		// Return
		return ans;
	}

	Track Track::copyTransform(ElementTransformatorBase const& transformator) const
	{
		// Create a new empty track
		Track ans;

		// Do what needs to be done at the beginning
		transformator.before(*this, ans);

		// Copy and transform elements
		for (Note                    const& init : notes_              ) { std::unique_ptr<Note                    > transf = transformator.transformNote             (init); if (transf != nullptr) { ans.notes_              .push_back(*transf); } }
		for (Event_PitchBend         const& init : pitchBends_         ) { std::unique_ptr<Event_PitchBend         > transf = transformator.transformPitchBend        (init); if (transf != nullptr) { ans.pitchBends_         .push_back(*transf); } }
		for (Event_ControlChange     const& init : controlChanges_     ) { std::unique_ptr<Event_ControlChange     > transf = transformator.transformControlChange    (init); if (transf != nullptr) { ans.controlChanges_     .push_back(*transf); } }
		for (Event_ProgramChange     const& init : programChanges_     ) { std::unique_ptr<Event_ProgramChange     > transf = transformator.transformProgramChange    (init); if (transf != nullptr) { ans.programChanges_     .push_back(*transf); } }
		for (Event_ChannelAftertouch const& init : channelAftertouches_) { std::unique_ptr<Event_ChannelAftertouch > transf = transformator.transformChannelAftertouch(init); if (transf != nullptr) { ans.channelAftertouches_.push_back(*transf); } }
		for (Event_KeyAftertouch     const& init : keyAftertouches_    ) { std::unique_ptr<Event_KeyAftertouch     > transf = transformator.transformKeyAftertouch    (init); if (transf != nullptr) { ans.keyAftertouches_    .push_back(*transf); } }
		for (std::shared_ptr<Event>  const  init : unsafeEvents_       ) { std::shared_ptr<Event                   > transf = transformator.transformUnsafeEvent      (init); if (transf != nullptr) { ans.unsafeEvents_       .push_back( transf); } }

		// Do what needs to be done at the end
		transformator.after(*this, ans);

		// Return
		return ans;
	}
}
