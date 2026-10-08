#include <Arduino.h>
#include "LED.h"
#include <OneButton.h>

// ---------- Pin mapping ----------
constexpr uint8_t PIN_LED1 = 15;   // LED ngoài (test board)
constexpr uint8_t PIN_LED2 = 2;    // LED built-in trên devboard
constexpr uint8_t PIN_BTN  = 5;    // Nút bấm duy nhất

// Mức logic bật LED / nhấn nút (giả định: LED active HIGH, nút nối GND)
constexpr uint8_t LED_ON_LEVEL  = HIGH;
constexpr uint8_t BTN_ON_LEVEL  = LOW;

LED led1(PIN_LED1, LED_ON_LEVEL);
LED led2(PIN_LED2, LED_ON_LEVEL);
LED *leds[2] = { &led1, &led2 };
uint8_t sel = 0;                   // LED đang được điều khiển: 0 = LED1, 1 = LED2

// activeLow = true khi nút kéo chân xuống GND, pull-up nội được bật
OneButton button(PIN_BTN, BTN_ON_LEVEL == LOW);

void btnClick();
void btnDoubleClick();
void btnLongPress();

void setup()
{
    Serial.begin(115200);

    led1.off();
    led2.off();

    button.setPressMs(1000);                 // giữ > 1 s mới tính là long press
    button.attachClick(btnClick);            // single click: bật/tắt LED đang chọn
    button.attachDoubleClick(btnDoubleClick);// double click: đổi LED đang chọn
    button.attachLongPressStart(btnLongPress);// giữ nút: LED đang chọn nháy 200 ms

    Serial.println("Dang dieu khien: LED1 (GPIO15)");
}

void loop()
{
    // Không dùng delay(): cả hai LED và nút đều chạy theo millis()
    led1.loop();
    led2.loop();
    button.tick();
}

void btnClick()
{
    leds[sel]->flip();
}

void btnDoubleClick()
{
    sel ^= 1;                                // chuyển LED1 <-> LED2
    Serial.println(sel == 0 ? "Dang dieu khien: LED1 (GPIO15)"
                            : "Dang dieu khien: LED2 (GPIO2, built-in)");
}

void btnLongPress()
{
    leds[sel]->blink(200);
}