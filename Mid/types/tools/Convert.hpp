#ifndef MID_CONVERT_HPP
#define MID_CONVERT_HPP

#include "Integers.hpp"

namespace MID
{
	// Provides conversions between native integral MIDI values and user-friendly floating point values.
	namespace Convert
	{
		// [0, 127] <--> [0, 1]
		namespace Param7bits
		{
			// [0, 127] --> [0, 1]
			float midi_to_float(UINT_7 velocity_midi);
			// [0, 1] --> [0, 127]
			UINT_8 float_to_midi(float velocity_float);
		}

		// [0, 16383] <--> [0, 1]
		namespace Param14bits
		{
			// [0, 16383] --> [0, 1]
			float midi_to_float(UINT_14 value_midi);
			// [0, 1] --> [0, 16383]
			UINT_16 float_to_midi(float value_float);
		}
		
		// [1, 127] <--> [0, 1]
		namespace Velocity
		{
			// [1, 127] --> [0, 1]
			float midi_to_float(UINT_7 velocity_midi);
			// [0, 1] --> [1, 127]
			UINT_7 float_to_midi(float velocity_float);
		}
		
		// [0, 127] <--> [0, 1]
		namespace Aftertouch
		{
			// [0, 127] --> [0, 1]
			float midi_to_float(UINT_7 aftertouch_midi);
			// [0, 1] --> [0, 127]
			UINT_7 float_to_midi(float aftertouch_float);
		}
		
		// [0, 16384[ <--> [-2, 2[
		namespace PitchBend
		{
			// [0, 16384[ --> [-2, 2[
			float midi_to_floatSemitones(UINT_14 pitchBend_midi);
			// [-2, 2[ --> [0, 16384
			UINT_16 floatSemitones_to_midi(float pitchBend_float);
		}
		
		// Microseconds per quarter note <--> BPM
		namespace Tempo
		{
			// Constants
			static constexpr double microsecondsInOneMinute = 60'000'000.0;
			static constexpr UINT_32 minMicrosecondsPerQuarter = 3000;
			static constexpr UINT_32 maxMicrosecondsPerQuarter = 12'000'000;
			static constexpr double minBpm = 5.0;
			static constexpr double maxBpm = 20000.0;
			
			// Microseconds per quarter note --> BPM
			double microsecondsPerQuarter_to_bpm(UINT_24 tempo_microsecondsPerQuarter);
			// BPM --> microseconds per quarter note
			UINT_32 bpm_to_microsecondsPerQuarter(double tempo_bpm);
		}
	}
}

#endif
