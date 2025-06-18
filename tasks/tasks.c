#include "FreeRTOS.h"
#include "task.h"
#include "hardware/hardware_init.h"
#include "ssd1306.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Estado global
volatile bool waiting_for_press = false;
volatile int expected_button = -1; 
volatile absolute_time_t start_time;
volatile int reaction_time_ms = -1;
volatile bool early_or_wrong_press = false;
volatile bool restart_attempt = false;

TaskHandle_t handle_button_monitor = NULL;
TaskHandle_t handle_wait_reaction = NULL;

void TaskWaitForReaction(void *params) {
  while (true) {
        reaction_time_ms = -1;
        early_or_wrong_press = false;
        restart_attempt = false;

        ssd1306_clear_display(ssd);
        ssd1306_draw_string(ssd, 30, 32, "Aguarde");
        render_on_display(ssd, &frame_area);

        // Espera aleatória entre 1 e 3 segundos (com possibilidade de interrupção)
        int wait_time = 1000 + (rand() % 2000);
        int waited = 0;
        while (waited < wait_time && !restart_attempt) {
            vTaskDelay(pdMS_TO_TICKS(10));
            waited += 10;
        }

        // Se foi interrompido por pressionamento precoce
        if (restart_attempt) {
            restart_attempt = false;
            continue;
        }

        // Sorteia botão esperado
        expected_button = rand() % 2; // 0 = A, 1 = B

        ssd1306_clear_display(ssd);
        ssd1306_draw_string(ssd, 30, 32, expected_button == 0 ? "ESQUERDA!" : "DIREITA!");
        render_on_display(ssd, &frame_area);

        gpio_put(LED_B, expected_button == 0 ? 1 : 0);  // Azul para A
        gpio_put(LED_R, expected_button == 1 ? 1 : 0);  // Vermelho para B
        gpio_put(LED_G, 0); // Desliga verde

        waiting_for_press = true;
        start_time = get_absolute_time();

        // Espera botão ou erro
        while (waiting_for_press && !early_or_wrong_press) {
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        // Reset LEDs
        gpio_put(LED_B, 0);
        gpio_put(LED_R, 0);
        gpio_put(LED_G, 1);

        if (early_or_wrong_press) {
            ssd1306_clear_display(ssd);
            ssd1306_draw_string(ssd, 30, 32, "Errado");
            render_on_display(ssd, &frame_area);

            gpio_put(LED_G, 0);
            vTaskDelay(pdMS_TO_TICKS(2000));
            gpio_put(LED_G, 1);
            continue;
        }

        vTaskDelay(pdMS_TO_TICKS(2000));

    }
}

void TaskButtonMonitor(void *params) {
     while (true) {
        if (!waiting_for_press && !early_or_wrong_press) {
            if (gpio_get(BOTAO_A) == 0 || gpio_get(BOTAO_B) == 0) {
                early_or_wrong_press = true;
                ssd1306_clear_display(ssd);
                ssd1306_draw_string(ssd, 30, 32, "Muito cedo");
                render_on_display(ssd, &frame_area);
                gpio_put(LED_R, 1);
                gpio_put(LED_G, 1);
                vTaskSuspend(handle_wait_reaction);
                vTaskDelay(pdMS_TO_TICKS(2000));
                vTaskResume(handle_wait_reaction);
                gpio_put(LED_R, 0);
                restart_attempt = true;
            }
        }
        else if (waiting_for_press) {
    if (gpio_get(BOTAO_A) == 0) {
        if (expected_button == 0) {
            absolute_time_t press_time = get_absolute_time();
            int64_t diff_us = absolute_time_diff_us(start_time, press_time);
            reaction_time_ms = (diff_us + 500) / 1000; 
            waiting_for_press = false;
        } else {
            early_or_wrong_press = true;
            waiting_for_press = false;
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
    else if (gpio_get(BOTAO_B) == 0) {
        if (expected_button == 1) {
            absolute_time_t press_time = get_absolute_time();
            int64_t diff_us = absolute_time_diff_us(start_time, press_time);
            reaction_time_ms = (diff_us + 500) / 1000;
            waiting_for_press = false;
        } else {
            early_or_wrong_press = true;
            waiting_for_press = false;
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }

void TaskDisplay(void *params) {
    char buffer[32];
    while (true) {
        if (reaction_time_ms >= 0) {
            snprintf(buffer, sizeof(buffer), "Tempo: %d ms", reaction_time_ms);
            ssd1306_clear_display(ssd);
            ssd1306_draw_string(ssd, 0, 32, buffer);
            render_on_display(ssd, &frame_area);
            vTaskSuspend(handle_button_monitor);
            vTaskDelay(pdMS_TO_TICKS(2000));
            vTaskResume(handle_button_monitor);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}