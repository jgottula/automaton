/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "obd/pid.h"


static const char _name_00[] PROGMEM = "";
static const char _name_01[] PROGMEM = "Monitor status";
static const char _name_02[] PROGMEM = "Freeze DTC";
static const char _name_03[] PROGMEM = "Fuel system status";
static const char _name_04[] PROGMEM = "Engine load value";
static const char _name_05[] PROGMEM = "Engine coolant temp";
static const char _name_06[] PROGMEM = "Short-term fuel trim (bank 1)";
static const char _name_07[] PROGMEM = "Long-term fuel trim (bank 1)";
static const char _name_08[] PROGMEM = "Short-term fuel trim (bank 2)";
static const char _name_09[] PROGMEM = "Long-term fuel trim (bank 2)";
static const char _name_0a[] PROGMEM = "Fuel pressure";
static const char _name_0b[] PROGMEM = "Intake manifold pressure";
static const char _name_0c[] PROGMEM = "Engine RPM";
static const char _name_0d[] PROGMEM = "Vehicle speed";
static const char _name_0e[] PROGMEM = "Timing advance";
static const char _name_0f[] PROGMEM = "Intake air temp";
static const char _name_10[] PROGMEM = "Mass air flow rate";
static const char _name_11[] PROGMEM = "Throttle position";
static const char _name_12[] PROGMEM = "Commanded secondary air status";
static const char _name_13[] PROGMEM = "Oxygen sensors present";
static const char _name_14[] PROGMEM = "O2 short-term trim (bank 1 sens 1)";
static const char _name_15[] PROGMEM = "O2 short-term trim (bank 1 sens 2)";
static const char _name_16[] PROGMEM = "O2 short-term trim (bank 1 sens 3)";
static const char _name_17[] PROGMEM = "O2 short-term trim (bank 1 sens 4)";
static const char _name_18[] PROGMEM = "O2 short-term trim (bank 2 sens 1)";
static const char _name_19[] PROGMEM = "O2 short-term trim (bank 2 sens 2)";
static const char _name_1a[] PROGMEM = "O2 short-term trim (bank 2 sens 3)";
static const char _name_1b[] PROGMEM = "O2 short-term trim (bank 2 sens 4)";
static const char _name_1c[] PROGMEM = "OBD standard conformance";
static const char _name_1d[] PROGMEM = "Oxygen sensors present";
static const char _name_1e[] PROGMEM = "Auxiliary input status";
static const char _name_1f[] PROGMEM = "Run time since engine start";
static const char _name_21[] PROGMEM = "Distance with MIL on";
static const char _name_22[] PROGMEM = "Fuel rail pressure";
static const char _name_23[] PROGMEM = "Fuel rail pressure";
static const char _name_24[] PROGMEM = "O2S1 WR lambda (voltage)";
static const char _name_25[] PROGMEM = "O2S2 WR lambda (voltage)";
static const char _name_26[] PROGMEM = "O2S3 WR lambda (voltage)";
static const char _name_27[] PROGMEM = "O2S4 WR lambda (voltage)";
static const char _name_28[] PROGMEM = "O2S5 WR lambda (voltage)";
static const char _name_29[] PROGMEM = "O2S6 WR lambda (voltage)";
static const char _name_2a[] PROGMEM = "O2S7 WR lambda (voltage)";
static const char _name_2b[] PROGMEM = "O2S8 WR lambda (voltage)";
static const char _name_2c[] PROGMEM = "Commanded EGR";
static const char _name_2d[] PROGMEM = "EGR error";
static const char _name_2e[] PROGMEM = "Commanded evaporative purge";
static const char _name_2f[] PROGMEM = "Fuel level input";
static const char _name_30[] PROGMEM = "Warm-ups since codes cleared";
static const char _name_31[] PROGMEM = "Distance since codes cleared";
static const char _name_32[] PROGMEM = "Evap system vapor pressure";
static const char _name_33[] PROGMEM = "Barometric pressure";
static const char _name_34[] PROGMEM = "O2S1 WR lambda (current)";
static const char _name_35[] PROGMEM = "O2S2 WR lambda (current)";
static const char _name_36[] PROGMEM = "O2S3 WR lambda (current)";
static const char _name_37[] PROGMEM = "O2S4 WR lambda (current)";
static const char _name_38[] PROGMEM = "O2S5 WR lambda (current)";
static const char _name_39[] PROGMEM = "O2S6 WR lambda (current)";
static const char _name_3a[] PROGMEM = "O2S7 WR lambda (current)";
static const char _name_3b[] PROGMEM = "O2S8 WR lambda (current)";
static const char _name_3c[] PROGMEM = "Catalyst temp (bank 1 sens 1)";
static const char _name_3d[] PROGMEM = "Catalyst temp (bank 2 sens 1)";
static const char _name_3e[] PROGMEM = "Catalyst temp (bank 1 sens 2)";
static const char _name_3f[] PROGMEM = "Catalyst temp (bank 2 sens 2)";
static const char _name_41[] PROGMEM = "Monitor status this drive cycle";
static const char _name_42[] PROGMEM = "Control module voltage";
static const char _name_43[] PROGMEM = "Absolute load value";
static const char _name_44[] PROGMEM = "Command equivalence ratio";
static const char _name_45[] PROGMEM = "Relative throttle position";
static const char _name_46[] PROGMEM = "Ambient air temperature";
static const char _name_47[] PROGMEM = "Absolute throttle position B";
static const char _name_48[] PROGMEM = "Absolute throttle position C";
static const char _name_49[] PROGMEM = "Accelerator pedal position D";
static const char _name_4a[] PROGMEM = "Accelerator pedal position E";
static const char _name_4b[] PROGMEM = "Accelerator pedal position F";
static const char _name_4c[] PROGMEM = "Commanded throttle actuator";
static const char _name_4d[] PROGMEM = "Time run with MIL on";
static const char _name_4e[] PROGMEM = "Time since trouble codes cleared";
static const char _name_4f[] PROGMEM = "Max value for equiv, O2, pressure";
static const char _name_50[] PROGMEM = "Max value for MAF sensor";
static const char _name_51[] PROGMEM = "Fuel type";
static const char _name_52[] PROGMEM = "Ethanol fuel ratio";
static const char _name_53[] PROGMEM = "Absolute evap system vapor pressure";
static const char _name_54[] PROGMEM = "Evap system vapor pressure";
static const char _name_55[] PROGMEM = "Secondary short trim (bank 1+3)";
static const char _name_56[] PROGMEM = "Secondary long trim (bank 1+3)";
static const char _name_57[] PROGMEM = "Secondary short trim (bank 2+4)";
static const char _name_58[] PROGMEM = "Secondary long trim (bank 2+4)";
static const char _name_59[] PROGMEM = "Fuel rail pressure";
static const char _name_5a[] PROGMEM = "Relative accelerator pedal position";
static const char _name_5b[] PROGMEM = "Hybrid battery pack remaining life";
static const char _name_5c[] PROGMEM = "Engine oil temp";
static const char _name_5d[] PROGMEM = "Fuel injection timing";
static const char _name_5e[] PROGMEM = "Engine fuel rate";
static const char _name_5f[] PROGMEM = "Emission requirement compliance";


const char *const obd_pid_names[] PROGMEM = {
	_name_00,
	// TODO
};
