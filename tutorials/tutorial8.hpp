#ifndef TUTORIAL_8_HPP
#define TUTORIAL_8_HPP

/*---------------------------------------------------------------------------.
                                                                             |
   TUTORIAL #8   :   REAL-LIFE EXAMPLE                                       |
                                                                             |
   So far, we have only added notes and other things manually, a task        |
   that does not require a library. This tutorial is a real-life example     |
   of using what we learned earlier to create a simple algorithmic melody.   |
                                                                             |
----------------------------------------------------------------------------*/

#include "../Mid/Lib.hpp"
using namespace MID;

#include <vector>
#include <random>

int randomInterval(std::vector<int> const& intervals)
{
    return intervals[rand() % intervals.size()];
}
void addArpeggio(Sample& sample, int& pos, int& lastPitch, std::vector<int> const& intervals, int minPitch, int maxPitch)
{
    // Select the new pitch:
    //   - only certain intervals with the last pitch are allowed
    //   - the new pitch should be between minPitch and maxPitch
    int newPitch;
    do {
        newPitch = lastPitch + randomInterval(intervals);
    } while (newPitch < minPitch || newPitch > maxPitch);

    // Add the arpeggio
    sample << Note(pos + 0.00, 0.25, newPitch     ).velocity(0.9)
           << Note(pos + 0.25, 0.25, newPitch +  4).velocity(0.6)
           << Note(pos + 0.50, 0.25, newPitch +  9).velocity(0.6)
           << Note(pos + 0.75, 0.25, newPitch + 12).velocity(0.6);

    // Update values
    pos++;
    lastPitch = newPitch;
}
float getPitchBendAt(float notePos, float pitchBendPeriod)
{
    // Get 0-1 position ratio within interval [0, pitchBendPeriod]
    float t = notePos / pitchBendPeriod;
    t -= floor(t);

    // Calc function (triangular oscillation between -1 and 1)
    if (t <= 0.5f) return  4.0f * t - 1.0f;
    else           return -4.0f * t + 3.0f;
}

void tutorial8(std::string const& fileName)
{
    // Initialize a MIDI sample
    Sample sample = Sample().tempo(176);

    // Algorithm options
    int nbArpeggios = 200;
    int minPitch(60), maxPitch(88);
    std::vector<int> intervals = {+1, +2, +3, +4, +6, -1, -2, -3, -4, -6};
    float pitchBendPeriod = 6; // unit = quarters

    // Add many random, non-redundant arpeggios
    int pos = 0;
    int lastPitch = (minPitch + maxPitch) / 2;
    for (int n = 0; n < nbArpeggios; n++) {
        addArpeggio(sample, pos, lastPitch, intervals, minPitch, maxPitch); // this call updates pos and lastPitch
    }

    // Add a continuous pitch bend
    int nbNotes = 4 * nbArpeggios;
    for (int noteIndex = 0; noteIndex < nbNotes; noteIndex++) {
        float notePos = std::max(0.f, 0.25f * (float)noteIndex - 0.05f);
        float pitchBendValue = getPitchBendAt(notePos, pitchBendPeriod);
        sample << PitchBend(notePos)(pitchBendValue);
    }

    // Save MIDI file
    sample.save(fileName);

    // Now listen to the resulting file or open it in a DAW.
}

#include "tutorial7.hpp" // back to the previous tutorial
#include "tutorial9.hpp" // to the next tutorial

#endif
