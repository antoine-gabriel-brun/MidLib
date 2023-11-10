#include "FileWriter.hpp"

#include "Exceptions.hpp"

namespace MID
{
	namespace
	{
		bool checkHasMidiFileExtension(std::string const& fileName)
		{
			size_t len = fileName.length();
			// Long enough?
			if (len < 4) return false;
			// Test
			size_t pos = len - 4;
			if (fileName[pos] != '.') return false;
			else if (tolower(fileName[pos + 1] != 'm')) return false;
			else if (tolower(fileName[pos + 2] != 'i')) return false;
			else if (tolower(fileName[pos + 3] != 'd')) return false;
			else return true;
		}
		std::ofstream tryMakeValidOFStreamWithValidMidiFileName_throws(std::string const& fileName)
		{
			// Ends with something like '.mid'?
			bool hasMidiFileExtension = checkHasMidiFileExtension(fileName);
			// If not, throw
			if (!hasMidiFileExtension) {
				throw Exceptions::MissingMidFileExtension();
			}
			// Try to create an OF-stream
			std::ofstream ans;
			try {
				ans = std::ofstream(fileName, std::ios::binary);
				if (!ans.is_open()) throw 0;
			} catch (...) {
				throw Exceptions::CouldNotCreateMidiFile(fileName);
			}
			// Return
			return ans;
		}
		
		constexpr UINT_16 mask_16_LSB = 0x00FFu;
		constexpr UINT_32 mask_32_LSB = 0x000000FFu;
		constexpr UINT_32 mask_28_LS7 = 0b1111111u;
		constexpr UINT_8 mask_8_firstBit = 0b10000000u;
	}

	FileWriter::FileWriter(std::string const& fileName) :
		stream_(tryMakeValidOFStreamWithValidMidiFileName_throws(fileName))
	{ }

	void FileWriter::write(UINT_7 data)
	{
		stream_.put((UINT_8)data);
	}
	void FileWriter::write(UINT_8 data)
	{
		stream_.put(data);
	}
	void FileWriter::write_2_MSBfirst(UINT_16 data)
	{
		stream_.put((UINT_8)(data >> 8));
		stream_.put((UINT_8)(data & mask_16_LSB));
	}
	void FileWriter::write_4_MSBfirst(UINT_32 data)
	{
		stream_.put((UINT_8)(data >> 24));
		stream_.put((UINT_8)((data >> 16) & mask_32_LSB));
		stream_.put((UINT_8)((data >> 8) & mask_32_LSB));
		stream_.put((UINT_8)(data & mask_32_LSB));
	}
	void FileWriter::write(std::string const& str)
	{
		stream_.write(str.c_str(), str.length());
	}
	void FileWriter::write_variableLength_max4bytes(UINT_28 data)
	{
		if (data <= 0b1111111u) {
			stream_.put((UINT_8)data);
		} else if (data <= 0b11111111111111u) {
			stream_.put((UINT_8)(data >> 7) | mask_8_firstBit);
			stream_.put((UINT_8)(data & mask_28_LS7));
		} else if (data <= 0b111111111111111111111u) {
			stream_.put((UINT_8)(data >> 14) | mask_8_firstBit);
			stream_.put(((UINT_8)(data >> 7) & mask_28_LS7) | mask_8_firstBit);
			stream_.put((UINT_8)(data & mask_28_LS7));
		} else {
			stream_.put((UINT_8)(data >> 21) | mask_8_firstBit);
			stream_.put(((UINT_8)(data >> 14) & mask_28_LS7) | mask_8_firstBit);
			stream_.put(((UINT_8)(data >> 7) & mask_28_LS7) | mask_8_firstBit);
			stream_.put((UINT_8)(data & mask_28_LS7));
		}
	}

	void FileWriter::skip(int nbBytes)
	{
		for (int i = 0; i < nbBytes; i++) {
			stream_.put(0);
		}
	}

	std::streampos FileWriter::getPos()
	{
		return stream_.tellp();
	}

	void FileWriter::seek(std::streampos pos)
	{
		stream_.seekp(pos);
	}
}
