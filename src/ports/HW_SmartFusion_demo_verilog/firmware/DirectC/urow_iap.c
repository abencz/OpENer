/* ************************************************************************ */
/*                                                                          */
/*  IAP 	       Copyright (C) Actel Corporation 2010                 */
/*  Version 1.0	       Release date February  17, 2010                      */
/*                                                                          */
/* ************************************************************************ */
/*                                                                          */
/*  Module:         urow_iap.h                                              */
/*                                                                          */
/*  Description:    Contains the function prototypes.                       */
/*                                                                          */
/* ************************************************************************ */

#include "a2fxxxm3.h"
#include "dpuser.h"
#include "dpcom.h"
#include "dpalg.h"
#include "dpcore.h"
#include "dpsecurity.h"
#include "dpdef.h"
#include "dputil.h"

#define NVM0_BUSY		        0x00000001uL

#define F2DSS_ENVM_BASE_UROW_ADDR       (uint32_t)0x60081E70
#define F2DSS_ENVM_BASE_UROW_CRC_ADDR   (uint32_t)0x60081E6E
#define F2DSS_ENVM_UROW_ADDR_OFFSET     0x81E70
#define F2DSS_ENVM_UROW_CRC_ADDR_OFFSET 0x81E6E

DPUCHAR actual_iap_urow[UROW_BYTE_LENGTH];
DPUCHAR expected_iap_urow[UROW_BYTE_LENGTH];
DPUINT expected_iap_urow_crc;
DPUINT actual_iap_urow_crc;
DPUINT tmp_iap_urow_crc;


/* Data structure of data stored in eNVM by IAP programming algorithm. */
typedef struct
{
    __IO uint8_t urow[UROW_BYTE_LENGTH];
} IAP_DATA_ENVM_TypeDef;

IAP_DATA_ENVM_TypeDef * g_iap_data = (IAP_DATA_ENVM_TypeDef *)F2DSS_ENVM_BASE_UROW_ADDR;
IAP_DATA_ENVM_TypeDef * g_iap_crc = (IAP_DATA_ENVM_TypeDef *)F2DSS_ENVM_BASE_UROW_CRC_ADDR;

void init_urow_nvm(void)
{
    NVIC_ClearPendingIRQ( ENVM0_IRQn );
    NVIC_EnableIRQ(ENVM0_IRQn );
    ENVM_REGS->ENABLE = 0x00000008; //Erase Error
    SYSREG->ENVM_CR = 0x92;  //PIPELINE(bit 6),SIXCYCLE(bit7)
    
    return;
}

void check_envm_status(void)
{
    uint32_t status;
    do {
        status = ENVM_REGS->STATUS;
    } while ((status & NVM0_BUSY));
}

void do_program_urow_nvm(void)
{
    /* writing data into envm */
    for (ADDR_OFFSET = 0;ADDR_OFFSET < UROW_BYTE_LENGTH; ADDR_OFFSET++)
    {
        check_envm_status();
        g_iap_data->urow[ADDR_OFFSET] = expected_iap_urow[ADDR_OFFSET];
    }
    
    /* Issue program command */ 
    check_envm_status();
    ADDR_OFFSET = 0;
    ENVM_REGS->CONTROL = (uint32_t) ( (0x10000000  + F2DSS_ENVM_UROW_ADDR_OFFSET ));
    check_envm_status();
    
    
    return;
}

void do_program_urow_crc_nvm(void)
{
    /* writing data into envm */
    for (ADDR_OFFSET = 0u; ADDR_OFFSET < 2u; ADDR_OFFSET++)
    {
        check_envm_status();
        g_iap_crc->urow[ADDR_OFFSET] = tmp_iap_urow_crc >> ADDR_OFFSET*8u;
    }
    
    /* Issue program command */ 
    check_envm_status();
    ADDR_OFFSET = 0;
    ENVM_REGS->CONTROL = (uint32_t) ( (0x10000000  + F2DSS_ENVM_UROW_CRC_ADDR_OFFSET ));
    check_envm_status();
    
    
    return;
}


void do_read_urow_nvm(void)
{
    for (ADDR_OFFSET = 0u; ADDR_OFFSET < UROW_BYTE_LENGTH; ADDR_OFFSET++)
    {
        actual_iap_urow[ADDR_OFFSET] = g_iap_data->urow[ADDR_OFFSET];       
    }
    return;
}

