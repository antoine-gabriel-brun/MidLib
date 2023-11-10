#ifndef TUTORIAL_4_HPP
#define TUTORIAL_4_HPP

/*-----------------------------------------------.
                                                 |
   TUTORIAL #4   :   PITCH BENDS                 |
                                                 |
   This tutorial shows how to add pitch bends.   |
                                                 |
------------------------------------------------*/

#include "../Mid/Lib.hpp"
using namespace MID;

void tutorial4(std::string const& fileName)
{
    // Initialize a MIDI sample
    Sample sample;
    sample.ticksPerQuarterDefault();
    UINT_16 ticksPerQuarter = sample.ticksPerQuarter();

    // Add four quarter Cs and a whole C with five different pitch bend values
    sample
        << Note(0, 1, 60)    // here we have grouped notes together and pitch
        << Note(1, 1, 60)    // bends together: we don't need to add events
        << Note(2, 1, 60)    // in chronological order. MID library internally
        << Note(3, 1, 60)    // sorts events before writing them to the file.
        << Note(4, 4, 60)
        << PitchBend(0)(0.0) // useless: reset pitch bend to 0 when the first note starts
        << PitchBend(1)(0.5) // set pitch bend to +1/2 semitone when the second note starts
        << PitchBend(2)(1.0) // set pitch bend to +1 semitones when the third note starts
        << PitchBend(3)(1.5) // set pitch bend to +1.5 semitone when the fourth note starts
        << PitchBend(4)(-.5) // set pitch bend to -1/2 semitone when the fifth note starts
        << PitchBend(8)();   // after the last note: other way to reset pitch bend to 0

    // This is how to understand the bizarre pitch bend syntax with two successive pairs of parentheses:
    {
        // Weird syntax seen above: at pos=0, pitch bend is 0.0
        sample << PitchBend(0)(0.0);

        // The following three statements show how to break it down, with the same effect:
        auto creator = PitchBend(0); // 1. Create a 'pitch bend creator' that can create pitch bend values at t=0...
        auto evt = creator(0.0);     // 2. Use it to create an event with the value 0.0...
        sample << evt;               // 3. Add this event to the sample.
    }

    // Now let's add a single long note with a continuous pitch bend slide (from normal to +1 semitone)
    int pos = 8;
    {
        // Options
        const int longNotePos = pos;
        const int longNoteDur = 4;
        const int nbValues = longNoteDur * ticksPerQuarter; // one pitch bend value per MIDI tick

        // Add the long note
        sample << Note(longNotePos, longNoteDur, 60);

        // Add a continuous pitch bend automation
        for (int i = 0; i <= nbValues; i++) {
            Pos p(longNotePos + (double)i / ticksPerQuarter); // pos from longNotePos to (longNotePos + longNoteDur)
            float val = (float)i / nbValues; // slide from 0 to +1 semitone
            sample.add(PitchBend(p)(val));   // add bend value (here we have chosen
                                             // to use of the long syntax '.add()')
        }

        // Update the current position for the next events
        pos += longNoteDur;
    }

    // This library treats pitch bend values as floating point numbers between -2.0f (included) and +2.0f (excluded).
    // If you don't want to use this convention, you can use MIDI values directly:
    {
        // 0 is the minimum raw MIDI pitch bend value
        sample << PitchBend(pos).fromMidi(0)
               << Note(pos, 2, 60);

        // 8192 is the raw MIDI value that means no deviation
        sample << PitchBend(pos + 2).fromMidi(8192)
               << Note(pos + 2, 1, 60);

        // 16383 is the maximum raw MIDI value
        sample << PitchBend(pos + 3).fromMidi(16383)
               << Note(pos + 3, 1, 60);

        // Update the current position for the next events
        pos += 4;
    }

    // Important: the actual meaning of a pitch bend value (in terms of pitch deviation) isn't
    // specified in the standard. However, 0.0f (MIDI 8192) always means no modulation, and most
    // implementations assume that the minimum value means -2 semitones and the maximum value means
    // (almost) +2 semitones. The MID library has been designed using this convention, but some
    // weird devices may, for example, treat the minimum value as -1 semitone and the maximum
    // value as +1 semitone. To test if a device follows the common convention, run this tutorial
    // and listen to the resulting MIDI file: here you should hear exactly the same pitch 4 times.
    {
        sample
            << Note(pos    , 1, 60) << PitchBend(pos    )()    //    C4  with pitch bend =  0
            << Note(pos + 1, 1, 61) << PitchBend(pos + 1)(-1)  // is C#4 with pitch bend = -1
            << Note(pos + 2, 1, 62) << PitchBend(pos + 2)(-2)  // is D4  with pitch bend = -2
            << Note(pos + 3, 1, 59) << PitchBend(pos + 3)(+1); // is B3  with pitch bend = +1
    }
    
    // Save MIDI file
    sample.save(fileName);

    // Now, listen to the generated file or open it in a DAW.
}

#include "tutorial3.hpp" // back to the previous tutorial
#include "tutorial5.hpp" // to the next tutorial

#endif
