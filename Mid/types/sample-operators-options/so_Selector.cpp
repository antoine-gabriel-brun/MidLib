#include "so_Selector.hpp"

namespace MID
{
	namespace SampleOperators
	{
		Selector::Selector() :
			Selector(false, false, false, false, false, false)
		{ }

		Selector Selector::all()
		{
			return { true, true, true, true, true, true };
		}

		Selector& Selector::notes         () { notes_          = true; return *this; }
		Selector& Selector::pitchBends    () { pitchBends_     = true; return *this; }
		Selector& Selector::controlChanges() { controlChanges_ = true; return *this; }
		Selector& Selector::programChanges() { programChanges_ = true; return *this; }
		Selector& Selector::aftertouches  () { aftertouches_   = true; return *this; }
		Selector& Selector::unsafeEvents  () { unsafeEvents_   = true; return *this; }

		Selector& Selector::noNotes         () { notes_          = false; return *this; }
		Selector& Selector::noPitchBends    () { pitchBends_     = false; return *this; }
		Selector& Selector::noControlChanges() { controlChanges_ = false; return *this; }
		Selector& Selector::noProgramChanges() { programChanges_ = false; return *this; }
		Selector& Selector::noAftertouches  () { aftertouches_   = false; return *this; }
		Selector& Selector::noUnsafeEvents  () { unsafeEvents_   = false; return *this; }
		
		bool Selector::selectNotes         () const { return notes_         ; }
		bool Selector::selectPitchBends    () const { return pitchBends_    ; }
		bool Selector::selectControlChanges() const { return controlChanges_; }
		bool Selector::selectProgramChanges() const { return programChanges_; }
		bool Selector::selectAftertouches  () const { return aftertouches_  ; }
		bool Selector::selectUnsafeEvents  () const { return unsafeEvents_  ; }
		
		Selector::Selector(bool notes, bool pitchBends, bool controlChanges, bool programChanges, bool aftertouches, bool unsafeEvents) :
			notes_(notes),
			pitchBends_(pitchBends),
			controlChanges_(controlChanges),
			programChanges_(programChanges),
			aftertouches_(aftertouches),
			unsafeEvents_(unsafeEvents)
		{ }
	}
}
