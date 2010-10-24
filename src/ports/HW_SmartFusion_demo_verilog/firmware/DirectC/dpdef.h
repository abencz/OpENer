/* ************************************************************************ */
/*                                                                          */
/*  IAP 	       Copyright (C) Actel Corporation 2010                 */
/*  Version 1.0	       Release date February  17, 2010                      */
/*                                                                          */
/* ************************************************************************ */
/*                                                                          */
/*  Module:         dpdef.h                                                 */
/*                                                                          */
/*  Description:    This file contains error code definitions		    */
/*	and special character definitions				    */
/*                                                                          */
/* ************************************************************************ */

#ifndef INC_DPDEFINE_H
#define INC_DPDEFINE_H

/************************************************************/
/* Erase Bits Definitions									*/
/************************************************************/
#define CORE_ERASE_BITS_BYTE0		0x1U		/*Bit 0 */
#define CORE_ERASE_BITS_BYTE1		0x0U		/*Bit 0 */
#define CORE_ERASE_BITS_BYTE2		0x0U		/*Bit 0 */
#define ULOCK_ERASE_BITS_BYTE0		0x2U		/*Bit 1 */
#define ULOCK_ERASE_BITS_BYTE1		0x0U		/*Bit 1 */
#define ULOCK_ERASE_BITS_BYTE2		0x0U		/*Bit 1 */
#define DMK_ERASE_BITS_BYTE0		0x4U		/*Bit 2 */
#define DMK_ERASE_BITS_BYTE1		0x0U		/*Bit 2 */
#define DMK_ERASE_BITS_BYTE2		0x0U		/*Bit 2 */
#define UKEY_ERASE_BITS_BYTE0		0x8U		/*Bit 3 */
#define UKEY_ERASE_BITS_BYTE1		0x0U		/*Bit 3 */
#define UKEY_ERASE_BITS_BYTE2		0x0U		/*Bit 3 */
#define FLOCK_ERASE_BITS_BYTE0		0x10U	    /*Bit 4 */
#define FLOCK_ERASE_BITS_BYTE1		0x0U		/*Bit 4 */
#define FLOCK_ERASE_BITS_BYTE2		0x0U		/*Bit 4 */
#define FPRM_ERASE_BITS_BYTE0		0xE0U	    /*Bits 5-10*/
#define FPRM_ERASE_BITS_BYTE1		0x7U		/*Bits 5-10*/
#define FPRM_ERASE_BITS_BYTE2		0x0U		/*Bits 5-10*/
#define FKEY_ERASE_BITS_BYTE0		0x0U		/*Bit 11*/
#define FKEY_ERASE_BITS_BYTE1		0x8U		/*Bit 11*/
#define FKEY_ERASE_BITS_BYTE2		0x0U		/*Bit 11*/
#define SECEN_ERASE_BITS_BYTE0		0x0U		/*Bit 12*/
#define SECEN_ERASE_BITS_BYTE1		0x10U       /*Bit 12*/
#define SECEN_ERASE_BITS_BYTE2		0x0U		/*Bit 12*/
#define VIRREF_ERASE_BITS_BYTE0		0x0U		/*Bit 13*/
#define VIRREF_ERASE_BITS_BYTE1		0x20U	    /*Bit 13*/
#define VIRREF_ERASE_BITS_BYTE2		0x0U		/*Bit 13*/
#define UROW_ERASE_BITS_BYTE0		0x0U		/*Bit 14*/
#define UROW_ERASE_BITS_BYTE1		0x40U	    /*Bit 14*/
#define UROW_ERASE_BITS_BYTE2		0x0U		/*Bit 14*/
#define FROM_ERASE_BITS_BYTE0		0x0U		/*Bit 14*/
#define FROM_ERASE_BITS_BYTE1		0x80U	    /*Bit 14*/
#define FORM_ERASE_BITS_BYTE2		0x07FU	    /*Bit 14*/

