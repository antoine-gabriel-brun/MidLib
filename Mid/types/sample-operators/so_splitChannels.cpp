#include "SampleOperators.hpp"

namespace MID
{
	namespace SampleOperators
	{
		TwoSamples splitChannels(Sample const& sample, UINT_8 channelWhereSecondBegins, WhereToKeepTheRest whereToKeepNoChannelEvents)
		{
			// Check for split index
			if (channelWhereSecondBegins > 16) {
				throw Exceptions::WrongSampleOperatorArgument("splitChannels", "channelWhereSecondBegins", "the split channel should be an integer between 0 and 16.");
			}

			// Decide where to keep no-channel events
			bool keepInFirst = (whereToKeepNoChannelEvents == WhereToKeepTheRest::first) || (whereToKeepNoChannelEvents == WhereToKeepTheRest::both);
			bool keepInSecond = (whereToKeepNoChannelEvents == WhereToKeepTheRest::second) || (whereToKeepNoChannelEvents == WhereToKeepTheRest::both);

			// Split and return
			return {
				trimChannels(sample, 0, channelWhereSecondBegins - 1, keepInFirst),
				trimChannels(sample, channelWhereSecondBegins, 15, keepInSecond)
			};
		}
	}
}
