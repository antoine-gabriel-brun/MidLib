#ifndef MID_INTEGERS_HPP
#define MID_INTEGERS_HPP

#include "Exceptions.hpp"

#include <cstdint>
#include <sstream>

namespace MID
{
	// Generic restricted width unsigned integer
	template <typename T, int bits> class UINT_LIMITED
	{
	public:
		// Constructors
		UINT_LIMITED() :
			value_(0u)
		{ }
		UINT_LIMITED(T value) :
			value_(value)
		{
			if (value_ >= exclusiveMax) {
				throw Exceptions::LimitedUintOutOfRange(value_, bits);
			}
		}
		
		// Conversion operator
		operator T() const
		{
			return value_;
		}

		// Bounds
		static constexpr T min = 0u;
		static constexpr T max = ((T)1u << bits) - (T)1u;

	private:
		// Maximum valid value (for checks)
		static constexpr T exclusiveMax = ((T)1u << bits);

		// Field
		T value_;
	};

	// Fixed-width unsigned integers for MIDI files: native widths
	/*  8-bits integer */ typedef uint8_t UINT_8;
	/* 16-bits integer */ typedef uint16_t UINT_16;
	/* 32-bits integer */ typedef uint32_t UINT_32;
	/* 64-bits integer */ typedef uint64_t UINT_64;
	
	// Fixed-width unsigned integers for MIDI files: restricted widths
	/*  4-bits integer */ typedef UINT_LIMITED<UINT_8, 4> UINT_4;    // for channel and MIDI event type (0-15)
	/*  7-bits integer */ typedef UINT_LIMITED<UINT_8, 7> UINT_7;    // for most MIDI values (0-127)
	/* 14-bits integer */ typedef UINT_LIMITED<UINT_16, 14> UINT_14; // for pitch bends
	/* 24-bits integer */ typedef UINT_LIMITED<UINT_32, 24> UINT_24; // for 3-bytes integers
	/* 28-bits integer */ typedef UINT_LIMITED<UINT_32, 28> UINT_28; // for variable-length integers
}

#endif
