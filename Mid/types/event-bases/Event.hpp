#ifndef MID_EVENT_HPP
#define MID_EVENT_HPP

#include "../time/Pos.hpp"
#include "../tools/Integers.hpp"
#include "../tools/FileWriter.hpp"

#include <vector>
#include <memory>

namespace MID
{
	// Base class for all MIDI events
	class Event
	{
	public:
		// Constructor
		Event(Pos const& pos, UINT_8 statusByte);

		// Accessors
		Pos pos() const;
		void setPos(Pos const& pos);
		void setPos(double pos);
		UINT_28 posAsMidiTicks() const; // not yet defined after the constructor is called
		UINT_8 statusByte() const;
		void computePosAsMidiTicks(UINT_32 ticksPerTimeUnit) const; // should be called once, just before writing the MIDI file
		
		// To implement
		virtual bool isNoteOn() const = 0;
		virtual bool isNoteOff() const = 0;
		virtual std::shared_ptr<Event> clone() const = 0;
		virtual void writeToFile(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_8 lastEventStatusByte) const = 0;

	protected:
		// Helper functions for event writing
		void writeEventHeaderToFile(FileWriter& fileWriter, UINT_28 deltaTicks) const;
		void writeEventHeaderToFile_canUseRunningStatus(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_8 lastEventStatusByte) const;
		void writeEventToFile_1byteAfterStatus_canUseRunningStatus(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_8 byte, UINT_8 lastEventStatusByte) const;
		void writeEventToFile_2bytesAfterStatus_canUseRunningStatus(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_8 byte0, UINT_8 byte1, UINT_8 lastEventStatusByte) const;
		
	private:
		// Fields
		Pos pos_;
		mutable UINT_28 posAsMidiTicks_; // computed just before writing the MIDI file
		UINT_8 statusByte_;
	};
}

#endif
