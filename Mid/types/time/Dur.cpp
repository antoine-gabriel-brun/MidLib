#include "Dur.hpp"

#include "Pos.hpp"
#include "../tools/Exceptions.hpp"

namespace MID
{
	Dur::Dur(double time) :
		TimeValue(time)
	{ }

	Pos Dur::toPos() const
	{
		return Pos(time());
	}

	Dur Dur::operator+(Dur const& duration) const
	{
		double a = this->time();
		double b = duration.time();
		double sum = a + b;
		return Dur(sum);
	}
	Dur Dur::operator-(Dur const& duration) const
	{
		double a = this->time();
		double b = duration.time();
		if (a < b) {
			throw Exceptions::TimeValueOperatorUnderflow();
		}
		double diff = a - b;
		return Dur(diff);
	}

	bool Dur::operator==(Dur const& other) const
	{
		double a = this->time();
		double b = other.time();
		return a == b;
	}
	bool Dur::operator!=(Dur const& other) const
	{
		double a = this->time();
		double b = other.time();
		return a != b;
	}
	bool Dur::operator<(Dur const& other) const
	{
		double a = this->time();
		double b = other.time();
		return a < b;
	}
	bool Dur::operator<=(Dur const& other) const
	{
		double a = this->time();
		double b = other.time();
		return a <= b;
	}
	bool Dur::operator>(Dur const& other) const
	{
		double a = this->time();
		double b = other.time();
		return a > b;
	}
	bool Dur::operator>=(Dur const& other) const
	{
		double a = this->time();
		double b = other.time();
		return a >= b;
	}

	Dur min(Dur const& a, Dur const& b)
	{
		double aa = a.time();
		double bb = b.time();
		double min = (aa <= bb) ? aa : bb;
		return Dur(min);
	}
	Dur max(Dur const& a, Dur const& b)
	{
		double aa = a.time();
		double bb = b.time();
		double max = (aa >= bb) ? aa : bb;
		return Dur(max);
	}
}

MID::Dur operator*(double mult, MID::Dur const& duration)
{
	if (mult >= 0) {
		return MID::Dur(mult * duration.time());
	} else {
		throw MID::Exceptions::WrongDurMultiplier();
	}
}
MID::Dur operator*(MID::Dur const& duration, double mult)
{
	if (mult >= 0) {
		return MID::Dur(duration.time() * mult);
	} else {
		throw MID::Exceptions::WrongDurMultiplier();
	}
}
