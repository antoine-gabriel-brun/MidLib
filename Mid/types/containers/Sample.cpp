#include "Sample.hpp"

#include "../../writefile/write-midi-file.hpp"
#include "../tools/Exceptions.hpp"
#include "../sample-operators/SampleOperators.hpp"

namespace MID
{
	Sample::Sample(size_t nbTracks) :
		ticksPerQuarter_(TicksPerQuarter::defaultValue),
		tempo_(Tempo::createDefault()),
		timeSignature_(TimeSignature::createDefault()),
		dur_(SampleDur::unspecified()),
		overlappingNotes_(OverlappingNotes::trim),
		tracks_(nbTracks),
		currentTrack_(0u)
	{ }

	Sample& Sample::ticksPerQuarter(UINT_16 ticksPerQuarter)
	{
		if (ticksPerQuarter > 0x7FFFu) {
			throw Exceptions::WrongTicksPerQuarterNote(true);
		} else if (ticksPerQuarter == 0) {
			throw Exceptions::WrongTicksPerQuarterNote(false);
		} else {
			ticksPerQuarter_ = ticksPerQuarter;
			return *this;
		}
	}
	Sample& Sample::ticksPerQuarterCoarse()
	{
		ticksPerQuarter_ = TicksPerQuarter::coarse;
		return *this;
	}
	Sample& Sample::ticksPerQuarterDefault()
	{
		ticksPerQuarter_ = TicksPerQuarter::defaultValue;
		return *this;
	}
	Sample& Sample::ticksPerQuarterFine()
	{
		ticksPerQuarter_ = TicksPerQuarter::fine;
		return *this;
	}
	Sample& Sample::ticksPerQuarterVeryFine()
	{
		ticksPerQuarter_ = TicksPerQuarter::veryFine;
		return *this;
	}
	
	Sample& Sample::tempo(Tempo const& tempo)
	{
		tempo_ = tempo;
		return *this;
	}
	Sample& Sample::tempoDefault()
	{
		tempo_ = Tempo::createDefault();
		return *this;
	}
	Sample& Sample::tempoMidi(UINT_24 microsecondsPerQuarter)
	{
		tempo_ = Tempo::fromMidi(microsecondsPerQuarter);
		return *this;
	}
	Sample& Sample::tempoBpm(double bpm)
	{
		tempo_ = Tempo::fromBpm(bpm);
		return *this;
	}
	Sample& Sample::tempo(double bpm)
	{
		return tempoBpm(bpm);
	}
	
	Sample& Sample::timeSignature(TimeSignature const& timeSignature)
	{
		timeSignature_ = timeSignature;
		return *this;
	}
	Sample& Sample::timeSignatureDefault()
	{
		timeSignature_ = TimeSignature::createDefault();
		return *this;
	}
	Sample& Sample::timeSignature(UINT_8 num, UINT_8 denom_4isQuarter_8isEighth_etc)
	{
		timeSignature_ = TimeSignature(num, denom_4isQuarter_8isEighth_etc);
		return *this;
	}
	
	Sample& Sample::dur(SampleDur const& dur)
	{
		dur_ = dur;
		return *this;
	}
	Sample& Sample::durDefault()
	{
		return durUnspecified();
	}
	Sample& Sample::durSpecified(Dur const& dur)
	{
		dur_ = SampleDur::specified(dur);
		return *this;
	}
	Sample& Sample::durSpecified(double dur)
	{
		dur_ = SampleDur::specified(dur);
		return *this;
	}
	Sample& Sample::durUnspecified()
	{
		dur_ = SampleDur::unspecified();
		return *this;
	}
	Sample& Sample::dur(Dur const& dur)
	{
		return durSpecified(dur);
	}
	Sample& Sample::dur(double dur)
	{
		return durSpecified(dur);
	}

