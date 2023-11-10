# MidLib

*C++ library for easy MIDI sample manipulation.*

MidLib allows users to handle MIDI samples inside C++ programs, and to save them as .mid files. The library supports notes, multi-tracks samples, channels, pitch bends, control and program changes, aftertouch, tempo, time signature, custom events, as well as many tool for manipulating and transforming musical samples.

## What is MidLib?

MidLib is a C++ library. Once included into a C++ project, users can create objects that store musical informations that can be exported to MIDI samples.

MidLib is both designed for simplicity and flexibility. For basic use (creating a sample - adding notes - saving as .mid file), it is very simple and straightforward. The underlying complexity - chunks, events, bytewise file writing - and the MIDI files specifications remains always hidden. However, advanced users can use it to create complex MIDI files that include most of the features offered by the MIDI standard.

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