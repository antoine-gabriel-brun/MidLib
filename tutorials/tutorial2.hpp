#ifndef TUTORIAL_2_HPP
#define TUTORIAL_2_HPP

/*----------------------------------------------------------.
                                                            |
   TUTORIAL #2   :   ADDING NOTES                           |
                                                            |
   This tutorial shows many ways to create and add notes.   |
                                                            |
-----------------------------------------------------------*/

#include "../Mid/Lib.hpp"
using namespace MID;

#include <vector>
#include <array>
#include <tuple>

void tutorial2(std::string const& fileName)
{
    // Initialize a MIDI sample with 4 tracks
    Sample sample(4);

    // As seen in Tutorial 1, the basic way to create a note is to specify a start position, duration, and pitch.
    sample << Note(0, 4, 48);

    // This is how you specify whether the second argument is the duration or the end position:
    sample << Note(4, Dur(2), 60);
    sample << Note(4, Pos(6), 60); // we just added the same note twice (4 + 2 = 6)
    
    // Positions and durations can be floating point values. Pitches can't.
    sample << Note(Pos(6.5), Dur(1.5), 60);

    // Velocity is the force with which a note is played.
    // Let's specify the velocity of the notes:
    sample << Note(8, 1, 60);                   // default: velocity=0.5 (equivalent to MIDI 64)
    sample << Note(9, 1, 60).velocity(0);       // lowest possible (equivalent to MIDI 1)
    sample << Note(10, 1, 60).velocity(1);      // highest possible (equivalent to MIDI 127)
    sample << Note(11, 1, 60).velocityMidi(99); // by MIDI velocity value (1-127)
    
    // Assigning notes to different channels allows MIDI devices and software to play them
    // using separate instruments. In the MID library, channels are zero-based and represented
    // by numbers between 0 and 15. This is how you assign a note to a specific channel:
    sample << Note(12, 1, 60);            // default: channel 0
    sample << Note(12, 1, 60).channel(0); // same (explicit syntax)
    sample << Note(13, 1, 60).channel(1); // channel 1
    sample << Note(14, 1, 60).channel(2); // channel 2

    // The add() function provides another syntax for adding notes or other addable things to a sample:
    sample.add(Note(15, 1, 60)); // same as 'sample << Note(15, 1, 60);'

    // This alternative syntax can be used to add a note to a specific MIDI track:
    sample.add(Note(15, 1, 60));    // default: track 0 (if sample.currentTrack() has not been called before)
    sample.add(Note(15, 1, 60), 1); // add to track 1
    sample.add(Note(15, 1, 60), 2); // add to track 2 (these 3 notes are simultaneous, but are part of 3 different tracks)

    // This is an easier way to add multiple notes to the same track:
    sample.currentTrack(3); // specifies that new notes will be added to track 3 by default
    sample << Note(16, 1, 60); // added to track 3
    sample << Note(17, 1, 60); // added to track 3
    sample.add(Note(18, 1, 60), 2); // added to track 2
    sample << Note(19, 1, 60); // added to track 3 again

    // The << notation allows you to add many notes at once:
    sample.currentTrack(1)
        << Note(20, 1, 60)
        << Note(21, 1, 64)
        << Note(22, 1, 67)
        << Note(22, 3, 72);

    // Notes and other addable things can be bulk-added (using 'add' or '<<').
    sample.currentTrack(0);
    {
        // Vector of notes
        auto notes = std::vector<Note>{{23, 1, 60}, {24, 1, 60}};
        sample.add(notes);
    }
    {
        // Vector of array of notes
        auto notes = std::vector<std::array<Note, 2>>{
            {Note{25, 1, 60}, Note{26, 1, 60}},
            {Note{27, 1, 60}, Note{28, 1, 60}}
        };
        sample << notes;
    }
    {
        // Tuple of notes
        auto notes = std::make_tuple(
            Note{29, 1, 60},
            Note{30, 1, 60},
            std::make_tuple(
                Note{31, 1, 60},
                Note{32, 1, 60}
            )
        );
        sample << notes;
    }
    {
        // ...or even: any collection or tuple of any
        // any collection or tuple of (...) of notes,
        // tuples, and other addable things...
    }

    // You never have to worry about adding notes or other things in chronological order.
    sample << Note(0, 12, 48);

    // Save MIDI file
    sample.save(fileName);
}

#include "tutorial1.hpp" // back to the previous tutorial
#include "tutorial3.hpp" // to the next tutorial

#endif
