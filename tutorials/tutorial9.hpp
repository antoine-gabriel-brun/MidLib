#ifndef TUTORIAL_9_HPP
#define TUTORIAL_9_HPP

/*----------------------------------------------------------------------.
                                                                        |
   TUTORIAL #9   :   VECTORS WITHIN SAMPLES                             |
                                                                        |
   This tutorial is about using class Sample as a combined container.   |
                                                                        |
-----------------------------------------------------------------------*/

#include "../Mid/Lib.hpp"
using namespace MID;

#include <cassert>
#define _USE_MATH_DEFINES // for old platforms
#include <math.h>
#include <random>

void tutorial9(std::string const& fileName)
{
    Sample sample(4); // 4 tracks

    // Once added to a sample, notes can still be accessed from outside.
    {
        // Add a note
        Note note = Note(0, 1, 60);
        sample.currentTrack(2) << note;

        // Access it back
        Note& ref = sample[2][0]; // ref refers to a note within the sample

        // Replace it with another note of a different pitch
        sample[2][0] = Note(0, 1, 61); // this replaces the 1st note in the 3rd track
        ref = Note(0, 1, 61);          // same
    }

    // Let's break down the '[2][0]' syntax:
    {
        // Short syntax using subscript operator ('[]')
        Track& track = sample[2]; // access the 3rd track in sample
        Note& ref = track[0];     // access the 1st note in track

        // Explicit syntax (same effect)
        track = sample.track(2);
        ref = track.note(0);
    }

    // What you access back is different (in terms of address) as what you added:
    {
        // Add a note 'x' into a track
        Note x = Note(0, 1, 60);
        sample.currentTrack(1) << x;

        // Access it back as 'y'
        Note& y = sample[1][0];     // y refers to a note within the sample...

        bool sameValue = (x == y);  // ...which has the same value as x, ...
        bool sameAddr = (&x == &y); // ...but not the same address, because
                                    // x was copied when added to the sample.
        assert(sameValue == true);
        assert(sameAddr == false);
    }

    // You can access not only the notes you added, but also all other events.
    {
        // Here we set a pitch bend value and access it back:
        sample.currentTrack(1) << PitchBend(0)(-0.7);
        auto& pitchBend = sample[1].pitchBend(0);
        pitchBend = sample[1].pitchBends()[0]; // same

        // Here we define 5 pitch bend values and then modify the third one:
        sample.currentTrack(0)
            << PitchBend(0)(-0.4)
            << PitchBend(1)(-0.8)
            << PitchBend(2)(-1.2)
            << PitchBend(3)(-1.6)
            << PitchBend(4)(-2.0);
        sample[0].pitchBend(2).semitones(0);
    }

    // This is how you manage tracks in a sample:
    {
		Sample sample2(10);
        
        // Get the current number of tracks
        size_t nb = sample2.nbTracks(); // 10

        // Add new empty tracks to the sample
		Track& newTrack1 = sample2.addEmptyTrack();  // 1. add a new empty track after the last track and get a reference to it
        Track& newTrack2 = sample2.addEmptyTrack(7); // 2. add a new empty track at the specified index and get a reference to it
		sample2.addEmptyTrack(7);                    // (in most cases, you won't keep the returned reference)
        sample2.addEmptyTracks(4);                   // 3. add a range of 4 new empty tracks after the last track
        sample2.addEmptyTracks(7, 4);                // 4. add a range of 4 new empty tracks at the specified index
        nb = sample2.nbTracks(); // 21

        // Remove tracks from the sample
		sample2.removeTrack(5);      // 1. remove track 5
        sample2.removeTracks(3, 10); // 2. remove 10 tracks, from track 3 to track 12
        nb = sample2.nbTracks(); // 10

        // Some of the above operations will reset 'currentTrack'
        // to 0 (if the current track has been deleted), or update
        // its value to keep it pointing to the correct track.

        // Completely erase the contents of a track
        sample2[3] = {};      // track 3 is replaced with a new empty track
        sample2[3] = Track(); // same
    }

    // All elements within a track can be accessed back using these functions:
    {
        // Get a reference to the second track
        Track& track = sample[1];

        // Get a reference to the internal std::vector that holds all elements of a given type within this track
        auto a = track.notes();
        auto b = track.pitchBends();
        auto c = track.controlChanges();
        auto d = track.programChanges();
        auto e = track.channelAftertouches();
        auto f = track.keyAftertouches();
        auto g = track.unsafeEvents();

        // Get the number of elements of a given type within this track
        size_t h = track.nbNotes();
        size_t i = track.nbPitchBends();
        // Also: nbControlChanges(), nbProgramChanges(), nbChannelAftertouches(), nbKeyAftertouches(), nbUnsafeEvents()

        // Get a reference to a note within this track by index (short syntax using subscript operator)
        Note& j = track[0];

        // Get a reference to an element of a given type within this track by index
        Note& k = track.note(0); // same as 'track[0]' and 'track.notes()[0]'
        auto& l = track.pitchBend(0); // same as 'track.pitchBends()[0]'
        // Also: controlChange(0), programChange(0), channelAftertouch(0), keyAftertouch(0), unsafeEvent(0)
    }

    // You can use all std::vector functions for track elements of any type.
    {
        // Clear
        sample[1].programChanges().clear();

        // Push
        sample[2].notes().push_back(Note(0, 1, 60));

        // Insert and erase
        auto& notes = sample[3].notes();
        int pos = 0;
        notes.insert(notes.begin() + pos, Note(0, 1, 62));
        notes.erase(notes.begin() + pos);

        // etc.
    }

    // In the previous tutorials, you learned that the MID library sorts events
    // for you. This only happens when you save a sample to a MIDI file, and
    // doesn't affect the sample object itself, so you can access added elements
    // with the guarantee that they have not been re-indexed in the meantime.
    {
        // Add two notes in reverse chronological order
        sample.currentTrack(3) << Note(1, 1, 60) << Note(0, 1, 62);

        // Access the first added note
        auto& a = sample[3][0]; // this is the note of pitch 60, even if this note is played after

        // Save the sample
        sample.save(fileName); // in the file, events are sorted correctly

        // Access the first added note again
        auto& b = sample[3][0]; // this is still the note of pitch 60
    }

    // All of these features allow you to use Sample as a container to implement
    // algorithms that require writing and reading back notes and other events.
    // Here is an example:
    {
        // Options
        const int tempo = 720;
        const int durMinutes = 2;
        const int nbNotes = tempo * durMinutes;
        const int minPitch(40), maxPitch(88);
        const float sinPeriodSeconds = 3.0961523f;
        const int nbRanges = nbNotes / 8;
        const int minRangeLen(2), maxRangeLen(5);

        // Sample settings
        sample.tempo(tempo).currentTrack(0);

        // Helper functions
        auto scalePitch = [=](float valAmpl1) -> int {
            float val01 = 0.5f * (valAmpl1 + 1.0f);
            return minPitch + (int)floor(val01 * (maxPitch - minPitch + 0.999f));
        };
        auto rand32 = []() -> uint32_t {
            return
                (rand() & 0xFF) << 24 |
                (rand() & 0xFF) << 16 |
                (rand() & 0xFF) << 8 |
                (rand() & 0xFF);
        };

        // Add many notes whose pitches are determined by a mathematical function
        for (int i = 0; i < nbNotes; i++) {
            float sinMult = (float)(2. * M_PI) / sinPeriodSeconds;
            float timeSeconds = (float)i * 60.0f / (float)tempo;
            float sinResult = sin(timeSeconds * sinMult);
            int pitch = scalePitch(sinResult);
            sample << Note(i, 0.5, pitch).velocity(0.8);
        }

        // Access the added notes to sort pitches in ascending order within some small note ranges
        for (int n = 0; n < nbRanges; n++) {
            int rangeLen = minRangeLen + rand32() % (maxRangeLen - minRangeLen + 1);
            int rangeStart = rand32() % (nbNotes - rangeLen);

            // Get back the pitches in this time range
            std::vector<int> pitches(rangeLen);
            for (int i = 0; i < rangeLen; i++) {
                pitches[i] = sample[0][rangeStart + i].pitch();
            }
            
            // Sort them
            std::sort(pitches.begin(), pitches.end());
            
            // Store sorted pitches
            for (int i = 0; i < rangeLen; i++) {
                sample[0][rangeStart + i].pitch(pitches[i]);
            }
        }

        // Listen to the result and open it in a DAW.
    }

    // Save MIDI file
    sample.save(fileName);
}

#include "tutorial8.hpp"  // back to the previous tutorial
#include "tutorial10.hpp" // to the next tutorial

#endif
