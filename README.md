# Điều khiển 2 LED bằng 1 nút bấm (PlatformIO + OneButton)

Dự án nhúng dùng một nút bấm duy nhất để điều khiển hai LED, phân biệt thao tác bằng thư viện [OneButton](https://github.com/mathertel/OneButton). Mở rộng từ dự án điều khiển 1 LED bằng nút bấm ban đầu.

## Chức năng

| Thao tác | Hành vi |
|---|---|
| Single click | Bật/tắt LED đang được điều khiển. Nếu LED đang nháy thì dừng nháy và tắt |
| Double click | Chuyển đối tượng điều khiển giữa LED1 và LED2. LED còn lại giữ nguyên trạng thái |
| Nhấn giữ | LED đang được điều khiển nháy, đổi trạng thái mỗi 200 ms |

Khi khởi động: cả hai LED tắt, đối tượng điều khiển mặc định là LED1.

## Phần cứng

Board giả định: ESP32 DevKit (`esp32dev`).

| Thành phần | GPIO | Ghi chú |
|---|---|---|
| LED1 (ngoài, trên test board) | 4 | Qua điện trở 220 Ω – 1 kΩ nối GND. Đổi bằng `LED1_PIN` |
| LED2 (built-in) | 2 | LED có sẵn trên devboard |
| Nút bấm | 5 | Nối giữa GPIO5 và GND, dùng pull-up nội |

Các chân và mức tích cực cấu hình bằng `build_flags` trong `platformio.ini` (`LED1_PIN`, `LED2_PIN`, `BTN_PIN`, `LED_ACT`, `BTN_ACT`), không cần sửa code.

## Cấu trúc

```
.
├── platformio.ini
├── include/LED.h     # Lớp LED không chặn: on/off/flip/blink/loop
└── src/main.cpp      # Khởi tạo, xử lý 3 sự kiện nút bấm
```

## Cách hoạt động

- `loop()` chỉ gọi `button.tick()` và `leds[i].loop()` cho từng LED, không dùng `delay()`. Hai LED nháy độc lập dựa trên `millis()`.
- Biến `current` giữ chỉ số LED đang điều khiển. Double click đổi `current`, single click và nhấn giữ tác động lên `leds[current]`.
- Ba callback: `attachClick`, `attachDoubleClick`, `attachLongPressStart`.

## Build và nạp

```bash
git clone <URL-repo-của-bạn>
cd led-onebutton-pio
pio run -t upload
pio device monitor
```

Serial 115200 in ra LED đang điều khiển và trạng thái mỗi thao tác.

## Lưu ý

- Do đăng ký double click, OneButton đợi hết cửa sổ click (mặc định 400 ms) mới xác nhận single click, nên LED đổi trạng thái hơi trễ. Chỉnh bằng `button.setClickMs(...)`.
- Ngưỡng nhấn giữ mặc định 800 ms, chỉnh bằng `button.setPressMs(...)`.
- Trên ESP32, tránh dùng GPIO1/GPIO3 (UART0) cho LED nếu còn dùng Serial.
