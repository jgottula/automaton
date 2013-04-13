/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "lib/FatFs/diskio.h"


DWORD get_fattime(void) {
	// TODO
	return 0;
}


DSTATUS disk_initialize(BYTE pdrv) {
	if (pdrv == 0) {
		// TODO
	}
	
	return disk_status(pdrv);
}

DSTATUS disk_status(BYTE pdrv) {
	if (pdrv == 0) {
		// TODO
	} else {
		return STA_NOINIT | STA_NODISK;
	}
}

DRESULT disk_read(BYTE pdrv, BYTE *buf, DWORD sector, BYTE count) {
	// TODO
}

DRESULT disk_write(BYTE pdrv, const BYTE *buf, DWORD sector, BYTE count) {
	// TODO
}

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buf) {
	switch (cmd) {
	case CTRL_SYNC:
		// TODO
	case GET_SECTOR_COUNT:
		// TODO
	case GET_SECTOR_SIZE:
		// TODO
	case GET_BLOCK_SIZE:
		// TODO
	case CTRL_ERASE_SECTOR:
		// TODO
	default:
		return RES_PARERR;
	}
}
