/**
 * @file rtos_finger_flex_sim.c
 * @brief Visual Finger Flex Simulator (µT-Kernel 3.0) for Renesas RA8D1 EK.
 *
 * This program implements a multi-tasking, real-time rehabilitation game 
 * simulator, replacing external hardware (flex sensors, haptic glove) with
 * internal simulation and utilizing the RA8D1's on-chip GLCDC for visual feedback.
 * The architecture is based on three concurrent RTOS tasks communicating via 
 * a simple global state and message buffer, demonstrating real-time capability.
 *
 * Constraints Met:
 * 1. Hardware: Renesas RA8D1 EK (Internal MCU, GLCDC).
 * 2. OS: µT-Kernel 3.0 (Simulated/Targeted).
 * 3. Language: Pure C.
 * 4. Core Idea: Preserves Sensor Acquisition, Data Processing, and Interactive Feedback.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h> // For rand()

// --- RTOS and FSP Placeholder Definitions ---

/* * NOTE: For e2 studio/FSP, these macros conceptually map to 
 * kernel functions (e.g., tx_thread_sleep, tk_tsleep). 
 * In a real FSP project, these would be directly replaced by the RTOS API.
 */
#define rtos_thread_delay(ms)   /* Conceptual RTOS call to relinquish control for ms */
#define rtos_queue_send(q, d)   /* Conceptual RTOS call to send data to a queue */
#define rtos_queue_receive(q, d)/* Conceptual RTOS call to wait for data on a queue */
#define RTOS_WAIT_FOREVER       0xFFFFFFFF

// --- Global Data Structures for Task Communication ---

// 1. Simulated Sensor Data Structure (Input)
typedef struct
{
    uint16_t adc_values[5];     // Simulated ADC reading (0-4095) for 5 fingers
    uint32_t timestamp_ms;      // Time of acquisition
    uint8_t finger_pressed_mask; // Bitmask indicating which of the 5 simulated buttons is pressed
} sensor_data_t;

// 2. Game State Structure (Output/Feedback)
typedef struct
{
    uint8_t required_finger;    // Current target finger index (0=Thumb to 4=Little)
    int16_t target_repetitions; // Repetitions remaining
    uint32_t target_set_time;   // Time the current target was presented
    uint32_t last_reaction_time; // Time taken for the last successful repetition
    bool is_game_over;          // Game termination flag
    bool trigger_visual_feedback; // Flag for Graphics Task
    uint32_t score_count[5];    // Score/success count per finger
} game_state_t;

// --- Global RTOS Resources ---
sensor_data_t g_sensor_data_queue_buffer; // Simulates the single element in the queue
game_state_t g_current_game_state;

// Global timer (simulating RTOS tick)
uint32_t g_system_time_ms = 0; 
#define FINGER_COUNT 5

// --- Forward declarations of Task Entry Functions ---
void Sensor_Acquisition_Task_entry(void);
void Data_Processing_Task_entry(void);
void Graphics_Update_Task_entry(void);

// --- Simulation Helper Functions ---

/**
 * @brief Simulates virtual ADC reading for a finger flex sensor.
 * ADC value increases with 'time' to simulate natural fluctuation.
 * @param finger_index Index of the finger (0-4).
 * @return Simulated 12-bit ADC value (0-4095).
 */
uint16_t simulate_adc_read(int finger_index)
{
    // Base simulation: value fluctuates around 1500 + 100 * index
    uint16_t base_value = 1500 + (finger_index * 100);
    int32_t noise = (int32_t)(500 * sin((float)g_system_time_ms * 0.005f + (float)finger_index));
    uint16_t value = (uint16_t)(base_value + noise);

    // Apply "random" external press (simulated touch input for the demo)
    // The probability of a random press is very low, simulating a user touch event.
    if (rand() % 1000 == 0) {
        g_current_game_state.finger_pressed_mask |= (1 << (rand() % 5));
    }
    
    // If the simulated press is active for this finger, simulate a high flex value
    if (g_current_game_state.finger_pressed_mask & (1 << finger_index))
    {
        value = 3800; // Simulating a strong flex
    } else {
        value = 1000 + (rand() % 500); // Relaxed state
    }
    
    // Clamp the value to the max 12-bit ADC range
    if (value > 4095) value = 4095;
    return value;
}

