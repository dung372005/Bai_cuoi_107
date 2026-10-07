# Điều khiển 2 LED bằng 1 nút nhấn (ESP32 + PlatformIO + OneButton)
<img width="591" height="1280" alt="image" src="https://github.com/user-attachments/assets/78386736-ad10-4795-b3f5-7a2ae68b61db" />


*Mạch thực tế: ESP32 DevKit trên breadboard, LED ngoài, LED built-in trên board và một nút nhấn.*

Dùng một nút nhấn duy nhất để điều khiển hai LED. Phân biệt single click / double click / giữ nút bằng thư viện [OneButton](https://github.com/mathertel/OneButton). Dự án phát triển từ dự án gốc điều khiển 1 LED bằng nút nhấn (ON/OFF bằng single click, nháy LED bằng nhấn giữ).

## Chức năng

| Thao tác | Hành vi |
|---|---|
| Double click | Chuyển LED đang được điều khiển: LED1 ⇄ LED2 |
| Single click | Bật/tắt LED đang được điều khiển |
| Nhấn giữ (> 1 s) | LED đang được điều khiển nháy chu kỳ 200 ms |

Mặc định sau khi cấp nguồn: hai LED tắt, đang điều khiển LED1. LED đang chọn được in ra Serial (115200 baud) mỗi lần double click.

## Phần cứng

- 1 board ESP32 DevKit (`esp32doit-devkit-v1`)
- 1 LED ngoài (LED1) + điện trở hạn dòng (khoảng 330 Ω)
- 1 LED built-in trên board (LED2)
- 1 nút nhấn, breadboard, dây nối

### Pin Mapping

Khai báo trong `platformio.ini`, mục `build_flags`.

| Macro | esp32doit-devkit-v1 | esp32c3_super_mini | Ghi chú |
|---|---|---|---|
| `LED_PIN` / `LED_ACT` (LED1, ngoài) | GPIO15 / HIGH | GPIO4 / HIGH | Anode → trở → GPIO, cathode → GND |
| `LED2_PIN` / `LED2_ACT` (LED2, built-in) | GPIO2 / HIGH | GPIO8 / LOW | LED có sẵn trên board |
| `BTN_PIN` / `BTN_ACT` (nút) | GPIO5 / LOW | GPIO9 / LOW | Nối GPIO với GND, dùng pull-up nội |

Chân của `esp32c3_super_mini` (LED1 = GPIO4) là giả định, đổi theo mạch thực tế.

Lưu ý: trên ESP32 DevKit, GPIO2/5/15 là chân strapping. Cách đấu trên vẫn ổn nhưng đừng giữ nút lúc reset hoặc nạp code.


## Cách hoạt động

1. `platformio.ini` định nghĩa chân và mức tích cực cho từng board qua `build_flags`.
2. `main.cpp` tạo hai đối tượng `LED` (`led1`, `led2`) và một mảng con trỏ `leds[]`; biến `sel` cho biết LED nào đang được điều khiển.
3. OneButton gọi callback tương ứng: `btnClick()` → `flip()`, `btnDoubleClick()` → đổi `sel`, `btnLongPress()` → `blink(200)`.
4. `loop()` chỉ gọi `led1.loop()`, `led2.loop()`, `button.tick()`. Không dùng `delay()`, mọi thứ chạy theo `millis()`.

## Build & nạp

```bash
pio run -e esp32doit-devkit-v1             # build
pio run -e esp32doit-devkit-v1 -t upload   # nạp
pio device monitor                         # xem Serial
```

## Kiểm thử

| # | Thao tác | Kết quả mong đợi |
|---|---|---|
| 1 | Cấp nguồn | Hai LED tắt |
| 2 | Single click | LED1 bật; click nữa thì tắt |
| 3 | Double click | Serial báo chuyển sang LED2 |
| 4 | Single click | LED2 (built-in) bật/tắt, LED1 giữ nguyên |
| 5 | Giữ nút > 1 s | LED đang chọn nháy 200 ms |
| 6 | Single click khi đang nháy | LED dừng nháy, chuyển ON/OFF |

## Lưu ý

- Do đã đăng ký double click, single click được xác nhận sau khoảng 400 ms (cửa sổ chờ click thứ hai của OneButton), nên LED phản hồi hơi trễ. Chỉnh bằng `button.setClickMs(...)`.
- Nếu đang nháy LED1 rồi double click sang LED2, LED1 vẫn tiếp tục nháy.
- Cần `LED.h` có các phương thức `off()`, `flip()`, `blink(ms)`, `loop()`.

## Link dự án

<>
