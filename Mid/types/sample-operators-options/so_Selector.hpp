#ifndef MID_SO_SELECTOR_HPP
#define MID_SO_SELECTOR_HPP

namespace MID
{
	namespace SampleOperators
	{
		// Specifies the elements to which a sample operator should be applied.
		class Selector
		{
		public:
			// Public constructor (doesn't keep anything initially)
			Selector();

			// Creator (keeps all initially)
			static Selector all();
			
			// Named constructor idiom: keep
			Selector& notes         ();
			Selector& pitchBends    ();
			Selector& controlChanges();
			Selector& programChanges();
			Selector& aftertouches  ();
			Selector& unsafeEvents  ();
			
			// Named constructor idiom: don't keep
			Selector& noNotes         ();
			Selector& noPitchBends    ();
			Selector& noControlChanges();
			Selector& noProgramChanges();
			Selector& noAftertouches  ();
			Selector& noUnsafeEvents  ();

			// Accessors
			bool selectNotes         () const;
			bool selectPitchBends    () const;
			bool selectControlChanges() const;
			bool selectProgramChanges() const;
			bool selectAftertouches  () const;
			bool selectUnsafeEvents  () const;

		private:
			// Private constructor
			Selector(bool notes, bool pitchBends, bool controlChanges, bool programChanges, bool aftertouches, bool unsafeEvents);

			// Fields
			bool notes_;
			bool pitchBends_;
			bool controlChanges_;
			bool programChanges_;
			bool aftertouches_;
			bool unsafeEvents_;
		};
	};
}

#endif
