#ifndef MID_WRITE_TEMPO_TRACK_CHUNK_HPP
#define MID_WRITE_TEMPO_TRACK_CHUNK_HPP

#include "../types/tools/FileWriter.hpp"
#include "../types/sample-options/TimeSignature.hpp"
#include "../types/event-types/ev_Tempo.hpp"
#include "../types/event-types/ev_EndOfTrack.hpp"

#include <vector>
#include <tuple>

namespace MID
{
	// Subroutine used in the writeMidiFile() function
	void writeChunk_tempoTrack(
		FileWriter& fileWriter,
		TimeSignature const& timeSignature,
		Event_Tempo const& tempo,
		Event_EndOfTrack const& endOfTrack,
		std::vector<std::tuple<std::streampos, UINT_32>>& toWriteLater
	);
}

#endif
