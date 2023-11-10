#include "so_DifferentTempos.hpp"

#include "../sample-operators/SampleOperators.hpp"
#include "../tools/Exceptions.hpp"
#include "../containers/Sample.hpp"

namespace MID
{
	namespace SampleOperators
	{
		Tempo mergeTemposAndStretchTime(Sample const& sample1, Sample const& sample2, DifferentTempos differentTempos, std::string const& callingOperatorName, bool& useStretched1_out, bool& useStretched2_out, double& stretchRatio1_out, double& stretchRatio2_out, Sample& sample1stretched_out, Sample& sample2stretched_out)
		{
			useStretched1_out = false;
			useStretched2_out = false;
				
			if (sample1.tempo() == sample2.tempo()) {
				return sample1.tempo();
			} else {
				if (differentTempos == DifferentTempos::ignoreKeepFirst) {
					return sample1.tempo();
				} else if (differentTempos == DifferentTempos::ignoreKeepSecond) {
					return sample2.tempo();
				} else if (differentTempos == DifferentTempos::keepFirstStretchSecond) {
					stretchRatio2_out = sample1.tempo().bpm() / sample2.tempo().bpm();
					sample2stretched_out = stretch(sample2, stretchRatio2_out, Selector::all());
					useStretched2_out = true;
					return sample1.tempo();
				} else if (differentTempos == DifferentTempos::stretchFirstKeepSecond) {
					stretchRatio1_out = sample2.tempo().bpm() / sample1.tempo().bpm();
					sample1stretched_out = stretch(sample1, stretchRatio1_out, Selector::all());
					useStretched1_out = true;
					return sample2.tempo();
				} else if (differentTempos == DifferentTempos::error) {
					throw Exceptions::WrongSampleOperatorArgument(callingOperatorName, "sample2", "the tempos of the two samples are different.");
				} else {
					throw 0; // won't happen
				}
			}
		}
	};
}
