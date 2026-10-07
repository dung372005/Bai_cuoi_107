# Điều khiển 2 LED bằng 1 nút nhấn (ESP32 + PlatformIO + OneButton)

Dùng một nút nhấn duy nhất để điều khiển hai LED. Phân biệt single click / double click / giữ nút bằng thư viện [OneButton](https://github.com/mathertel/OneButton).

## Chức năng

| Thao tác | Hành vi |
|---|---|
| Double click | Chuyển LED đang được điều khiển: LED1 ⇄ LED2 |
| Single click | Bật/tắt LED đang được điều khiển |
| Nhấn giữ (> 1 s) | LED đang được điều khiển nháy chu kỳ 200 ms |

Mặc định sau khi cấp nguồn: hai LED tắt, đang điều khiển LED1. LED đang chọn được in ra Serial (115200 baud) mỗi lần double click.

## Pin Mapping (giả định board ESP32 DevKit)

| Thiết bị | GPIO | Ghi chú |
|---|---|---|
| LED1 | 15 | LED ngoài trên test board, anode → trở 330 Ω → GPIO15, cathode → GND |
| LED2 | 2 | LED built-in trên devboard |
| Nút nhấn | 5 | Một chân nối GPIO5, chân kia nối GND; dùng pull-up nội |

Mức tích cực khai báo ở đầu `src/main.cpp` (`LED_ON_LEVEL`, `BTN_ON_LEVEL`). Đổi nếu mạch của bạn khác.

Lưu ý: GPIO2, GPIO5, GPIO15 là chân strapping của ESP32. Cách đấu trên (LED có trở, nút kéo xuống GND khi nhấn) không ảnh hưởng quá trình boot, nhưng đừng giữ nút khi reset/nạp code.

## Cấu trúc dự án

```
.
├── platformio.ini
├── README.md
├── .gitignore
├── include/
├── lib/
│   └── LED/          # (hoặc LED.h trong include/) lớp LED của dự án gốc
└── src/
    └── main.cpp
```

## Build & nạp

```bash
pio run                  # build
pio run -t upload        # nạp
pio device monitor       # xem Serial
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
- Không dùng `delay()`, mọi thứ chạy không chặn trong `loop()`.
- Cần `LED.h` của dự án gốc có các phương thức `off()`, `flip()`, `blink(ms)`, `loop()`.

## Link dự án

<điền link GitHub public tại đây>
