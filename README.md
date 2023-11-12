# MidLib: C++ library for easy MIDI sample generation

The MidLib library allows users to **generate MIDI files** using C++ programs. It supports MIDI notes, multi-track samples, channels, pitch bends, control changes, instruments, aftertouch, tempo, time signature, custom events, as well as many tools to **create and transform musical samples** as complex as you like.

![MidLib viewed within a DAW](http://antoinegabrielbrun.com/wp-content/uploads/2023/11/MidLib-rational.png)

## How does MidLib work?

MidLib is a **C++ library**. Once included in a C++ project, it allows you to create musical objects and export them to **MIDI files**.

The MidLib library is designed for both **simplicity** and **flexibility**:

* The basic use (create a sample - add notes - save as `.mid` file) is **straightforward**. The underlying complexity of the MIDI file specifications (chunks, events, byte-wise file writing...) is hidden.
* However, advanced users can use MidLib to create complex MIDI files that take advantage of **most of the features** offered by the MIDI standard.

## Is MidLib easy to use?

The main purpose of the MidLib library is to make creating MIDI files **as easy as possible**. To this end, all the things that make the MIDI specification tricky have been wrapped into a simple syntax that hides all the underlying misery:

* Notes are added **in one step**: MidLib handles NoteOn and NoteOff events and transparently controls what happens if some notes overlap.
* You don't need to add notes and events in chronological order: MidLib **sorts events for you**.
* Most of the magic numbers are hidden and replaced with **user-friendly names**. You don't need to know the controller index for a particular control change, or the program change index for a particular instrument: you will refer to them in your code with words like `Accordion`, `Pan`, or `ChannelVolume`.
* The tricky stuff like chunks, variable-length sizes, running status, 14-bit and 28-bit integers, etc. are **completely hidden** in the library's subsoil and handled without any action on your part.

## Which standard MIDI features are not supported by the MidLib library?

Despite the attempt to make MidLib as complete as possible, some features of the MIDI format are not supported by the library:

* MidLib can only **write** MIDI files. It is not designed to *open* existing MIDI files, *play* MIDI samples as audio, or handle real-time MIDI streams.
* The only available output MIDI file format is **format 1** (multitrack MIDI file).
* The only time mode is **beat-based**. SMPTE time is not supported.
* Some MIDI events of lesser interest such as cue points, lyrics, etc. are not natively supported. Using them requires **extending** the library by deriving the Event base class.

## How do I get started?

* Download the library files and add them to your project.
* Include the library and use the library's namespace:

		#include "Mid/Lib.hpp"
		using namespace MID;

* Within a function, create a new empty MIDI sample; add notes defined by position, duration, and pitch; and finally save the resulting MIDI file:

		int main() {
		    Sample sample;
		    for (int pos = 0; pos < 16; pos++) {
		        sample << Note(pos, 1, 60);
		    }
		    sample.save("sample.mid");
		}
		
This is how you generate a MIDI file using MidLib.

Now, run and read the files in the `Mid/tutorials` folder to discover the other features available.

## Who wrote MidLib and why?

This library was written by French composer [Antoine Gabriel Brun](http://antoinegabrielbrun.com/) for his personal use.

It comes after [ScoreGen](https://github.com/antoine-gabriel-brun/ScoreGen/), another library he wrote earlier, which allows to generate **musical scores** with C++ programs.

The two libraries have advantages that complement each other:

* Unlike ScoreGen, MidLib allows the generation of musical content that is not rhythm-dependent: durations are not additions of atomic rhythmic figures, but are **completely free**. In addition, it supports some features like pitch bends, aftertouch, panning... that don't exist in the score world.

* However, using MidLib instead of ScoreGen also brings some limitations, since the MIDI format doesn't provide any support for some of the elements found on scores, such as slurs, articulations, textual dynamics, etc. ScoreGen supports **all of these notations**.

In both cases, the goal is to provide composers with a programming interface to realize their ideas for [algorithmic composition](https://en.wikipedia.org/wiki/Algorithmic_composition).

In both cases, the goal is to provide composers with a programming interface to realize their ideas for <a href="https://en.wikipedia.org/wiki/Algorithmic_composition" target="_blank">algorithmic composition</a>.

## References to MIDI file specifications

The references used when writing the MidLib library are:

* <http://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html>
* <http://www.somascape.org/midi/tech/mfile.html>

**Good luck with MidLib, put it to good use!**

![MidLib, artist view](http://antoinegabrielbrun.com/wp-content/uploads/2023/11/MidLib-glowing.jpg)
