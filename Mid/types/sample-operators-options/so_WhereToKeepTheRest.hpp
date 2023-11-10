#ifndef MID_SO_WHERE_TO_KEEP_THE_REST_HPP
#define MID_SO_WHERE_TO_KEEP_THE_REST_HPP

namespace MID
{
	namespace SampleOperators
	{
		// Specifies where to store data that can't be split by a sample operator.
		enum class WhereToKeepTheRest {
			// Remaining elements are retained in the first resulting sample.
			first,

			// Remaining elements are retained in the second resulting sample.
			second,

			// Remaining elements are not retained anywhere.
			none,

			// Remaining elements are retained in both resulting samples.
			both,

			// Same as 'first'.
			defaultBehavior = first
		};
	};
}

#endif
