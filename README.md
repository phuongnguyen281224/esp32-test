# ESP32-S3 Docker ESP-IDF Environment

Dự án này cung cấp một môi trường phát triển ESP32-S3 được đóng gói bằng Docker dựa trên ESP-IDF. Mọi công cụ, chuỗi biên dịch và phụ thuộc đều nằm trong container giúp bạn có thể xây dựng firmware ổn định trên mọi máy.

## Cấu trúc

```
.
├── Dockerfile              # Định nghĩa image ESP-IDF
├── docker-compose.yml      # Cấu hình dịch vụ phát triển
└── firmware                # Mã nguồn ESP-IDF mẫu
    ├── CMakeLists.txt
    ├── sdkconfig.defaults
    └── main
        ├── CMakeLists.txt
        └── main.c
```

## Yêu cầu
- Docker
- Docker Compose v2
- Quyền truy cập thiết bị `/dev/ttyUSB0` (hoặc cập nhật lại trong `docker-compose.yml`).

## Cách sử dụng

1. **Build image**
   ```bash
   docker compose build
   ```
2. **Mở shell trong container**
   ```bash
   docker compose run --rm esp32-dev bash
   ```
3. **Biên dịch firmware** (bên trong container)
   ```bash
   idf.py set-target esp32s3
   idf.py build
   ```
4. **Flash và monitor** (kết nối bo mạch ở `/dev/ttyUSB0`)
   ```bash
   idf.py -p /dev/ttyUSB0 flash monitor
   ```

Firmware mẫu (`firmware/main/main.c`) sẽ in thông tin chip và thông báo định kỳ để đảm bảo ESP-IDF hoạt động đúng trong container.
