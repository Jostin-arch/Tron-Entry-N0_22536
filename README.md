# Rehabilitation System for Stroke Recovery: Hard Real-Time Visual Finger Flex Simulator (Renesas RA8D1)

## 🏆 TRON Competition Entry | Project ID: 22536

[cite_start]This project delivers a complete, hard real-time rehabilitation system designed around the **Renesas RA8D1 Microcontroller** and its **Arm Cortex-M85** core[cite: 301, 303]. [cite_start]It implements a Visual Finger Flex Simulator, successfully porting the complexity of an external Haptic Glove System into a **deterministic, resource-efficient, purely software-based solution**[cite: 282, 308].

[cite_start]The architecture demonstrates maximum utilization of the RA8D1's on-chip peripherals and robust concurrency management under a Real-Time Operating System (RTOS)[cite: 287].

---

## ⚡ Technical Architecture: Hard Real-Time Concurrency

[cite_start]The system is engineered as a multi-tasking application, strictly adhering to hard real-time constraints through three decoupled, priority-driven tasks[cite: 284, 289, 291]. [cite_start]This design ensures that time-critical sensor data acquisition and processing are never delayed by the lower-priority graphics rendering loop[cite: 299, 314, 316]. [cite_start]The target OS is **µT-Kernel 3.0**, which guarantees deterministic and predictable task execution essential for medical simulations[cite: 3, 13, 307].

### 1. RTOS Task Structure (FreeRTOS / $\mu$T-Kernel 3.0 Target)

| Task ID | Component Name | Role | Priority | Execution Pattern | Core Function |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **T1** | **Sensor Acquisition** (`g_sensor_acquisition_thread_entry.c`) | [cite_start]**Data Producer.** Simulates ADC polling and timestamping[cite: 298]. | **Highest** | [cite_start]**Time-Driven** (Fixed 50 ms loop using `vTaskDelay`) [cite: 169, 292, 298] | [cite_start]Ensures precise $20\text{Hz}$ acquisition frequency, guaranteeing the T$_{sample}$ integrity and removing jitter[cite: 315]. |
| **T2** | **Data Processing** (`g_data_processing_thread_entry.c`) | [cite_start]**Logic Core & Consumer.** Executes game state machine logic[cite: 298]. | **Medium** | [cite_start]**Event-Driven** (Blocks indefinitely with `xQueueReceive(..., portMAX\_DELAY)`) [cite: 29, 189] | [cite_start]Guarantees zero-latency processing of every acquired data packet[cite: 298]. |
| **T3** | **Graphics Update** (`g_graphics_update_thread_entry.c`) | [cite_start]**Display Driver.** Renders game state, UI, and visual feedback[cite: 298]. | **Medium-Low** | [cite_start]**Time-Driven** (Fixed 33 ms loop using `vTaskDelay`) [cite: 246] | [cite_start]Maintains a stable target frame rate of $\approx 30$ FPS (for visual feedback)[cite: 224]. |

### 2. Inter-Task Communication (ITC) Protocols

[cite_start]Tasks are decoupled using two non-blocking mechanisms[cite: 296, 316]:

| Mechanism | Sender(s) $\rightarrow$ Receiver(s) | Data Type | Usage / Rationale |
| :--- | :--- | :--- | :--- |
| **FreeRTOS Message Queue** (`g_sensor_data_queue`) | T1 $\rightarrow$ T2 | [cite_start]`sensor_data_t` (Timestamp & 5 ADC values) [cite: 35, 36, 158] | [cite_start]**Lossless and Event-Driven:** T2 blocks until a packet arrives, ensuring reliable, synchronous processing[cite: 189]. |
| **Shared Global Buffer** (`g_sensor_data_queue_buffer`) | T1/T2 $\rightarrow$ T3 | [cite_start]`sensor_data_t` & `game_state_t` [cite: 51, 52] | [cite_start]**High-Speed Peek:** T3 reads the latest visual data directly from the buffer, decoupling the graphics loop from the sensor pipeline's critical path[cite: 235]. |

---

## 💻 Hardware Abstraction and Resource Utilization

