#include "collect-events.hpp"

#include "../types/containers/Track.hpp"
#include "../types/event-types/ev_NoteOnOff.hpp"
#include "../types/sample-options/OverlappingNotes.hpp"
#include "deal-with-overlapping-notes.hpp"

#include <memory>
#include <algorithm>

namespace MID
{
	// Helper functions
	namespace
	{
		// Algorithm steps
		template <typename T_Event>
		void collect(std::vector<Event*>& collector_out, std::vector<T_Event>& events)
		{
			for (T_Event& ev: events) {
				collector_out.push_back(&ev);
			}
		}
		void collectUnsafe(std::vector<Event*>& collector_out, std::vector<std::shared_ptr<Event>> const& events)
		{
			for (std::shared_ptr<Event> const& ev: events) {
				collector_out.push_back(ev.get());
			}
		}
		std::vector<Event*> collectEvents_raw_cleverPreOrder(Track& track, std::vector<Event*>& toBeDeleted)
		{
			// This vector will collect all events from the track
			std::vector<Event*> trackEvents;

			// If they occur at the same time as a note-on, pitch bends,
			// control changes, and especially program changes must be sent first.
			collect(trackEvents, track.pitchBends());
			collect(trackEvents, track.controlChanges());
			collect(trackEvents, track.programChanges());

			// Note-off before note-on, so that after sorting chronologically, everything
			// is in order. This is meant to turn a note off before it's hit again.
			for (Note const& note : track.notes()) {
				Event* noteOff = note.calcNoteOff_newRawPtr();
				toBeDeleted.push_back(noteOff);
				trackEvents.push_back(noteOff);
			}
			for (Note const& note : track.notes()) {
				Event* noteOn = note.calcNoteOn_newRawPtr();
				toBeDeleted.push_back(noteOn);
				trackEvents.push_back(noteOn);
			}

			// If it occurs at the same time as a note-on, an aftertouch event must be sent second.
			collect(trackEvents, track.channelAftertouches());
			collect(trackEvents, track.keyAftertouches());

			// Collect unsafe events
			collectUnsafe(trackEvents, track.unsafeEvents());

			// Return all collected events
			return trackEvents;
		}
		bool comparer(Event* a, Event* b)
		{
			return a->pos() < b->pos();
		}
		void sortChronologically(std::vector<Event*>& trackEvents)
		{
			// Stable sort prevents the NoteOFF of a first note
			// and the NoteON of a second one, that both occur
			// at the same, from being exchanged; it also ensures
			// program changes occur before NoteOn events, etc.
			std::stable_sort(trackEvents.begin(), trackEvents.end(), comparer);
		}

		// Collect and format events (uses functions above)
		std::vector<Event*> collect_sorted_cleaned(Track& track, OverlappingNotes overlappingNotes, std::vector<Event*>& toBeDeleted)
		{
			// Collect events (clever order)
			std::vector<Event*> trackEvents = collectEvents_raw_cleverPreOrder(track, toBeDeleted);

			// Sort them chronologically, but for simultaneous ones, keep the clever order (stable sort)
			sortChronologically(trackEvents);

			// Clean them (i.e. deal with overlapping notes)
			trackEvents = dealWithOverlappingNotes(trackEvents, overlappingNotes, toBeDeleted);

			// Return
			return trackEvents;
		}
	}
	
	// Main function
	std::vector<std::vector<Event*>> collectEventsInTracks_sorted_cleaned(Sample& sample, std::vector<Event*>& toBeDeleted)
	{
		std::vector<std::vector<Event*>> ans;

		// Collect events in each track
		for (int trackIndex = 0; trackIndex < sample.nbTracks(); trackIndex++) {
			ans.push_back(collect_sorted_cleaned(
				sample.track(trackIndex),
				sample.overlappingNotes(),
				toBeDeleted
			));
		}
		
		// Return
		return ans;
	}
}
