#ifndef MID_SAMPLE_OPERATORS_HPP
#define MID_SAMPLE_OPERATORS_HPP

#include "../containers/Sample.hpp"
#include "../sample-operators-tools/so_ElementTransformatorBase.hpp"
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

#include <array>
#include <vector>
#include <map>

namespace MID
{
	// Provides a set of operations that can be performed on entire samples.
	// These functions are also available as non-static member functions of class Sample.
	// The [[nodiscard]] attributes warn a user who accidentally tries to use these functions to modify an existing sample.
	namespace SampleOperators
	{
		// Create a new sample by copying some elements. Use the 'what' argument to specify which elements are affected.
		[[nodiscard]] Sample copy(Sample const& sample, Selector const& what = Selector::all());

		// Create a new sample in which the pitch of all notes is transposed by the specified interval.
		[[nodiscard]] Sample transpose(Sample const& sample, int semitonesOffset, TransposedPitchOutOfRange transposedPitchOutOfRange = TransposedPitchOutOfRange::defaultBehavior);

		// Create a new sample in which all notes is mapped to new pitches. The 'map' argument indicates how the old pitches
		// should be mapped to the new ones; for example, if map[60] is 62, all middle Cs will become middle Ds. You can
		// specify a -1 pitch target to discard specific pitches; for example, if map[60] is -1, all middle Cs will be discarded.
		[[nodiscard]] Sample pitchMap(Sample const& sample, std::array<int, 128> const& map);
		[[nodiscard]] Sample pitchMap(Sample const& sample, std::vector<int> const& map, UnspecifiedMapValue unspecifiedMapValue = UnspecifiedMapValue::defaultBehavior);
		[[nodiscard]] Sample pitchMap(Sample const& sample, std::map<int, int> const& map, UnspecifiedMapValue unspecifiedMapValue = UnspecifiedMapValue::defaultBehavior);

		// Create a new sample in which all notes is mapped to new pitches. The 'map' argument indicates how the old pitches
		// should be mapped to the new ones; for example, if map[60] is 62, all middle Cs will become middle Ds. You can
		// specify a -1 pitch target to discard specific pitches; for example, if map[60] is -1, all middle Cs will be discarded.
		// Use the 'what' argument to specify which elements are affected.
		[[nodiscard]] Sample channelMap(Sample const& sample, std::array<int, 16> const& map, Selector const& what = Selector::all());
		[[nodiscard]] Sample channelMap(Sample const& sample, std::vector<int> const& map, Selector const& what = Selector::all(), UnspecifiedMapValue unspecifiedMapValue = UnspecifiedMapValue::defaultBehavior);
		[[nodiscard]] Sample channelMap(Sample const& sample, std::map<int, int> const& map, Selector const& what = Selector::all(), UnspecifiedMapValue unspecifiedMapValue = UnspecifiedMapValue::defaultBehavior);
		[[nodiscard]] Sample channelMap(Sample const& sample, std::vector<int> const& map, UnspecifiedMapValue unspecifiedMapValue);
		[[nodiscard]] Sample channelMap(Sample const& sample, std::map<int, int> const& map, UnspecifiedMapValue unspecifiedMapValue);

		// Create a new sample in which all notes and events occur in reverse order.
		[[nodiscard]] Sample reverse(Sample const& sample, Selector const& what = Selector::all(), PreserveCurves preserveCurves = PreserveCurves::defaultBehavior);
		[[nodiscard]] Sample reverse(Sample const& sample, PreserveCurves preserveCurves);

		// Create a new sample in which all notes and events occur after a specified time offset.
		[[nodiscard]] Sample delay(Sample const& sample, double timeOffset, Selector const& what = Selector::all());

		// Create a new sample in which time runs faster or slower by the specified ratio. This doesn't change the tempo of the sample, but it does retime all the events.
		[[nodiscard]] Sample stretch(Sample const& sample, double timeRatio, Selector const& what = Selector::all());
		
