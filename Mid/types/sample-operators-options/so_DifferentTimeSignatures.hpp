#ifndef MID_SO_DIFFERENT_TIME_SIGNATURES_HPP
#define MID_SO_DIFFERENT_TIME_SIGNATURES_HPP

#include "../sample-options/TimeSignature.hpp"

#include <string>

namespace MID
{
	namespace SampleOperators
	{
		// Specifies what happens when an operation is performed between samples that don't have the same time signature.
		enum class DifferentTimeSignatures {
			// The time signature of the first sample is retained.
			keepFirst,

			// The time signature of the second sample is retained.
			keepSecond,

			// The default time signature 4/4 is used.
			setTimeSignatureDefault,

			// An exception is thrown if the two samples don't have the same time signature.
			error,

			// Same as 'keepFirst'.
			defaultBehavior = keepFirst
		};

		// Merge the time signatures of two samples according to the option set by DifferentTimeSignatures.
		TimeSignature mergeTimeSignatures(TimeSignature const& timeSignature1, TimeSignature const& timeSignature2, DifferentTimeSignatures differentTimeSignatures, std::string const& callingOperatorName);
	};
}

#endif
