#include "SampleOperators.hpp"

namespace MID
{
	namespace SampleOperators
	{
		TwoSamples splitTracks(Sample const& sample, int indexTrackWhereSecondBegins)
		{
			// Check for split index
			int nbTracksFirst = indexTrackWhereSecondBegins;
			int nbTracksSecond = (int)sample.nbTracks() - indexTrackWhereSecondBegins;
			if (nbTracksFirst < 0 || nbTracksSecond < 0) {
				throw Exceptions::WrongSampleOperatorArgument("splitTracks", "indexTrackWhereSecondBegins", "the index should be an integer between 0 and the number of tracks (included).");
			}

			// Split and return
			return {
				trimTracks(sample, 0, indexTrackWhereSecondBegins),
				trimTracks(sample, indexTrackWhereSecondBegins, sample.nbTracks())
			};
		}
	}
}
