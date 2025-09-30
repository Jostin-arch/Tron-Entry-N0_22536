#ifndef APP_COMMON_H_
#define APP_COMMON_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h> // For rand()

// 1. FREE RTOS INCLUDES AND GLOBAL QUEUE HANDLE DECLARATION
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

// Declares the handle for the queue (defined in a generated FSP file)
extern QueueHandle_t g_sensor_data_queue;

// --- 2. CONSTANTS AND STRUCTURES (No changes needed here) ---

#define FINGER_COUNT 5
#define FLEX_THRESHOLD 512
#define WIN_CONDITION_SCORE 10

typedef struct {
    uint32_t timestamp_ms;
    uint16_t adc_values[FINGER_COUNT];
} sensor_data_t;

typedef struct {
    bool is_game_over;
    bool trigger_visual_feedback;
    uint8_t target_finger;
    uint8_t current_score_required;
    uint32_t score_count[FINGER_COUNT];
} game_state_t;


// --- 3. GLOBAL VARIABLES (Shared State - MUST use extern) ---

// Declares that these variables exist, but reserves memory elsewhere (in app_common.c)
extern sensor_data_t g_sensor_data_queue_buffer;
extern game_state_t g_current_game_state;


// --- 4. HELPER / SIMULATION FUNCTIONS (Prototypes ONLY) ---

// Only the function signatures (prototypes) remain
uint16_t simulate_adc_read(int finger_index);
void ra8d1_glcdc_init();
void lvgl_draw_game_screen(game_state_t *state, sensor_data_t *latest_sensor_data);
void lvgl_trigger_visual_feedback();


#endif /* APP_COMMON_H_ */
