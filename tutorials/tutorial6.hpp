#ifndef TUTORIAL_6_HPP
#define TUTORIAL_6_HPP

/*------------------------------------------------------------------.
                                                                    |
   TUTORIAL #6   :   PROGRAM CHANGES                                |
                                                                    |
   This tutorial is about specifying which instrument is playing.   |
                                                                    |
-------------------------------------------------------------------*/

#include "../Mid/Lib.hpp"
using namespace MID;

void tutorial6(std::string const& fileName)
{
    // Initialize a MIDI sample
    Sample sample;

    // In the MIDI standard, each channel (not each track) is played by an independent instrument.
    // The instrument playing each channel can be changed at any time using Program change events.
    // Here is how to change the instrument playing channels 0, 1 and 2 at t=0:
    double pos = 0;
    sample << Instrument(pos, 0).ocarina();
    sample << Instrument(pos, 1).harpsichord();
    sample << Instrument(pos, 2).mutedTrumpet();

    // If you know the MIDI index of the instrument you want to use, you can specify it directly.
    // Note that unlike most MIDI instrument lists, the instrument index is zero-based.
    sample << Instrument(pos, 3)(61); // 61 (62 in common base-1 instrument lists) stands for 'brass section'

    // This resets channel 4 to the default instrument (Acoustic grand piano) at t=pos.
    sample << Instrument(pos, 4)();

    // To make the program changes audible, we now add some notes to the affected channels:
    int nbChannels = 5;
    for (int i = 0; i < nbChannels; i++) {
        int pos = i;
        const int dur = 1;
        int pitch = 60 + i;
        sample << Note(pos, dur, pitch).channel(i);
    }

    // When dealing with instruments, channel 9 is special: all notes in it
    // are treated as percussion sounds. This means that a program change
    // with a given instrument index represents a different instrument on
    // that channel. To demonstrate this behavior, let's generate a middle C
    // followed by a middle D on each channel successively. We will hear these
    // two notes 15 times played by a violin, but 1 time played by a low tom:
    {
        int initialPos = 6;
        for (int channel = 0; channel <= 15; channel++) { // channel 9 is special
            int pos = initialPos + channel;
            int instrumentIndex = 40; // 40 (41 in common base-1 instrument lists) represents 'violin'
                                      // on every channel but represents 'low floor tom' on channel 9
            sample << Instrument(pos, channel)(instrumentIndex)  // change the instrument at t=pos
                   << Note(pos      , 0.3, 60).channel(channel)  // add a Middle C at t=pos
                   << Note(pos + 0.5, 0.3, 62).channel(channel); // add a Middle D at t=pos+0.5
        }
    }
    // In the MID library, the special channel is called 'channel 9' because
    // channel indices are zero-based. Other MIDI devices and software may
    // count channels from 1 and call this special channel 'channel 10'.

    // You can access all of the special channel 9 standard drum sounds by name:
    sample << Instrument(23, 9).drum_maracas(); // all of these names are prefixed by 'drum_'
    
    // Instruments intended for special channel 9 can't be added to other channels, and vice versa.
    try {
        sample << Instrument(23, 0).drum_triangle();
    } catch (Exception const&) { /* throws an exception: triangle is valid on channel 9 only */ }
    try {
        sample << Instrument(23, 9).piano();
    } catch (Exception const&) { /* throws an exception: piano isn't valid on channel 9 */ }

    // If necessary, here's how you get around this limitation:
    sample << Instrument(23, 0).allowWrongChannel().drum_triangle();
    sample << Instrument(23, 9).allowWrongChannel().piano();

    // Save MIDI file
    sample.save(fileName);

    // Now, listen to the generated file in a player that supports MIDI instruments.
}

#include "tutorial5.hpp" // back to the previous tutorial
#include "tutorial7.hpp" // to the next tutorial

#endif
