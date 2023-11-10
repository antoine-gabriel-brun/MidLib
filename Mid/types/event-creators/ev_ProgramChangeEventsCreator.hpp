#ifndef MID_EV_PROGRAM_CHANGE_EVENTS_CREATOR_HPP
#define MID_EV_PROGRAM_CHANGE_EVENTS_CREATOR_HPP

#include "../event-types/ev_ProgramChange.hpp"
#include "../time/Pos.hpp"
#include "../tools/Integers.hpp"
#include "../tools/Exceptions.hpp"

namespace MID
{
	// Instrument: Class for program change creation.
	// Different from class Event_ProgramChange, which represents a MIDI event.
	// Alias name: Instrument = ProgramChangeEventsCreator.
	class ProgramChangeEventsCreator
	{
	public:
		// Constructor
		template<typename PosOrNumeric, typename dummy = enableIfPosOrNumeric<PosOrNumeric>>
		ProgramChangeEventsCreator(PosOrNumeric const& pos, UINT_4 channel = 0) :
			pos_(Pos(pos)),
			channel_(channel),
			forbidWrongChannel_(true)
		{ }
		
		// Allow instruments intended for special channel 9 on other channels, and vice versa
		ProgramChangeEventsCreator& allowWrongChannel()
		{
			forbidWrongChannel_ = false;
			return *this;
		}
		
		// Get program change event from instrument index
		Event_ProgramChange operator()(UINT_7 instrumentIndex, char specialChannel9 = 'u')
		{
			if (forbidWrongChannel_ && specialChannel9 == 's' && channel_ != 9) {
				throw Exceptions::InstrumentIntendedForSpecialChannel9();
			} else if (forbidWrongChannel_ && specialChannel9 == 'n' && channel_ == 9) {
				throw Exceptions::InstrumentNotIntendedForSpecialChannel9();
			} else {
				return Event_ProgramChange(pos_, instrumentIndex, channel_, specialChannel9);
			}
		}
		
		// Get default program change event
		Event_ProgramChange operator()()
		{
			// Same as acousticGrandPiano() when not on special channel 9:
			return operator()(0, 'u');
		}
		
