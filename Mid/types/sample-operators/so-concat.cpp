#include "SampleOperators.hpp"

namespace MID
{
	namespace SampleOperators
	{
		Sample concat(Sample const& sample1, Sample const& sample2,
			DifferentTicksPerQuarter differentTicksPerQuarter,
			DifferentTempos differentTempos,
			DifferentTimeSignatures differentTimeSignatures,
			DifferentOverlappingNotes differentOverlappingNotes)
		{
			std::string const operatorName = "concat";
			
			// Number of tracks (= number of tracks of the sample with the largest number of tracks)
			size_t nbTracks1 = sample1.nbTracks();
			size_t nbTracks2 = sample2.nbTracks();
			size_t nbTracks = (nbTracks1 >= nbTracks2) ? nbTracks1 : nbTracks2;
			size_t nbCommonTracks = (nbTracks1 <= nbTracks2) ? nbTracks1 : nbTracks2;

			// Ticks per quarter
			UINT_16 ticksPerQuarter = mergeTicksPerQuarter(sample1.ticksPerQuarter(), sample2.ticksPerQuarter(), differentTicksPerQuarter, operatorName);

			// Tempo and time stretch
			bool useStretched1; double stretchRatio1; Sample sample1stretched;
			bool useStretched2;	double stretchRatio2; Sample sample2stretched;
			Tempo tempo = mergeTemposAndStretchTime(sample1, sample2, differentTempos, operatorName, useStretched1, useStretched2, stretchRatio1, stretchRatio2, sample1stretched, sample2stretched);
			Sample sample1select = useStretched1 ? sample1stretched : sample1;
			Sample sample2select = useStretched2 ? sample2stretched : sample2;

			// Time signature
			TimeSignature timeSignature = mergeTimeSignatures(sample1.timeSignature(), sample2.timeSignature(), differentTimeSignatures, operatorName);

			// Sample duration
			double timeOffset;
			if (sample1select.dur().isSpecified()) {
				timeOffset = sample1select.dur().time();
			} else {
				timeOffset = sample1select.calcEndPos().time();
			}
			SampleDur sampleDur =
				(sample2select.dur().isSpecified()) ?
				SampleDur::specified(timeOffset + sample2select.dur().time()) :
				sampleDur = SampleDur::unspecified();

			// Overlapping notes
			OverlappingNotes overlappingNotes = mergeNotesOverlappingModes(sample1.overlappingNotes(), sample2.overlappingNotes(), differentOverlappingNotes, operatorName);

			// Create the sample
			Sample ans = Sample(nbTracks)
				.ticksPerQuarter(ticksPerQuarter)
				.tempo(tempo)
				.timeSignature(timeSignature)
				.dur(sampleDur)
				.overlappingNotes(overlappingNotes);

			// Add the common tracks
			for (unsigned int i = 0; i < nbCommonTracks; i++) {
				ans[i] = Track::concat(sample1select[i], sample2select[i], timeOffset);
			}
			
			// Add the remaining tracks (only one of these two loops will be executed)
			for (size_t i = nbCommonTracks; i < nbTracks1; i++) {
				ans[(unsigned int)i] = sample1select[(unsigned int)i];
			}
			for (size_t i = nbCommonTracks; i < nbTracks2; i++) {
				ans[(unsigned int)i] = Track::concat(Track(), sample2select[(unsigned int)i], timeOffset);
			}

			// Return
			return ans;
		}
	}
}
