#include "crop-after-EOT.hpp"

#include "../types/event-types/ev_NoteOnOff.hpp"
#include "../types/time/Pos.hpp"

#include <array>

namespace MID
{
	// Helper functions
	namespace
	{
		bool shouldKeep(Event* eventInTrack, Pos const& pos_endOfTrack)
		{
			if (eventInTrack->pos() <= pos_endOfTrack) {
				if (eventInTrack->pos() == pos_endOfTrack && eventInTrack->isNoteOn()) {
					// Exactly at EOT, and should be discarded in such case
					// (a NoteON event at end of track makes no sense, while a NoteOFF event does)
					return false;
				} else {
					// Before EOT, or at EOT ans should not be discarded in such case
					return true;
				}
			} else {
				// After EOT
				return false;
			}
		}
		void updateCorrispondingONs(std::array<int, 128>& begunNotEnded_indicesOfCorrispondingON, Event* eventInTrack, int lastWrittenIndex)
		{
			if (eventInTrack->isNoteOn()) {
				UINT_7 pitch = ((Event_NoteOnOff*)eventInTrack)->pitch();
				begunNotEnded_indicesOfCorrispondingON[pitch] = lastWrittenIndex;
			} else if (eventInTrack->isNoteOff()) {
				UINT_7 pitch = ((Event_NoteOnOff*)eventInTrack)->pitch();
				begunNotEnded_indicesOfCorrispondingON[pitch] = -1;
			}
		}
		void keepEvents(std::vector<Event*>& events_out, std::vector<Event*> const& eventsInTrack, std::array<int, 128>& begunNotEnded_indicesOfCorrispondingON, Pos const& pos_endOfTrack)
		{
			int lastWrittenIndex = -1;
			
			for (Event* eventInTrack : eventsInTrack) {
				if (shouldKeep(eventInTrack, pos_endOfTrack)) {
					events_out.push_back(eventInTrack);
					lastWrittenIndex++;
					updateCorrispondingONs(begunNotEnded_indicesOfCorrispondingON, eventInTrack, lastWrittenIndex);
				}
			}
		}
		Event_NoteOnOff* createNewNoteOff_newRawPtr(Event_NoteOnOff const* corrispondingON, Pos const& pos)
		{
			return Event_NoteOnOff::off_newRawPtr(
				pos,
				corrispondingON->pitch(),
				corrispondingON->getChannel()
			);
		}
		void addMissingOFFs(std::vector<Event*>& events_out, std::array<int, 128> const& begunNotEnded_indicesOfCorrispondingON, Pos const& pos_endOfTrack, std::vector<Event*>& toBeDeleted)
		{
			for (int pitch = 0; pitch < 128; pitch++) {
				int indexOfCorrispondingON = begunNotEnded_indicesOfCorrispondingON[pitch];
				if (indexOfCorrispondingON != -1) {
					Event_NoteOnOff* corrispondingON = (Event_NoteOnOff*)events_out[indexOfCorrispondingON];
					Event_NoteOnOff* newOFF = createNewNoteOff_newRawPtr(corrispondingON, pos_endOfTrack);
					toBeDeleted.push_back(newOFF);
					events_out.push_back(newOFF);
				}
			}
		}
		std::vector<Event*> cropAfterEOT_inTrack(std::vector<Event*> const& eventsInTrack, Pos const& pos_endOfTrack, std::vector<Event*>& toBeDeleted)
		{
			std::vector<Event*> ans;

			// Only keep events before EOT;
			// discard events that happen exactly at EOT and begin something (e.g. NoteON);
			// keep track of NoteON events that don't have a corrisponding NoteOFF.
			std::array<int, 128> begunNotEnded_indicesOfCorrispondingON;
			begunNotEnded_indicesOfCorrispondingON.fill(-1);
			keepEvents(ans, eventsInTrack, begunNotEnded_indicesOfCorrispondingON, pos_endOfTrack);

			// Add missing NoteOFF events
			addMissingOFFs(ans, begunNotEnded_indicesOfCorrispondingON, pos_endOfTrack, toBeDeleted);

			// Return
			return ans;
		}
	}

	// Main function
	void cropAfterEOT(
		std::vector<std::vector<Event*>>& events,
		Event_EndOfTrack const& endOfTrack,
		std::vector<Event*>& toBeDeleted)
	{
		int nbTracks = (int)events.size();
		Pos const& pos_endOfTrack = endOfTrack.pos();

		// Crop each track
		for (int i = 0; i < nbTracks; i++) {
			events[i] = cropAfterEOT_inTrack(events[i], pos_endOfTrack, toBeDeleted);
		}
	}
}
