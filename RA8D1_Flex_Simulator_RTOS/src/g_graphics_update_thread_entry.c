#include "app_common.h"
#include "hal_data.h"

/* g_graphics_update_thread entry function */
/* pvParameters contains TaskHandle_t */
void g_graphics_update_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

    // T3 Main Loop
    while (1)
    {
        // 1. Check Game Status and Suspend if Over
        if (g_current_game_state.is_game_over)
        {
            printf("[GFX] Game Over, suspending thread.\n");
            vTaskSuspend(NULL); // Suspend self (T3)
        }

        // 2. Draw Screen and Feedback

        // Get the latest sensor data from the global buffer
        sensor_data_t *latest_sensor_data = &g_sensor_data_queue_buffer;

        // Conceptual call to draw the GUI (LVGL/GUIX)
        lvgl_draw_game_screen(&g_current_game_state, latest_sensor_data);

        // Trigger visual/haptic feedback if requested by T2
        if (g_current_game_state.trigger_visual_feedback)
        {
            lvgl_trigger_visual_feedback();
            g_current_game_state.trigger_visual_feedback = false; // Reset the flag
        }

        // 3. Delay (Cap Frame Rate)
        vTaskDelay(pdMS_TO_TICKS(33));
    }
}
