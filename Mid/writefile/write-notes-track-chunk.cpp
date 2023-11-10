#include "write-notes-track-chunk.hpp"

namespace MID
{
	namespace
	{
		void writeChunk_notesTrack(
			FileWriter& fileWriter,
			std::vector<Event*> const& eventsInTrack,
			Event_EndOfTrack const& endOfTrack,
			std::vector<std::tuple<std::streampos, UINT_32>>& toWriteLater)
		{
			// Chunk head bytes
			fileWriter.write("MTrk");
			// Chunk length after chunk header: unknown for now
			std::tuple<std::streampos, UINT_32> writeLater(fileWriter.getPos(), 0);
			fileWriter.skip(4);
			std::streampos posBeforeChunkContents = fileWriter.getPos();

			// Events (use running status)
			UINT_32 lastEventPos = 0u;
			UINT_8 lastEventStatusByte = 0u;
			for (Event* eventInTrack : eventsInTrack) {
				eventInTrack->writeToFile(fileWriter, eventInTrack->posAsMidiTicks() - lastEventPos, lastEventStatusByte);
				lastEventPos = eventInTrack->posAsMidiTicks();
				lastEventStatusByte = eventInTrack->statusByte();
			}
			endOfTrack.writeToFile(fileWriter, endOfTrack.posAsMidiTicks() - lastEventPos);

			// Add the previously unknown chunk length
			UINT_32 len = (UINT_32)(fileWriter.getPos() - posBeforeChunkContents);
			std::get<1>(writeLater) = len;
			toWriteLater.push_back(writeLater);
		}
	}

	void writeChunks_notesTracks(
		FileWriter& fileWriter,
		std::vector<std::vector<Event*>> const& eventsInTracks,
		Event_EndOfTrack const& endOfTrack,
		std::vector<std::tuple<std::streampos, UINT_32>>& toWriteLater)
	{
		// Write each track
		for (std::vector<Event*> const& eventsInTrack : eventsInTracks) {
			writeChunk_notesTrack(
				fileWriter,
				eventsInTrack,
				endOfTrack,
				toWriteLater
			);
		}
	}
}
