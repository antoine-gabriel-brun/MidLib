# MidLib

**C++ library for easy MIDI sample manipulation**

MidLib allows users to handle MIDI samples inside C++ programs, and to save them as `.mid` files. The library supports notes, multi-tracks samples, channels, pitch bends, control and program changes, aftertouch, tempo, time signature, custom events, as well as many tool for manipulating and transforming musical samples as complex as desired.

## What is MidLib?

MidLib is a C++ library. Once included into a C++ project, users can create objects that store musical informations and can be exported to MIDI files.

MidLib is both designed for simplicity and flexibility. For basic use (creating a sample - adding notes - saving as `.mid` file), it is very simple and straightforward. The underlying complexity (chunks, events, bytewise file writing) and the MIDI files specifications remains always hidden. However, advanced users can use it to create complex MIDI files that make use of most of the features offered by the MIDI standard.

## Is MidLib easy to use?

The main purpose of the MidLib library is to make it as easy as possible for the user to generate MIDI files. To this aim, all things that make the MIDI specifications tricky have been covered by a simple syntax that hides all the underlying misery. For instance:

* You don't need to add notes and events chronologically: MidLib sorts events for you.
* Notes are added in a single step: MidLib handles NoteOn and NoteOff events, and controls in a transparent way what happens if some notes overlap.
* Most magic numbers are hidden and replaced by user-friendly names. You won't need to know the controller index for a given control change or an program change index for an instrument, but will name them in your code using words like Pan, Accordion or ChannelVolume.
* The tricky things like chunks, variable-length sizes, running status, 14-bits and 28-bits integers etc. are completely hidden and handled without any action on your part.

## What features does the MidLib library *not* support?

* MidLib can only **write** MIDI files. As it stands, it is not designed to *open* existing MIDI files, to *play* MIDI samples as audio, or to build realtime MIDI devices.
* The only underlying MIDI file format available is format 1 (multitrack MIDI file).
* SMPTE times are not supported. The only time mode is beat-based.
* Some MIDI events of less interess like Lyrics, Cue point, etc. are not natively supported. Using them could necessitate to extend the library on one's own by deriving the base event class.

## How to get started?

* Add the library files into your project.
* Include the library and use the library's namespace:

	#include "Mid/Lib.hpp"
	using namespace MID;

* Inside a function, create an empty MIDI sample:

	Sample sample;
	 
* Add notes defined by position, duration, and pitch:

	sample << Note(0, 1, 60);

* Save the resulting MIDI file:

	sample.save("sample.mid");
		
Now, read and run the files inside the folder `Mid/tutorials/` to learn about all the other features.

## Who wrote the MidLib library and how?

This library was written by composer [Antoine Gabriel Brun](http://antoinegabrielbrun.com/) for his personal needs.

The references about the MIDI files specifications used when writing the MidLib library are:

* <http://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html>
* <http://www.somascape.org/midi/tech/mfile.html>

Good luck with MidLib, put it to good use!