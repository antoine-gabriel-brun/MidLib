#include "so_DifferentOverlappingNotes.hpp"

#include "../tools/Exceptions.hpp"

namespace MID
{
	namespace SampleOperators
	{
		OverlappingNotes mergeNotesOverlappingModes(OverlappingNotes overlappingNotes1, OverlappingNotes overlappingNotes2, DifferentOverlappingNotes differentOverlappingNotes, std::string const& callingOperatorName)
		{
			if (overlappingNotes1 == overlappingNotes2) {
				return overlappingNotes1;
			} else {
				if (differentOverlappingNotes == DifferentOverlappingNotes::keepFirst) {
					return overlappingNotes1;
				} else if (differentOverlappingNotes == DifferentOverlappingNotes::keepSecond) {
					return overlappingNotes2;
				} else if (differentOverlappingNotes == DifferentOverlappingNotes::setOverlappingNotesDefault) {
					return OverlappingNotes::trim;
				} else if (differentOverlappingNotes == DifferentOverlappingNotes::error) {
					throw Exceptions::WrongSampleOperatorArgument(callingOperatorName, "sample2", "the note overlapping modes of the two samples are different.");
				} else {
					throw 0; // won't happen
				}
			}
		}
	};
}
