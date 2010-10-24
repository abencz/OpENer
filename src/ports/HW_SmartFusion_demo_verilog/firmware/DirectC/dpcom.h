/* ************************************************************************ */
/*                                                                          */
/*  IAP 	       Copyright (C) Actel Corporation 2010                 */
/*  Version 1.0	       Release date February  17, 2010                      */
/*                                                                          */
/* ************************************************************************ */
/*                                                                          */
/*  Module:         dpcom.h                                                 */
/*                                                                          */
/*  Description:    Contains functions prototypes needed to access the data */
/*	from external flash or other means of communication		    */
/*                                                                          */
/* ************************************************************************ */

#ifndef INC_DPCOM_H
#define INC_DPCOM_H

extern DPULONG return_bytes;
extern DPULONG requested_bytes;
extern DPULONG image_size;
extern DPUCHAR * page_buffer_ptr;
extern uint32_t ADDR_OFFSET;
/* user attention is required.  PAGE_BUFFER_SIZE needs to be specified in bytes */
#define PAGE_BUFFER_SIZE 8192
#define F2DSS_EMC_BASE_ADDR 0x70000000

/* 
* Data block ID definitions
*/
#define Header_ID		        0U
#define ACT_UROW_DESIGN_NAME_ID         1U
#define BsrPattern_ID 			2U  
#define SILSIG_ID 			3U		
#define CHECKSUM_ID			4U
#define datastream_ID 			5U
#define rlock_ID 			6U
#define FRomAddressMask_ID 		7U
#define FRomStream_ID 	    	        8U

/* These defintions are the same as NVM block zoro.  They are defined to aviod confusion when pogram and verify NVM functions are called. */
#define NVM_OFFSET		        5U
#define NvmParSize_ID 			9U 
#define NumOfPart_ID 			10U
#define NvmAddr_ID 			11U
#define NvmData_ID 			12U
#define NvmProtect_ID 			13U


#define NvmParSize_0_ID 		9U 
#define NumOfPart_0_ID 			10U
#define NvmAddr_0_ID 			11U
#define NvmData_0_ID 			12U
#define NvmProtect_0_ID 		13U
#define NvmParSize_1_ID 		14U
#define NumOfPart_1_ID 			15U
#define NvmAddr_1_ID 			16U
#define NvmData_1_ID 			17U
#define NvmProtect_1_ID 		18U
#define NvmParSize_2_ID 		19U
#define NumOfPart_2_ID 			20U
#define NvmAddr_2_ID 			21U
#define NvmData_2_ID 			22U
#define NvmProtect_2_ID 		23U
#define NvmParSize_3_ID 		24U
#define NumOfPart_3_ID 			25U
#define NvmAddr_3_ID 			26U
#define NvmData_3_ID 			27U
#define NvmProtect_3_ID 		28U
#define UKEY_ID				29U
#define DMK_ID				30U
#define KDATA_ID			31U
#define ULOCK_ID			32U

/*
* Location of special variables needed in the header section of the image file
*/
#define BTYES_PER_TABLE_RECORD	        9U
#define ACTEL_HEADER_SIZE		24U
#define HEADER_SIZE_OFFSET		24U
#define IMAGE_SIZE_OFFSET		25U
#define MIN_IMAGE_SIZE			56U
#define M_DEVICE_OFFSET			30U
#define ID_OFFSET			31U

#ifndef ENABLE_V85_DAT_SUPPORT
#define DEVICE_FAMILY_OFFSET            55U
#define DEVICE_NAME_OFFSET              56U
#define ID_MASK_OFFSET                  57U
#define SD_TILES_OFFSET                 61U
#define MAP_ROWS_OFFSET                 62U
#define BSR_LENGTH_OFFSET               64U
#define SE_WAIT_OFFSET                  66U
#define DUAL_KEY_SUPPORT_OFFSET         67U
#endif

#define JTAG_EXIT2_DR 0
#define JTAG_EXIT1_DR 1
#define JTAG_SHIFT_DR 2 
#define JTAG_PAUSE_DR 3
#define JTAG_SELECT_IR 4
#define JTAG_UPDATE_DR 5
#define JTAG_CAPTURE_DR 6
#define JTAG_SELECT_DR 7
#define JTAG_EXIT2_IR 8
#define JTAG_EXIT1_IR 9
#define JTAG_SHIFT_IR 10
#define JTAG_PAUSE_IR 11
#define JTAG_RUN_TEST_IDLE 12
#define JTAG_UPDATE_IR 13
#define JTAG_CAPTURE_IR 14
#define JTAG_TEST_LOGIC_RESET 15

DPUCHAR * dp_get_data(DPUCHAR var_ID,DPULONG bit_index);
DPUCHAR * dp_get_header_data(DPULONG bit_index);
void dp_get_page_data(DPULONG image_requested_address);
void dp_get_data_block_address(DPUCHAR requested_var_ID);
DPUCHAR * dp_get_data_block_element_address(DPULONG bit_index);
DPULONG dp_get_bytes(DPUCHAR var_ID,DPULONG byte_index,DPUCHAR requested_bytes);
DPULONG dp_get_header_bytes(DPULONG byte_index,DPUCHAR requested_bytes);
void dp_get_and_DRSCAN_in(unsigned char Variable_ID,unsigned int total_bits_to_shift, unsigned long start_bit_index);
void dp_get_and_DRSCAN_in_out(unsigned char Variable_ID,unsigned int total_bits_to_shift, unsigned long start_bit_index, unsigned char *tdo_data);
#endif /* INC_DPCOM_H */