		// Get program change event from instrument name (normal channel)
		Event_ProgramChange acousticGrandPiano          () { return operator()(  0, 'n'); }
		Event_ProgramChange brightAcousticPiano         () { return operator()(  1, 'n'); }
		Event_ProgramChange electricGrandPiano          () { return operator()(  2, 'n'); }
		Event_ProgramChange honkyTonkPiano              () { return operator()(  3, 'n'); }
		Event_ProgramChange electricPiano1_rhodesPiano  () { return operator()(  4, 'n'); }
		Event_ProgramChange electricPiano2_chorusedPiano() { return operator()(  5, 'n'); }
		Event_ProgramChange harpsichord                 () { return operator()(  6, 'n'); }
		Event_ProgramChange clavinet                    () { return operator()(  7, 'n'); }
		Event_ProgramChange celesta                     () { return operator()(  8, 'n'); }
		Event_ProgramChange glockenspiel                () { return operator()(  9, 'n'); }
		Event_ProgramChange musicBox                    () { return operator()( 10, 'n'); }
		Event_ProgramChange vibraphone                  () { return operator()( 11, 'n'); }
		Event_ProgramChange marimba                     () { return operator()( 12, 'n'); }
		Event_ProgramChange xylophone                   () { return operator()( 13, 'n'); }
		Event_ProgramChange tubularBells                () { return operator()( 14, 'n'); }
		Event_ProgramChange dulcimer_santur             () { return operator()( 15, 'n'); }
		Event_ProgramChange drawbarOrgan_hammond        () { return operator()( 16, 'n'); }
		Event_ProgramChange percussiveOrgan             () { return operator()( 17, 'n'); }
		Event_ProgramChange rockOrgan                   () { return operator()( 18, 'n'); }
		Event_ProgramChange churchOrgan                 () { return operator()( 19, 'n'); }
		Event_ProgramChange reedOrgan                   () { return operator()( 20, 'n'); }
		Event_ProgramChange accordion_french            () { return operator()( 21, 'n'); }
		Event_ProgramChange harmonica                   () { return operator()( 22, 'n'); }
		Event_ProgramChange tangoAccordion_bandoneon    () { return operator()( 23, 'n'); }
		Event_ProgramChange acousticGuitar_nylon        () { return operator()( 24, 'n'); }
		Event_ProgramChange acousticGuitar_steel        () { return operator()( 25, 'n'); }
		Event_ProgramChange electricGuitar_jazz         () { return operator()( 26, 'n'); }
		Event_ProgramChange electricGuitar_clean        () { return operator()( 27, 'n'); }
		Event_ProgramChange electricGuitar_muted        () { return operator()( 28, 'n'); }
		Event_ProgramChange overdrivenGuitar            () { return operator()( 29, 'n'); }
		Event_ProgramChange distortionGuitar            () { return operator()( 30, 'n'); }
		Event_ProgramChange guitarharmonics             () { return operator()( 31, 'n'); }
		Event_ProgramChange acousticBass                () { return operator()( 32, 'n'); }
		Event_ProgramChange electricBass_fingered       () { return operator()( 33, 'n'); }
		Event_ProgramChange electricBass_picked         () { return operator()( 34, 'n'); }
		Event_ProgramChange fretlessBass                () { return operator()( 35, 'n'); }
		Event_ProgramChange slapBass1                   () { return operator()( 36, 'n'); }
		Event_ProgramChange slapBass2                   () { return operator()( 37, 'n'); }
		Event_ProgramChange synthBass1                  () { return operator()( 38, 'n'); }
		Event_ProgramChange synthBass2                  () { return operator()( 39, 'n'); }
		Event_ProgramChange violin                      () { return operator()( 40, 'n'); }
		Event_ProgramChange viola                       () { return operator()( 41, 'n'); }
		Event_ProgramChange cello                       () { return operator()( 42, 'n'); }
		Event_ProgramChange contrabass                  () { return operator()( 43, 'n'); }
		Event_ProgramChange tremoloStrings              () { return operator()( 44, 'n'); }
		Event_ProgramChange pizzicatoStrings            () { return operator()( 45, 'n'); }
		Event_ProgramChange orchestralHarp              () { return operator()( 46, 'n'); }
		Event_ProgramChange timpani                     () { return operator()( 47, 'n'); }
		Event_ProgramChange stringEnsemble1_strings     () { return operator()( 48, 'n'); }
		Event_ProgramChange stringEnsemble2_slowstrings () { return operator()( 49, 'n'); }
		Event_ProgramChange synthStrings1               () { return operator()( 50, 'n'); }
		Event_ProgramChange synthStrings2               () { return operator()( 51, 'n'); }
		Event_ProgramChange choirAahs                   () { return operator()( 52, 'n'); }
		Event_ProgramChange voiceOohs                   () { return operator()( 53, 'n'); }
		Event_ProgramChange synthVoice                  () { return operator()( 54, 'n'); }
		Event_ProgramChange orchestraHit                () { return operator()( 55, 'n'); }
		Event_ProgramChange trumpet                     () { return operator()( 56, 'n'); }
		Event_ProgramChange trombone                    () { return operator()( 57, 'n'); }
		Event_ProgramChange tuba                        () { return operator()( 58, 'n'); }
		Event_ProgramChange mutedTrumpet                () { return operator()( 59, 'n'); }
		Event_ProgramChange frenchHorn                  () { return operator()( 60, 'n'); }
		Event_ProgramChange brassSection                () { return operator()( 61, 'n'); }
		Event_ProgramChange synthBrass1                 () { return operator()( 62, 'n'); }
		Event_ProgramChange synthBrass2                 () { return operator()( 63, 'n'); }
		Event_ProgramChange sopranoSax                  () { return operator()( 64, 'n'); }
		Event_ProgramChange altoSax                     () { return operator()( 65, 'n'); }
		Event_ProgramChange tenorSax                    () { return operator()( 66, 'n'); }
		Event_ProgramChange baritoneSax                 () { return operator()( 67, 'n'); }
		Event_ProgramChange oboe                        () { return operator()( 68, 'n'); }
		Event_ProgramChange englishHorn                 () { return operator()( 69, 'n'); }
		Event_ProgramChange bassoon                     () { return operator()( 70, 'n'); }
		Event_ProgramChange clarinet                    () { return operator()( 71, 'n'); }
		Event_ProgramChange piccolo                     () { return operator()( 72, 'n'); }
		Event_ProgramChange flute                       () { return operator()( 73, 'n'); }
		Event_ProgramChange recorder                    () { return operator()( 74, 'n'); }
		Event_ProgramChange panFlute                    () { return operator()( 75, 'n'); }
		Event_ProgramChange blownBottle                 () { return operator()( 76, 'n'); }
		Event_ProgramChange shakuhachi                  () { return operator()( 77, 'n'); }
		Event_ProgramChange whistle                     () { return operator()( 78, 'n'); }
		Event_ProgramChange ocarina                     () { return operator()( 79, 'n'); }
		Event_ProgramChange lead1_squareWave            () { return operator()( 80, 'n'); }
		Event_ProgramChange lead2_sawToothWave          () { return operator()( 81, 'n'); }
		Event_ProgramChange lead3_calliope              () { return operator()( 82, 'n'); }
		Event_ProgramChange lead4_chiffer               () { return operator()( 83, 'n'); }
		Event_ProgramChange lead5_charang               () { return operator()( 84, 'n'); }
		Event_ProgramChange lead6_voiceSolo             () { return operator()( 85, 'n'); }
		Event_ProgramChange lead7_fifths                () { return operator()( 86, 'n'); }
		Event_ProgramChange lead8_bassPlusLead          () { return operator()( 87, 'n'); }
		Event_ProgramChange pad1_newAgeFantasia         () { return operator()( 88, 'n'); }
		Event_ProgramChange pad2_warm                   () { return operator()( 89, 'n'); }
		Event_ProgramChange pad3_polySynth              () { return operator()( 90, 'n'); }
		Event_ProgramChange pad4_choirSpaceVoice        () { return operator()( 91, 'n'); }
		Event_ProgramChange pad5_bowedGlass             () { return operator()( 92, 'n'); }
		Event_ProgramChange pad6_metallicPro            () { return operator()( 93, 'n'); }
		Event_ProgramChange pad7_halo                   () { return operator()( 94, 'n'); }
		Event_ProgramChange pad8_sweep                  () { return operator()( 95, 'n'); }
		Event_ProgramChange fX1_rain                    () { return operator()( 96, 'n'); }
		Event_ProgramChange fX2_soundtrack              () { return operator()( 97, 'n'); }
		Event_ProgramChange fX3_crystal                 () { return operator()( 98, 'n'); }
		Event_ProgramChange fX4_atmosphere              () { return operator()( 99, 'n'); }
		Event_ProgramChange fX5_brightness              () { return operator()(100, 'n'); }
		Event_ProgramChange fX6_goblins                 () { return operator()(101, 'n'); }
		Event_ProgramChange fX7_echoesAndDrops          () { return operator()(102, 'n'); }
		Event_ProgramChange fX8_sciFiStarTheme          () { return operator()(103, 'n'); }
		Event_ProgramChange sitar                       () { return operator()(104, 'n'); }
		Event_ProgramChange banjo                       () { return operator()(105, 'n'); }
		Event_ProgramChange shamisen                    () { return operator()(106, 'n'); }
		Event_ProgramChange koto                        () { return operator()(107, 'n'); }
		Event_ProgramChange kalimba                     () { return operator()(108, 'n'); }
		Event_ProgramChange bagpipe                     () { return operator()(109, 'n'); }
		Event_ProgramChange fiddle                      () { return operator()(110, 'n'); }
		Event_ProgramChange shanai                      () { return operator()(111, 'n'); }
		Event_ProgramChange tinkleBell                  () { return operator()(112, 'n'); }
		Event_ProgramChange agogo                       () { return operator()(113, 'n'); }
		Event_ProgramChange steelDrums                  () { return operator()(114, 'n'); }
		Event_ProgramChange woodblock                   () { return operator()(115, 'n'); }
		Event_ProgramChange taikoDrum                   () { return operator()(116, 'n'); }
		Event_ProgramChange melodicTom                  () { return operator()(117, 'n'); }
		Event_ProgramChange synthDrum                   () { return operator()(118, 'n'); }
		Event_ProgramChange reverseCymbal               () { return operator()(119, 'n'); }
		Event_ProgramChange guitarFretNoise             () { return operator()(120, 'n'); }
		Event_ProgramChange breathNoise                 () { return operator()(121, 'n'); }
		Event_ProgramChange seashore                    () { return operator()(122, 'n'); }
		Event_ProgramChange birdTweet                   () { return operator()(123, 'n'); }
		Event_ProgramChange telephoneRing               () { return operator()(124, 'n'); }
		Event_ProgramChange helicopter                  () { return operator()(125, 'n'); }
		Event_ProgramChange applause                    () { return operator()(126, 'n'); }
		Event_ProgramChange gunshot                     () { return operator()(127, 'n'); }
		
