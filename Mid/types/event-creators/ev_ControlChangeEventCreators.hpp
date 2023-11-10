#ifndef MID_EV_CONTROL_CHANGE_EVENTS_CREATORS_HPP
#define MID_EV_CONTROL_CHANGE_EVENTS_CREATORS_HPP

#include "../event-types/ev_ControlChange.hpp"
#include "../time/Pos.hpp"
#include "../tools/Integers.hpp"
#include "../traits/isNumeric.hpp"

#include <array>

// Provides names for all standard control changes.
namespace MID
{
	// Generic types of control changes
	class ControlChange_Base
	{
	public:
		// Constructor
		ControlChange_Base(Pos const& pos, UINT_4 channel = 0) :
			pos_(pos),
			channel_(channel)
		{ }
	protected:
		// Fields
		Pos pos_;
		UINT_4 channel_;
	};
	template<int I> class ControlChange_NoArgs : public ControlChange_Base
		// For controllers that don't accept arguments.
		// I is the controller Index.
	{
	public:
		// Constructor
		template<typename PosOrNumeric, typename dummy = enableIfPosOrNumeric<PosOrNumeric>>
		ControlChange_NoArgs(PosOrNumeric const& pos, UINT_4 channel = 0) :
			ControlChange_Base(Pos(pos), channel)
		{ }
		// Get event (default getter)
		Event_ControlChange operator()()
		{
			return Event_ControlChange(pos_, I, 0, channel_);
		}
	};
	template<int I> class ControlChange_LowResolution : public ControlChange_Base
		// For controllers that support low definition values.
		// I is the controller Index.
	{
	public:
		// Constructor
		template<typename PosOrNumeric, typename dummy = enableIfPosOrNumeric<PosOrNumeric>>
		ControlChange_LowResolution(PosOrNumeric const& pos, UINT_4 channel = 0) :
			ControlChange_Base(Pos(pos), channel)
		{ }
		// Get event
		Event_ControlChange fromMidi(UINT_7 value)
		{
			return Event_ControlChange(pos_, I, value, channel_);
		}
		template<typename Numeric, typename dummy = enableIfNumeric<Numeric>>
		Event_ControlChange fromFloat(Numeric value01)
		{
			return fromMidi(Convert::Param7bits::float_to_midi((float)value01));
		}
		// Default getter
		template<typename Numeric, typename dummy = enableIfNumeric<Numeric>>
		Event_ControlChange operator()(Numeric value01)
		{
			return fromFloat((float)value01);
		}
	};
	template<int I> class ControlChange_Switch : public ControlChange_Base
		// For controllers that represent a switch (on/off value).
		// I is the controller Index.
	{
	public:
		// Constructor
		template<typename PosOrNumeric, typename dummy = enableIfPosOrNumeric<PosOrNumeric>>
		ControlChange_Switch(PosOrNumeric const& pos, UINT_4 channel = 0) :
			ControlChange_Base(Pos(pos), channel)
		{ }
		// Get event (default getter)
		Event_ControlChange operator()(bool value)
		{
			return Event_ControlChange(pos_, I, value ? 127 : 0, channel_);
		}
	};
	template<int M, int L> class ControlChange_HighResolution : public ControlChange_Base
		// For controllers that support high definition values.
		// M is the controller index for the Most significant byte.
		// L is the controller index for the Least significant byte.
	{
	public:
		// Constructor
		template<typename PosOrNumeric, typename dummy = enableIfPosOrNumeric<PosOrNumeric>>
		ControlChange_HighResolution(PosOrNumeric const& pos, UINT_4 channel = 0) :
			ControlChange_Base(Pos(pos), channel)
		{ }
		// Get event(s)
		std::array<Event_ControlChange, 2> highResolutionFromMidiValues(UINT_7 valueMSB, UINT_7 valueLSB)
		{
			// Return 2 events
			return std::array<Event_ControlChange, 2>{
				Event_ControlChange(pos_, M, valueMSB, channel_),
				Event_ControlChange(pos_, L, valueLSB, channel_)
			};
		}
		std::array<Event_ControlChange, 2> highResolutionFromMidiValue(UINT_14 value)
		{
			// Most significant byte (0-127) and least significant byte (0-127) of the value
			UINT_8 valueMSB = value >> 7;
			UINT_8 valueLSB = value & 0b1111111;
			// Return 2 events
			return highResolutionFromMidiValues(valueMSB, valueLSB);
		}
		Event_ControlChange lowResolutionFromMidiValue(UINT_7 value)
		{
			// Return 1 event
			return Event_ControlChange(pos_, M, value, channel_);
		}
		template<typename Numeric, typename dummy = enableIfNumeric<Numeric>>
		std::array<Event_ControlChange, 2> highResolutionFromFloat(Numeric value01)
		{
			return highResolutionFromMidiValue(Convert::Param14bits::float_to_midi((float)value01));
		}
		template<typename Numeric, typename dummy = enableIfNumeric<Numeric>>
		Event_ControlChange lowResolutionFromFloat(Numeric value01)
		{
			return lowResolutionFromMidiValue(Convert::Param7bits::float_to_midi((float)value01));
		}
		// Intermediate default getters: don't specify the resolution and silently choose low resolution
		Event_ControlChange fromMidi(UINT_7 value)
		{
			return lowResolutionFromMidiValue(value);
		}
		template<typename Numeric, typename dummy = enableIfNumeric<Numeric>>
		Event_ControlChange fromFloat(Numeric value01)
		{
			return lowResolutionFromFloat(value01);
		}
		// Default getter
		template<typename Numeric, typename dummy = enableIfNumeric<Numeric>>
		Event_ControlChange operator()(Numeric value01)
		{
			return lowResolutionFromFloat((float)value01);
		}
	};

