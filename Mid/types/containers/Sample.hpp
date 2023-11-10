#ifndef MID_SAMPLE_HPP
#define MID_SAMPLE_HPP

#include "Track.hpp"
#include "../sample-options/TicksPerQuarter.hpp"
#include "../sample-options/Tempo.hpp"
#include "../sample-options/TimeSignature.hpp"
#include "../sample-options/SampleDur.hpp"
#include "../sample-options/OverlappingNotes.hpp"

#include "../sample-operators-tools/so_TwoSamples.hpp"
#include "../sample-operators-options/so_Selector.hpp"
#include "../sample-operators-options/so_TransposedPitchOutOfRange.hpp"
#include "../sample-operators-options/so_UnspecifiedMapValue.hpp"
#include "../sample-operators-options/so_WhereToKeepTheRest.hpp"
#include "../sample-operators-options/so_PreserveCurves.hpp"
#include "../sample-operators-options/so_DifferentTempos.hpp"
#include "../sample-operators-options/so_DifferentSampleDurs.hpp"
#include "../sample-operators-options/so_DifferentTicksPerQuarter.hpp"
#include "../sample-operators-options/so_DifferentTimeSignatures.hpp"
#include "../sample-operators-options/so_DifferentOverlappingNotes.hpp"

#include <string>
#include <array>
#include <vector>
#include <map>

namespace MID
{
	// Main library class. Represents a complete MIDI sample.
	class Sample
	{
	public:
		// Constructor, destructor
		Sample(size_t nbTracks = 1u);

		// Named parameter idiom: ticksPerQuarter
		Sample& ticksPerQuarter(UINT_16 ticksPerQuarter);
		Sample& ticksPerQuarterCoarse();
		Sample& ticksPerQuarterDefault();
		Sample& ticksPerQuarterFine();
		Sample& ticksPerQuarterVeryFine();

		// Named parameter idiom: tempo
		Sample& tempo(Tempo const& tempo);
		Sample& tempoDefault();
		Sample& tempoMidi(UINT_24 microsecondsPerQuarter);
		Sample& tempoBpm(double bpm);
		Sample& tempo(double bpm);
		
		// Named parameter idiom: timeSignature
		Sample& timeSignature(TimeSignature const& timeSignature);
		Sample& timeSignatureDefault();
		Sample& timeSignature(UINT_8 num, UINT_8 denom_4isQuarter_8isEighth_etc);
		
		// Named parameter idiom: dur
		Sample& dur(SampleDur const& dur);
		Sample& durDefault();
		Sample& durSpecified(Dur const& dur);
		Sample& durSpecified(double dur);
		Sample& durUnspecified();
		Sample& dur(Dur const& dur);
		Sample& dur(double dur);

		// Named parameter idiom: overlappingNotes
		Sample& overlappingNotes(OverlappingNotes overlappingNotes);
		Sample& overlappingNotesTrim();
		Sample& overlappingNotesMerge();
		Sample& overlappingNotesIgnore();
		Sample& overlappingNotesError();

		// Accessors
		UINT_16 ticksPerQuarter() const;
		Tempo const& tempo() const;
		TimeSignature const& timeSignature() const;
		SampleDur const& dur() const;
		OverlappingNotes overlappingNotes() const;
		unsigned int currentTrack() const;
		
		// Track (const and non-const) accessors
		Track& track(unsigned int index);
		Track& operator[](unsigned int index);
		Track const& track(unsigned int index) const;
		Track const& operator[](unsigned int index) const;

		// Tracks: number, add, remove
		size_t nbTracks() const;
		Track& addEmptyTrack(); // add an empty track after the last track
		Track& addEmptyTrack(unsigned int index); // add an empty track at the specified index (can change currentTrack in order to keep it pointing to the correct track)
		void addEmptyTracks(size_t nb); // add empty tracks after the last track
		void addEmptyTracks(unsigned int index, size_t nb); // add empty tracks at the specified index (can change currentTrack in order to keep it pointing to the correct track)
		void removeTrack(unsigned int index); // remove a track (will reset currentTrack to 0 if the current track was deleted, or perrhaps change it in order to keep it pointing to the correct track)
		void removeTracks(unsigned int index, size_t nb); // remove a range of tracks (will reset currentTrack to 0 if the current track was deleted, or perrhaps change it in order to keep it pointing to the correct track)

