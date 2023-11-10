#include "ev_Meta.hpp"

namespace MID
{
	Event_Meta::Event_Meta(Pos const& pos, UINT_7 type) :
		Event(pos, 0xFFu),
		type_(type)
	{ }
	
	UINT_7 Event_Meta::type() const
	{
		return type_;
	}
	
	bool Event_Meta::isNoteOn() const
	{
		return false;
	}
	bool Event_Meta::isNoteOff() const
	{
		return false;
	}
	void Event_Meta::writeToFile(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_8 lastEventStatusByte) const
	{
		writeToFile(fileWriter, deltaTicks);
	}

	void Event_Meta::writeMetaEventToFile(FileWriter& fileWriter, UINT_28 deltaTicks, std::vector<UINT_8> const& bytesAfterLength) const
	{
		// Header
		writeToFile_header_type_length(fileWriter, deltaTicks, (UINT_28)(UINT_32)bytesAfterLength.size());
		// Data
		for (UINT_8 byte : bytesAfterLength) {
			fileWriter.write(byte);
		}
	}
	void Event_Meta::writeMetaEventToFile(FileWriter& fileWriter, UINT_28 deltaTicks, std::string const& bytesAfterLength) const
	{
		// Header
		writeToFile_header_type_length(fileWriter, deltaTicks, (UINT_28)(UINT_32)bytesAfterLength.length());
		// Data
		for (char byte : bytesAfterLength) {
			fileWriter.write((UINT_8)byte);
		}
	}
	
	void Event_Meta::writeToFile_header_type_length(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_28 length) const {
		// Event header
		writeEventHeaderToFile(fileWriter, deltaTicks);
		// Type
		fileWriter.write(type_);
		// Length
		fileWriter.write_variableLength_max4bytes(length);
	}
}