void do_read_urow_crc_nvm(void)
{
    actual_iap_urow_crc = 0u;
    for (ADDR_OFFSET = 0u; ADDR_OFFSET < 2u; ADDR_OFFSET++)
    {
        actual_iap_urow_crc |= (g_iap_crc->urow[ADDR_OFFSET]) << (ADDR_OFFSET*8);
    }
    return;
}


void dp_compute_urow_crc(DPUCHAR value)
{
    DPUINT idx, tmp;
    
    for (idx = 0; idx < 8; idx++)
    {
        tmp = (value ^ tmp_iap_urow_crc) & 0x01;
        tmp_iap_urow_crc >>= 1;
        if (tmp)
            tmp_iap_urow_crc ^= 0x8408;
        value >>= 1;
    }
    return;
}

void read_urow_nvm(void)
{
    DPUCHAR index;
    init_urow_nvm();
    do_read_urow_nvm();
    do_read_urow_crc_nvm();
    
    tmp_iap_urow_crc = 0;
    for (index = 0;index < UROW_BYTE_LENGTH; index++)
    {
        dp_compute_urow_crc(actual_iap_urow[index]);
    }
    
    expected_iap_urow_crc = tmp_iap_urow_crc;
    if (actual_iap_urow_crc == expected_iap_urow_crc)
    {
        cycle_count =  ((DPUINT)actual_iap_urow[12] >> 6U) | ((DPUINT)actual_iap_urow[13] << 2U);
        #ifdef ENABLE_DEBUG
        dp_display_text("\n\rIAP Urow: ");
        dp_display_array(actual_iap_urow,UROW_BYTE_LENGTH,HEX);
        #endif
    }
    else 
    {
        cycle_count = 0u;
    }
    
    return;
}


void program_urow_nvm(void)
{
    DPUCHAR index;
    
    read_urow_nvm();
    
    #ifdef ENABLE_DEBUG
    dp_display_text("\r\nProgramming IAP UROW...");
    #endif
    
    for(index = 0u; index < 18u;index++)
    {
        expected_iap_urow[index] = 0u;
    }
    
    if ((device_security_flags & IS_ERASE_ONLY) == 0U)
    {
        #ifdef CORE_SUPPORT
        cycle_count++;
        if (cycle_count == 1023U)
        {
            cycle_count = 0U;
        }
        else if (cycle_count == 1024U)
        {
            cycle_count = 1U;
        }
        else
        {
        }
        #endif
    }
    
    if ((device_security_flags & PERM_LOCK_BIT) && (device_security_flags & ULAWE))
    {
        device_security_flags |= IS_RESTORE_DESIGN;
    }
    if (((device_security_flags & IS_ERASE_ONLY) == 0U) || 
    (device_security_flags & IS_RESTORE_DESIGN))
    {
        /* Constucting the UROW data */
        if ((device_security_flags & IS_RESTORE_DESIGN) == 0U)
        {
            for (index=0U; index < 2U; index++)
            {
                expected_iap_urow[index+14U] = (DPUCHAR) dp_get_bytes(CHECKSUM_ID,index,1U);
            }
            for (index=0U; index < 9U; index++)
            {
                expected_iap_urow[index + 4U] = (DPUCHAR) dp_get_bytes(ACT_UROW_DESIGN_NAME_ID,index,1U);
            }
        }
    }
    
    expected_iap_urow[12] |= ((DPUCHAR) (cycle_count << 6));
    expected_iap_urow[13] = (DPUCHAR) (cycle_count >> 2);
    expected_iap_urow[3] |= IAP_PROGRAMMING_METHOD << 5;
    expected_iap_urow[3] |= (ALGO_VERSION & 0xfu) << 1u;
    if (ALGO_VERSION & 0x40)
    {
        expected_iap_urow[0] |= 0x20;
    }
    else
    {
        expected_iap_urow[0] |= 0x1;
    }
    
    expected_iap_urow[2] |= ((ALGO_VERSION >> 4u) & 1u) << 7u;
    expected_iap_urow[3] |= ((ALGO_VERSION >> 5u) & 1u);
    expected_iap_urow[0] |= DC_PROGRAMMING_SW << 6u;
    expected_iap_urow[1] |= (DC_PROGRAMMING_SW >> 2u) & 0x3u;
    expected_iap_urow[1] |= DC_SOFTWARE_VERSION << 2u;
    expected_iap_urow[2] |= (DC_SOFTWARE_VERSION >> 6u) & 0x1;
    
    tmp_iap_urow_crc = 0;
    for (index = 0;index < UROW_BYTE_LENGTH; index++)
    {
        dp_compute_urow_crc(expected_iap_urow[index]);
    }
    expected_iap_urow_crc = tmp_iap_urow_crc;
    
    do_program_urow_nvm();
    do_program_urow_crc_nvm();
    
    /* readback and verify UROW */
    do_read_urow_nvm();
    do_read_urow_crc_nvm();
    
    for ( index = 0U; index < UROW_BYTE_LENGTH; index++ )
    {
        if (actual_iap_urow[index] != expected_iap_urow[index])
        {
            error_code = DPE_PROGRAM_UROW_ERROR;
            break;
        }
    }
    if (expected_iap_urow_crc != actual_iap_urow_crc)
    {
        error_code = DPE_PROGRAM_UROW_ERROR;
    }
    
    return;
}