		// Create a new sample from a time range of the original sample.
		[[nodiscard]] Sample trim(Sample const& sample, double rangeBegin, double rangeEnd, PreserveCurves preserveCurves = PreserveCurves::defaultBehavior);
		[[nodiscard]] Sample trim(Sample const& sample, double rangeBegin, PreserveCurves preserveCurves = PreserveCurves::defaultBehavior);
		
		// Create a new sample from a range of tracks of the original sample.
		[[nodiscard]] Sample trimTracks(Sample const& sample, size_t rangeBegin, size_t rangeEndExclusive);
		
		// Create a new sample in which notes are only retained if their pitch fits within the specified range.
		[[nodiscard]] Sample trimPitches(Sample const& sample, UINT_7 min, UINT_7 max, bool keepNoPitchEvents = true);
		
		// Create a new sample in which events are only retained if their channel fits within the specified range.
		[[nodiscard]] Sample trimChannels(Sample const& sample, UINT_4 min, UINT_4 max, bool keepNoChannelEvents = true);

		// Create two new samples by splitting the sample at the specified time position.
		[[nodiscard]] TwoSamples split(Sample const& sample, double posWhereSecondBegins, PreserveCurves preserveCurves = PreserveCurves::defaultBehavior);
		
		// Create two new samples by splitting the tracks at the specified track index.
		[[nodiscard]] TwoSamples splitTracks(Sample const& sample, int indexTrackWhereSecondBegins);
		
		// Create two new samples by splitting the sample below and above the specified pitch.
		[[nodiscard]] TwoSamples splitPitches(Sample const& sample, UINT_8 pitchWhereSecondBegins, WhereToKeepTheRest whereToKeepNoPitchEvents = WhereToKeepTheRest::defaultBehavior);
		
		// Create two new samples by splitting the sample below and above the specified channel.
		[[nodiscard]] TwoSamples splitChannels(Sample const& sample, UINT_8 channelWhereSecondBegins, WhereToKeepTheRest whereToKeepNoChannelEvents = WhereToKeepTheRest::defaultBehavior);

		// Merge two samples by appending the second to the end of the first. You can specify what happens if the two samples don't have the same tempo.
		[[nodiscard]] Sample concat(Sample const& sample1, Sample const& sample2,
			DifferentTicksPerQuarter differentTicksPerQuarter = DifferentTicksPerQuarter::defaultBehavior,
			DifferentTempos differentTempos = DifferentTempos::defaultBehavior,
			DifferentTimeSignatures differentTimeSignatures = DifferentTimeSignatures::defaultBehavior,
			DifferentOverlappingNotes differentOverlappingNotes = DifferentOverlappingNotes::defaultBehavior);

		// Merge two samples by appending the tracks of the second to the first. You can specify what happens if the two samples don't have the same tempo.
		[[nodiscard]] Sample concatTracks(Sample const& sample1, Sample const& sample2,
			DifferentTicksPerQuarter differentTicksPerQuarter = DifferentTicksPerQuarter::defaultBehavior,
			DifferentTempos differentTempos = DifferentTempos::defaultBehavior,
			DifferentTimeSignatures differentTimeSignatures = DifferentTimeSignatures::defaultBehavior,
			DifferentSampleDurs differentSampleDurs = DifferentSampleDurs::defaultBehavior,
			DifferentOverlappingNotes differentOverlappingNotes = DifferentOverlappingNotes::defaultBehavior);

		// Merge two samples by mixing the contents of their tracks. You can specify what happens if the two samples don't have the same tempo.
		[[nodiscard]] Sample merge(Sample const& sample1, Sample const& sample2,
			DifferentTicksPerQuarter differentTicksPerQuarter = DifferentTicksPerQuarter::defaultBehavior,
			DifferentTempos differentTempos = DifferentTempos::defaultBehavior,
			DifferentTimeSignatures differentTimeSignatures = DifferentTimeSignatures::defaultBehavior,
			DifferentSampleDurs differentSampleDurs = DifferentSampleDurs::defaultBehavior,
			DifferentOverlappingNotes differentOverlappingNotes = DifferentOverlappingNotes::defaultBehavior);
	};

	// Each sample operator is implemented in a separate .cpp file.
}

#endif
