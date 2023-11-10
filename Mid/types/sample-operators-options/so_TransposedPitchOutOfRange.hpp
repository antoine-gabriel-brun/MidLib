#ifndef MID_SO_TRANSPOSED_PITCH_OUT_OF_RANGE_HPP
#define MID_SO_TRANSPOSED_PITCH_OUT_OF_RANGE_HPP

namespace MID
{
	namespace SampleOperators
	{
		// Enumeration of possible behaviors when transposing a sample results in out-of-range pitches.
		enum class TransposedPitchOutOfRange
		{
			// Discard the invalid notes in the resulting sample; don't throw an exception.
			ignore,

			// Throw an exception.
			error,

			// Same as 'ignore'.
			defaultBehavior = ignore
		};
	}
}

#endif
