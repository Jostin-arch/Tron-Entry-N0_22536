#include "app_common.h"
#include "hal_data.h"

/* g_data_processing_thread entry function */
/* pvParameters contains TaskHandle_t */
void g_data_processing_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

    sensor_data_t received_data;

    // Initialize the target/score on startup
    g_current_game_state.target_finger = 0;
    g_current_game_state.current_score_required = 10;

    // The main thread loop
    while (1)
    {
        // 1. Receive Data (Replacing rtos_queue_receive)
        // Wait for data to arrive from the Acquisition Task (T1).
        // portMAX_DELAY makes this task block/wait indefinitely until data arrives.
        BaseType_t status = xQueueReceive(g_sensor_data_queue, &received_data, portMAX_DELAY);

        if (pdPASS == status) // Data was successfully received
        {
            // --- 2. Game Logic (Core Processing) ---

            // Check if the current target finger is flexed (value > threshold)
            if (received_data.adc_values[g_current_game_state.target_finger] > FLEX_THRESHOLD)
            {
                g_current_game_state.score_count[g_current_game_state.target_finger]++;

                // Trigger visual feedback flag for the Graphics Task (T3)
                g_current_game_state.trigger_visual_feedback = true;

                // Check for win condition for the current finger
                if (g_current_game_state.score_count[g_current_game_state.target_finger] >= g_current_game_state.current_score_required)
                {
                    printf("[LOGIC] Target finger %d reached score %d.\n",
                           g_current_game_state.target_finger, g_current_game_state.current_score_required);

                    // Move to the next target finger and update requirement
                    g_current_game_state.target_finger = (g_current_game_state.target_finger + 1) % FINGER_COUNT;
                    g_current_game_state.current_score_required += 2;

                    if (g_current_game_state.current_score_required > 20) {
                         g_current_game_state.is_game_over = true;
                    }
                }
            }
        }

        // 3. Delay (Relinquish control and allow other tasks to run)
        // Replaces rtos_thread_delay(10)
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
