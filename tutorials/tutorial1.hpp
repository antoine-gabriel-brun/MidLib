#ifndef TUTORIAL_1_HPP
#define TUTORIAL_1_HPP

/*-----------------------------------------------------------------------------------------.
                                                                                           |
   TUTORIAL #1   :   BASIC USE                                                             |
                                                                                           |
   This tutorial shows how to create a simple MIDI sample, add notes to it, and save it.   |
                                                                                           |
------------------------------------------------------------------------------------------*/

#include "../Mid/Lib.hpp" // To use the MID library, include Lib.hpp, located in the Mid directory.
using namespace MID;      // This saves you typing 'MID::' in front of each MID class name.

void tutorial1(std::string const& fileName)
{
    // Initialize a MIDI sample
    Sample sample;

    // Add notes defined by position, duration, and pitch
    sample << Note(0, 1, 60); // 60 is the Middle C (C4).
    sample << Note(1, 1, 64); // this note begins at t=1.
    sample << Note(2, 2, 67); // this one begins at t=2 and is a half-note (duration=2).

    // Save MIDI file
    sample.save(fileName); // instead of 'fileName', you can specify the desired file name in double quotes (e.g. "sample.mid")

    // Now, listen to the generated file or open it in a DAW.
}

#include "tutorial2.hpp" // to the next tutorial

#endif