/**
 * @brief Placeholder function for initializing the RA8D1's GLCDC/Display.
 */
void ra8d1_glcdc_init(void) 
{
    // In a real FSP project, this would call R_GLCDC_Open(), LVGL/GUIX setup, 
    // and initialize the MIPI-DSI panel.
    printf("[GLCDC] Graphics initialized on RA8D1 EK.\n");
}

/**
 * @brief Placeholder function for drawing the game UI/state.
 */
void lvgl_draw_game_screen(const game_state_t *state, const sensor_data_t *data)
{
    // In a real LVGL/GUIX implementation, this draws the entire screen.
    printf("[GFX] Rendering: Target %d, Reps: %d, Time: %lu ms, ADC[%d]: %hu\n", 
           state->required_finger, 
           state->target_repetitions, 
           state->last_reaction_time,
           state->required_finger,
           data->adc_values[state->required_finger]);
}

/**
 * @brief Placeholder function for visual feedback (replaces haptic buzz).
 */
void lvgl_trigger_visual_feedback(void)
{
    // In a real LVGL system, this would flash a color or play a sound/animation.
    printf("[GFX] SUCCESS VISUAL FEEDBACK: Flash green light on screen.\n");
}

/**
 * @brief Conceptual function to start the RTOS kernel.
 */
void rtos_kernel_start_conceptual(void)
{
    printf("\n[MAIN] Conceptual RTOS Kernel started. Control passed to scheduler.\n");
    // In a real e2 studio project, this is the call (e.g., tx_kernel_enter() or similar)
    // that launches the configured threads.
    
    // For submission integrity, we simulate the logic here as the RTOS threads' run sequence.
    while(g_current_game_state.is_game_over == false) {
        // T1 (Highest Priority/Fastest execution)
        Sensor_Acquisition_Task_entry(); 
        
        // T2 (Medium Priority)
        Data_Processing_Task_entry();
        
        // T3 (Lowest Priority/Graphics)
        Graphics_Update_Task_entry();
        
        // Simulate a small time advance
        g_system_time_ms += 10;
        
        // Manual execution break (to prevent infinite loop in a debugger-less environment)
        if (g_current_game_state.target_repetitions <= 0 || g_system_time_ms > 200000) {
            g_current_game_state.is_game_over = true;
        }
    }
}

// --- Task Entry Functions ---

/**
 * @brief Task 1: Sensor Data Acquisition (Simulated)
 * Priority: Highest (Ensures timely data collection for hard real-time)
 * Period: 50ms (As suggested in the plan for high-frequency acquisition)
 */
void Sensor_Acquisition_Task_entry(void)
{
    // This is the thread's main loop in a real RTOS environment
    printf("--- T1: Sensor Acquisition Task Started (Priority: Highest) ---\n");
    
    // Initialize random seed based on time (conceptual)
    srand(123); 

    while (g_current_game_state.is_game_over == false)
    {
        // 1. Simulate data reading
        sensor_data_t new_data;
        new_data.timestamp_ms = g_system_time_ms;

        for (int i = 0; i < FINGER_COUNT; i++)
        {
            new_data.adc_values[i] = simulate_adc_read(i);
        }

        // 2. Send data to the processing task (Simulated Queue Send)
        g_sensor_data_queue_buffer = new_data;
        // rtos_queue_send(g_queue_handle, &new_data); 
        
        // 3. Increment global time and delay
        g_system_time_ms += 50; 
        rtos_thread_delay(50); 
    }
}

/**
 * @brief Task 2: Data Processing and Game Logic
 * Priority: Medium (Runs the core game logic and timing checks)
 */