		// Specify the current track index
		Sample& currentTrack(unsigned int currentTrackIndex);
		
		// Add anything (notes, events, collections of the above...)
		template<typename T> // use current track
		void add(T const& addableElementOrElements)
		{
			tracks_[currentTrack_].add(addableElementOrElements);
		}
		template<typename T> // specify track index
		void add(T const& addableElementOrElements, unsigned int trackIndex)
		{
			tracks_[trackIndex].add(addableElementOrElements);
		}
		template<typename T> // with '<<'
		Sample& operator<<(T const& addableElementOrElements)
		{
			add(addableElementOrElements);
			return *this;
		}
		
		// Add unsafe (for events that should not be added to a track manually)
		template<typename T> // use current track
		void addUnsafe(T const& unsafeEvent)
		{
			tracks_[currentTrack_].addUnsafe(unsafeEvent);
		}
		template<typename T> // specify track index
		void addUnsafe(T const& unsafeEvent, unsigned int trackIndex)
		{
			tracks_[trackIndex].addUnsafe(unsafeEvent);
		}

		// Get end position (dur if specified, or position of the last event)
		Pos calcEndPos() const;

		// Copy options
		Sample createWithSameOptions(size_t nbTracks) const;
		Sample createWithSameOptions_sameTrackNumber() const;

		// Transform
		Sample copyTransform(ElementTransformatorBase const& transformator) const;