/************************************************************/
/* Error code definitions				    */
/************************************************************/
#define DPE_SUCCESS                 0U
#define DPE_IDCODE_ERROR            6U
#define DPE_CHECKSUM_ERROR          9U
#define DPE_INIT_FAILURE	    25U
#define DPE_ERASE_ERROR		    8U
#define DPE_CORE_PROGRAM_ERROR	    10U
#define DPE_CORE_VERIFY_ERROR	    11U
#define DPE_PROGRAM_RLOCK_ERROR     10U
#define DPE_POLL_ERROR		    7U
#define DPE_PROGRAM_UROW_ERROR      24U
#define DPE_FROM_VERIFY_ERROR       20U
#define DPE_FROM_PROGRAM_ERROR      10U
#define DPE_DMK_VERIFY_ERROR        31U
#define DPE_UROW_ERROR              11U
#define DPE_VERIFY_ERROR	    12U
#define DPE_NVM_PROGRAM_ERROR       41U
#define DPE_NVM_VERIFY_ERROR	    39U
#define DPE_SEC_PROGRAMMING_ERROR   22U
#define DPE_AES_PROGRAMMING_ERROR   23U
#define DPE_ULOCK_ERROR             16U
#define DPE_MATCH_ERROR             35U
#define DPE_AUTHENTICATION_FAILURE  18U
#define DPE_CORE_ENC_ERROR          33U
#define DPE_CORE_PLAIN_ERROR        37U
#define DPE_FROM_ENC_ERROR          34U
#define DPE_FROM_PLAIN_ERROR        36U
#define DPE_NVM0_ENC_ERROR          47U
#define DPE_NVM0_PLAIN_ERROR        49U
#define DPE_NVM1_ENC_ERROR          47U
#define DPE_NVM1_PLAIN_ERROR        49U
#define DPE_NVM2_ENC_ERROR          47U
#define DPE_NVM2_PLAIN_ERROR        49U
#define DPE_NVM3_ENC_ERROR          47U
#define DPE_NVM3_PLAIN_ERROR        49U
#define DPE_USER_LOCK_SETTING_ERROR 42U
#define DPE_UROW_SETTING_ERROR      43U
#define DPE_AES_SEC_ERROR           31U
#define DPE_SILSIG_PROGRAM_ERROR    14U
#define DPE_PROGRAM_SECURITY_ERROR  38U
#define DPE_NVM_ACCESS_ERROR        54U
#define DPE_CRC_MISMATCH	        100U
#define DPE_JTAG_STATE_NOT_HANDLED  110U
#define DPE_ACTION_NOT_FOUND        150U
#define DPE_ACTION_NOT_SUPPORTED    151U
#define DPE_CODE_NOT_ENABLED        152U
#define CALIBRATION_OVERLAP_ERROR   153U
#define DPE_SECURITY_BIT_MISMATCH   154U
#define DPE_DAT_VERSION_MISMATCH    160U
#define DPE_HARDWARE_NOT_SELECTED   170U
/************************************************************/
/* Block Support status bits				    */
/************************************************************/
#define DAT_SUPPORT_STATUS_OFFSET   39U
#define CORE_DAT_SUPPORT_BIT        0x0001U
#define FROM_DAT_SUPPORT_BIT	    0x0002U
#define NVM_DAT_SUPPORT_BIT	    0x0004U 
#define NVM0_DAT_SUPPORT_BIT	    0x0008U
#define NVM1_DAT_SUPPORT_BIT	    0x0010U
#define NVM2_DAT_SUPPORT_BIT	    0x0020U
#define NVM3_DAT_SUPPORT_BIT	    0x0040U
#define NVM_DAT_VERIFY_SUPPORT_BIT  0x0080U
#define SEC_DAT_SUPPORT_BIT	    0x0100U
#define AES_DAT_SUPPORT_BIT	    0x0200U
#define CORE_DAT_ENCRYPTION_BIT	    0x0400U
#define FROM_DAT_ENCRYPTION_BIT	    0x0800U
#define	NVM0_DAT_ENCRYPTION_BIT	    0x1000U
#define	NVM1_DAT_ENCRYPTION_BIT	    0x2000U
#define	NVM2_DAT_ENCRYPTION_BIT	    0x4000U
#define	NVM3_DAT_ENCRYPTION_BIT	    0x8000U