#ifdef ENABLE_DEBUG
void dp_display_urow_nvm(void)
{
    DPUCHAR algo_version;
    DPUCHAR programmer;
    DPUCHAR programming_method;
    DPUCHAR software_version;
    
    algo_version = (actual_iap_urow[3] >> 1) & 0xfu;
    if ((actual_iap_urow[0] & 0x1u) != (actual_iap_urow[0] & 0x20u) >> 5u)
    {
        algo_version |= ((actual_iap_urow[2] >> 7u ) & 0x1) << 4u;
        algo_version |= (actual_iap_urow[3] & 0x1u) << 5u;
        algo_version |= ((actual_iap_urow[0] >> 5u) & 0x1u) << 6u;
    }
    programming_method = (actual_iap_urow[3] >> 5u) & 0x7u;
    programmer = ((actual_iap_urow[0] >> 6u) & 0x3u) | ((actual_iap_urow[1] & 0x3u) << 0x2u);
    software_version = (actual_iap_urow[1] >> 2u) | (actual_iap_urow[2] & 0x1 << 6u);
    
    dp_display_text("\r\nUser information: ");
    dp_display_text("\r\nCYCLE COUNT: ");
    dp_display_value(cycle_count,DEC);
    dp_display_text("\r\nCHECKSUM = ");
    dp_display_array(&actual_iap_urow[14],2u, HEX);
    dp_display_text("\r\nDesign Name = ");
    dp_display_array(&actual_iap_urow[4],9u, HEX);
    
    dp_display_text("\r\nProgramming Method: ");
    switch(programming_method)
    {
        case IEEE1532_PM:
        dp_display_text("IEEE1532");
        break;
        case STAPL_PM:
        dp_display_text("STAPL");
        break;
        case DIRECTC_PM:
        dp_display_text("DIRECTC");
        break;
        case PDB_PM:
        dp_display_text("PDB");
        break;
        case SVF_PM:
        dp_display_text("SVF");
        break;
        case IAP_PM:
        dp_display_text("IAP");
        break;
        default:
        dp_display_text("Unknown");
        break;
    };
    dp_display_text("\r\nAlgorithm Version: = ");
    dp_display_value(algo_version, DEC);
    dp_display_text("\r\nProgrammer: = ");
    dp_display_value(algo_version, DEC);
    
    dp_display_text("\r\nProgrammmer: ");
    switch(programmer)
    {
        case FP:
        dp_display_text("Flash Pro");
        break;
        case FPLITE:
        dp_display_text("Flash Pro Lite");
        break;
        case FP3:
        dp_display_text("FP3");
        break;
        case SCULPTW:
        dp_display_text("Sculptor");
        break;
        case BPW:
        dp_display_text("BP Programmer");
        break;
        case DIRECTCP:
        dp_display_text("DirectC");
        break;
        case STP:
        dp_display_text("STAPL");
        break;
        case FP4:
        dp_display_text("FP4");
        break;
        default:
        dp_display_text("Unknown");
        break;
    };
    
    dp_display_text("\r\nSoftware Version = ");
    dp_display_value(software_version, DEC);
    
    dp_display_text("\r\n==================================================");
    return;
}
#endif
