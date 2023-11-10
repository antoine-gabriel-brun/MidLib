#include "write-tempo-track-chunk.hpp"

#include "../types/event-types/ev_TimeSignature.hpp"

namespace MID
{
	void writeChunk_tempoTrack(
		FileWriter& fileWriter,
		TimeSignature const& timeSignature,
		Event_Tempo const& tempo,
		Event_EndOfTrack const& endOfTrack,
		std::vector<std::tuple<std::streampos, UINT_32>>& toWriteLater)
	{
		// Chunk head bytes
		fileWriter.write("MTrk");
		// Chunk length after chunk header: unknown for now
		std::tuple<std::streampos, UINT_32> writeLater(fileWriter.getPos(), 0);
		fileWriter.skip(4);
		std::streampos posBeforeChunkContents = fileWriter.getPos();
		
		// Events
		Event_TimeSignature(timeSignature.num(), timeSignature.denom_4isQuarter_8isEighth_etc()).writeToFile(fileWriter, 0u);
		tempo.writeToFile(fileWriter, 0u);
		endOfTrack.writeToFile(fileWriter, endOfTrack.posAsMidiTicks());

		// Add the previously unknown chunk length
		UINT_32 len = (UINT_32)(fileWriter.getPos() - posBeforeChunkContents);
		std::get<1>(writeLater) = len;
		toWriteLater.push_back(writeLater);
	}
}