	// Standard control changes
	typedef ControlChange_HighResolution<0  , 32 > BankSelect;
	typedef ControlChange_HighResolution<1  , 33 > ModulationWheel;
	typedef ControlChange_HighResolution<2  , 34 > BreathControl;
	typedef ControlChange_HighResolution<4  , 36 > FootController;
	typedef ControlChange_HighResolution<5  , 37 > PortamentoTime;
	typedef ControlChange_HighResolution<6  , 38 > DataEntry;
	typedef ControlChange_HighResolution<7  , 39 > ChannelVolume;
	typedef ControlChange_HighResolution<8  , 40 > Balance;
	typedef ControlChange_HighResolution<10 , 42 > Pan;
	typedef ControlChange_HighResolution<11 , 43 > ExpressionController;
	typedef ControlChange_HighResolution<12 , 44 > EffectControl1;
	typedef ControlChange_HighResolution<13 , 45 > EffectControl2;
	typedef ControlChange_HighResolution<16 , 48 > GeneralPurposeController1;
	typedef ControlChange_HighResolution<17 , 49 > GeneralPurposeController2;
	typedef ControlChange_HighResolution<18 , 50 > GeneralPurposeController3;
	typedef ControlChange_HighResolution<19 , 51 > GeneralPurposeController4;
	typedef ControlChange_HighResolution<99 , 98 > NonRegisteredParameterNumber;
	typedef ControlChange_HighResolution<101, 100> RegisteredParameterNumber;
	typedef ControlChange_Switch        <64>       Sustain;
	typedef ControlChange_Switch        <65>       Portamento;
	typedef ControlChange_Switch        <66>       Sostenuto;
	typedef ControlChange_Switch        <67>       SoftPedal;
	typedef ControlChange_Switch        <68>       LegatoFootswitch;
	typedef ControlChange_Switch        <69>       Hold2;
	typedef ControlChange_LowResolution <70>       SoundController1;
	typedef ControlChange_LowResolution <71>       SoundController2;
	typedef ControlChange_LowResolution <72>       SoundController3;
	typedef ControlChange_LowResolution <73>       SoundController4;
	typedef ControlChange_LowResolution <74>       SoundController5;
	typedef ControlChange_LowResolution <75>       SoundController6;
	typedef ControlChange_LowResolution <76>       SoundController7;
	typedef ControlChange_LowResolution <77>       SoundController8;
	typedef ControlChange_LowResolution <78>       SoundController9;
	typedef ControlChange_LowResolution <79>       SoundController10;
	typedef ControlChange_LowResolution <80>       GeneralPurposeController5;
	typedef ControlChange_LowResolution <81>       GeneralPurposeController6;
	typedef ControlChange_LowResolution <82>       GeneralPurposeController7;
	typedef ControlChange_LowResolution <83>       GeneralPurposeController8;
	typedef ControlChange_LowResolution <84>       PortamentoControl;
	typedef ControlChange_LowResolution <88>       HighResolutionVelocityPrefix;
	typedef ControlChange_LowResolution <91>       Effects1depth;
	typedef ControlChange_LowResolution <92>       Effects2depth;
	typedef ControlChange_LowResolution <93>       Effects3depth;
	typedef ControlChange_LowResolution <94>       Effects4depth;
	typedef ControlChange_LowResolution <95>       Effects5depth;
	typedef ControlChange_LowResolution <96>       DataIncrement;
	typedef ControlChange_LowResolution <97>       DataDecrement;
	typedef ControlChange_NoArgs        <120>      AllSoundOff;
	typedef ControlChange_NoArgs        <121>      ResetAllControllers;
	typedef ControlChange_Switch        <122>      LocalControlOnOff;
	typedef ControlChange_NoArgs        <123>      AllNotesOff;
	typedef ControlChange_NoArgs        <124>      OmniModeOff;
	typedef ControlChange_NoArgs        <125>      OmniModeOn;
	typedef ControlChange_LowResolution <126>      PolyModeOnOff;
	typedef ControlChange_NoArgs        <127>      PolyModeOn;

	// Custom control change (use when you know the controller number, or for non-standard control changes)
	template <int I> using CustomControlChange = ControlChange_LowResolution<I>;
}

#endif
