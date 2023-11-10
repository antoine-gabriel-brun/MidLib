#ifndef MID_SO_TWO_SAMPLES_HPP
#define MID_SO_TWO_SAMPLES_HPP

#include <utility>

namespace MID
{
	// Forward declaration
	class Sample;

	namespace SampleOperators
	{
		// Represents a pair of samples created by splitting a sample into two parts.
		typedef std::pair<Sample, Sample> TwoSamples;
	}
}

#endif
