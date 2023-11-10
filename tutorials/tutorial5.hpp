#ifndef TUTORIAL_5_HPP
#define TUTORIAL_5_HPP

/*---------------------------------------------------------------------------------------.
                                                                                         |
   TUTORIAL #5   :   CONTROL CHANGES                                                     |
                                                                                         |
   Control changes are used to automate anything. This tutorial shows how to use them.   |
                                                                                         |
----------------------------------------------------------------------------------------*/

#include "../Mid/Lib.hpp"
using namespace MID;

void tutorial5(std::string const& fileName)
{
    // Initialize a MIDI sample with 2 tracks and 4 whole notes on each track
    Sample sample(2);
    sample.currentTrack(0) << Note(0, 4, 48) << Note(4, 4, 48) << Note(8, 4, 48) << Note(12, 4, 48);
    sample.currentTrack(1) << Note(0, 4, 60) << Note(4, 4, 60) << Note(8, 4, 60) << Note(12, 4, 60);

    // In the MIDI standard, many parameters are encoded using 'control changes'.
    // For example, this is the case with the pan (i.e. how much the sound comes from the left or right).
    // Here we add 4 control change events that affect the pan:
    sample.currentTrack(0)
        << Pan(0)(0.0) // at t=0, pan is completely left
        << Pan(1)(0.2)
        << Pan(2)(0.8)
        << Pan(3)(1.0); // at t=3, pan is completely right

    // In the example above, we call operator() on Pan objects to create
    // the appropriate events. However, you can use many other functions,
    // depending on whether you want to create low resolution or high
    // resolution MIDI events, or whether you want to specify the values
    // as floating point numbers or as a raw MIDI values.
    Pan(0)(0.5); // default syntax shown above: low resolution event from floating point value
    Pan(0).lowResolutionFromFloat(0.5); // same
    Pan(0).highResolutionFromFloat(0.5); // same, but creates 2 events to achieve high resolution
    Pan(0).lowResolutionFromMidiValue(64); // low resolution from raw MIDI value (0-127)
    Pan(0).highResolutionFromMidiValue(8192); // high resolution from raw MIDI value (0-16383)
    Pan(0).highResolutionFromMidiValues(64, 0); // high resolution from 2 raw MIDI values (0-127)

    // High resolution values are internally encoded as 2 consecutive control changes
    // with different controller indices. Some devices and software don't support this
    // feature; in this case, they will see an automation on two different controllers
    // (named Pan MSB and Pan LSB, for example, or anything else). This should be a
    // good reason for you to always use low resolution control changes if you don't
    // know whether the target software or device supports high resolution values.
    // For this reason, the following three syntaxes should generally be preferred over
    // the ones listed above. They generate a single low resolution control change:
    Pan(0)(0.5);           // default syntax
    Pan(0).fromFloat(0.5); // same (low resolution from float)
    Pan(0).fromMidi(64);   // same, but accepts a raw MIDI value (0-127)

    // Some specific control changes, known as 'switches', are created using a boolean value:
    sample << Sustain(0)(true); // switch sustain ON at t=0

    // Many others only accept low resolution values:
    sample << GeneralPurposeController5(0)(0.5);           // default syntax
    sample << GeneralPurposeController5(0).fromFloat(0.5); // same
    sample << GeneralPurposeController5(0).fromMidi(64);   // from raw MIDI value (0-127)

    // Finally, some others don't take arguments at all.
    sample << AllNotesOff(0)(); // don't forget the empty () pair

    // If you know the controller number for a particular purpose,
    // you may use it directly. The following does the same thing
    // in the second track as we initially did in the first one:
    sample.currentTrack(1)
        << CustomControlChange<10>(0)(0.0) // 10 is the controller number for pan
        << CustomControlChange<10>(2)(0.2)
        << CustomControlChange<10>(4)(0.8)
        << CustomControlChange<10>(6)(1.0);

    // You can always specify the channel number for a control change.
    double pos = 10;
    int channel = 3;
    sample << ModulationWheel(pos, channel).highResolutionFromMidiValues(28, 96);
    sample << ChannelVolume(pos, channel)(0.75);
    sample << OmniModeOn(pos, channel)();
    sample << CustomControlChange<10>(pos, channel)(0.5);

    // The list of all control changes defined by the standard
    // is available in file 'ControlChangesCreators.hpp'.
    // Here are some common ones you may need frequently:
    pos = 12;
	sample << ChannelVolume       (pos)(0.5);
	sample << Pan                 (pos)(0.5);
	sample << ModulationWheel     (pos)(0.5);
	sample << ExpressionController(pos)(0.5);
	sample << Balance             (pos)(0.5);
	sample << PortamentoControl   (pos)(0.5);
	sample << Sustain             (pos)(true);
	sample << Portamento          (pos)(true);
	sample << Sostenuto           (pos)(true);
	sample << LegatoFootswitch    (pos)(true);
	sample << AllNotesOff         (pos)();

    // Save MIDI file
    sample.save(fileName);
}

#include "tutorial6.hpp" // to the next tutorial
#include "tutorial4.hpp" // back to the previous tutorial

#endif
