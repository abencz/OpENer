/*******************************************************************************
 * (c) Copyright 2009-2010 Actel Corporation.  All rights reserved.
 *
 * SmartFusion Microcontroller Subsystem ENVM bare metal software driver public API.
 *
 * SVN $Revision: 2825 $
 * SVN $Date: 2010-07-15 12:50:39 +0100 (Thu, 15 Jul 2010) $
 */
#ifndef MSS_NVM_H_
#define MSS_NVM_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif 

/***************************************************************************//**
 * Possible return values from function NVM_write().
 */
typedef enum nvm_status
{
    NVM_SUCCESS = 0,
    NVM_PROTECTION_ERROR,
    NVM_WRITE_ERROR,
    NVM_INVALID_ADDRESS
} nvm_status_t;

/***************************************************************************//**
 * This function writes the content of the buffer passed as parameter to Fusion
 * NVM. The data is written from the memory location specified by the first
 * parameter. This address is the absolute address in the processor's memory
 * space at which the NVM is located.
 *
 * @param start_addr    This is the address at which data willbe written.
 *                      This address is the absolute address in the processor's
 *                      memory space at which the NVM is located.
 * @param p_data        This is a pointer to the buffer holding the data to be
 *                      written into NVM.
 * @param nb_bytes      This is the number of bytes to be written into NVM.
 * @return              The return value indicates if the write was successful.
 *                      Possible values are:
 *                          - NVM_SUCCESS
 *                          - NVM_PROTECTION_ERROR
 *                          - NVM_WRITE_ERROR
 *                          - NVM_INVALID_ADDRESS
 */
nvm_status_t
NVM_write
(
 	uint32_t start_addr,
 	const uint8_t * p_data,
 	size_t nb_bytes
);

#ifdef __cplusplus
}
#endif

#endif /*MSS_NVM_H_*/
