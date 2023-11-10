#ifndef TUTORIAL_10_HPP
#define TUTORIAL_10_HPP

/*--------------------------------------------------------------------.
                                                                      |
   TUTORIAL #10   :   SAMPLE OPERATORS                                |
                                                                      |
   This tutorial shows how to perform operations on entire samples.   |
                                                                      |
---------------------------------------------------------------------*/

#include "../Mid/Lib.hpp"
using namespace MID;

#include <random>

void tutorial10(std::string const& fileName)
{
    Sample sample, sample2;
    sample << Note(0, 1, 60) << Note(1, 1, 64) << Note(2, 2, 67);

    // The MID library allows some operations that affect entire samples. Such operations
    // are called 'sample operators'. They are accessible in the SampleOperators namespace,
    // so you can write 'using namespace SampleOperators' wherever you want to use them.
    {
        using namespace SampleOperators;
        sample2 = transpose(sample, +3);
    }

    using namespace SampleOperators;
    
    // Copying some elements from a sample: copy()
    {
        // This is how you get a copy of a sample:
        sample2 = copy(sample); // same as 'sample2 = sample;'

        // You can select what you want to keep in the copied sample:
        sample2 = copy(sample, Selector().all()); // all (this is the default)
        sample2 = copy(sample, Selector().notes()); // only notes
        sample2 = copy(sample, Selector().pitchBends().programChanges()); // only pitch bends and program changes
        sample2 = copy(sample, Selector::all().noNotes()); // all except notes
    }
    
    // Transposing a sample: transpose()
    {
        sample2 = transpose(sample, -12); // get a copy one octave lower

        // You can specify what happens when some transposed notes are outside the 0-127 range:
        sample2 = transpose(sample, -12, TransposedPitchOutOfRange::error); // throws an exception in such a case
        sample2 = transpose(sample, -12, TransposedPitchOutOfRange::ignore); // discards out-of-range notes (this is the default)
    }

    // Changing pitches: pitchMap()
    {
        sample2 = pitchMap(sample, std::map<int, int>{{60, 62}}); // converts all middle Cs to middle Ds

        // You can provide a std::array or a std::vector to indicate how the pitches should be mapped:
        std::array<int, 128> map; // value at index i indicates what happens to notes with pitch i
        for (int i = 0; i < 128; i++) map[i] = (i % 12 == 0) ? i : -1; // -1 is a special value that means 'discard'
        sample2 = pitchMap(sample, map); // keeps all Cs and discards all other notes
    
        // You can also provide a std::map, as in the first example
        std::map<int, int> map2;
        for (int i = 0; i < 128; i++) map2[i] = rand() % 128;
        sample2 = pitchMap(sample, map2); // maps all pitches to constant random pitches
    
        // You can specify what happens to notes that are not specified in the std::map or in the std::vector:
        sample2 = pitchMap(sample, map2, UnspecifiedMapValue::error); // throws an exception in such a case
        sample2 = pitchMap(sample, map2, UnspecifiedMapValue::discard); // discards unspecified notes
        sample2 = pitchMap(sample, map2, UnspecifiedMapValue::leaveUnchanged); // leaves unspecified notes unchanged (this is the default)
    }

    // Changing channels: channelMap()
    {
        sample2 = channelMap(sample, std::map<int, int>{{0, 1}, {1, 0}}); // assigns channel-0 events to channel 1 and vice versa

        // You can provide a std::map to indicate how the channels should be mapped, as in the above example:
        std::map<int, int> map;
        map[0] = 3;  // events on channel 0 are moved to channel 3
        map[1] = -1; // events on channel 1 are discarded
        sample2 = channelMap(sample, map);

        // You can also provide a std::array or a std::vector:
        std::array<int, 16> map2; // value at index i indicates what happens to events on channel i
        for (int i = 0; i < 15; i++) map2[i] = i + 1;
        map2[15] = -1;
        sample2 = channelMap(sample, map2); // adds 1 to the channel of each event, discards events on the last channel 15

        // You can specify what happens to events that are not specified in the std::map or in the std::vector:
        sample2 = channelMap(sample, map, UnspecifiedMapValue::discard); // discards events with channel unspecified by the map
        sample2 = channelMap(sample, map, UnspecifiedMapValue::leaveUnchanged); // leaves unspecified events unchanged (this is the default)
        for (int i = 0; i < 16; i++) map[i] = i; // (this prevents the next instruction from throwing an exception)
        sample2 = channelMap(sample, map, UnspecifiedMapValue::error); // throws an exception if a value is not specified

        // You can also select which events are affected by the channel mapping:
        sample2 = channelMap(sample, map, Selector::all().noNotes()); // all except notes (they will be left unchanged)
        sample2 = channelMap(sample, map, Selector().programChanges()); // only program changes

        // Redefining the channel of the program change events may result in unexpected behavior due to the special channel 9.
        sample2 = channelMap(sample, std::map<int, int>{{0, 9}}); // normal instruments on channel 0 become drums on special channel 9
    }

    // Reversing time: reverse()
    {
        sample2 = reverse(sample);

        // You can choose what to reverse:
        sample2 = reverse(sample, Selector().pitchBends()); // only pitch bends
        sample2 = reverse(sample, Selector::all().noNotes()); // all except notes

        // If the duration of the sample is specified, the end time (which
        // becomes t=0 in the reversed sample) is set to that duration.
        // If not, the end time is set to the position of the last event.
        Sample a = Sample().durSpecified(12);
        a << Note(0, 4, 60);
        Sample a_rev = reverse(a); // reversed from t=12
        Sample b = Sample().durUnspecified();
        b << Note(0, 4, 60);
        Sample b_rev = reverse(a); // reversed from t=4

        // Most events have an effect after their value is set. For instance, a pitch
        // bend event defines the pitch bend value after it occurs. For this reason,
        // reversing time by simply copying events in reverse order can lead to incorrect
        // automation curves, because after reversing time, the effect should be taken
        // into account before it occurs. You can specify how this issue is addressed:
        sample2 = reverse(sample, PreserveCurves::preserveCurves); // preserve the aspect of the automation curves by adding or moving the necessary events (this is the default)
        sample2 = reverse(sample, PreserveCurves::simplyCopyEvents); // ignore the issue and simply copy events in reverse order
    }

    // Applying a time offset: delay()
    {
        sample2 = delay(sample, 8);

        // You can choose what to delay:
        sample2 = delay(sample, 8, Selector().pitchBends().programChanges()); // only pitch bends and program changes
        sample2 = delay(sample, 8, Selector::all().noNotes().noAftertouches()); // all except notes and aftertouch events

        // Negative delays are not allowed. Use trimTime() instead.
    }

    // Stretching time: stretch()
    {
        sample2 = stretch(sample, 0.5);

        // The time ratio indicates the stretch factor.
        sample2 = stretch(sample, 0.5); // twice as fast: all time values are multiplied by 0.5
        sample2 = stretch(sample, 2); // twice as slow: all time values are multiplied by 2

        // You can choose what to stretch:
        sample2 = stretch(sample, 0.5, Selector::all()); // stretches all notes and events (this is the default)
        sample2 = stretch(sample, 0.5, Selector().notes()); // stretches notes only

        // The stretch() operator doesn't change the tempo of the sample. It stretches
        // the time by multiplying each position by the time ratio. Negative ratios are
        // not allowed. Use reverse() and stretch() with a positive ratio instead.
    }

    // Trimming a sample to keep only a time range: trim()
    {
        sample2 = trim(sample, 16, 32);

        // The first number is the time position where the trimmed sample begins;
        // the second (if specified) is the time position where it ends.
        sample2 = trim(sample, 12, 16); // keeps from t=12 to t=16; duration=4
        sample2 = trim(sample, 12); // keeps from t=12 to the end

        // Most events remain in effect after their value is set. For instance,
        // a pitch bend event keeps its effect after it has occurred. Trimming
        // a sample by discarding events that occured before the trimming position
        // can result in incorrect automation curves: if an event occured before
        // the trimming position, its value is not copied at the beginning of
        // the trimmed sample. You can specify how this issue is addressed:
        sample2 = trim(sample, 12, PreserveCurves::preserveCurves); // preserve the aspect of the automation curves by adding the necessary events at the beginning of the trimmed sample (this is the default)
        sample2 = trim(sample, 12, PreserveCurves::simplyCopyEvents); // ignore the issue and simply copy events that occur from the trimming position
    }

    // Trimming a sample to keep only a range of tracks: trimTracks()
    {
        Sample x(10); // sample x has 10 tracks
        sample2 = trimTracks(x, 1, 5);

        // The first number is the index of the first track you want to keep in the trimmed sample;
        // the second is the index of the first track you don't want to keep after the range.
        sample2 = trimTracks(x, 1, 5); // keeps tracks 1, 2, 3 and 4
        sample2 = trimTracks(x, 0, 10); // keeps all tracks
        sample2 = trimTracks(x, 7, 7); // keeps nothing
    }

    // Trimming a sample to keep only pitches within a range: trimPitches()
    {
        sample2 = trimPitches(sample, 48, 72);

        // The first number is the first pitch you want to keep;
        // the second is the last pitch you want to keep.
        sample2 = trimPitches(sample, 60, 72); // keeps notes from C4 to C5, included
        sample2 = trimPitches(sample, 60, 61); // keeps only middle Cs and middle C#s

        // You can specify what happens to other events that have no pitch:
        sample2 = trimPitches(sample, 60, 72, true); // keeps all events (this is the default)
        sample2 = trimPitches(sample, 60, 72, false); // only keeps notes
    }

    // Trimming a sample to keep only events within a channel range: trimChannels()
    {
        sample2 = trimChannels(sample, 2, 8);

        // The first number is the first channel you want to keep;
        // the second is the last channel you want to keep.
        sample2 = trimChannels(sample, 2, 6); // keeps events whose channel is between 2 and 6, included

        // You can specify what happens to other events that have no channel:
        sample2 = trimChannels(sample, 10, 15, true); // keeps all events (this is the default)
        sample2 = trimChannels(sample, 10, 15, false); // keeps events with channel only
    }

    // Splitting a sample at a time position: split()
    {
        TwoSamples result = split(sample, 8); // the number indicates the time at which the sample should be split
        Sample half1 = result.first;  // contains notes and events before t=8
        Sample half2 = result.second; // contains notes and events after t=8

        // As for the sample operator trim(), you can specify whether the
        // events that still have an effect at the split position should
        // be copied at the beginning of the second resulting sample:
        result = split(sample, 8, PreserveCurves::preserveCurves); // preserve the aspect of the automation curves by adding the necessary events at the beginning of the second half (this is the default)
        result = split(sample, 8, PreserveCurves::simplyCopyEvents); // ignore the issue and simply copy events into the resulting sample where they occur
    }
    
    // Splitting the tracks of a sample: splitTracks()
    {
        Sample x(10); // sample x has 10 tracks
        TwoSamples result = splitTracks(x, 5); // the number indicates the first track to be included in the second half
        Sample half1 = result.first;  // contains tracks from 0 to 4
        Sample half2 = result.second; // contains tracks from 5 to 9
    }
    
    // Splitting the pitches of a sample: splitPitches()
    {
        TwoSamples result = splitPitches(sample, 60); // the number indicates the first pitch to be included in the second half
        Sample half1 = result.first;  // contains pitches from 0 to 59
        Sample half2 = result.second; // contains pitches from 60 to 127

        // You can specify where to hold events that do not have a pitch.
        result = splitPitches(sample, 60, WhereToKeepTheRest::first); // keep them in the first half (this is the default)
        result = splitPitches(sample, 60, WhereToKeepTheRest::second); // keep them in the second half
        result = splitPitches(sample, 60, WhereToKeepTheRest::none); // don't keep them anywhere
        result = splitPitches(sample, 60, WhereToKeepTheRest::both); // keep them in both halves
    }

    // Splitting the channels of a sample: splitChannels()
    {
        TwoSamples result = splitChannels(sample, 8); // the number indicates the first channel to be included in the second half
        Sample half1 = result.first;  // contains events whose channel is between 0 and 7
        Sample half2 = result.second; // contains events whose channel is between 8 and 15

        // You can specify where to hold events that do not have a channel.
        result = splitChannels(sample, 8, WhereToKeepTheRest::first); // keep them in the first half (this is the default)
        result = splitChannels(sample, 8, WhereToKeepTheRest::second); // keep them in the second half
        result = splitChannels(sample, 8, WhereToKeepTheRest::none); // don't keep them anywhere
        result = splitChannels(sample, 8, WhereToKeepTheRest::both); // keep them in both halves
    }

    // Concatenating two samples, one after the other: concat()
    {
        Sample x; x << Note(0, 4, 60);
        Sample y; y << Note(0, 4, 60);
        Sample result = concat(x, y);

        // If the duration of the first sample is specified, the time at
        // which the second sample is added is set to that duration. If not,
        // the second sample is added at the time position of its last event.
        result = concat(x.durSpecified(12), y); // y is added at t=12
        result = concat(x.durUnspecified(), y); // y is added at t=4

        // You can specify what happens if the two samples don't have the same options.
        // For more information about what each option means, see the definition of the enums.
        result = concat(x, y,
            // This is the default: the resulting sample has the same precision as the most precise.
            // Other options are: keepMinimum, keepFirst, keepSecond, setTicksPerQuarterDefault.
            DifferentTicksPerQuarter::keepMaximum,

            // This is the default: the tempo of the first sample is retained, and the second sample is time-stretched to match it.
            // Other options are: stretchKeepSecond, ignoreKeepFirst, ignoreKeepSecond, error.
            DifferentTempos::keepFirstStretchSecond,
            
            // This is the default: the time signature of the first sample is retained.
            // Other options are: keepSecond, setTimeSignatureDefault, error.
            DifferentTimeSignatures::keepFirst,
            
            // This is the default: the note overlapping mode of the first sample is retained.
            // Other options are: keepSecond, setOverlappingNotesDefault, error.
            DifferentOverlappingNotes::keepFirst
        );
    }

    // Concatenating the tracks of two samples, one below the other: concatTracks()
    {
        Sample x(10); // sample x has 10 tracks
        Sample y(10); // sample y has 10 tracks
        Sample result = concatTracks(x, y); // result has 20 tracks

        // You can specify what happens if the two samples don't have the same options.
        // For more information about what each option means, see the definition of the enums.
        result = concatTracks(x, y,
            DifferentTicksPerQuarter::keepMaximum,   //
            DifferentTempos::keepFirstStretchSecond, // see above in the section dedicated to 'concat'
            DifferentTimeSignatures::keepFirst,      //

            // This is the default: if both durations are specified, the resulting sample has the same duration as the longest.
            // Other options are: keepMinimum, keepFirst, keepSecond, setUnspecifiedDuration, error.
            DifferentSampleDurs::keepMaximum,
            
            DifferentOverlappingNotes::keepFirst // see above in the section dedicated to 'concat'
        );
    }

    // Merging two samples: merge()
    {
        Sample x; x << Note(0, 4, 60);
        Sample y; y << Note(0, 8, 62);
        Sample result = merge(x, y); // all contents are mixed

        // You can specify what happens if the two samples don't have the same options.
        // For more information about what each option means, see the definition of the enums.
        result = merge(x, y,
            DifferentTicksPerQuarter::keepMaximum,   //
            DifferentTempos::keepFirstStretchSecond, //
            DifferentTimeSignatures::keepFirst,      // see above in the sections dedicated to 'concat' and 'concatTracks'
            DifferentSampleDurs::keepMaximum,        //
            DifferentOverlappingNotes::keepFirst     //
        );
    }

    // Defining a custom operator that affects the elements of an entire sample
    {
        // Here we show a built-in way to create a custom sample operator that takes
        // a single sample as an argument. Let's take as an example an operator who
        // discards 50% of the notes randomly, and leaves all other MIDI events unchanged.

        // We first derive the ElementTransformatorBase class to specify how our operator
        // affects each type of element.
		class ElementTransformator_RandomDiscarder : public ElementTransformatorBase
		{
		public:
			// We override the transformNote() function to specify what happens to notes
			virtual std::unique_ptr<Note> transformNote(Note const& note) const override
			{
				if (rand() % 2 == 0) {
                    // In this case, we simply return the note unchanged, but we could also
                    // change the pitch or any other note parameter before returning it.
					return copy(note);
				} else {
                    // We randomly discard the note 50% of the time.
					return discard<Note>();
				}
			}
            // Here we don't override the other virtual functions because we don't want to change the other events.
            // Otherwise, we could override the other functions to modify the other events of the sample; see the
            // definition of the ElementTransformatorBase class to see the available functions.
		};

        // Now we use it to call the copyTransform() function on the sample.
        sample2 = sample.copyTransform(ElementTransformator_RandomDiscarder());
    }

    // All built-in sample operators are also available as non-static member functions.
    // The use of either syntax is a matter of taste.
    {
        Sample a1 = transpose(sample, -6);
        Sample a2 = sample.transpose(-6); // same
        
        TwoSamples b1 = split(sample, 2);
        TwoSamples b2 = sample.split(2); // same

        Sample c1 = concatTracks(sample, a1);
        Sample c2 = sample.concatTracks(a1); // same
    }

    // Here is an example of using sample operators to create a very small two-voice canon at the octave.
    {
        // Create a short melody
        Sample voice = Sample().timeSignature(3, 4);
        voice << Note(0, 1, 60) << Note(1, 1, 64) << Note(2, 1, 67) << Note(3, 3, 72);

        // Use it twice to create a canon
        sample2 = concatTracks(
            voice,
            voice.transpose(-12).delay(3)
        );
    }

    // Save MIDI file
    sample2.save(fileName);
}

#include "tutorial9.hpp"  // back to the previous tutorial
#include "tutorial11.hpp" // to the next tutorial

#endif
