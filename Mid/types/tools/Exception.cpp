#include "Exception.hpp"

namespace MID
{
	std::string const& Exception::msg() const
	{
		return msg_;
	}

	Exception::Exception(std::string const& msg) :
		msg_(msg)
	{ }
}
