#ifndef MID_EXCEPTIONS_HPP
#define MID_EXCEPTIONS_HPP

#include "Exception.hpp"

#include <string>

namespace MID
{
	// These are all the exception that can be thrown by the library.
	namespace Exceptions
	{
		class WrongTicksPerQuarterNote : public Exception { public: WrongTicksPerQuarterNote
		(bool tooLarge) : Exception(
			"The number of ticks per quarter note " + (tooLarge ? std::string("is too large because it can only be stored in the 15 least significant bits (the 16th is 0).") : std::string("cannot be zero."))) { } };
		
		class WrongTrackInsertionIndex : public Exception { public: WrongTrackInsertionIndex
		() : Exception(
			"Wrong track insertion index.") { } };
		
		class WrongTrackDeletionIndex : public Exception { public: WrongTrackDeletionIndex
		() : Exception(
			"Wrong track deletion index.") { } };
		
		class WrongTrackDeletionIndices : public Exception { public: WrongTrackDeletionIndices
		() : Exception(
			"Wrong track deletion indices.") { } };
		
		class LimitedUintOutOfRange : public Exception { public: LimitedUintOutOfRange
		(unsigned long long val, int nbBits) : Exception(
			"The value " + std::to_string(val) + " can't be stored in " + std::to_string(nbBits) + " bits.") { } };
		
		class WrongMidiEventTypeValue : public Exception { public: WrongMidiEventTypeValue
		() : Exception(
			"The type of a MIDI event should be represented by an integer between 8 and 14.") { } };
		
		class OverlappingNotesTreatedAsError : public Exception { public: OverlappingNotesTreatedAsError
		() : Exception(
			"Overlapping notes with the OverlappingNotes::error sample option.") { } };
		
		class InstrumentIntendedForSpecialChannel9 : public Exception { public: InstrumentIntendedForSpecialChannel9
		() : Exception(
			"This instrument is intended for special channel 9 only.") { } };
		
		class InstrumentNotIntendedForSpecialChannel9 : public Exception { public: InstrumentNotIntendedForSpecialChannel9
		() : Exception(
			"This instrument is not intended for special channel 9.") { } };
		
		class ConversionOutOfBounds : public Exception { public: ConversionOutOfBounds
		(std::string const& name, std::string const& min, std::string const& max) : Exception(
			"The " + name + " value should be between " + min + " and " + max + ".") { } };
		
		class WrongSpecialChannel9Specifier : public Exception { public: WrongSpecialChannel9Specifier
		() : Exception(
			"Invalid special channel specifier. Valid values are 's' for special drum channel, 'n' for normal channel, and 'u' for unspecified channel type.") { } };
		
		class MissingMidFileExtension : public Exception { public: MissingMidFileExtension
		() : Exception(
			"Could not create a MIDI file without a .mid extension.") { } };
		
		class CouldNotCreateMidiFile : public Exception { public: CouldNotCreateMidiFile
		(std::string const& fileName) : Exception(
			"Could not create the MIDI file named '" + fileName + "'.") { } };
		
		class NoteStartAfterNoteEnd : public Exception { public: NoteStartAfterNoteEnd
		() : Exception(
			"The note should note have its start after its end.") { } };
		
		class SampleDursComparisonWithAlmostOneUnspecified : public Exception { public: SampleDursComparisonWithAlmostOneUnspecified
		() : Exception(
			"To compare sample durations, both should be specified.") { } };
		
		class AccessingDurOfUnspecifiedSampleDur : public Exception { public: AccessingDurOfUnspecifiedSampleDur
		() : Exception(
			"This SampleDur has an unspecified duration.") { } };
		
		class NullNumeratorInTimeSignature : public Exception { public: NullNumeratorInTimeSignature
		() : Exception(
			"The numerator of a time signature should not be zero.") { } };
		
		class WrongDenominatorInTimeSignature : public Exception { public: WrongDenominatorInTimeSignature
		() : Exception(
			"The denominator of a time signature should be a power of 2 between 1 and 128.") { } };
		
		class WrongTimeValue : public Exception { public: WrongTimeValue
		() : Exception(
			"The time value should be positive or zero.") { } };
		
		class WrongDurMultiplier : public Exception { public: WrongDurMultiplier
		() : Exception(
			"The time multiplier should not be negative.") { } };
		
		class TimeValueOperatorUnderflow : public Exception { public: TimeValueOperatorUnderflow
		() : Exception(
			"The arithmetic operation between two time values caused an underflow (negative time value).") { } };
		
		class WrongMidiTicksPerMetroTick : public Exception { public: WrongMidiTicksPerMetroTick
		() : Exception(
			"There should be at least one MIDI tick per metro tick.") { } };
		
		class SampleOperatorFailed : public Exception { public: SampleOperatorFailed
		(std::string const& operatorName, std::string const& msg) : Exception(
			"Sample operator " + operatorName + " failed: " + msg) { } };
		
		class WrongErrorBehavior : public Exception { public: WrongErrorBehavior
		() : Exception(
			"You should specify 'error' or 'ignore' as the error behavior.") { } };
		
		class WrongSampleOperatorArgument : public Exception { public: WrongSampleOperatorArgument
		(std::string const& operatorName, std::string const& argumentName, std::string const& msg) : Exception(
			"Wrong argument " + argumentName +  " in sample operator " + operatorName + ": " + msg) { } };
	}
}

#endif
