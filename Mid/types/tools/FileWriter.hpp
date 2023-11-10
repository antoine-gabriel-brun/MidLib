#ifndef MID_FILE_WRITER_HPP
#define MID_FILE_WRITER_HPP

#include "Integers.hpp"

#include <string>
#include <fstream>

namespace MID
{
	// Encapsulates I/O operations for writing to MIDI files
	class FileWriter
	{
	public:
		// Constructor
		FileWriter(std::string const& fileName);

		// Write append
		void write(UINT_7 data);
		void write(UINT_8 data);
		void write_2_MSBfirst(UINT_16 data);
		void write_4_MSBfirst(UINT_32 data);
		void write(std::string const& str);
		void write_variableLength_max4bytes(UINT_28 data);

		// Skip while writing
		void skip(int nbBytes);

		// Get the next write position
		std::streampos getPos();

		// Set next write position
		void seek(std::streampos pos);

	private:
		// Fields: Underlying stream
		std::ofstream stream_;
	};
}

#endif