		// Sample operators (also available as free functions; see 'SampleOperators.hpp' for more information).
		// The [[nodiscard]] attributes warn a user who accidentally tries to use these functions to modify an existing sample.
		[[nodiscard]] Sample copy(SampleOperators::Selector const& what = SampleOperators::Selector::all());
		[[nodiscard]] Sample transpose(int semitonesOffset, SampleOperators::TransposedPitchOutOfRange transposedPitchOutOfRange = SampleOperators::TransposedPitchOutOfRange::defaultBehavior);
		[[nodiscard]] Sample pitchMap(std::array<int, 128> const& map);
		[[nodiscard]] Sample pitchMap(std::vector<int> const& map, SampleOperators::UnspecifiedMapValue unspecifiedMapValue = SampleOperators::UnspecifiedMapValue::defaultBehavior);
		[[nodiscard]] Sample pitchMap(std::map<int, int> const& map, SampleOperators::UnspecifiedMapValue unspecifiedMapValue = SampleOperators::UnspecifiedMapValue::defaultBehavior);
		[[nodiscard]] Sample channelMap(std::array<int, 16> const& map, SampleOperators::Selector const& what = SampleOperators::Selector::all());
		[[nodiscard]] Sample channelMap(std::vector<int> const& map, SampleOperators::Selector const& what = SampleOperators::Selector::all(), SampleOperators::UnspecifiedMapValue unspecifiedMapValue = SampleOperators::UnspecifiedMapValue::defaultBehavior);
		[[nodiscard]] Sample channelMap(std::map<int, int> const& map, SampleOperators::Selector const& what = SampleOperators::Selector::all(), SampleOperators::UnspecifiedMapValue unspecifiedMapValue = SampleOperators::UnspecifiedMapValue::defaultBehavior);
		[[nodiscard]] Sample channelMap(std::vector<int> const& map, SampleOperators::UnspecifiedMapValue unspecifiedMapValue);
		[[nodiscard]] Sample channelMap(std::map<int, int> const& map, SampleOperators::UnspecifiedMapValue unspecifiedMapValue);
		[[nodiscard]] Sample reverse(SampleOperators::Selector const& what = SampleOperators::Selector::all(), SampleOperators::PreserveCurves preserveCurves = SampleOperators::PreserveCurves::defaultBehavior);
		[[nodiscard]] Sample reverse(SampleOperators::PreserveCurves preserveCurves);
		[[nodiscard]] Sample delay(double timeOffset, SampleOperators::Selector const& what = SampleOperators::Selector::all());
		[[nodiscard]] Sample stretch(double timeRatio, SampleOperators::Selector const& what = SampleOperators::Selector::all());
		[[nodiscard]] Sample trim(double rangeBegin, double rangeEnd, SampleOperators::PreserveCurves preserveCurves = SampleOperators::PreserveCurves::defaultBehavior);
		[[nodiscard]] Sample trim(double rangeBegin, SampleOperators::PreserveCurves preserveCurves = SampleOperators::PreserveCurves::defaultBehavior);
		[[nodiscard]] Sample trimTracks(size_t rangeBegin, size_t rangeEndExclusive);
		[[nodiscard]] Sample trimPitches(UINT_7 min, UINT_7 max, bool keepNoPitchEvents = true);
		[[nodiscard]] Sample trimChannels(UINT_4 min, UINT_4 max, bool keepNoChannelEvents = true);
		[[nodiscard]] SampleOperators::TwoSamples split(double posWhereSecondBegins, SampleOperators::PreserveCurves preserveCurves = SampleOperators::PreserveCurves::defaultBehavior);
		[[nodiscard]] SampleOperators::TwoSamples splitTracks(int indexTrackWhereSecondBegins);
		[[nodiscard]] SampleOperators::TwoSamples splitPitches(UINT_8 pitchWhereSecondBegins, SampleOperators::WhereToKeepTheRest whereToKeepNoPitchEvents = SampleOperators::WhereToKeepTheRest::defaultBehavior);
		[[nodiscard]] SampleOperators::TwoSamples splitChannels(UINT_8 channelWhereSecondBegins, SampleOperators::WhereToKeepTheRest whereToKeepNoChannelEvents = SampleOperators::WhereToKeepTheRest::defaultBehavior);
		[[nodiscard]] Sample concat(Sample const& other, SampleOperators::DifferentTicksPerQuarter differentTicksPerQuarter = SampleOperators::DifferentTicksPerQuarter::defaultBehavior, SampleOperators::DifferentTempos differentTempos = SampleOperators::DifferentTempos::defaultBehavior, SampleOperators::DifferentTimeSignatures differentTimeSignatures = SampleOperators::DifferentTimeSignatures::defaultBehavior, SampleOperators::DifferentOverlappingNotes differentOverlappingNotes = SampleOperators::DifferentOverlappingNotes::defaultBehavior);
		[[nodiscard]] Sample concatTracks(Sample const& other, SampleOperators::DifferentTicksPerQuarter differentTicksPerQuarter = SampleOperators::DifferentTicksPerQuarter::defaultBehavior, SampleOperators::DifferentTempos differentTempos = SampleOperators::DifferentTempos::defaultBehavior, SampleOperators::DifferentTimeSignatures differentTimeSignatures = SampleOperators::DifferentTimeSignatures::defaultBehavior, SampleOperators::DifferentSampleDurs differentSampleDurs = SampleOperators::DifferentSampleDurs::defaultBehavior, SampleOperators::DifferentOverlappingNotes differentOverlappingNotes = SampleOperators::DifferentOverlappingNotes::defaultBehavior);
		[[nodiscard]] Sample merge(Sample const& other, SampleOperators::DifferentTicksPerQuarter differentTicksPerQuarter = SampleOperators::DifferentTicksPerQuarter::defaultBehavior, SampleOperators::DifferentTempos differentTempos = SampleOperators::DifferentTempos::defaultBehavior, SampleOperators::DifferentTimeSignatures differentTimeSignatures = SampleOperators::DifferentTimeSignatures::defaultBehavior, SampleOperators::DifferentSampleDurs differentSampleDurs = SampleOperators::DifferentSampleDurs::defaultBehavior, SampleOperators::DifferentOverlappingNotes differentOverlappingNotes = SampleOperators::DifferentOverlappingNotes::defaultBehavior);

		// Export as MIDI file
		void save(std::string const& fileName);
	
	private:
		// Fields: options (affected by Named parameter idiom)
		UINT_16 ticksPerQuarter_;
		Tempo tempo_;
		TimeSignature timeSignature_;
		SampleDur dur_;
		OverlappingNotes overlappingNotes_;

		// Other fields: tracks
		std::vector<Track> tracks_; // store notes and other MIDI events
		unsigned int currentTrack_; // track to which elements are added if no index is specified
	};
}

#endif
