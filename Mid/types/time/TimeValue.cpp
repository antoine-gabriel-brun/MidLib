#include "TimeValue.hpp"

#include "../tools/Exceptions.hpp"

namespace MID
{
	double TimeValue::time() const
	{
		return time_;
	}
	bool TimeValue::isZero() const
	{
		return time_ == 0.;
	}

	TimeValue::TimeValue(double time) :
		time_(time)
	{
		if (!(time >= 0.)) { // also checks for NaN.
			throw Exceptions::WrongTimeValue();
		}
	}
}
