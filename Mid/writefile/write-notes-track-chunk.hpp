#ifndef MID_WRITE_NOTES_TRACK_CHUNK_HPP
#define MID_WRITE_NOTES_TRACK_CHUNK_HPP

#include "../types/tools/FileWriter.hpp"
#include "../types/event-types/ev_EndOfTrack.hpp"

#include <vector>
#include <tuple>

namespace MID
{
	// Subroutine used in the writeMidiFile() function
	void writeChunks_notesTracks(
		FileWriter& fileWriter,
		std::vector<std::vector<Event*>> const& eventsInTracks,
		Event_EndOfTrack const& endOfTrack,
		std::vector<std::tuple<std::streampos, UINT_32>>& toWriteLater
	);
}

#endif
