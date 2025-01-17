/**
* @file
* @brief Application Timer module
*
* @copyright 2018 Silicon Laboratories Inc.
*/


/* Z-Wave includes */
#include <AppTimer.h>
#include <SizeOf.h>
#include <ZW_system_startup_api.h>
#include <zpal_retention_register.h>
//#define DEBUGPRINT
#include "DebugPrint.h"

#include <FreeRTOS.h>
#include <task.h>
#include <ZW_typedefs.h>

/**
* AppTimer is a singleton, thus containing own object.
*/
SAppTimer g_AppTimer;

void AppTimerInit(uint8_t iTaskNotificationBitNumber, void * ReceiverTask)
{
  TimerLiaisonInit(    
                    &g_AppTimer.TimerLiaison,
                    sizeof_array(g_AppTimer.aTimerPointerArray),
                    g_AppTimer.aTimerPointerArray,
                    iTaskNotificationBitNumber,
                    (TaskHandle_t) ReceiverTask
                  );
}


bool AppTimerRegister(
                SSwTimer* pTimer,
                bool bAutoReload,
                void(*pCallback)(SSwTimer* pTimer)
              )
{
  ESwTimerLiaisonStatus status = TimerLiaisonRegister(&g_AppTimer.TimerLiaison,
                                                      pTimer,
                                                      bAutoReload,
                                                      pCallback);

  DPRINTF("AppTimerRegister() id=%d\n", pTimer->Id);

  // Do not fail if timer is already registered, just return
  if (status == ESWTIMERLIAISON_STATUS_ALREADY_REGISTRERED)
  {
    return true;
  }

  if (status == ESWTIMERLIAISON_STATUS_SUCCESS)
  {
    g_AppTimer.DeepSleepPersistent[pTimer->Id] = false;
    g_AppTimer.pDeepSleepCallback[pTimer->Id]  = NULL;
    return true;
  }

  return false;
}

void AppTimerNotificationHandler(void)
{
  TimerLiaisonNotificationHandler(&g_AppTimer.TimerLiaison);
}


void AppTimerSetReceiverTask(void * ReceiverTask)
{
  TimerLiaisonSetReceiverTask(&g_AppTimer.TimerLiaison,
                              (TaskHandle_t) ReceiverTask);
}

void AppTimerStopAll(void)
{
  /*Stops all timers */
  for (uint32_t i = 0; i < g_AppTimer.TimerLiaison.iTimerCount; i ++)
  {
    if (g_AppTimer.DeepSleepPersistent[i])
    {
      AppTimerDeepSleepPersistentStop(g_AppTimer.aTimerPointerArray[i]);
    }
    else
    {
      TimerStop(g_AppTimer.aTimerPointerArray[i]);
    }
  }
}

ZW_WEAK ESwTimerStatus 
AppTimerDeepSleepPersistentStop(SSwTimer* pTimer)
{
  UNUSED(pTimer);
  return ESWTIMER_STATUS_SUCCESS;
}
