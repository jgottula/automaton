/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "conv/unit.h"


/* convert km/hr (x10) to m/s (x10) */
uint16_t conv_km_hr_to_m_s(uint16_t km_hr) {
	/* divide by 3.6 (multiply by 5/18) */
	return (km_hr * 5) / 18;
}

/* convert km (x10) to mi (x10) */
uint16_t conv_km_to_mi(uint16_t km) {
	/* divide by 1.60934 (multiply by 64/103) */
	return (km * 64) / 103;
}
