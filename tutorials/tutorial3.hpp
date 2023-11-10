#ifndef TUTORIAL_3_HPP
#define TUTORIAL_3_HPP

/*-------------------------------------------------------------------------------------------------.
                                                                                                   |
   TUTORIAL #3   :   SAMPLE OPTIONS                                                                |
                                                                                                   |
   There are many options you can set when creating a MIDI sample. This tutorial shows them all.   |
                                                                                                   |
--------------------------------------------------------------------------------------------------*/

#include "../Mid/Lib.hpp"
using namespace MID;

#include <random>

void tutorial3(std::string const& fileName)
{
    // Sample option #1: number of tracks
    Sample sample(4); // 4 tracks
    Sample sample2;   // 1 track (without arguments, the number of tracks is set to 1)

    // Sample option #2: ticks per quarter
    sample.ticksPerQuarterDefault();
    {
        // Ticks per quarter is the number of subdivisions allowed in the duration of one quarter note.
        // You can see above how to set it to the default value (which is 96).
        // We recommend that you leave this at the default unless you notice inaccurate results or have special needs.
        // You can specify any custom integer between 1 and 32767:
        sample2.ticksPerQuarter(12);    // if you do this, you won't be able to divide quarters into more than 12 equally spaced parts
        sample2.ticksPerQuarter(32633); // this is a very unusual value because you can't divide it into any number of parts

        // If you don't know how many you need, select a predefined value:
        sample2.ticksPerQuarterCoarse  (); // low accuracy
        sample2.ticksPerQuarterDefault (); // you already know this one
        sample2.ticksPerQuarterFine    (); // both high accuracy and divisibility
        sample2.ticksPerQuarterVeryFine(); // both very high accuracy and divisibility

        // The predefined values are also available as integers...
        UINT_16 veryFine = TicksPerQuarter::veryFine;
        
        // ...so the next two instructions are equivalent:
        sample2.ticksPerQuarterFine();
        sample2.ticksPerQuarter(TicksPerQuarter::fine);
    }

    // Sample options #3: tempo
    sample.tempo(216);
    {
        // To set the tempo to the default value of 60 BPM:
        sample2.tempoDefault();
        
        // To use a custom tempo:
        sample2.tempo(120);        // using BPM (beats per minute)
        sample2.tempoBpm(120);     // same (explicit form)
        sample2.tempoMidi(500000); // using raw MIDI value: the argument is the duration of 1 quarter, in microseconds
        
        // If necessary, you can also use a Tempo object:
        Tempo tempo = Tempo::fromBpm(30);
        sample2.tempo(tempo);
    }
    
    // Sample option #4: time signature
    sample.timeSignature(3, 8);
    {
        // To set time signature to the default value of 4/4:
        sample2.timeSignatureDefault();

        // To use a custom time signature:
        sample2.timeSignature(5, 16);
        
        // If necessary, you can also use a TimeSignature object
        TimeSignature timeSignature(7, 2);
        sample2.timeSignature(timeSignature);
    }

    // Sample option #5: duration
    sample.dur(80);
    {
        // To set duration to the default value (unspecified duration):
        sample2.durUnspecified(); // the sample ends at the last event;
                                  // you let the program determine the
                                  // duration when saving to a file
        sample2.durDefault(); // same

        // To use a custom duration:
        sample2.dur(12); // the sample lasts 12 quarter notes:
                         // you set the duration manually (this
                         // trims all events after the end)
        sample2.durSpecified(12); // same (explicit form)

        // If necessary, you can also use a SampleDur object
        SampleDur dur = SampleDur::specified(12);
        sample2.dur(dur);
    }

    // Sample option #6: overlapping notes
    sample.overlappingNotesMerge();
    {
        // It can sometimes happen that you add overlapping notes to a sample.
        // This may be by accident, or it may be because you feel that writing
        // an algorithm to avoid note collisions would be too time-consuming.
        {
            // first case: accident
            Sample s;
            s << Note(0, 4, 60) << Note(2, 4, 60);
        }
        {
            // second case: hard to avoid collisions
            sample.currentTrack(3);
            for (int n = 0; n < 1000; n++) {
                sample << Note(rand() % 72, 3, 48 + rand() % 37);
            }
        }
        
        // Two notes are said to overlap if the second note begins before
        // the first ends, if they are in the same track, if they are on
        // the same channel, AND if they are of the same pitch.
        Note(0, 4, 60), Note(1, 4, 62);            // don't overlap (different pitches)
        Note(0, 4, 60), Note(4, 4, 60);            // don't overlap (note 2 starts when note 1 ends, not before)
        Note(0, 4, 60), Note(1, 4, 60).channel(5); // don't overlap (different channel)
        Note(0, 4, 60), Note(1, 4, 60);            // overlap if added to the *same* track of the same sample

        // When a sample is saved as a MIDI file, the MID library must
        // handle overlapping notes to produce coherent note-on and
        // note-off events. You can choose which algorithm to use:
        sample2.overlappingNotesTrim();   // the default: correct overlapping notes by silently trimming them
        sample2.overlappingNotesMerge();  // correct overlapping notes by silently merging them
        sample2.overlappingNotesIgnore(); // leave overlapping notes as they are and output incoherent events
        sample2.overlappingNotesError();  // throw an exception if notes overlap

        // If necessary, you can also use the OverlappingNotes enum:
        sample2.overlappingNotes(OverlappingNotes::error);

        // For more information about these 4 possible algorithms, please read
        // the header file in which the OverlappingNotes enumeration is declared.
    }

    // The options can be specified in any order as a chain (Named parameter idiom):
    {
        // Non-chained version
        Sample a = Sample(4);
        a.ticksPerQuarterVeryFine();
        a.tempo(104);
        a.timeSignature(5, 8);
        a.dur(24);
        a.overlappingNotesError();

        // Chained version
        Sample b = Sample(4)
            .ticksPerQuarterVeryFine()
            .tempo(104)
            .timeSignature(5, 8)
            .dur(24)
            .overlappingNotesError();

        // Here is another version that also gives the same result:
        Sample c = Sample(4).overlappingNotesError().dur(24);
        { /* (do some operations on c) */ }
        c.timeSignature(5, 8).tempo(104);
        { /* (do some other operations on c) */ }
        c.ticksPerQuarterVeryFine();
    }

    // Save MIDI file
    sample.save(fileName);
}

#include "tutorial2.hpp" // back to the previous tutorial
#include "tutorial4.hpp" // to the next tutorial

#endif
