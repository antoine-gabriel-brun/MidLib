#include "write-midi-file.hpp"

#include "../types/tools/FileWriter.hpp"

#include "collect-events.hpp"
#include "compute-EOT.hpp"
#include "crop-after-EOT.hpp"
#include "write-header-chunk.hpp"
#include "write-tempo-track-chunk.hpp"
#include "write-notes-track-chunk.hpp"

#include <vector>
#include <tuple>

namespace MID
{
	namespace
	{
		void computeMidiTicks(std::vector<std::vector<Event*>>& eventsInTracks, Event_EndOfTrack& endOfTrack, UINT_16 ticksPerQuarter)
		{
			for (std::vector<Event*>& eventsInTrack : eventsInTracks) {
				for (Event* trackEvent : eventsInTrack) {
					trackEvent->computePosAsMidiTicks(ticksPerQuarter);
				}
			}
			endOfTrack.computePosAsMidiTicks(ticksPerQuarter);
		}
		void writeChunks(
			Sample const& sample,
			std::string const& fileName,
			std::vector<std::vector<Event*>> const& eventsInTracks,
			Event_EndOfTrack const& endOfTrack)
		{
			// Create the file writer
			FileWriter fileWriter(fileName);
			
			// Keep the positions in file where something should be written later
			std::vector<std::tuple<std::streampos, UINT_32>> positionsInFile_and_values;
			
			// Write chunks
			writeChunk_header(fileWriter, sample.nbTracks(), sample.ticksPerQuarter());
			writeChunk_tempoTrack(fileWriter, sample.timeSignature(), sample.tempo(), endOfTrack, positionsInFile_and_values);
			writeChunks_notesTracks(fileWriter, eventsInTracks, endOfTrack, positionsInFile_and_values);

			// Write remaining data
			for (std::tuple<std::streampos, UINT_32> const& positionInFile_and_value : positionsInFile_and_values) {
				fileWriter.seek(std::get<0>(positionInFile_and_value));
				fileWriter.write_4_MSBfirst(std::get<1>(positionInFile_and_value));
			}
		}
		void deletePointers(std::vector<Event*>& toBeDeleted)
		{
			// Delete pointers to created events
			for (Event* e : toBeDeleted) {
				delete e;
			}
		}
	}

	void writeMidiFile(Sample& sample, std::string const& fileName)
	{
		// Create a container for raw pointers that need to be deleted
		std::vector<Event*> toBeDeleted;

		try {
			// Collect all events in notes tracks (sorted chronologically, and cleaned)
			std::vector<std::vector<Event*>> eventsInTracks = collectEventsInTracks_sorted_cleaned(sample, toBeDeleted);
			
			// Compute end of track (which can be specified or set to the last event), and crop after it
			Event_EndOfTrack endOfTrack = computeEndOfTrack(eventsInTracks, sample.dur());
			if (sample.dur().isSpecified()) {
				cropAfterEOT(eventsInTracks, endOfTrack, toBeDeleted);
			}
			
			// Compute MIDI ticks from floating point positions
			computeMidiTicks(eventsInTracks, endOfTrack, sample.ticksPerQuarter());

			// Write chunks
			writeChunks(sample, fileName, eventsInTracks, endOfTrack);

		} catch (...) {
			// Error: delete pointers to created events
			deletePointers(toBeDeleted);
			// Rethrow
			throw;
		}

		// No error: delete pointers to created events
		deletePointers(toBeDeleted);
	}
}
