#ifndef MID_SO_DIFFERENT_TEMPOS_HPP
#define MID_SO_DIFFERENT_TEMPOS_HPP

#include <string>

namespace MID
{
	// Forward declaration
	class Sample;
	class Tempo;

	namespace SampleOperators
	{
		// Specifies what happens when an operation is performed between samples that don't have the same tempo.
		enum class DifferentTempos {
			// The tempo of the second sample is ignored.
			// The beats are aligned together using the tempo of the first sample.
			// The time relationship between the two samples is not preserved.
			ignoreKeepFirst,

			// The tempo of the first sample is ignored.
			// The beats are aligned together using the tempo of the second sample.
			// The time relationship between the two samples is not preserved.
			ignoreKeepSecond,

			// The first sample remains unchanged.
			// The second sample is stretched to match the tempo of the first sample.
			// The time relationship between the two samples is preserved.
			keepFirstStretchSecond,

			// The second sample remains unchanged.
			// The first sample is stretched to match the tempo of the second sample.
			// The time relationship between the two samples is preserved.
			stretchFirstKeepSecond,

			// An exception is thrown if the two samples don't have the same tempo.
			error,

			// Same as 'keepFirstStretchSecond'.
			defaultBehavior = keepFirstStretchSecond
		};

		// Merge the tempos of two samples according to the option set by DifferentTempos. If necessary, time-stretch a sample.
		Tempo mergeTemposAndStretchTime(Sample const& sample1, Sample const& sample2, DifferentTempos differentTempos, std::string const& callingOperatorName, bool& useStretched1_out, bool& useStretched2_out, double& stretchRatio1_out, double& stretchRatio2_out, Sample& sample1stretched_out, Sample& sample2stretched_out);
	};
}

#endif