	Sample& Sample::overlappingNotes(OverlappingNotes overlappingNotes)
	{
		overlappingNotes_ = overlappingNotes;
		return *this;
	}
	Sample& Sample::overlappingNotesTrim()
	{
		overlappingNotes_ = OverlappingNotes::trim;
		return *this;
	}
	Sample& Sample::overlappingNotesMerge()
	{
		overlappingNotes_ = OverlappingNotes::merge;
		return *this;
	}
	Sample& Sample::overlappingNotesIgnore()
	{
		overlappingNotes_ = OverlappingNotes::ignore;
		return *this;
	}
	Sample& Sample::overlappingNotesError()
	{
		overlappingNotes_ = OverlappingNotes::error;
		return *this;
	}

	UINT_16 Sample::ticksPerQuarter() const
	{
		return ticksPerQuarter_;
	}
	Tempo const& Sample::tempo() const
	{
		return tempo_;
	}
	TimeSignature const& Sample::timeSignature() const
	{
		return timeSignature_;
	}
	SampleDur const& Sample::dur() const
	{
		return dur_;
	}
	OverlappingNotes Sample::overlappingNotes() const
	{
		return overlappingNotes_;
	}
	unsigned int Sample::currentTrack() const
	{
		return currentTrack_;
	}
	
	Track& Sample::track(unsigned int index)
	{
		return tracks_[index];
	}
	Track& Sample::operator[](unsigned int index)
	{
		return track(index);
	}
	Track const& Sample::track(unsigned int index) const
	{
		return tracks_[index];
	}
	Track const& Sample::operator[](unsigned int index) const
	{
		return track(index);
	}

	size_t Sample::nbTracks() const
	{
		return tracks_.size();
	}
	Track& Sample::addEmptyTrack()
	{
		tracks_.push_back({});
		return tracks_.back();
	}
	Track& Sample::addEmptyTrack(unsigned int index)
	{
		if (index <= nbTracks()) {
			if (index <= currentTrack_) {
				currentTrack_++;
			}
			auto it = tracks_.begin() + index;
			return *tracks_.insert(it, 1, {});
		} else {
			throw Exceptions::WrongTrackInsertionIndex();
		}
	}
	void Sample::addEmptyTracks(size_t nb)
	{
		tracks_.resize(tracks_.size() + nb);
	}
	void Sample::addEmptyTracks(unsigned int index, size_t nb)
	{
		if (index <= nbTracks()) {
			if (index <= currentTrack_) {
				currentTrack_ += (unsigned int)nb;
			}
			auto it = tracks_.begin() + index;
			tracks_.insert(it, nb, {});
		} else {
			throw Exceptions::WrongTrackInsertionIndex();
		}
	}
	void Sample::removeTrack(unsigned int index)
	{
		if (index < nbTracks()) {
			if (index == currentTrack_) {
				currentTrack_ = 0;
			} else if (index <= currentTrack_) {
				currentTrack_--;
			}
			tracks_.erase(tracks_.begin() + index);
		} else {
			throw Exceptions::WrongTrackDeletionIndex();
		}
	}
	void Sample::removeTracks(unsigned int index, size_t nb)
	{
		unsigned int endExclusive = index + (unsigned int)nb;
		if (index < nbTracks() && endExclusive <= nbTracks()) {
			if (index <= currentTrack_) {
				if (currentTrack_ < endExclusive) {
					currentTrack_ = 0;
				} else {
					currentTrack_ -= (unsigned int)nb;
				}
			}
			auto it = tracks_.begin() + index;
			tracks_.erase(it, it + nb);
		} else {
			throw Exceptions::WrongTrackDeletionIndices();
		}
	}

	Sample& Sample::currentTrack(unsigned int currentTrackIndex)
	{
		currentTrack_ = currentTrackIndex;
		return *this;
	}

	Pos Sample::calcEndPos() const
	{
		if (dur_.isSpecified()) {
			// Specified duration
			return dur_.dur().toPos();
		} else {
			// Computed duration: max position of an event in all tracks
			Pos maxPos = Pos(0.);
			for (Track const& track : tracks_) {
				maxPos = max(maxPos, track.calcEndPos());
			}
			return maxPos;
		}
	}

