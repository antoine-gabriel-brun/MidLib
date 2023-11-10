#ifndef TUTORIAL_7_HPP
#define TUTORIAL_7_HPP

/*--------------------------------------------------------------------------------------------.
                                                                                              |
   TUTORIAL #7   :   AFTERTOUCH                                                               |
                                                                                              |
   Here is how to use aftertouch to send additional pressure values after a note is struck.   |
                                                                                              |
---------------------------------------------------------------------------------------------*/

#include "../Mid/Lib.hpp"
using namespace MID;

void addChord(Sample& sample, int pos, bool twoDifferentChannels)
{
    sample /* C4 */ << Note(pos, 8, 60).channel(0)                             // the 2 lowest notes are
           /* E4 */ << Note(pos, 8, 64).channel(0)                             // on default channel 0;
           /* G4 */ << Note(pos, 8, 67).channel(twoDifferentChannels ? 1 : 0)  // the 2 highest notes are on default channel 0 or
           /* C5 */ << Note(pos, 8, 72).channel(twoDifferentChannels ? 1 : 0); // on channel 1, depending on 'twoDifferentChannels'.
}

void tutorial7(std::string const& fileName)
{
    // Initialize a MIDI sample
    Sample sample;

    // Add a long chord at t=0
    addChord(sample, 0, false); // see the definition of addChord() above

    // Send a pressure of 0.8 at t=4
    sample << Aftertouch(4)(0.8); // this will affect all notes on default channel 0 (i.e. all 4 notes in the chord)

    // You can specify which notes are affected by aftertouch by targeting a specific channel or pitch
    addChord(sample, 8, true); // add a new chord that uses both channels 0 and 1 (see the definition of addChord() above)
    float pressureHi = 0.95f, pressureLo = 0.15f;
    sample << Aftertouch(10)(pressureHi)        // this affects the lowest 2 notes of the chord (channel 0 is the default)
           << Aftertouch(10, 0)(pressureHi)     // same with explicit channel 0
           << Aftertouch(11, 1)(pressureHi)     // this affects the highest 2 notes of the chord (channel 1)
           << Aftertouch(12, 0)(60, pressureLo) // this only affects the lowest note on channel 0 (middle C)
           << Aftertouch(13, 1)(72, pressureLo) // this only affects the highest note (C5)
           << Aftertouch(14)(72, pressureHi);   // no note is affected: there is no C5 on channel 0
    // You can't target all the notes currently playing on all channels: you have to send an aftertouch event on each channel.

    // Save MIDI file
    sample.save(fileName);
}

#include "tutorial6.hpp" // back to the previous tutorial
#include "tutorial8.hpp" // to the next tutorial

#endif
