# MidLib

**C++ library for easy MIDI sample manipulation**

MidLib allows users to handle MIDI samples inside C++ programs, and to save them as `.mid` files. The library supports notes, multi-tracks samples, channels, pitch bends, control and program changes, aftertouch, tempo, time signature, custom events, as well as many tool for manipulating and transforming musical samples.

## What is MidLib?

MidLib is a C++ library. Once included into a C++ project, users can create objects that store musical informations that can be exported to MIDI samples.

MidLib is both designed for simplicity and flexibility. For basic use (creating a sample - adding notes - saving as `.mid` file), it is very simple and straightforward. The underlying complexity - chunks, events, bytewise file writing - and the MIDI files specifications remains always hidden. However, advanced users can use it to create complex MIDI files that include most of the features offered by the MIDI standard.

The main library purpose is to make it as easy as possible for the user to create MIDI files. To this aim, all things that make the MIDI specifications tricky have been covered by a simple syntax that hides all this misery. For instance:

* You don't need to add notes and events chronologically: MidLib is sorting events for you when exporting your created samples to MIDI files.
* Notes are added in a single step: MidLib handles NoteOn and NoteOff events for you, and controls what happens if some notes overlap.
* Most magic numbers are hidden and replaced by user-friendly names. You never need to know a controller index or an instrument number, and can freely play with Pan, Accordion or ChannelVolume.
* The tricky things like variable-length integers, chunks, running status, 14-bits integers etc. are completely hidden and handled without any action on your part.

## What features does the MidLib library *not* support?

* MidLib can only write MIDI files. It is not designed to open existing MIDI files, to play MIDI samples as audio, or to build realtime MIDI devices.
* The only MIDI file format available is format 1 (multitrack MIDI file).
* SMPTE times are not supported. The only time mode is beat-based.
* Some rare MIDI events are not natively supported. Their use could force users to extend the library by deriving the base Event class.

## How to get started?

* Add the library files to your project.
* `\#Include` the library and use the library's namespace:

		#include "Mid/Lib.hpp"
		using namespace MID;

* Inside a function, create an empty MIDI sample:

		Sample sample;
	 
* Add notes defined by position, duration, and pitch:

		sample << Note(0, 1, 60);

* Save the resulting MIDI file:

		sample.save("sample.mid");
		
Now, read and run the files inside the folder `Mid/tutorials` to learn about all the other features.

## Who wrote the MidLib library and how?

This library was written by composer [Antoine Gabriel Brun](http://antoinegabrielbrun.com/) for his personal needs.

The references about the MIDI files specifications used when writing the MidLib library are:

* <http://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html>
* <http://www.somascape.org/midi/tech/mfile.html>

_____________


Good luck with MidLib, put it to good use!