#ifndef MID_EXCEPTION_HPP
#define MID_EXCEPTION_HPP

#include <string>

namespace MID
{
	// Exception thrown by the library
	class Exception
	{
	public:
		// Accessor
		std::string const& msg() const;

	protected:
		// Protected constructor: forces users to derive the class to create custom exceptions
		Exception(std::string const& msg);
		
	private:
		//Field
		std::string msg_;
	};
}

#endif
