#include "SampleOperators.hpp"

namespace MID
{
	namespace SampleOperators
	{
		TwoSamples split(Sample const& sample, double posWhereSecondBegins, PreserveCurves preserveCurves)
		{
			// Check for split index
			if (posWhereSecondBegins < 0.) {
				throw Exceptions::WrongSampleOperatorArgument("splitTime", "posWhereSecondBegins", "the split position should not be negative.");
			} else if (sample.dur().isSpecified() && posWhereSecondBegins > sample.dur().time()) {
				throw Exceptions::WrongSampleOperatorArgument("splitTime", "posWhereSecondBegins", "if the sample duration is specified, the split position should not exceed it.");
			}

			// Split and return
			return {
				trim(sample, 0, posWhereSecondBegins, preserveCurves),
				trim(sample, posWhereSecondBegins, preserveCurves)
			};
		}
	}
}
