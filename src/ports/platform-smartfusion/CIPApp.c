/*******************************************************************************
 * Copyright (c) 2009, Rockwell Automation, Inc.
 * All rights reserved. 
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "local.h"
#include "typedefs.h"
#include "trace.h"

#include "lwipinc.h"

#include "opener_user_conf.h"
#include "opener_api.h"
#include "networkhandler.h"
#include "cipcommon.h"
#include "serial.h"
#include "threads.h"

#include "mss_gpio/mss_gpio.h"


#define DEMO_APP_INPUT_ASSEMBLY_NUM                0x301
#define DEMO_APP_OUTPUT_ASSEMBLY_NUM               0x302
#define DEMO_APP_CONFIG_ASSEMBLY_NUM               0x303
#define DEMO_APP_HEARBEAT_INPUT_ONLY_ASSEMBLY_NUM  0x304
#define DEMO_APP_HEARBEAT_LISTEN_ONLY_ASSEMBLY_NUM 0x305
#define DEMO_APP_EXPLICT_ASSEMBLY_NUM              0x306


/* global variables for demo application (4 assembly data fields) */
EIP_UINT8 g_assemblydata301[32]; /* Input */
EIP_UINT8 g_assemblydata302[32]; /* Output */
EIP_UINT8 g_assemblydata303[10]; /* Config */
EIP_UINT8 g_assemblydata306[32]; /* Explicit */

extern void write_val(unsigned val);

void setLED(int x, int y)
	{
	MSS_GPIO_set_outputs( ~x );  // the LED outputs are inverted
	write_val(x);
	}

int getswitches(void)
	{
	return 0;
	}

EIP_STATUS
IApp_Init(void)
{
  /* create 3 assembly object instances*/
  /*INPUT*/
  createAssemblyObject(DEMO_APP_INPUT_ASSEMBLY_NUM, &g_assemblydata301[0], sizeof(g_assemblydata301));

  /*OUTPUT*/
  createAssemblyObject(DEMO_APP_OUTPUT_ASSEMBLY_NUM, &g_assemblydata302[0], sizeof(g_assemblydata302));

  /*CONFIG*/
  createAssemblyObject(DEMO_APP_CONFIG_ASSEMBLY_NUM, &g_assemblydata303[0], sizeof(g_assemblydata303));

  /*Heart-beat output assembly for Input only connections */
  createAssemblyObject(DEMO_APP_HEARBEAT_INPUT_ONLY_ASSEMBLY_NUM, 0, 0);

  /*Heart-beat output assembly for Listen only connections */
  createAssemblyObject(DEMO_APP_HEARBEAT_LISTEN_ONLY_ASSEMBLY_NUM, 0, 0);

  /* assembly for explicit messaging */
  createAssemblyObject(DEMO_APP_EXPLICT_ASSEMBLY_NUM, &g_assemblydata306[0], sizeof(g_assemblydata306));

  configureExclusiveOwnerConnectionPoint(0, DEMO_APP_OUTPUT_ASSEMBLY_NUM, DEMO_APP_INPUT_ASSEMBLY_NUM, DEMO_APP_CONFIG_ASSEMBLY_NUM);
  configureInputOnlyConnectionPoint(0, DEMO_APP_HEARBEAT_INPUT_ONLY_ASSEMBLY_NUM, DEMO_APP_INPUT_ASSEMBLY_NUM, DEMO_APP_CONFIG_ASSEMBLY_NUM);
  configureListenOnlyConnectionPoint(0, DEMO_APP_HEARBEAT_LISTEN_ONLY_ASSEMBLY_NUM, DEMO_APP_INPUT_ASSEMBLY_NUM, DEMO_APP_CONFIG_ASSEMBLY_NUM);

  return EIP_OK;
}

void
IApp_IOConnectionEvent(unsigned int pa_unOutputAssembly,
    unsigned int pa_unInputAssembly, EIOConnectionEvent pa_eIOConnectionEvent)
{
  /* maintain a correct output state according to the connection state*/

  (void) pa_unOutputAssembly; /* suppress compiler warning */
  pa_unInputAssembly = pa_unInputAssembly; /* suppress compiler warning */
  pa_eIOConnectionEvent = pa_eIOConnectionEvent; /* suppress compiler warning */
}

EIP_STATUS
IApp_AfterAssemblyDataReceived(S_CIP_Instance *pa_pstInstance)
{
  /*handle the data received e.g., update outputs of the device */

  if (pa_pstInstance->nInstanceNr == DEMO_APP_OUTPUT_ASSEMBLY_NUM)
    {
      /* Data for the output assembly has been received. */
      setLED(g_assemblydata302[0], g_assemblydata302[1]);
    }
  else if (pa_pstInstance->nInstanceNr == DEMO_APP_EXPLICT_ASSEMBLY_NUM)
    {
      /* do something interesting with the new data from
       * the explicit set-data-attribute message */
    }

  return EIP_OK;
}

bool
IApp_BeforeAssemblyDataSend(S_CIP_Instance *pa_pstInstance)
{
  /*update data to be sent e.g., read inputs of the device */
  /*In this sample app we mirror the data from out to inputs on data receive
   * therefore we need nothing to do here. Just return true to inform that
   * the data is new.
   */

  if (pa_pstInstance->nInstanceNr == DEMO_APP_INPUT_ASSEMBLY_NUM)
    {
    g_assemblydata301[0] = getswitches();
    }

  else if (pa_pstInstance->nInstanceNr == DEMO_APP_EXPLICT_ASSEMBLY_NUM)
    {
      /* do something interesting with the existing data
       * for the explicit get-data-attribute message */
    }
  return true;
}

EIP_STATUS
IApp_ResetDevice(void)
{
  /* add reset code here*/
  return EIP_OK;
}

EIP_STATUS
IApp_ResetDeviceToInitialConfiguration(void)
{
  /*rest the parameters */


  /*than perform device reset*/
  IApp_ResetDevice();
  return EIP_OK;
}

void *
IApp_CipCalloc(unsigned pa_nNumberOfElements, unsigned pa_nSizeOfElement)
{
  extern void *calloc(unsigned,unsigned);
  return calloc(pa_nNumberOfElements, pa_nSizeOfElement);
}

void
IApp_CipFree(void *pa_poData)
{
  (void) pa_poData;
}

void
IApp_RunIdleChanged(EIP_UINT32 pa_nRunIdleValue)
{
  (void) pa_nRunIdleValue;
}

