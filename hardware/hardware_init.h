#ifndef HARDWARE_INIT_H
#define HARDWARE_INIT_H

#include "pico/stdlib.h"
#include "ssd1306.h"

// Definições de pinos
#define BOTAO_A 5
#define BOTAO_B 6
#define LED_G 11
#define LED_B 12  
#define LED_R 13  
#define I2C_SDA 14
#define I2C_SCL 15

// Variáveis globais
extern uint8_t ssd[ssd1306_buffer_length];
extern struct render_area frame_area;

// Protótipos
void hardware_init(void);

#endif