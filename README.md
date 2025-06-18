
# Tarefa: Roteiro de FreeRTOS #2 - EmbarcaTech 2025

Autores: **Luan Felipe Azzi e Paulo**

Curso: Residência Tecnológica em Sistemas Embarcados

Instituição: EmbarcaTech - HBr

Campinas, junho de 2025

---

# Reaction Time Game com FreeRTOS e OLED SSD1306

Este projeto implementa um jogo de tempo de reação usando o microcontrolador **Raspberry Pi Pico (RP2040)**, rodando **FreeRTOS** com um display OLED **SSD1306** via I2C e dois botões físicos.

## Como Funciona

1. O sistema inicia mostrando "Aguarde" no display
2. Após um tempo aleatório (1-3s), mostra qual botão pressionar
3. O usuário deve pressionar o botão correto o mais rápido possível
4. O sistema mostra o tempo de reação em milissegundos
5. LEDs fornecem feedback visual:
   - Verde: pronto/aguardando
   - Azul/Vermelho: indica qual botão pressionar (azul para A, vermelho para B)
   - Amarelo: erro/pressão antecipada

## Componentes Usados

- **Raspberry Pi Pico W / RP2040**
- **FreeRTOS**
- **Display OLED SSD1306 (128x64) via I2C**
- **2 botões táteis (entrada com pull-up)**
- **3 LEDs (verde, azul e vermelho) para feedback visual**

## Mapeamento de Pinos

| Função       | GPIO |
|--------------|------|
| Botão A      | 5    |
| Botão B      | 6    |
| LED Verde    | 11   |
| LED Azul     | 12   |
| LED Vermelho | 13   |
| I2C SDA      | 14   |
| I2C SCL      | 15   |

---

## 📜 Licença
GNU GPL-3.0.
