#ifndef MID_SO_DIFFERENT_OVERLAPPING_NOTES_HPP
#define MID_SO_DIFFERENT_OVERLAPPING_NOTES_HPP

#include "../sample-options/OverlappingNotes.hpp"

#include <string>

namespace MID
{
	namespace SampleOperators
	{
		// Specifies what happens when an operation is performed between samples that don't have the same notes overlapping mode.
		enum class DifferentOverlappingNotes {
			// The notes overlapping mode of the first sample is retained.
			keepFirst,

			// The notes overlapping mode of the second sample is retained.
			keepSecond,

			// The default notes overlapping mode trim is used.
			setOverlappingNotesDefault,

			// An exception is thrown if the two samples don't have the same notes overlapping mode.
			error,

			// Same as 'keepFirst'.
			defaultBehavior = keepFirst
		};

		// Merge the notes overlapping mode of two samples according to the option set by DifferentOverlappingNotes.
		OverlappingNotes mergeNotesOverlappingModes(OverlappingNotes overlappingNotes1, OverlappingNotes overlappingNotes2, DifferentOverlappingNotes differentOverlappingNotes, std::string const& callingOperatorName);
	};
}

#endif
