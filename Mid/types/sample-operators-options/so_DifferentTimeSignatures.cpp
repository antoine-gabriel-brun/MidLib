#include "so_DifferentTimeSignatures.hpp"

#include "../tools/Exceptions.hpp"

namespace MID
{
	namespace SampleOperators
	{
		TimeSignature mergeTimeSignatures(TimeSignature const& timeSignature1, TimeSignature const& timeSignature2, DifferentTimeSignatures differentTimeSignatures, std::string const& callingOperatorName)
		{
			if (timeSignature1 == timeSignature2) {
				return timeSignature1;
			} else {
				if (differentTimeSignatures == DifferentTimeSignatures::keepFirst) {
					return timeSignature1;
				} else if (differentTimeSignatures == DifferentTimeSignatures::keepSecond) {
					return timeSignature2;
				} else if (differentTimeSignatures == DifferentTimeSignatures::setTimeSignatureDefault) {
					return TimeSignature::createDefault();
				} else if (differentTimeSignatures == DifferentTimeSignatures::error) {
					throw Exceptions::WrongSampleOperatorArgument(callingOperatorName, "sample2", "the time signatures of the two samples are different.");
				} else {
					throw 0; // won't happen
				}
			}
		}
	};
}