/* Core code enable flags */
#if (defined CORE_SUPPORT && defined CORE_PLAIN)
    #define CORE_CODE_PLAIN_SUPPORT_BIT     0x0001U
#else
#define CORE_CODE_PLAIN_SUPPORT_BIT     0x0000U
#endif

#if (defined CORE_SUPPORT && defined CORE_ENCRYPT)
    #define CORE_CODE_ENCRYPTION_BIT	0x0400U
#else
#define CORE_CODE_ENCRYPTION_BIT	0x0000U
#endif

/* FROM code enable flags */
#if (defined FROM_SUPPORT && defined FROM_PLAIN)
    #define FROM_CODE_PLAIN_SUPPORT_BIT     0x0002U
#else
#define FROM_CODE_PLAIN_SUPPORT_BIT     0x0000U
#endif

#if (defined FROM_SUPPORT && defined FROM_ENCRYPT)
    #define FROM_CODE_ENCRYPTION_BIT	0x0800U
#else
#define FROM_CODE_ENCRYPTION_BIT	0x0000U
#endif

/* NVM code enable flags */
#if (defined NVM_SUPPORT && defined NVM_PLAIN)
    #define NVM_CODE_PLAIN_SUPPORT_BITS     0x003CU
#else
#define NVM_CODE_PLAIN_SUPPORT_BITS     0x0000U
#endif

#if (defined NVM_SUPPORT && defined NVM_ENCRYPT)
    #define NVM_CODE_ENCRYPTION_BITS	0xF000U
#else
#define NVM_CODE_ENCRYPTION_BITS	0x0000U
#endif

/* Security code enable flags */
#ifdef SECURITY_SUPPORT 
#define SEC_CODE_SUPPORT_BIT	        0x0100U
#else 
#define SEC_CODE_SUPPORT_BIT		0x0000U
#endif

/************************************************************/
/* Family code definitions			            */
/************************************************************/
#define AXX_BIT         0x1u
#define AFS_BIT         0x2u
#define SFS_BIT         0x4u

#define CALIBRATION_BIT 0x10U
#define DUAL_KEY_BIT    0x20U
#define DAS_BIT         0x40U
#define SMART_ERASE_BIT 0x80U

/************************************************************/
/* Data file Tags to indicate M1, M3 or M7 data             */
/************************************************************/
#ifndef ENABLE_CODE_SPACE_OPTIMIZATION
#define M7		7U
#define M1		1U
#define P1		3U
#endif

#if ((defined ENABLE_V85_DAT_SUPPORT) || (!defined ENABLE_CODE_SPACE_OPTIMIZATION) || defined ENABLE_DAS_SUPPORT)
/************************************************************/
/* Device specific parameters				    */
/************************************************************/
/* Supported A3PE/A3PEL/AGLE     */
#define	AXXE600X_ID			0x023261CFU
#define	AXXE600X_ID_MASK	        0x03FFFFFFU
#define	AXXE600X_SD			6U
#define	AXXE600X_ROWS		        3444U
#define	AXXE600X_COLS		        1184U
#define AXXE600X_BSR_BIT_LENGTH         1056U
#define AXXE600X_SE_WAIT_CYCLE          87U

#define	AXXE1500X_ID			0x0253A1CFU
#define	AXXE1500X_ID_MASK	        0x03FFFFFFU
#define	AXXE1500X_SD			10U
#define	AXXE1500X_ROWS		        5644U
#define	AXXE1500X_COLS		        1956U
#define AXXE1500X_BSR_BIT_LENGTH        1740U
#define AXXE1500X_SE_WAIT_CYCLE         137U

#define AXXE3000X_ID		        0x0274E1CFU
#define AXXE3000X_ID_MASK	        0x03FFFFFFU
#define AXXE3000X_SD		        14U
#define AXXE3000X_ROWS		        7844U
#define AXXE3000X_COLS		        2728U
#define AXXE3000X_BSR_BIT_LENGTH        2424U
#define AXXE3000X_SE_WAIT_CYCLE         187U

