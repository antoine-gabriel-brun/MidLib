#ifndef MID_TIME_VALUE_HPP
#define MID_TIME_VALUE_HPP

#include "../tools/Integers.hpp"
#include "../tools/Exceptions.hpp"

namespace MID
{
	// Base class for Pos and Dur
	class TimeValue
	{
	public:
		// Accessors
		double time() const;
		bool isZero() const;

	protected:
		// Constructor
		explicit TimeValue(double time); // checks time >= 0

	private:
		// Field
		double time_;
	};
}

#endif
