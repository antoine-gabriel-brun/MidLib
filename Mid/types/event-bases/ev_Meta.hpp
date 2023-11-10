#ifndef MID_EV_META_HPP
#define MID_EV_META_HPP

#include "Event.hpp"
#include "../tools/Integers.hpp"
#include "../tools/FileWriter.hpp"

#include <vector>
#include <string>

namespace MID
{
	// Base class for Meta MIDI events (these include tempo, time signature, end of track...)
	class Event_Meta : public Event
	{
	public:
		// Constructor
		Event_Meta(Pos const& pos, UINT_7 type);

		// Accessors
		UINT_7 type() const;
		
		// Implementations
		virtual bool isNoteOn() const override;
		virtual bool isNoteOff() const override;
		virtual void writeToFile(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_8 lastEventStatusByte) const override;

	protected:
		// Helper functions for event writing
		void writeMetaEventToFile(FileWriter& fileWriter, UINT_28 deltaTicks, std::vector<UINT_8> const& bytesAfterLength) const;
		void writeMetaEventToFile(FileWriter& fileWriter, UINT_28 deltaTicks, std::string const& bytesAfterLength) const;

	private:
		// Internal helper function
		void writeToFile_header_type_length(FileWriter& fileWriter, UINT_28 deltaTicks, UINT_28 length) const;

		// Fields
		UINT_7 type_;
		virtual void writeToFile(FileWriter& fileWriter, UINT_28 deltaTicks) const = 0;
	};
}

#endif
