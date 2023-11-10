#ifndef MID_EV_NOTE_ON_OFF_EVENTS_CREATOR_HPP
#define MID_EV_NOTE_ON_OFF_EVENTS_CREATOR_HPP

#include "../event-types/ev_NoteOnOff.hpp"
#include "../time/Pos.hpp"
#include "../time/Dur.hpp"
#include "../tools/Integers.hpp"
#include "../tools/Convert.hpp"
#include "../traits/isNumeric.hpp"

namespace MID
{
	// MIDI note that can generate NoteON and NoteOFF events.
	// Different from class Event_NoteOnOff, which represents a single On or Off MIDI event.
	// Alias name: Note = NoteOnOffEventsCreator.
	class NoteOnOffEventsCreator
	{
	public:
		// Constructors
		template<typename PosOrNumeric, typename dummy = enableIfPosOrNumeric<PosOrNumeric>>
		NoteOnOffEventsCreator(PosOrNumeric const& start, Pos const& end, UINT_7 pitch) :
			start_(Pos(start)),
			end_(end),
			pitch_(pitch),
			velocity_(64), // affected by Named parameter idiom
			channel_(0)    // affected by Named parameter idiom
		{
			if (start_ > end_) {
				throw Exceptions::NoteStartAfterNoteEnd();
			}
		}
		template<typename PosOrNumeric, typename DurOrNumeric, typename dummy1 = enableIfPosOrNumeric<PosOrNumeric>, typename dummy2 = enableIfDurOrNumeric<DurOrNumeric>>
		NoteOnOffEventsCreator(PosOrNumeric const& start, DurOrNumeric const& dur, UINT_7 pitch) :
			start_(Pos(start)),
			end_(start_ + Dur(dur)),
			pitch_(pitch),
			velocity_(64), // affected by Named parameter idiom
			channel_(0)    // affected by Named parameter idiom
		{ }

		// Named parameter idiom
		template<typename PosOrNumeric1, typename PosOrNumeric2, typename dummy1 = enableIfPosOrNumeric<PosOrNumeric1>, typename dummy2 = enableIfPosOrNumeric<PosOrNumeric2>>
		NoteOnOffEventsCreator& startEnd(PosOrNumeric1 const& start, PosOrNumeric2 const& end)
		{
			start_ = Pos(start);
			end_ = Pos(end);
			
			if (start_ <= end_) {
				return *this;
			} else {
				throw Exceptions::NoteStartAfterNoteEnd();
			}
		}
		template<typename PosOrNumeric, typename DurOrNumeric, typename dummy1 = enableIfPosOrNumeric<PosOrNumeric>, typename dummy2 = enableIfDurOrNumeric<DurOrNumeric>>
		NoteOnOffEventsCreator& startDur(PosOrNumeric const& start, DurOrNumeric const& dur)
		{
			start_ = Pos(start);
			end_ = start_ + Dur(dur);
			return *this;
		}
		NoteOnOffEventsCreator& pitch(UINT_7 pitch);
		template<typename Numeric, typename dummy = enableIfNumeric<Numeric>>
		NoteOnOffEventsCreator& velocity(Numeric velocity)
		{
			velocity_ = Convert::Velocity::float_to_midi((float)velocity);
			return *this;
		}
		NoteOnOffEventsCreator& velocityMidi(UINT_7 velocity);
		NoteOnOffEventsCreator& channel(UINT_4 channel);

		// Accessors
		Pos const& start() const;
		Pos const& end() const;
		UINT_7 pitch() const;
		UINT_7 velocity() const;
		UINT_4 channel() const;
		Dur calcDur() const;
		Event_NoteOnOff* calcNoteOn_newRawPtr() const;
		Event_NoteOnOff* calcNoteOff_newRawPtr() const;

		// Comparison operators
		bool operator==(NoteOnOffEventsCreator const& other) const;
		bool operator!=(NoteOnOffEventsCreator const& other) const;

	private:
		// Fields:
		Pos start_;
		Pos end_; // must be declared after 'start_'
		UINT_7 pitch_;
		UINT_7 velocity_;
		UINT_4 channel_;
	};

	typedef NoteOnOffEventsCreator Note;
}

#endif
