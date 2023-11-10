#include "Event.hpp"

namespace MID
{
	Event::Event(Pos const& pos, UINT_8 statusByte) :
		pos_(pos),
		posAsMidiTicks_(), // this will be computed just before writing the MIDI file
		statusByte_(statusByte)
	{ }
	
	Pos Event::pos() const
	{
		return pos_;
	}
	void Event::setPos(Pos const& pos)
	{
		pos_ = pos;
	}
	void Event::setPos(double pos)
	{
		pos_ = Pos(pos);
	}
	UINT_28 Event::posAsMidiTicks() const
	{
		return posAsMidiTicks_;
	}
	UINT_8 Event::statusByte() const
	{
		return statusByte_;
	}
	void Event::computePosAsMidiTicks(UINT_32 ticksPerTimeUnit) const
	{
		posAsMidiTicks_ = (UINT_28)(UINT_32)round(pos_.time() * ticksPerTimeUnit);
	}

	void Event::writeEventHeaderToFile(FileWriter& fileWriter, UINT_28 deltaTicks) const
	{
		// Delta time (1 byte to 4 bytes)
		fileWriter.write_variableLength_max4bytes(deltaTicks);
		// Status byte
		fileWriter.write(statusByte_);
	}
	void Event::writeEventHeaderToFile_canUseRunningStatus(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_8 lastEventStatusByte) const
	{
		// Delta time (1 byte to 4 bytes)
		fileWriter.write_variableLength_max4bytes(deltaTicks);
		// Status byte (or nothing if same as previous)
		if (statusByte_ != lastEventStatusByte) {
			fileWriter.write(statusByte_);
		} // else { nothing: running status }
	}
	void Event::writeEventToFile_1byteAfterStatus_canUseRunningStatus(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_8 byte, UINT_8 lastEventStatusByte) const
	{
		// Event header
		writeEventHeaderToFile_canUseRunningStatus(fileWriter, deltaTicks, lastEventStatusByte);
		// Data
		fileWriter.write(byte);
	}
	void Event::writeEventToFile_2bytesAfterStatus_canUseRunningStatus(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_8 byte0, UINT_8 byte1, UINT_8 lastEventStatusByte) const
	{
		// Event header
		writeEventHeaderToFile_canUseRunningStatus(fileWriter, deltaTicks, lastEventStatusByte);
		// Data
		fileWriter.write(byte0);
		fileWriter.write(byte1);
	}
}
