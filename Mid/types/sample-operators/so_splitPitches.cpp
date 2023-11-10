#include "SampleOperators.hpp"

namespace MID
{
	namespace SampleOperators
	{
		TwoSamples splitPitches(Sample const& sample, UINT_8 pitchWhereSecondBegins, WhereToKeepTheRest whereToKeepNoPitchEvents)
		{
			// Check for split index
			if (pitchWhereSecondBegins > 128) {
				throw Exceptions::WrongSampleOperatorArgument("splitPitches", "pitchWhereSecondBegins", "the split pitch should be an integer between 0 and 128.");
			}

			// Decide where to keep no-pitch events
			bool keepInFirst = (whereToKeepNoPitchEvents == WhereToKeepTheRest::first) || (whereToKeepNoPitchEvents == WhereToKeepTheRest::both);
			bool keepInSecond = (whereToKeepNoPitchEvents == WhereToKeepTheRest::second) || (whereToKeepNoPitchEvents == WhereToKeepTheRest::both);

			// Split and return
			return {
				trimPitches(sample, 0, pitchWhereSecondBegins, keepInFirst),
				trimPitches(sample, pitchWhereSecondBegins + 1, 127, keepInSecond)
			};
		}
	}
}
