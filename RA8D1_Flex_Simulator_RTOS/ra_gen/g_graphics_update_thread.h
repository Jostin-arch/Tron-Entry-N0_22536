/* generated thread header file - do not edit */
#ifndef G_GRAPHICS_UPDATE_THREAD_H_
#define G_GRAPHICS_UPDATE_THREAD_H_
#include "bsp_api.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void g_graphics_update_thread_entry(void * pvParameters);
                #else
extern void g_graphics_update_thread_entry(void *pvParameters);
#endif
FSP_HEADER
FSP_FOOTER
#endif /* G_GRAPHICS_UPDATE_THREAD_H_ */
