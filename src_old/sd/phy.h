/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_SD_PHY_H
#define AUTOMATON_SD_PHY_H


#include "std.h"


enum sd_cmd {
	SD_CMD_GO_IDLE_STATE           = 0,  // R1
	SD_CMD_SEND_OP_COND            = 1,  // R1
	SD_CMD_SWITCH_FUNC             = 6,  // R1
	SD_CMD_SEND_IF_COND            = 8,  // R7
	SD_CMD_SEND_CSD                = 9,  // R1
	SD_CMD_SEND_CID                = 10, // R1
	SD_CMD_STOP_TRANSMISSION       = 12, // R1b
	SD_CMD_SEND_STATUS             = 13, // R2
	SD_CMD_SET_BLOCKLEN            = 16, // R1
	SD_CMD_READ_SINGLE_BLOCK       = 17, // R1
	SD_CMD_READ_MULTIPLE_BLOCK     = 18, // R1
	SD_CMD_WRITE_BLOCK             = 24, // R1*
	SD_CMD_WRITE_MULTIPLE_BLOCK    = 25, // R1
	SD_CMD_PROGRAM_CSD             = 27, // R1
	SD_CMD_SET_WRITE_PROT          = 28, // R1b
	SD_CMD_CLR_WRITE_PROT          = 29, // R1b
	SD_CMD_SEND_WRITE_PROT         = 30, // R1
	SD_CMD_ERASE_WR_BLK_START_ADDR = 32, // R1
	SD_CMD_ERASE_WR_BLK_END_ADDR   = 33, // R1
	SD_CMD_ERASE                   = 38, // R1b
	SD_CMD_LOCK_UNLOCK             = 42, // R1
	SD_CMD_APP_CMD                 = 55, // R1
	SD_CMD_GEN_CMD                 = 56, // R1
	SD_CMD_READ_OCR                = 58, // R3
	SD_CMD_CRC_ON_OFF              = 59, // R1
	
	SD_ACMD_SD_STATUS              = 13, // R2
	SD_ACMD_SEND_NUM_WR_BLOCKS     = 22, // R1
	SD_ACMD_SET_WR_BLK_ERASE_COUNT = 23, // R1
	SD_ACMD_SEND_OP_COND           = 41, // R1
	SD_ACMD_SET_CLR_CARD_DETECT    = 42, // R1
	SD_ACMD_SEND_SCR               = 51, // R1
};

enum sd_r1_bit {
	SD_R1_PARAM_ERR     = _BV(6),
	SD_R1_ADDR_ERR      = _BV(5),
	SD_R1_ERASE_SEQ_ERR = _BV(4),
	SD_R1_COM_CRC_ERR   = _BV(3),
	SD_R1_ILLEGAL_CMD   = _BV(2),
	SD_R1_ERASE_RESET   = _BV(1),
	SD_R1_IDLE_STATE    = _BV(0),
};

enum sd_r2_bit {
	SD_R2_OUT_OF_RANGE     = _BV(7),
	SD_R2_CSD_OVERWRITE    = _BV(7),
	SD_R2_ERASE_PARAM      = _BV(6),
	SD_R2_WP_VIOLATION     = _BV(5),
	SD_R2_CARD_ECC_FAILED  = _BV(4),
	SD_R2_CC_ERROR         = _BV(3),
	SD_R2_ERROR            = _BV(2),
	SD_R2_WP_ERASE_SKIP    = _BV(1),
	SD_R2_LOCK_UNLOCK_FAIL = _BV(1),
	SD_R2_CARD_LOCKED      = _BV(0),
};


typedef uint8_t sd_r1;
typedef struct {
	sd_r1 r1;
	uint8_t r2;
} sd_r2;
typedef struct {
	sd_r1 r1;
	uint32_t ocr;
} sd_r3;
typedef struct {
	sd_r1 r1;
	uint32_t bits;
} sd_r7;


void sd_phy_init(void);

bool sd_phy_card_present(void);


#endif
