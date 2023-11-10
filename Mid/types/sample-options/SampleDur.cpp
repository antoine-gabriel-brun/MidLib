#include "SampleDur.hpp"

#include "../tools/Exceptions.hpp"

namespace MID
{
	SampleDur SampleDur::unspecified()
	{
		return SampleDur();
	}
	SampleDur SampleDur::specified(Dur const& dur)
	{
		return SampleDur(dur);
	}
	SampleDur SampleDur::specified(double dur)
	{
		return SampleDur(Dur(dur));
	}

	bool SampleDur::isSpecified() const
	{
		return isSpecified_;
	}
	Dur const& SampleDur::dur() const
	{
		if (isSpecified_) {
			return dur_;
		} else {
			throw Exceptions::AccessingDurOfUnspecifiedSampleDur();
		}
	}
	double SampleDur::time() const
	{
		if (isSpecified_) {
			return dur_.time();
		} else {
			throw Exceptions::AccessingDurOfUnspecifiedSampleDur();
		}
	}
		
	bool SampleDur::operator==(SampleDur const& other) const
	{
		if (isSpecified_ ^ other.isSpecified_) {
			// One is specified, the other is not
			return false;
		} else if (isSpecified_) {
			// Both are specified
			return dur_ == other.dur_;
		} else {
			// None is specified
			return true;
		}
	}
	bool SampleDur::operator!=(SampleDur const& other) const
	{
		if (isSpecified_ ^ other.isSpecified_) {
			// One is specified, the other is not
			return true;
		} else if (isSpecified_) {
			// Both are specified
			return dur_ != other.dur_;
		} else {
			// None is specified
			return false;
		}
	}
	bool SampleDur::operator<(SampleDur const& other) const
	{
		if (isSpecified_ && other.isSpecified_) {
			// Both are specified
			return dur_ < other.dur_;
		} else {
			// Almost one is unspecified
			throw Exceptions::SampleDursComparisonWithAlmostOneUnspecified();
		}
	}
	bool SampleDur::operator<=(SampleDur const& other) const
	{
		if (isSpecified_ && other.isSpecified_) {
			// Both are specified
			return dur_ <= other.dur_;
		} else {
			// Almost one is unspecified
			throw Exceptions::SampleDursComparisonWithAlmostOneUnspecified();
		}
	}
	bool SampleDur::operator>(SampleDur const& other) const
	{
		if (isSpecified_ && other.isSpecified_) {
			// Both are specified
			return dur_ > other.dur_;
		} else {
			// Almost one is unspecified
			throw Exceptions::SampleDursComparisonWithAlmostOneUnspecified();
		}
	}
	bool SampleDur::operator>=(SampleDur const& other) const
	{
		if (isSpecified_ && other.isSpecified_) {
			// Both are specified
			return dur_ >= other.dur_;
		} else {
			// Almost one is unspecified
			throw Exceptions::SampleDursComparisonWithAlmostOneUnspecified();
		}
	}

	SampleDur::SampleDur() :
		isSpecified_(false),
		dur_(Dur(0.))
	{ }
	SampleDur::SampleDur(Dur const& duration) :
		isSpecified_(true),
		dur_(duration)
	{ }

	SampleDur min(SampleDur const& a, SampleDur const& b)
	{
		return (a <= b) ? a : b;
	}
	SampleDur max(SampleDur const& a, SampleDur const& b)
	{
		return (a >= b) ? a : b;
	}
}
