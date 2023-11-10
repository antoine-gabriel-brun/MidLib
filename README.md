# MidLib

**C++ library for easy MIDI sample manipulation**

*The MidLib library allows users to generate MIDI files using C++ programs. It supports MIDI notes, multi-track samples, channels, pitch bends, control changes, instruments, aftertouch, tempo, time signature, custom events, as well as many tool to manipulate and transform musical samples as complex as you like.*

## What is MidLib?

MidLib is a C++ library. Once included in a C++ project, it allows you to create musical objects and export them to MIDI files.

The MidLib library is designed for both simplicity and flexibility. The basic use (create a sample - add notes - save as `.mid` file) is very simple and straightforward, and the underlying complexity of the MIDI file specifications (chunks, events, byte-wise file writing...) is always hidden. However, advanced users can use it to create complex MIDI files that take advantage of most of the features offered by the MIDI standard.

## Is MidLib easy to use?

The main purpose of the MidLib library is to make creating MIDI files as easy as possible. To this end, all the things that make the MIDI specification tricky have been wrapped into a simple syntax that hides all the underlying misery. For instance:

* You don't need to add notes and events in chronological order: MidLib sorts events for you.
* Notes are added in one step: MidLib handles NoteOn and NoteOff events and transparently controls what happens if some notes overlap.
* Most of the magic numbers are hidden and replaced with user-friendly names. You don't need to know the controller index for a given control change, or the program change index for an instrument: you'll name them in your code using words like Accordion, Pan, or ChannelVolume.
* The tricky stuff like chunks, variable-length sizes, running status, 14-bit and 28-bit integers, etc. are completely hidden and handled without any action on your part.

## Which standard MIDI features are *not* supported by the MidLib library?

* MidLib can only **write** MIDI files. It is not designed to *open* existing MIDI files, *play* MIDI samples as audio, or handle real-time MIDI streams.
* The only underlying MIDI file format available is **format 1** (multitrack MIDI file).
* SMPTE times are not supported. The only time mode is **beat-based**.
* Some less interesting MIDI events such as lyrics, cue points, etc. are not natively supported. Using them may require **extending** the library by deriving the base event class.

## How do I get started?

* Download the library files and add them to your project.
* Include the library and use the library's namespace:

		#include "Mid/Lib.hpp"
		using namespace MID;

* Within a function, create a new empty MIDI sample:

		Sample sample;
	 
* Add notes defined by position, duration, and pitch:

		sample << Note(0, 1, 60);

* Save the resulting MIDI file:

		sample.save("sample.mid");
		
Now, read and run the files in the folder `Mid/tutorials/` to learn all the other features.

## Who wrote MidLib and how?

This library was written by French composer [Antoine Gabriel Brun](http://antoinegabrielbrun.com/) for his personal use.

The references about MIDI file specifications used when writing the MidLib library are:

* <http://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html>
* <http://www.somascape.org/midi/tech/mfile.html>

Good luck with MidLib, put it to good use!