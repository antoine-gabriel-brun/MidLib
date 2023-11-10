#ifndef MID_LIB_HPP
#define MID_LIB_HPP

/* Including this file includes all MID library files. */

// Sample: Main library class
#include "types/containers/Sample.hpp"

// Unit conversions
#include "types/tools/Convert.hpp"

// Error types
#include "types/tools/Exceptions.hpp"

// Event creators
#include "types/event-creators/ev_NoteOnOffEventsCreator.hpp"     // Note
#include "types/event-creators/ev_PitchBendEventsCreator.hpp"     // PitchBend
#include "types/event-creators/ev_ProgramChangeEventsCreator.hpp" // Instrument
#include "types/event-creators/ev_AftertouchEventsCreator.hpp"    // Aftertouch
#include "types/event-creators/ev_ControlChangeEventCreators.hpp" // Standard control changes

// Sample operators
#include "types/sample-operators/SampleOperators.hpp"

// Events that users should not use directly
#include "types/event-types/ev_Tempo.hpp"
#include "types/event-types/ev_TimeSignature.hpp"
#include "types/event-types/ev_EndOfTrack.hpp"

#endif