/* Supported A3P/A3PL/AGL     */
#define AXX030X_ID			0x049011CFU
#define AXX030X_ID_MASK   	        0x07FFFFFFU
#define AXX030X_SD   		        2U
#define AXX030X_ROWS 		        625U
#define AXX030X_COLS 		        412U
#define AXX030X_BSR_BIT_LENGTH          288U

#define AXX060X_ID			0x029121CFU
#define AXX060X_ID_MASK   	        0x06FFFFFFU
#define AXX060X_SD   		        2U
#define AXX060X_ROWS 		        1244U
#define AXX060X_COLS 		        412U
#define AXX060X_BSR_BIT_LENGTH          372U

#define AXX125X_ID   		        0x02A121CFU
#define AXX125X_ID_MASK		        0x06FFFFFFU
#define AXX125X_SD   		        4U
#define AXX125X_ROWS 		        1244U
#define AXX125X_COLS 		        798U
#define AXX125X_BSR_BIT_LENGTH          564U

#define AXX250X_ID			0x02A141CFU
#define AXX250X_ID_MASK                 0x06FFFFFFU
#define AXX250X_SD			4U
#define AXX250X_ROWS			2300U
#define AXX250X_COLS			798U
#define AXX250X_BSR_BIT_LENGTH          708U

#define AXX400X_ID			0x02B141CFU
#define AXX400X_ID_MASK                 0x06FFFFFFU
#define AXX400X_SD			6U
#define AXX400X_ROWS			2300U
#define AXX400X_COLS			1184U
#define AXX400X_BSR_BIT_LENGTH          900U

#define	AXX600X_ID			0x02b261CFU
#define	AXX600X_ID_MASK		        0x06FFFFFFU
#define	AXX600X_SD			6U
#define	AXX600X_ROWS			3444U
#define	AXX600X_COLS			1184U
#define AXX600X_BSR_BIT_LENGTH          1056U

#define AXX1000X_ID			0x12C281CFU
#define AXX1000X_ID_MASK		0x06FFFFFFU
#define AXX1000X_SD			8U
#define AXX1000X_ROWS		        4500U
#define AXX1000X_COLS		        1570U
#define AXX1000X_BSR_BIT_LENGTH         1392U

/* Supported AGLP     */
#define AGLP030X_ID			0x0E1011CFU
#define AGLP030X_ID_MASK   	        0x0FFFFFFFU
#define AGLP030X_SD   		        2U
#define AGLP030X_ROWS 		        625U
#define AGLP030X_COLS 		        412U
#define AGLP030X_BSR_BIT_LENGTH         288U

#define AGLP060X_ID			0x0E1121CFU
#define AGLP060X_ID_MASK   	        0x0EFFFFFFU
#define AGLP060X_SD   		        2U
#define AGLP060X_ROWS 		        1244U
#define AGLP060X_COLS 		        412U
#define AGLP060X_BSR_BIT_LENGTH         372U

#define AGLP125X_ID   		        0x0E2121CFU
#define AGLP125X_ID_MASK		0x0EFFFFFFU
#define AGLP125X_SD   		        4U
#define AGLP125X_ROWS 		        1244U
#define AGLP125X_COLS 		        798U
#define AGLP125X_BSR_BIT_LENGTH         564U

#define AXXN010X_ID			0x069041CFU
#define AXXN020X_ID			0x069081CFU
#define AXXN060X_ID			0x039521CFU
#define AXXN125X_ID 		        0x03A521CFU
#define AXXN250X_ID			0x03A541CFU

/* Supported AFS Devices */
#define AFS090_ID			0x031921CFU
#define AFS090_ID_MASK                  0x0BFFFFFFU
#define AFS090_SD			3U
#define AFS090_ROWS			1244U
#define AFS090_COLS			605U
#define AFS090_BSR_BIT_LENGTH           468U

#define AFS250_ID			0x032141CFU
#define AFS250_ID_MASK                  0x0FFFFFFFU
#define AFS250_SD			4U
#define AFS250_ROWS			2300U
#define AFS250_COLS			798U
#define AFS250_BSR_BIT_LENGTH           708U

