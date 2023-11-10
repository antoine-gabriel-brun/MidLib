#ifndef MID_TRACK_HPP
#define MID_TRACK_HPP

#include "../event-creators/ev_NoteOnOffEventsCreator.hpp"
#include "../event-types/ev_PitchBend.hpp"
#include "../event-types/ev_ControlChange.hpp"
#include "../event-types/ev_ProgramChange.hpp"
#include "../event-types/ev_ChannelAftertouch.hpp"
#include "../event-types/ev_KeyAftertouch.hpp"
#include "../sample-operators-tools/so_ElementTransformatorBase.hpp"

#include <vector>
#include <tuple>
#include <memory>
#include <type_traits>

namespace MID
{
	// MIDI track containing notes and MIDI events
	class Track
	{
	public:
		// Event adders for single addable elements
		void add(Note                    const& note             );
		void add(Event_PitchBend         const& pitchBend        );
		void add(Event_ControlChange     const& controlChange    );
		void add(Event_ProgramChange     const& programChange    );
		void add(Event_ChannelAftertouch const& channelAftertouch);
		void add(Event_KeyAftertouch     const& keyAftertouch    );

		// Unsafe event adder for events that should not be added to a notes track manually
		template<typename T_Event>
		void addUnsafe(T_Event const& unsafeEvent)
		{
			unsafeEvents_.push_back(std::make_shared<T_Event>(unsafeEvent));
		}
		
		// Bulk adder for iterables (simple template: any iterable containing addable elements defined above)
		template<typename T_Iterable>
		void add(T_Iterable const& collectionOfAddableElements)
		{
			for (auto const& addableElement : collectionOfAddableElements) {
				add(addableElement);
			}
		}
		
		// Bulk adder for tuples (crazy variadic template metaprogramming: any tuple of addable elements defined above)
		template <size_t I = 0, typename... Ts_Addables> typename std::enable_if<I == sizeof...(Ts_Addables), void>::type
		add(std::tuple<Ts_Addables...> tupleOfAddableElements)
		{
		    // If iterated through all values, then simply return.
		    return;
		}
		template<size_t Index = 0, typename... Ts_Addables> typename std::enable_if<(Index < sizeof...(Ts_Addables)), void>::type
		add(std::tuple<Ts_Addables...> const& tupleOfAddableElements)
		{
			add(std::get<Index>(tupleOfAddableElements));
			add<Index + 1>(tupleOfAddableElements);
		}
		
		// Accessors: reference to vector
		std::vector<Note                   >& notes              ();
		std::vector<Event_PitchBend        >& pitchBends         ();
		std::vector<Event_ControlChange    >& controlChanges     ();
		std::vector<Event_ProgramChange    >& programChanges     ();
		std::vector<Event_ChannelAftertouch>& channelAftertouches();
		std::vector<Event_KeyAftertouch    >& keyAftertouches    ();
		std::vector<std::shared_ptr<Event> >& unsafeEvents       ();

		// Accessors: const reference to vector
		std::vector<Note                   > const& notes              () const;
		std::vector<Event_PitchBend        > const& pitchBends         () const;
		std::vector<Event_ControlChange    > const& controlChanges     () const;
		std::vector<Event_ProgramChange    > const& programChanges     () const;
		std::vector<Event_ChannelAftertouch> const& channelAftertouches() const;
		std::vector<Event_KeyAftertouch    > const& keyAftertouches    () const;
		std::vector<std::shared_ptr<Event> > const& unsafeEvents       () const;

		// Accessors: vector size
		size_t nbNotes              () const;
		size_t nbPitchBends         () const;
		size_t nbControlChanges     () const;
		size_t nbProgramChanges     () const;
		size_t nbChannelAftertouches() const;
		size_t nbKeyAftertouches    () const;
		size_t nbUnsafeEvents       () const;
		
		// Accessors: reference to element by index
		Note                   & note             (unsigned int index);
		Event_PitchBend        & pitchBend        (unsigned int index);
		Event_ControlChange    & controlChange    (unsigned int index);
		Event_ProgramChange    & programChange    (unsigned int index);
		Event_ChannelAftertouch& channelAftertouch(unsigned int index);
		Event_KeyAftertouch    & keyAftertouch    (unsigned int index);
		std::shared_ptr<Event> & unsafeEvent      (unsigned int index);

		// Accessors: const reference to element by index
		Note                    const& note             (unsigned int index) const;
		Event_PitchBend         const& pitchBend        (unsigned int index) const;
		Event_ControlChange     const& controlChange    (unsigned int index) const;
		Event_ProgramChange     const& programChange    (unsigned int index) const;
		Event_ChannelAftertouch const& channelAftertouch(unsigned int index) const;
		Event_KeyAftertouch     const& keyAftertouch    (unsigned int index) const;
		std::shared_ptr<Event>  const& unsafeEvent      (unsigned int index) const;
		
		// Default accessor: (const or non-const) reference to note by index
		Note& operator[](unsigned int index);
		Note const& operator[](unsigned int index) const;

		// Get end position (position of the last event)
		Pos calcEndPos() const;

		// Merge and concat tracks
		static Track merge(Track const& track1, Track const& track2);
		static Track concat(Track const& track1, Track const& track2, double timeOffset);

		// Transform
		Track copyTransform(ElementTransformatorBase const& transformator) const;

	private:
		// Fields
		std::vector<Note                   > notes_              ;
		std::vector<Event_PitchBend        > pitchBends_         ;
		std::vector<Event_ControlChange    > controlChanges_     ;
		std::vector<Event_ProgramChange    > programChanges_     ;
		std::vector<Event_ChannelAftertouch> channelAftertouches_;
		std::vector<Event_KeyAftertouch    > keyAftertouches_    ;
		std::vector<std::shared_ptr<Event> > unsafeEvents_       ;
	};
}

#endif
