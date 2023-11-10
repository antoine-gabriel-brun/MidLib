#ifndef MID_WRITE_HEADER_CHUNK_HPP
#define MID_WRITE_HEADER_CHUNK_HPP

#include "../types/tools/FileWriter.hpp"
#include "../types/tools/Integers.hpp"


namespace MID
{
	// Subroutine used in the writeMidiFile() function
	void writeChunk_header(
		FileWriter& fileWriter,
		size_t nbTracks,
		UINT_16 ticksPerQuarter
	);
}

#endif