#define AFS600_ID			0x033261CFU
#define AFS600_ID_MASK		        0x0FFFFFFFU
#define AFS600_SD			6U
#define AFS600_ROWS			3444U
#define AFS600_COLS			1184U
#define AFS600_BSR_BIT_LENGTH           1056U
#define AFS600_SE_WAIT_CYCLE            87U

#define AFS1500_ID			0x0353A1CFU
#define AFS1500_ID_MASK		        0x0FFFFFFFU
#define AFS1500_SD			10U
#define AFS1500_ROWS		        5644U
#define AFS1500_COLS		        1956U
#define AFS1500_BSR_BIT_LENGTH          1740U
#define AFS1500_SE_WAIT_CYCLE           137U
#else
#define AFS600_ID			0x033261CFU
#define AFS1500_ID                      0x0353A1CFU
#endif

#ifndef ENABLE_CODE_SPACE_OPTIMIZATION
#define M7KDATA0        0x45U
#define M7KDATA1        0x49U
#define M7KDATA2        0x66U
#define M7KDATA3        0x73U
#define M7KDATA4        0x3FU
#define M7KDATA5        0x5FU
#define M7KDATA6        0x01U
#define M7KDATA7        0x26U
#define M7KDATA8        0x11U
#define M7KDATA9        0xE9U
#define M7KDATA10       0xEEU
#define M7KDATA11       0x2EU
#define M7KDATA12       0x3AU
#define M7KDATA13       0x62U
#define M7KDATA14       0x37U
#define M7KDATA15       0xE1U

#define M1KDATA0        0x77U
#define M1KDATA1        0x50U
#define M1KDATA2        0xE9U
#define M1KDATA3        0x8FU
#define M1KDATA4        0xB1U
#define M1KDATA5        0x1EU
#define M1KDATA6        0x29U
#define M1KDATA7        0x3EU
#define M1KDATA8        0x86U
#define M1KDATA9        0x88U
#define M1KDATA10       0xB4U
#define M1KDATA11       0xCCU
#define M1KDATA12       0x48U
#define M1KDATA13       0x65U
#define M1KDATA14       0xDDU
#define M1KDATA15       0xACU

#define P1KDATA0        0x15U
#define P1KDATA1        0x7dU
#define P1KDATA2        0x69U
#define P1KDATA3        0x38U
#define P1KDATA4        0xaeU
#define P1KDATA5        0x09U
#define P1KDATA6        0x5fU
#define P1KDATA7        0x5eU
#define P1KDATA8        0x17U
#define P1KDATA9        0x4eU
#define P1KDATA10       0x5aU
#define P1KDATA11       0x37U
#define P1KDATA12       0x14U
#define P1KDATA13       0xe5U
#define P1KDATA14       0xa9U
#define P1KDATA15       0xe7U
#endif

#if ((!defined ENABLE_CODE_SPACE_OPTIMIZATION) || (defined ENABLE_V85_DAT_SUPPORT))
    #define FCBYTE0         0x86U
#define FCBYTE1         0x00U
#define FCBYTE2         0x50U
#define FCBYTE3         0x43U
#define FCBYTE4         0x64U
#define FCBYTE5         0x9CU
#define FCBYTE6         0x52U
#define FCBYTE7         0x40U
#define FCBYTE8         0xC6U
#define FCBYTE9         0x73U
#define FCBYTE10        0xB0U
#define FCBYTE11        0xFBU
#define FCBYTE12        0x75U
#define FCBYTE13        0xE7U
#define FCBYTE14        0xFFU
#define FCBYTE15        0xFDU
#endif



/* General delay and cycle parameters */
#define ISC_ENABLE_CYCLES              3u
#define ISC_ENABLE_DELAY               2157u
#define ISC_DISABLE_DELAY              288u
#define ISC_NOOP_CYCLES                3u
#define ISC_NOOP_DELAY                 264u
#define BYPASS_DELAY                   288u
#define POLL_DELAY                     100u
#define ISC_ERASE_CYCLES               3u
#define ERASE_POLL_DELAY               1000u
#define HIGHZ_CYCLES                   1u                                       
#define DESELECT_ALL_TILES_CYCLES      1u
#define AES_INIT_CYCLES                3u                                      
#define AES_INIT_DELAY                 190u
#define AES_MODE_CYCLES                1u
#define USERCODE_CYCLES                3u                                       