		// Get program change event from synonym instrument name (normal channel)
		Event_ProgramChange accordion     () { return accordion_french    (); }
		Event_ProgramChange electricGuitar() { return electricGuitar_clean(); }
		Event_ProgramChange guitar        () { return acousticGuitar_nylon(); }
		Event_ProgramChange harp          () { return orchestralHarp      (); }
		Event_ProgramChange horn          () { return frenchHorn          (); }
		Event_ProgramChange organ         () { return churchOrgan         (); }
		Event_ProgramChange piano         () { return acousticGrandPiano  (); }
		Event_ProgramChange sawToothWave  () { return lead2_sawToothWave  (); }
		Event_ProgramChange sax           () { return tenorSax            (); }
		Event_ProgramChange squareWave    () { return lead1_squareWave    (); }
		
		// Get program change event from instrument name (special channel 9)
		Event_ProgramChange drum_highQ           () { return operator()(27, 's'); } // added in General MIDI 2
		Event_ProgramChange drum_slap            () { return operator()(28, 's'); } // added in General MIDI 2
		Event_ProgramChange drum_scratchPush     () { return operator()(29, 's'); } // added in General MIDI 2
		Event_ProgramChange drum_scratchPull     () { return operator()(30, 's'); } // added in General MIDI 2
		Event_ProgramChange drum_sticks          () { return operator()(31, 's'); } // added in General MIDI 2
		Event_ProgramChange drum_squareClick     () { return operator()(32, 's'); } // added in General MIDI 2
		Event_ProgramChange drum_metronomeClick  () { return operator()(33, 's'); } // added in General MIDI 2
		Event_ProgramChange drum_metronomeBell   () { return operator()(34, 's'); } // added in General MIDI 2
		Event_ProgramChange drum_acousticBassDrum() { return operator()(35, 's'); }
		Event_ProgramChange drum_bassDrum1       () { return operator()(36, 's'); }
		Event_ProgramChange drum_sideStick       () { return operator()(37, 's'); }
		Event_ProgramChange drum_acousticSnare   () { return operator()(38, 's'); }
		Event_ProgramChange drum_handClap        () { return operator()(39, 's'); }
		Event_ProgramChange drum_electricSnare   () { return operator()(40, 's'); }
		Event_ProgramChange drum_lowFloorTom     () { return operator()(41, 's'); }
		Event_ProgramChange drum_closedHiHat     () { return operator()(42, 's'); }
		Event_ProgramChange drum_highFloorTom    () { return operator()(43, 's'); }
		Event_ProgramChange drum_pedalHiHat      () { return operator()(44, 's'); }
		Event_ProgramChange drum_lowTom          () { return operator()(45, 's'); }
		Event_ProgramChange drum_openHiHat       () { return operator()(46, 's'); }
		Event_ProgramChange drum_lowMidTom       () { return operator()(47, 's'); }
		Event_ProgramChange drum_highMidTom      () { return operator()(48, 's'); }
		Event_ProgramChange drum_crashCymbal1    () { return operator()(49, 's'); }
		Event_ProgramChange drum_highTom         () { return operator()(50, 's'); }
		Event_ProgramChange drum_rideCymbal1     () { return operator()(51, 's'); }
		Event_ProgramChange drum_chineseCymbal   () { return operator()(52, 's'); }
		Event_ProgramChange drum_rideBell        () { return operator()(53, 's'); }
		Event_ProgramChange drum_tambourine      () { return operator()(54, 's'); }
		Event_ProgramChange drum_splashCymbal    () { return operator()(55, 's'); }
		Event_ProgramChange drum_cowbell         () { return operator()(56, 's'); }
		Event_ProgramChange drum_crashCymbal2    () { return operator()(57, 's'); }
		Event_ProgramChange drum_vibraSlap       () { return operator()(58, 's'); }
		Event_ProgramChange drum_rideCymbal2     () { return operator()(59, 's'); }
		Event_ProgramChange drum_highBongo       () { return operator()(60, 's'); }
		Event_ProgramChange drum_lowBongo        () { return operator()(61, 's'); }
		Event_ProgramChange drum_muteHighConga   () { return operator()(62, 's'); }
		Event_ProgramChange drum_openHighConga   () { return operator()(63, 's'); }
		Event_ProgramChange drum_lowConga        () { return operator()(64, 's'); }
		Event_ProgramChange drum_highTimbale     () { return operator()(65, 's'); }
		Event_ProgramChange drum_lowTimbale      () { return operator()(66, 's'); }
		Event_ProgramChange drum_highAgogo       () { return operator()(67, 's'); }
		Event_ProgramChange drum_lowAgogo        () { return operator()(68, 's'); }
		Event_ProgramChange drum_cabasa          () { return operator()(69, 's'); }
		Event_ProgramChange drum_maracas         () { return operator()(70, 's'); }
		Event_ProgramChange drum_shortWhistle    () { return operator()(71, 's'); }
		Event_ProgramChange drum_longWhistle     () { return operator()(72, 's'); }
		Event_ProgramChange drum_shortGuiro      () { return operator()(73, 's'); }
		Event_ProgramChange drum_longGuiro       () { return operator()(74, 's'); }
		Event_ProgramChange drum_claves          () { return operator()(75, 's'); }
		Event_ProgramChange drum_highWoodBlock   () { return operator()(76, 's'); }
		Event_ProgramChange drum_lowWoodBlock    () { return operator()(77, 's'); }
		Event_ProgramChange drum_muteCuica       () { return operator()(78, 's'); }
		Event_ProgramChange drum_openCuica       () { return operator()(79, 's'); }
		Event_ProgramChange drum_muteTriangle    () { return operator()(80, 's'); }
		Event_ProgramChange drum_openTriangle    () { return operator()(81, 's'); }
		Event_ProgramChange drum_shaker          () { return operator()(82, 's'); } // added in General MIDI 2
		Event_ProgramChange drum_jingleBell      () { return operator()(83, 's'); } // added in General MIDI 2
		Event_ProgramChange drum_belltree        () { return operator()(84, 's'); } // added in General MIDI 2
		Event_ProgramChange drum_castanets       () { return operator()(85, 's'); } // added in General MIDI 2
		Event_ProgramChange drum_muteSurdo       () { return operator()(86, 's'); } // added in General MIDI 2
		Event_ProgramChange drum_openSurdo       () { return operator()(87, 's'); } // added in General MIDI 2
		
