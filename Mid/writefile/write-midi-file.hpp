#ifndef MID_WRITE_MIDI_FILE_HPP
#define MID_WRITE_MIDI_FILE_HPP

#include "../types/containers/Sample.hpp"

#include <string>

namespace MID
{
	// Write a sample into a file
	void writeMidiFile(Sample& sample, std::string const& fileName);
}

#endif
