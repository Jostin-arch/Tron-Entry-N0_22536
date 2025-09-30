#include "app_common.h"
#include "hal_data.h"

/* Renamed to match the name required by the FSP Linker configuration. */
void g_sensor_acquisition_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

    // The main loop for this task
    while (1)
    {
        // 1. Acquire Data (from your original code)
        sensor_data_t new_data;

        // Use the actual RTOS system time for the timestamp
        new_data.timestamp_ms = xTaskGetTickCount() * portTICK_PERIOD_MS;

        // Simulate reading 5 fingers
        for (int i = 0; i < FINGER_COUNT; i++)
        {
            new_data.adc_values[i] = simulate_adc_read(i);
        }

        // 2. Send Data to Queue
        // Store the data in the global buffer for the Graphics Task (T3) to peek at.
        g_sensor_data_queue_buffer = new_data;

        // Send the data packet to the Processing Task (T2) via the FreeRTOS Queue.
        // 0 means don't wait if the queue is full.
        xQueueSend(g_sensor_data_queue, &new_data, 0);

        // 3. Delay
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