/* ARRAY delay and cycle parameters */
#define ISC_VERIFY0_CYCLES             3u
#define ISC_VERIFY0_DELAY              264u
#define ISC_VERIFY1_CYCLES             3u
#define ISC_VERIFY1_DELAY              264u
#define DESCRAMBLE_CYCLES              3u
#define DESCRAMBLE_DELAY               69u
#define ISC_ERASE_TIGHTEN_CYCLES       3u
#define ERASE_TIGHTEN_POLL_DELAY       10u
#define SMART_ERASE_POLL_DELAY         2003u
#define ISC_PROGRAM_CYCLES             3u
#define ISC_INCREMENT_CYCLES           3u
#define ISC_ADDRESS_SHIFT_CYCLES       1u
#define ISC_DATA_SHIFT_CYCLES          3u
#define ISC_PROGRAM_RLOCK_CYCLES       3u
#define ISC_PROGRAM_RDLC_CYCLES        1u
#define READ_FACTORY_DELAY             330u
#define READ_FACTORY_CYCLES            3u
#define FACTORY_ADDRESS_SHIFT_CYCLES   1u                                       

/* FROM delay and cycle parameters */
#define ISC_READ_UFROM_CYCLES          3u
#define ISC_READ_UFROM_DELAY           159u
#define ISC_VERIFY_UFROM_CYCLES		   3u
#define ISC_VERIFY_UFROM_DELAY 		   159u
#define ISC_PROGRAM_UFROM_CYCLES       5u
#define ISC_PROGRAM_UFROM_DELAY        10000u
#define ISC_PROGRAM_ENC_UFROM_CYCLES   5u
#define ISC_UFROM_ADDR_SHIFT_CYCLES    1u
#define ISC_DESCRAMBLE_UFROM_CYCLES    1u
#define ISC_DESCRAMBLE_UFROM_DELAY	   69u

/* UROW delay and cycle parameters */
#define ISC_READ_UROW_CYCLES           3u
#define ISC_READ_UROW_DELAY            330u
#define ISC_PROGRAM_UROW_CYCLES        15u

/* NVM delay and cycle parameters */
#define ACCESS_NVM_CYCLES              3u
#define ACCESS_NVM_DELAY               3u
#define ACCESS_NVM_POLL_DELAY          100u
#define NVM_ADDRESS_SHIFT_CYCLES       3u
#define NVM_ADDRESS_SHIFT_DELAY        20u
#define NVM_ADDRESS_SHIFT_POLL_CYCLES  3u
#define NVM_ADDRESS_SHIFT_POLL_DELAY   100u
#define NVM_DATA_SHIFT_CYCLES          3u
#define NVM_DATA_SHIFT_DELAY           6u
#define NVM_PROGRAM_CYCLES             3u
#define NVM_PROGRAM_POLL_DELAY         100u
#define NVM_READ_CYCLES                3u
#define NVM_READ_DELAY                 30u
#define NVM_DATA_SHIFT_ENC_CYCLES      3u
#define NVM_DATA_SHIFT_ENC_DELAY       46u
#define NVM_PROGRAM_ENC_CYCLES         3u
#define NVM_PROGRAM_ENC_DELAY          11535u

/* Security delay and cycle parameters */
#define ISC_PROGRAM_UKEY_CYCLES        3u
#define ISC_PROGRAM_DMK_CYCLES         15u
#define ISC_PROGRAM_SECURITY_CYCLES    3u
#define ISC_QUERY_SECURITY_CYCLES      3u
#define ISC_MATCH_UKEY_CYCLES          3u
#define ISC_MATCH_UKEY_DELAY           1438u
#define ISC_VERIFY_DMK_CYCLES          3u
#define ISC_VERIFY_DMK_DELAY           104u

#endif /* INC_DPDEFINE_H */

