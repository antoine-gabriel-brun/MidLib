#ifndef TUTORIAL_11_HPP
#define TUTORIAL_11_HPP

/*---------------------------------------------------------------.
                                                                 |
   TUTORIAL #11   :   COMMON MISTAKES                            |
                                                                 |
   Here are some common sources of bugs and how to avoid them.   |
                                                                 |
----------------------------------------------------------------*/

#include "../Mid/Lib.hpp"
using namespace MID;

#include <thread>

void tutorial11(std::string const& fileName)
{
    Sample sample(5);

    // Mistake #1: Side effects when adding elements with <<
    {
        // Let's add 8 consecutive notes to the sample:
        int pos = 0;
        sample << Note(pos++, 1, 60)
               << Note(pos++, 1, 62)
               << Note(pos++, 1, 64)
               << Note(pos++, 1, 65)
               << Note(pos++, 1, 67)
               << Note(pos++, 1, 69)
               << Note(pos++, 1, 71)
               << Note(pos++, 1, 72);

        // Here you might expect to hear an ascending scale. But depending on your
        // compiler, you may also get a descending one! Before C++17, the evaluation
        // order of the operands of operator << is not guaranteed, and your compiler
        // can (and probably will) evaluate them in reverse order.
        // --> If you are using a modern compiler AND don't care about
        //     compatibility with older ones, you can forget about this warning.
        // --> Otherwise, keep in mind that operations like ++ that have side
        //     effects should not be used in conjunction with <<s chained together.
    }
    
    // Mistake #2: Adding note-on and note-off events to a sample
    {
        // The MID library provides a class that represents a note-on or a note-off event.
        Event_NoteOnOff noteEvent = Event_NoteOnOff::on(Pos(0), 60, 127);

        // However, unlike many other events that can be added directly to
        // a sample, the following (without "quotes") would not compile:
        "sample << NoteOnOff::on(Pos(0), 60, 127)";
        "sample << NoteOnOff::off(Pos(1), 60)";

        // Note events should only be added using class Note, which automatically adds a note-on/note-off pair.
        sample.currentTrack(1) << Note(0, 1, 60).velocityMidi(127);

        // This limitation is for your safety, so that every note-on has a corresponding note-off.
        // If you really want to go beyond this limitation, you can use the addUnsafe() function:
        sample.addUnsafe(Event_NoteOnOff::on(Pos(0), 60, 127));
        sample.addUnsafe(Event_NoteOnOff::off(Pos(1), 60));

        // Using this dangerous trick, you can add any MIDI event wherever you want. But keep in mind
        // that this is almost certainly a bad idea: do not use this feature unless you really have to.
        sample.addUnsafe(Event_EndOfTrack(Pos(10)));
        sample.addUnsafe(Event_Tempo::fromMidi(1234567, Pos(12)));
    }

    // Mistake #3: Misunderstanding how the MID library sorts events for you
    {
        // According to the MIDI file specifications, the events within each track must be in
        // chronological order. The MID library takes care of this for you, allowing you to add
        // notes or other elements in any order, regardless of their final position in the sample.
        // However, there are times when you should consider the order in which you add events.
        {
            // -- Case #1: Adding notes in any order
            // Here the MID library sorts note-on and note-off events.
            // You don't need to worry about anything.
            sample.currentTrack(2);
            sample << Note(1, 2, 60); // we wrote the 2nd note first
            sample << Note(0, 4, 62);
            // Sorted events: note-on 62, note-on 60, note-off 60, note-off 62, end-of-track
        }
        {
            // -- Case #2: Adding a note where another note of the same pitch ends
            // The note-off of the 1st note will be written before the note-on of the 2nd note.
            // You don't need to worry about anything.
            sample << Note(4, 4, 58); // we wrote the 2nd note first
            sample << Note(0, 4, 58);
            // Sorted events: note-on 58, note-off 58, note-on 58, note-off 58, end-of-track
        }
        {
            // -- Case #3: Adding simultaneous note events and other events in any order
            // The MID library decides whether a given event should be written before or after a simultaneous note-on.
            // You don't need to worry about anything.
            sample << Aftertouch(0)(64, 0.5);   // aftertouch should come after note-on, but we wrote it before
            sample << Note(0, 1, 64);           // note starts at the same time as the other 2 events
            sample << Instrument(0).panFlute(); // program change should come before note-on, but we wrote it after
            // Sorted events: program-change, note-on, aftertouch, note-off, end-of-track
        }
        {
            // -- Case #4: Adding simultaneous events whose type makes it impossible to determine which should occur first
            // In this case only, the order in which you add events matters.
            // Events will be appended to the file in the same order as they were added.
            sample << Instrument(0)();         // this program change is added first and therefore has no effect
            sample << Instrument(0).bagpipe(); // this program change is added second and is the one that will be heard
            // *Unsorted* events: program-change Acoustic grand piano, program-change Bagpipe, end-of-track
        }

        // All of these cases are common when you add multiple layers of information to the same track. For
        // example, you might first add a melody that lasts for the entire duration of the sample, then a bass
        // line, and then many pitch-bend and pan events that occur at random times. The explanations above
        // show how the MID library sorts these events and in which cases the order is left up to the user.
    }
    
    // Mistake #4: Accessing track elements by multiple threads simultaneously
    {
        // The containers used by the MID library are not thread-safe.
        // This code (with 'if (true)' instead of 'if (false)') would
        // sometimes throw, and sometimes run fine:

        if (false) {
            // Options
            sample.currentTrack(4);
            const int nbNotesPerThread = 40;
            int currentPos = 0;

            // Function to be executed within threads
            auto fct = [&sample, &currentPos, nbNotesPerThread]() {
                for (int i = 0; i < nbNotesPerThread; i++) {
                    sample << Note(currentPos++, 1, 36 + i);
                }
            };

            // Run it twice at the same time (will perrhaps throw)
            std::thread thread1(fct);
            std::thread thread2(fct);
            thread1.join();
            thread2.join();
        }
    }
    
    // Mistake #5: discarding the result of a sample operator
    {
        // Sample operators return copied versions of samples, leaving them
        // unchanged. Thus they can't be used to modify samples directly. For
        // instance, the second statement below (without "quotes") has no effect:
        Sample s;
        "s.transpose(+3)";

        // Here is the correct way to transpose s:
        s = s.transpose(+3);
        
        // To prevent you from accidentally discarding the returned value
        // of sample operators, they are all declared nodiscard. Therefore,
        // the following statement (without "quotes") should issue a warning:
        "s.transpose(+3)";
    }

    // Save MIDI file
    sample.save(fileName);
}

#include "tutorial10.hpp" // back to the previous tutorial

#endif
