#include <Arduino.h>
#include <OneButton.h>
#include "LED.h"

// Hai LED, một nút bấm duy nhất.
//   single click : bật/tắt LED đang được chọn
//   double click : chuyển LED đang điều khiển (LED1 <-> LED2)
//   giữ nút      : LED đang chọn nháy, đổi trạng thái mỗi 200 ms

LED leds[] = { LED(LED1_PIN, LED_ACT), LED(LED2_PIN, LED_ACT) };
const uint8_t LED_COUNT = sizeof(leds) / sizeof(leds[0]);
uint8_t current = 0;  // chỉ số LED đang được điều khiển

OneButton button(BTN_PIN, !BTN_ACT);

void onClick();
void onDoubleClick();
void onLongPress();

void setup()
{
    Serial.begin(115200);
    for (uint8_t i = 0; i < LED_COUNT; i++) leds[i].begin();

    button.attachClick(onClick);
    button.attachDoubleClick(onDoubleClick);
    button.attachLongPressStart(onLongPress);

    Serial.println("Dang dieu khien: LED1");
}

void loop()
{
    // Không dùng delay(): cả hai LED nháy độc lập và nút luôn được quét.
    for (uint8_t i = 0; i < LED_COUNT; i++) leds[i].loop();
    button.tick();
}

void onClick()
{
    leds[current].flip();
    Serial.printf("LED%u: %s\n", current + 1, leds[current].isOn() ? "ON" : "OFF");
}

void onDoubleClick()
{
    // Chỉ đổi đối tượng điều khiển, LED cũ giữ nguyên trạng thái (kể cả đang nháy).
    current = (current + 1) % LED_COUNT;
    Serial.printf("Dang dieu khien: LED%u\n", current + 1);
}

void onLongPress()
{
    leds[current].blink(200);
    Serial.printf("LED%u: BLINK 200ms\n", current + 1);
}