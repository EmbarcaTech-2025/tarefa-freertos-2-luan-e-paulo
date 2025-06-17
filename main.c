#include "FreeRTOS.h"
#include "task.h"
#include "tasks/tasks.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include <string.h>
#include <stdlib.h>
#include "hardware/hardware_init.h"

extern TaskHandle_t handle_button_monitor; 
extern TaskHandle_t handle_wait_reaction;

int main() {
    hardware_init(); // Inicialização movida para hardware_init.c

    xTaskCreate(TaskWaitForReaction, "Reacao", 1024, NULL, 2, &handle_wait_reaction);
    xTaskCreate(TaskButtonMonitor, "Botoes", 512, NULL, 1, &handle_button_monitor);
    xTaskCreate(TaskDisplay, "Display", 1024, NULL, 3, NULL);

    vTaskStartScheduler();

    while (1) {}
}