	Sample Sample::createWithSameOptions(size_t nbTracks) const
	{
		Sample ans(nbTracks);

		ans.ticksPerQuarter_  = ticksPerQuarter_ ;
		ans.tempo_            = tempo_           ;
		ans.timeSignature_    = timeSignature_   ;
		ans.dur_              = dur_             ;
		ans.overlappingNotes_ = overlappingNotes_;

		return ans;
	}
	Sample Sample::createWithSameOptions_sameTrackNumber() const
	{
		return createWithSameOptions(tracks_.size());
	}

	Sample Sample::copyTransform(ElementTransformatorBase const& transformator) const
	{
		// Create a sample with the same options
		Sample ans = createWithSameOptions_sameTrackNumber();

		// Do what needs to be done at the beginning
		transformator.before(*this, ans);

		// Copy transformed tracks
		size_t nbTracks = tracks_.size();
		for (unsigned int i = 0; i < nbTracks; i++) {
			ans.tracks_[i] = tracks_[i].copyTransform(transformator);
		}

		// Do what needs to be done at the end
		transformator.after(*this, ans);

		// Return
		return ans;
	}

	Sample Sample::copy(SampleOperators::Selector const& what)
	{
		return SampleOperators::copy(*this, what);
	}
	Sample Sample::transpose(int semitonesOffset, SampleOperators::TransposedPitchOutOfRange transposedPitchOutOfRange)
	{
		return SampleOperators::transpose(*this, semitonesOffset, transposedPitchOutOfRange);
	}
	Sample Sample::pitchMap(std::array<int, 128> const& map)
	{
		return SampleOperators::pitchMap(*this, map);
	}
	Sample Sample::pitchMap(std::vector<int> const& map, SampleOperators::UnspecifiedMapValue unspecifiedMapValue)
	{
		return SampleOperators::pitchMap(*this, map, unspecifiedMapValue);
	}
	Sample Sample::pitchMap(std::map<int, int> const& map, SampleOperators::UnspecifiedMapValue unspecifiedMapValue)
	{
		return SampleOperators::pitchMap(*this, map, unspecifiedMapValue);
	}
	Sample Sample::channelMap(std::array<int, 16> const& map, SampleOperators::Selector const& what)
	{
		return SampleOperators::channelMap(*this, map, what);
	}
	Sample Sample::channelMap(std::vector<int> const& map, SampleOperators::Selector const& what, SampleOperators::UnspecifiedMapValue unspecifiedMapValue)
	{
		return SampleOperators::channelMap(*this, map, what, unspecifiedMapValue);
	}
	Sample Sample::channelMap(std::map<int, int> const& map, SampleOperators::Selector const& what, SampleOperators::UnspecifiedMapValue unspecifiedMapValue)
	{
		return SampleOperators::channelMap(*this, map, what, unspecifiedMapValue);
	}
	Sample Sample::channelMap(std::vector<int> const& map, SampleOperators::UnspecifiedMapValue unspecifiedMapValue)
	{
		return SampleOperators::channelMap(*this, map, unspecifiedMapValue);
	}
	Sample Sample::channelMap(std::map<int, int> const& map, SampleOperators::UnspecifiedMapValue unspecifiedMapValue)
	{
		return SampleOperators::channelMap(*this, map, unspecifiedMapValue);
	}
	Sample Sample::reverse(SampleOperators::Selector const& what, SampleOperators::PreserveCurves preserveCurves)
	{
		return SampleOperators::reverse(*this, what, preserveCurves);
	}
	Sample Sample::reverse(SampleOperators::PreserveCurves preserveCurves)
	{
		return SampleOperators::reverse(*this, preserveCurves);
	}
	Sample Sample::delay(double timeOffset, SampleOperators::Selector const& what)
	{
		return SampleOperators::delay(*this, timeOffset, what);
	}
	Sample Sample::stretch(double timeRatio, SampleOperators::Selector const& what)
	{
		return SampleOperators::stretch(*this, timeRatio, what);
	}
	Sample Sample::trim(double rangeBegin, double rangeEnd, SampleOperators::PreserveCurves preserveCurves)
	{
		return SampleOperators::trim(*this, rangeBegin, rangeEnd, preserveCurves);
	}
	Sample Sample::trim(double rangeBegin, SampleOperators::PreserveCurves preserveCurves)
	{
		return SampleOperators::trim(*this, rangeBegin, preserveCurves);
	}
	Sample Sample::trimTracks(size_t rangeBegin, size_t rangeEndExclusive)
	{
		return SampleOperators::trimTracks(*this, rangeBegin, rangeEndExclusive);
	}
	Sample Sample::trimPitches(UINT_7 min, UINT_7 max, bool keepNoPitchEvents)
	{
		return SampleOperators::trimPitches(*this, min, max, keepNoPitchEvents);
	}
	Sample Sample::trimChannels(UINT_4 min, UINT_4 max, bool keepNoChannelEvents)
	{
		return SampleOperators::trimChannels(*this, min, max, keepNoChannelEvents);
	}
	SampleOperators::TwoSamples Sample::split(double posWhereSecondBegins, SampleOperators::PreserveCurves preserveCurves)
	{
		return SampleOperators::split(*this, posWhereSecondBegins, preserveCurves);
	}
	SampleOperators::TwoSamples Sample::splitTracks(int indexTrackWhereSecondBegins)
	{
		return SampleOperators::splitTracks(*this, indexTrackWhereSecondBegins);
	}
	SampleOperators::TwoSamples Sample::splitPitches(UINT_8 pitchWhereSecondBegins, SampleOperators::WhereToKeepTheRest whereToKeepNoPitchEvents)
	{
		return SampleOperators::splitPitches(*this, pitchWhereSecondBegins, whereToKeepNoPitchEvents);
	}
	SampleOperators::TwoSamples Sample::splitChannels(UINT_8 channelWhereSecondBegins, SampleOperators::WhereToKeepTheRest whereToKeepNoChannelEvents)
	{
		return SampleOperators::splitChannels(*this, channelWhereSecondBegins, whereToKeepNoChannelEvents);
	}
	Sample Sample::concat(Sample const& other, SampleOperators::DifferentTicksPerQuarter differentTicksPerQuarter, SampleOperators::DifferentTempos differentTempos, SampleOperators::DifferentTimeSignatures differentTimeSignatures, SampleOperators::DifferentOverlappingNotes differentOverlappingNotes)
	{
		return SampleOperators::concat(*this, other, differentTicksPerQuarter, differentTempos, differentTimeSignatures, differentOverlappingNotes);
	}
	Sample Sample::concatTracks(Sample const& other, SampleOperators::DifferentTicksPerQuarter differentTicksPerQuarter, SampleOperators::DifferentTempos differentTempos, SampleOperators::DifferentTimeSignatures differentTimeSignatures, SampleOperators::DifferentSampleDurs differentSampleDurs, SampleOperators::DifferentOverlappingNotes differentOverlappingNotes)
	{
		return SampleOperators::concatTracks(*this, other, differentTicksPerQuarter, differentTempos, differentTimeSignatures, differentSampleDurs, differentOverlappingNotes);
	}
	Sample Sample::merge(Sample const& other, SampleOperators::DifferentTicksPerQuarter differentTicksPerQuarter, SampleOperators::DifferentTempos differentTempos, SampleOperators::DifferentTimeSignatures differentTimeSignatures, SampleOperators::DifferentSampleDurs differentSampleDurs, SampleOperators::DifferentOverlappingNotes differentOverlappingNotes)
	{
		return SampleOperators::merge(*this, other, differentTicksPerQuarter, differentTempos, differentTimeSignatures, differentSampleDurs, differentOverlappingNotes);
	}

	void Sample::save(std::string const& fileName)
	{
		writeMidiFile(*this, fileName);
	}
}
