#ifndef MID_SO_PRESERVE_CURVES_HPP
#define MID_SO_PRESERVE_CURVES_HPP

namespace MID
{
	namespace SampleOperators
	{
		// Specifies what happens when a sample operator is applied and the automation curve aspect is not preserved by simply copying events.
		enum class PreserveCurves {
			// Events are simply copied, regardless of the appearance of the automation curves.
			simplyCopyEvents,

			// The sample operator tries to preserve the aspect of the automation curves by adding or moving the necessary events.
			preserveCurves,

			// Same as 'preserveCurves'.
			defaultBehavior = preserveCurves
		};
	};
}

#endif
