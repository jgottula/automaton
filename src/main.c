/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "std.h"
#include "io.h"


noreturn void main(void) {
	io_write(PORTB, 0xaa, 0xff);
	
	
	while (true);
}