		// Get program change event from synonym instrument name (special channel 9)
		Event_ProgramChange drum_agogo      () { return drum_highAgogo       (); }
		Event_ProgramChange drum_bassDrum   () { return drum_acousticBassDrum(); }
		Event_ProgramChange drum_bongo      () { return drum_highBongo       (); }
		Event_ProgramChange drum_conga      () { return drum_openHighConga   (); }
		Event_ProgramChange drum_crashCymbal() { return drum_crashCymbal1    (); }
		Event_ProgramChange drum_cuica      () { return drum_openCuica       (); }
		Event_ProgramChange drum_guiro      () { return drum_longGuiro       (); }
		Event_ProgramChange drum_hiHat      () { return drum_pedalHiHat      (); }
		Event_ProgramChange drum_rideCymbal () { return drum_rideCymbal1     (); }
		Event_ProgramChange drum_snare      () { return drum_acousticSnare   (); }
		Event_ProgramChange drum_timbale    () { return drum_lowTimbale      (); }
		Event_ProgramChange drum_tom        () { return drum_lowMidTom       (); }
		Event_ProgramChange drum_triangle   () { return drum_openTriangle    (); }
		Event_ProgramChange drum_whistle    () { return drum_shortWhistle    (); }
		Event_ProgramChange drum_woodBlock  () { return drum_highWoodBlock   (); }
	
	protected:
		// Fields
		Pos pos_;
		UINT_4 channel_;
		bool forbidWrongChannel_;
	};

	typedef ProgramChangeEventsCreator Instrument;
}

#endif
