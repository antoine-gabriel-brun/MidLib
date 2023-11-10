#include "TimeSignature.hpp"

#include "../tools/Exceptions.hpp"

namespace MID
{
	TimeSignature::TimeSignature(UINT_8 num, UINT_8 denom_4isQuarter_8isEighth_etc) :
		num_(num),
		denom_4isQuarter_8isEighth_etc_(denom_4isQuarter_8isEighth_etc)
	{
		if (num_ == 0) {
			throw Exceptions::NullNumeratorInTimeSignature();
		} else if (
			denom_4isQuarter_8isEighth_etc_ != 1 &&
			denom_4isQuarter_8isEighth_etc_ != 2 &&
			denom_4isQuarter_8isEighth_etc_ != 4 &&
			denom_4isQuarter_8isEighth_etc_ != 8 &&
			denom_4isQuarter_8isEighth_etc_ != 16 &&
			denom_4isQuarter_8isEighth_etc_ != 32 &&
			denom_4isQuarter_8isEighth_etc_ != 64 &&
			denom_4isQuarter_8isEighth_etc_ != 128) {
			throw Exceptions::WrongDenominatorInTimeSignature();
		}
	}
	TimeSignature TimeSignature::createDefault()
	{
		return TimeSignature(4u, 4u);
	}

	UINT_8 TimeSignature::num() const
	{
		return num_;
	}
	UINT_8 TimeSignature::denom_4isQuarter_8isEighth_etc() const
	{
		return denom_4isQuarter_8isEighth_etc_;
	}

	bool TimeSignature::operator==(TimeSignature const& other) const
	{
		return
			(num_                            == other.num_                           ) &&
			(denom_4isQuarter_8isEighth_etc_ == other.denom_4isQuarter_8isEighth_etc_);
	}
	bool TimeSignature::operator!=(TimeSignature const& other) const
	{
		return
			(num_                            != other.num_                           ) ||
			(denom_4isQuarter_8isEighth_etc_ != other.denom_4isQuarter_8isEighth_etc_);
	}
}
