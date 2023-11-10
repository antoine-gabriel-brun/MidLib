#ifndef MID_SO_UNSPECIFIED_MAP_VALUE_HPP
#define MID_SO_UNSPECIFIED_MAP_VALUE_HPP

namespace MID
{
	namespace SampleOperators
	{
		// Specifies what happens if the map of a sample operator doesn't specify a value.
		enum class UnspecifiedMapValue {
			// Elements not specified by the map are discarded.
			discard,

			// Elements not specified by the map are left unchanged.
			leaveUnchanged,

			// An exception is thrown if a sample operator doesn't specify a value.
			error,

			// Same as 'leaveUnchanged'.
			defaultBehavior = leaveUnchanged
		};
	};
}

#endif
