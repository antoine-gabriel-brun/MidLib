#include "deal-with-overlapping-notes.hpp"

#include "../types/event-types/ev_NoteOnOff.hpp"
#include "../types/tools/Exceptions.hpp"

#include <array>

namespace MID
{
	// Helper functions and constants
	namespace
	{
		// Note counter: two-dimensional array that keeps track of the number
		// of notes that currently turn ON a given pitch on a given channel
		constexpr int nbPitches = 128;
		constexpr int nbChannels = 16;
		typedef std::array<std::array<int, nbPitches>, nbChannels> NoteCounter;
		NoteCounter initializeNoteCounter()
		{
			NoteCounter ans;
			
			for (int channel = 0; channel < nbChannels; channel++) {
				for (int pitch = 0; pitch < nbPitches; pitch++) {
					ans[channel][pitch] = 0;
				}
			}

			return ans;
		}

		// 4 possible algorithms to deal with overlapping notes
		std::vector<Event*> dealWithOverlappingNotes_trim(std::vector<Event*>& trackEvents, std::vector<Event*>& toBeDeleted)
		{
			// ON notes counter
			NoteCounter noteCounter = initializeNoteCounter();
			
			// Copy events; trim if necessary
			std::vector<Event*> ans;
			Event_NoteOnOff* noteOnOff;
			for (Event* originalEvent : trackEvents) {
				if (originalEvent->isNoteOn()) {
					// Note ON:
					noteOnOff = (Event_NoteOnOff*)originalEvent;
					// If the pitch isn't currently OFF, first end the note
					if (noteCounter[noteOnOff->getChannel()][noteOnOff->pitch()]++ != 0) {
						Event_NoteOnOff* newNoteOff = new Event_NoteOnOff(Event_NoteOnOff::off(
							noteOnOff->pos(),
							noteOnOff->pitch(),
							noteOnOff->getChannel()
						));
						toBeDeleted.push_back(newNoteOff);
						ans.push_back(newNoteOff);
					}
					// Copy
					ans.push_back(originalEvent);
				} else if (originalEvent->isNoteOff()) {
					// Note OFF: copy if the pitch is currently ON once
					noteOnOff = (Event_NoteOnOff*)originalEvent;
					if (--noteCounter[noteOnOff->getChannel()][noteOnOff->pitch()] == 0) {
						ans.push_back(originalEvent);
					}
				} else {
					// Neither a note ON nor a note OFF: always copy
					ans.push_back(originalEvent);
				}
			}
	
			// Return
			return ans;
		}
		std::vector<Event*> dealWithOverlappingNotes_merge(std::vector<Event*>& trackEvents)
		{
			// ON notes counter
			NoteCounter noteCounter = initializeNoteCounter();
			
			// Copy events; merge if necessary
			std::vector<Event*> ans;
			Event_NoteOnOff* noteOnOff;
			for (Event* originalEvent : trackEvents) {
				if (originalEvent->isNoteOn()) {
					// Note ON: copy if the pitch is currently OFF
					noteOnOff = (Event_NoteOnOff*)originalEvent;
					if (noteCounter[noteOnOff->getChannel()][noteOnOff->pitch()]++ == 0) {
						ans.push_back(originalEvent);
					}
				} else if (originalEvent->isNoteOff()) {
					// Note OFF: copy if the pitch is currently ON once
					noteOnOff = (Event_NoteOnOff*)originalEvent;
					if (--noteCounter[noteOnOff->getChannel()][noteOnOff->pitch()] == 0) {
						ans.push_back(originalEvent);
					}
				} else {
					// Neither a note ON nor a note OFF: always copy
					ans.push_back(originalEvent);
				}
			}
	
			// Return
			return ans;
		}
		std::vector<Event*> dealWithOverlappingNotes_ignore(std::vector<Event*>& trackEvents)
		{
			// Copy events
			return trackEvents;
		}
		std::vector<Event*> dealWithOverlappingNotes_error(std::vector<Event*>& trackEvents)
		{
			// ON notes counter
			NoteCounter noteCounter = initializeNoteCounter();
			
			// Copy events; throw if overlapping
			std::vector<Event*> ans;
			Event_NoteOnOff* noteOnOff;
			for (Event* originalEvent : trackEvents) {
				if (originalEvent->isNoteOn()) {
					// Note ON: copy if the pitch is currently OFF; else, throw
					noteOnOff = (Event_NoteOnOff*)originalEvent;
					if (noteCounter[noteOnOff->getChannel()][noteOnOff->pitch()]++ == 0) {
						ans.push_back(originalEvent);
					} else {
						throw Exceptions::OverlappingNotesTreatedAsError();
					}
				} else if (originalEvent->isNoteOff()) {
					// Note OFF: copy
					noteOnOff = (Event_NoteOnOff*)originalEvent;
					noteCounter[noteOnOff->getChannel()][noteOnOff->pitch()]--;
					ans.push_back(originalEvent);
				} else {
					// Neither a note ON nor a note OFF: always copy
					ans.push_back(originalEvent);
				}
			}
	
			// Return
			return ans;
		}
	}

	// Main function
	std::vector<Event*> dealWithOverlappingNotes(std::vector<Event*>& trackEvents, OverlappingNotes mode, std::vector<Event*>& toBeDeleted)
	{
		switch (mode)
		{
		case OverlappingNotes::trim:
			return dealWithOverlappingNotes_trim(trackEvents, toBeDeleted);
		case OverlappingNotes::merge:
			return dealWithOverlappingNotes_merge(trackEvents);
		case OverlappingNotes::ignore:
			return dealWithOverlappingNotes_ignore(trackEvents);
		case OverlappingNotes::error:
			return dealWithOverlappingNotes_error(trackEvents);
		default:
			throw; // won't happen
		}
	}
}