#include "SampleOperators.hpp"

namespace MID
{
	namespace SampleOperators
	{
		Sample trimTracks(Sample const& sample, size_t rangeBegin, size_t rangeEndExclusive)
		{
			// Check for trim indices
			if (rangeBegin > sample.nbTracks()) {
				throw Exceptions::WrongSampleOperatorArgument("trimTracks", "rangeBegin", "the index should be an integer between 0 and the number of tracks (included).");
			} else if (rangeEndExclusive > sample.nbTracks()) {
				throw Exceptions::WrongSampleOperatorArgument("trimTracks", "rangeEndExclusive", "the index should be an integer between 0 and the number of tracks (included).");
			} else if (rangeEndExclusive <= rangeBegin) {
				return sample.createWithSameOptions(0);
			}
			
			// Get the resulting number of tracks
			size_t nbTracks = rangeEndExclusive - rangeBegin;

			// Create an empty sample with the same options
			Sample ans = sample.createWithSameOptions(nbTracks);

			// Copy the tracks
			for (unsigned int i = 0; i < nbTracks; i++) {
				ans[i] = sample[(unsigned int)rangeBegin + i];
			}

			// Return
			return ans;
		}
	}
}
