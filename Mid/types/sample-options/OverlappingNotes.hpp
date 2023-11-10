#ifndef MID_OVERLAPPING_NOTES_HPP
#define MID_OVERLAPPING_NOTES_HPP

namespace MID
{
	// Enumeration of possible behaviors when trying to save a sample
	// that contains overlapping notes. Two notes are said to overlap
	// if the second note begins before the first ends, if they are in
	// the same track, if they are on the same channel, AND if they are
	// of the same pitch.
	enum class OverlappingNotes
	{
		// Correct overlapping notes by silently trimming them.
		// If two notes overlap, the first will be trimmed just before the second begins.
		// The resulting file will contain only well-formed (though sometimes null-length) notes, in the same number as the notes added to the sample.
		trim,

		// Correct overlapping notes by silently merging them.
		// A merged note begins where the first note of the overlapping group of notes begins, and ends where the last of them ends.
		// The resulting file will contain only well-formed notes.
		merge,

		// Don't try to correct overlapping notes at all.
		// Save the file with all note-on and note-off events where the overlapping notes begin and end.
		// The resulting file, though valid, will trigger successive note-on and note-off events on the same pitch, resulting in undefined behavior.
		ignore,

		// Don't try to correct overlapping notes at all.
		// Treat overlapping notes as an error and throw an Exceptions::OverlappingNotesTreatedAsError exception if any are found.
		// In such a case, no file will be generated.
		error
	};
}

#endif