[cite_start]The project minimizes external dependencies, demonstrating that the RA8D1 is capable of hosting a complex, graphics-intensive application entirely on-chip[cite: 309, 319, 322].

### On-Chip Simulation and Peripheral Use

| Original External Component | Replaced By On-Chip Logic | RA8D1 Feature Utilized | Implementation Note |
| :--- | :--- | :--- | :--- |
| **Flex Sensors & External ADC** | [cite_start]`Sensor_Acquisition_Task` [cite: 321] [cite_start]& `simulate_adc_read()` [cite: 67] | **RTOS Timer** | [cite_start]Generates virtual ADC readings (e.g., $100$ to $900$) [cite: 83, 178] [cite_start]at a fixed $50\text{ms}$ interval[cite: 147]. |
| [cite_start]**Haptic Feedback (Glove)** [cite: 321] | [cite_start]Visual Feedback Flag (`trigger\_visual\_feedback`) [cite: 47] | [cite_start]**GLCDC/2D Drawing Engine** [cite: 304, 321] | [cite_start]T2 sets the flag on success [cite: 203][cite_start], and T3 renders the visual cue[cite: 117]. |
| [cite_start]**External Flutter App** [cite: 293] | [cite_start]`Graphics_Update_Task` (T3) [cite: 321] | [cite_start]**Arm Cortex-M85 Core & GLCDC** [cite: 305, 321] | [cite_start]Enables high-performance visual feedback and UI rendering directly on the embedded system[cite: 304]. |

### Game Logic (`g_data_processing_thread_entry.c`)

The **Data Processing Task (T2)** implements the core state machine logic:
* [cite_start]**Flexion Check:** Determines success by checking if the ADC value for the `required\_finger` is greater than the **FLEX\_THRESHOLD (3000)**[cite: 178, 195].
* [cite_start]**Progression Logic:** T2 advances the `target\_finger` using the modulo operator [cite: 205] [cite_start]and implements difficulty scaling by increasing the `current\_score\_required` by **2** after each set completion[cite: 209, 182, 198, 204, 206].
* [cite_start]**Reaction Time:** The task calculates reaction time by subtracting the `target\_set\_time` from the current timestamp upon successful flex[cite: 200, 201].
* [cite_start]**Termination:** T2 sets `is\_game\_over = true` [cite: 215] [cite_start]when the total repetitions are complete, leading T3 to self-suspend [cite: 232, 233, 234] and conserve power.

---

## 📦 Project Structure and Portability

[cite_start]The project adheres to a modular design for seamless integration with the Renesas Flexible Software Package (FSP)[cite: 324].

| File Name | Description | RTOS Role & Function |
| :--- | :--- | :--- |
| `g_sensor_acquisition_thread_entry.c` | [cite_start]**Task 1 Source.** Uses `xTaskGetTickCount()` [cite: 159] [cite_start]for timing and `xQueueSend()` [cite: 166] to reliably send data. | **Producer** (Highest Priority) |
| `g_data_processing_thread_entry.c` | [cite_start]**Task 2 Source.** Uses `xQueueReceive()` [cite: 189] [cite_start]with `portMAX\_DELAY` to wait for data and runs the core game logic[cite: 191]. | **Consumer & Logic Core** (Medium Priority) |
| `g_graphics_update_thread_entry.c` | [cite_start]**Task 3 Source.** Calls `lvgl_draw_game_screen()` [cite: 237] [cite_start]and uses `vTaskSuspend(NULL)` [cite: 248] to end execution. | **Display Driver** (Medium-Low Priority) |
| `app_common.h` / `app_common.c` | [cite_start]Defines structs (`sensor_data_t`, `game_state_t`) [cite: 34, 40] [cite_start]and contains function prototypes, abstracting the application logic from the underlying RTOS API[cite: 326]. | **Shared State & Abstraction Layer** |
| `hal_entry.c` | [cite_start]Standard FSP entry point where the C runtime is set up and pins are configured[cite: 254, 255, 256, 257]. | **System Initialization** |

---

**Members:** JOSTIN JAISON & KEERTHANA N

***Redefining Recovery. [cite_start]Rewiring Motion.*** [cite: 280]