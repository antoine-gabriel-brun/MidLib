#include "write-header-chunk.hpp"

namespace MID
{
	void writeChunk_header(
		FileWriter& fileWriter,
		size_t nbTracks,
		UINT_16 ticksPerQuarter)
	{
		// Chunk head bytes
		fileWriter.write("MThd");
		// Chunk length after chunk header: 6
		fileWriter.write_4_MSBfirst(6u);
		
		// MIDI format: 1
		fileWriter.write_2_MSBfirst((UINT_16)1u);
		// Nb tracks: one more than 'nbTracks' because of the tempo track
		fileWriter.write_2_MSBfirst((UINT_16)(1u + nbTracks));
		// Tick division
		fileWriter.write_2_MSBfirst(ticksPerQuarter);
	}
}