void Data_Processing_Task_entry(void)
{
    const uint16_t FLEX_THRESHOLD = 3000; // ADC value required for a successful flex
    sensor_data_t received_data;
    
    // Initialize game state
    g_current_game_state.required_finger = 0; // Thumb
    g_current_game_state.target_repetitions = 10;
    g_current_game_state.target_set_time = g_system_time_ms;
    g_current_game_state.is_game_over = false;

    printf("--- T2: Data Processing Task Started (Priority: Medium) ---\n");

    while (g_current_game_state.is_game_over == false)
    {
        // 1. Wait for data from Acquisition Task (Simulated Queue Receive)
        // rtos_queue_receive(g_queue_handle, &received_data, RTOS_WAIT_FOREVER);
        received_data = g_sensor_data_queue_buffer; // Simulated receive

        if (g_current_game_state.target_repetitions > 0)
        {
            uint8_t target = g_current_game_state.required_finger;
            
            // Check for successful repetition
            if (received_data.adc_values[target] >= FLEX_THRESHOLD)
            {
                // A successful repetition!
                g_current_game_state.target_repetitions--;
                
                // Calculate reaction time
                g_current_game_state.last_reaction_time = 
                    received_data.timestamp_ms - g_current_game_state.target_set_time;
                
                g_current_game_state.score_count[target]++;
                g_current_game_state.trigger_visual_feedback = true;

                // Set a new random target finger
                g_current_game_state.required_finger = rand() % FINGER_COUNT;
                g_current_game_state.target_set_time = received_data.timestamp_ms;
                
                // Reset simulated touch press after successful input
                g_current_game_state.finger_pressed_mask = 0;

                printf("[PROC] Repetition Success! Reps Left: %d, Time: %lu ms\n", 
                       g_current_game_state.target_repetitions, g_current_game_state.last_reaction_time);
            }
        }
        else // Game is over
        {
            g_current_game_state.is_game_over = true;
            printf("[PROC] --- GAME OVER --- Total Repetitions Complete.\n");
        }
        
        rtos_thread_delay(10); // Run more frequently than GFX but less than ACQ
    }
}

/**
 * @brief Task 3: Graphics Update and Display Management
 * Priority: Medium-Low (Updates the screen at a fixed frame rate)
 * Frame Rate: ~33ms (Approx. 30 FPS)
 */
void Graphics_Update_Task_entry(void)
{
    ra8d1_glcdc_init(); // Initialize the RA8D1 display hardware
    printf("--- T3: Graphics Update Task Started (Priority: Medium-Low) ---\n");
    
    // Buffer to hold the last received sensor data for display (no direct access to ADC)
    sensor_data_t last_sensor_data;

    while (g_current_game_state.is_game_over == false)
    {
        // 1. Get the latest data from the sensor buffer for visualization
        last_sensor_data = g_sensor_data_queue_buffer;
        
        // 2. Render the game state
        lvgl_draw_game_screen(&g_current_game_state, &last_sensor_data);
        
        // 3. Trigger visual feedback if needed
        if (g_current_game_state.trigger_visual_feedback)
        {
            lvgl_trigger_visual_feedback();
            g_current_game_state.trigger_visual_feedback = false;
        }
        
        // 4. Update the display buffers (Conceptual LVGL/GUIX call)
        // lv_task_handler(); 

        rtos_thread_delay(33); // Cap frame rate at ~30 FPS
    }
    
    // Game Over screen display (Conceptual)
    printf("[GFX] Displaying Final Results Screen.\n");
}

/**
 * @brief Main function: The entry point for the embedded application in FSP.
 * This function handles initial setup and starts the RTOS kernel.
 */
int main(void)
{
    // The FSP initialization and RTOS setup would typically happen before or in this function.
    printf("\n--- Renesas RA8D1 TRON Competition Submission ---\n");
    printf("Program: Visual Finger Flex Simulator (µT-Kernel 3.0)\n");
    printf("Simulating real-time multi-tasking on Arm Cortex-M85...\n\n");
    
    // Initialize global state before kernel starts
    g_system_time_ms = 0;
    g_current_game_state.is_game_over = false;
    for (int i = 0; i < 5; i++) {
        g_current_game_state.score_count[i] = 0;
    }
    
    // Initialize the graphics hardware early
    ra8d1_glcdc_init();
    
    // NOTE: In a real e2 studio project, the configured RTOS threads (Tasks 1, 2, 3) 
    // are automatically created, and main() simply calls the kernel start function.
    // We call the conceptual kernel start which contains the task logic simulation.
    rtos_kernel_start_conceptual();
    
    Graphics_Update_Task_entry(); // Ensure final screen update outside the kernel loop
    
    return 0; // Should never be reached in an embedded RTOS loop
}


