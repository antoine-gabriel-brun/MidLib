#include "Pos.hpp"

#include "Dur.hpp"
#include "../tools/Exceptions.hpp"

namespace MID
{
	Pos::Pos(double time) :
		TimeValue(time)
	{ }

	Dur Pos::toDuration() const
	{
		return Dur(time());
	}

	Pos Pos::operator+(Dur const& dur) const
	{
		double a = this->time();
		double b = dur.time();
		double sum = a + b;
		return Pos(sum);
	}
	Pos Pos::operator-(Dur const& dur) const
	{
		double a = this->time();
		double b = dur.time();
		if (a < b) {
			throw Exceptions::TimeValueOperatorUnderflow();
		}
		double diff = a - b;
		return Pos(diff);
	}
	Dur Pos::operator-(Pos const& pos) const
	{
		double a = this->time();
		double b = pos.time();
		if (a < b) {
			throw Exceptions::TimeValueOperatorUnderflow();
		}
		double diff = a - b;
		return Dur(diff);
	}

	bool Pos::operator==(Pos const& other) const
	{
		double a = this->time();
		double b = other.time();
		return a == b;
	}
	bool Pos::operator!=(Pos const& other) const
	{
		double a = this->time();
		double b = other.time();
		return a != b;
	}
	bool Pos::operator<(Pos const& other) const
	{
		double a = this->time();
		double b = other.time();
		return a < b;
	}
	bool Pos::operator<=(Pos const& other) const
	{
		double a = this->time();
		double b = other.time();
		return a <= b;
	}
	bool Pos::operator>(Pos const& other) const
	{
		double a = this->time();
		double b = other.time();
		return a > b;
	}
	bool Pos::operator>=(Pos const& other) const
	{
		double a = this->time();
		double b = other.time();
		return a >= b;
	}

	Pos min(Pos const& a, Pos const& b)
	{
		double aa = a.time();
		double bb = b.time();
		double min = (aa <= bb) ? aa : bb;
		return Pos(min);
	}
	Pos max(Pos const& a, Pos const& b)
	{
		double aa = a.time();
		double bb = b.time();
		double max = (aa >= bb) ? aa : bb;
		return Pos(max);
	}
}
