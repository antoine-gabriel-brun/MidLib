#include "Convert.hpp"

#include "Exceptions.hpp"

namespace MID
{
	namespace Convert
	{
		namespace Param7bits
		{
			float midi_to_float(UINT_7 value_midi)
			{
				return (float)((UINT_8)value_midi) * 0.0078740157480315f;
			}
			UINT_8 float_to_midi(float value_float)
			{
				if (value_float >= 0.f && value_float <= 1.f) {
					return (UINT_8)floor(value_float * 127.998f);
				} else {
					throw Exceptions::ConversionOutOfBounds("parameter", "0.0f", "1.0f");
				}
			}
		}
		
		namespace Param14bits
		{
			float midi_to_float(UINT_14 value_midi)
			{
				return (float)((UINT_16)value_midi) * 0.000061038881768f;
			}
			UINT_16 float_to_midi(float value_float)
			{
				if (value_float >= 0.f && value_float <= 1.f) {
					return (UINT_16)floor(value_float * 16383.998f);
				} else {
					throw Exceptions::ConversionOutOfBounds("parameter", "0.0f", "1.0f");
				}
			}
		}
		
		namespace Velocity
		{
			float midi_to_float(UINT_7 velocity_midi)
			{
				if (velocity_midi >= 1) {
					return (float)((UINT_8)velocity_midi - (UINT_8)1u) * 7.9365079365e-3f;
				} else {
					throw Exceptions::ConversionOutOfBounds("velocity", "1", "127");
				}
			}
			UINT_7 float_to_midi(float velocity_float)
			{
				if (velocity_float >= 0.f && velocity_float <= 1.f) {
					return (UINT_7)((UINT_8)1u + (UINT_8)floor(velocity_float * 126.998f));
				} else {
					throw Exceptions::ConversionOutOfBounds("velocity", "0.0f", "1.0f");
				}
			}
		}
		
		namespace Aftertouch
		{
			float midi_to_float(UINT_7 aftertouch_midi)
			{
				return Param7bits::midi_to_float(aftertouch_midi);
			}
			UINT_7 float_to_midi(float aftertouch_float)
			{
				return Param7bits::float_to_midi(aftertouch_float);
			}
		}
		
		namespace PitchBend
		{
			float midi_to_floatSemitones(UINT_14 pitchBend_midi)
			{
				return (float)(((UINT_16)pitchBend_midi) << 2) * 0.00006103515625f - 2.f;
			}
			UINT_16 floatSemitones_to_midi(float pitchBend_float)
			{
				UINT_16 ans;
				if (pitchBend_float >= -2.f && pitchBend_float < 2.f && (ans = (UINT_16)floor((0.25f * (pitchBend_float + 2.f)) * 16384u)) < 16384u) {
					return ans;
				} else {
					throw Exceptions::ConversionOutOfBounds("pitch bend", "-2.0f (included)", "2.0f (excluded)");
				}
			}
		}
		
		namespace Tempo
		{
			double microsecondsPerQuarter_to_bpm(UINT_24 tempo_microsecondsPerQuarter)
			{
				if (tempo_microsecondsPerQuarter >= minMicrosecondsPerQuarter && tempo_microsecondsPerQuarter <= maxMicrosecondsPerQuarter) {
					return microsecondsInOneMinute / tempo_microsecondsPerQuarter;
				} else {
					throw Exceptions::ConversionOutOfBounds("tempo", std::to_string(minMicrosecondsPerQuarter) + " microseconds per quarter note", std::to_string(maxMicrosecondsPerQuarter) + " microseconds per quarter note");
				}
			}
			UINT_32 bpm_to_microsecondsPerQuarter(double tempo_bpm)
			{
				if (tempo_bpm >= minBpm && tempo_bpm <= maxBpm) {
					return (UINT_32)round(microsecondsInOneMinute / tempo_bpm);
				} else {
					throw Exceptions::ConversionOutOfBounds("tempo", std::to_string(minBpm) + " bpm", std::to_string(maxBpm) + " bpm");
				}
			}
		}
	}
}
